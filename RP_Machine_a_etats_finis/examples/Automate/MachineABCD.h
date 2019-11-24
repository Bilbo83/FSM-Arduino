#pragma once
//*******************************************************
//					MachineABCD 
//------------------------------------------------------
/*                       e3[n<10]
                   -----------------
            e1    \/     e2        |    e4
*---->|A|-------->|B|------------>|C|--------->||D||
       /\                e5                      |
       |------------------------------------------
*/
class MachineABCD : public Machine
{
public:
	MachineABCD() : Machine(size_t(Etats::fin)) {}
	~MachineABCD() {};
	//table des états
	enum Etats : uint8_t
	{
		A,
		B,
		C,
		D,
		fin
	};
	//table des évènements.
	enum Evenements : uint8_t
	{
		E1,	//apres 't_attente'
		E2,  //apres 't_attente'
		E3,  //apres 't_attente'
		E4	//après 'nb_boucle'
	};
	//variables spécifiques MachineABCD:
	uint16_t cptr_boucle;
	uint16_t  nb_boucle = 10; 
	ms_t  t_attente = 2000;
};
MachineABCD machineABCD;

//--------------------------------------------------------------
// Fonctions de la machine
//--------------------------------------------------------------
void A_entree()
{
	Serial.println(F("ABCD: A"));
	machineABCD.cptr_boucle = machineABCD.nb_boucle;
	machineABCD.startChrono(machineABCD.t_attente);
}
void A_evnmt(uint8_t ev)
{
	if (ev == Machine::Evenements::FIN_TEMPO) machineABCD.etatSuivant(MachineABCD::Etats::B);
}
//------------------------------------------------------------
void B_entree()
{
	Serial.println(F("ABCD: B"));
	machineABCD.startChrono(machineABCD.t_attente);
}
void B_evnmt(uint8_t ev)
{
	if (ev == Machine::Evenements::FIN_TEMPO) machineABCD.etatSuivant(MachineABCD::Etats::C);
}
//------------------------------------------------------------
void C_entree()
{
	Serial.println(F("ABCD: C"));
	machineABCD.startChrono(machineABCD.t_attente);
}
void C_evnmt(uint8_t ev)
{	
	if (ev == Machine::Evenements::FIN_TEMPO)
	{
		Serial.print(F("ABCD Cptr_boucle= ")); Serial.println(machineABCD.cptr_boucle);
		if (--machineABCD.cptr_boucle == 0) machineABCD.etatSuivant(MachineABCD::Etats::D);
		else machineABCD.etatSuivant(MachineABCD::Etats::B);
	}
}
//------------------------------------------------------------
void D_entree()
{
	Serial.println(F("ABCD: D"));
	machineABCD.startChrono(machineABCD.t_attente*5);
}
void D_evnmt(uint8_t ev)
{
	if (ev == Machine::Evenements::FIN_TEMPO) machineABCD.etatSuivant(MachineABCD::Etats::A);
}
//------------------------------------------------------------
void machineABCDinitEtats()
{
	machineABCD.initEtat(MachineABCD::Etats::A, A_entree, nullptr, nullptr, A_evnmt);
	machineABCD.initEtat(MachineABCD::Etats::B, B_entree, nullptr, nullptr, B_evnmt);
	machineABCD.initEtat(MachineABCD::Etats::C, C_entree, nullptr, nullptr, C_evnmt);
	machineABCD.initEtat(MachineABCD::Etats::D, D_entree, nullptr, nullptr, D_evnmt);
}