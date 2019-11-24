#pragma once
//*************************************************************
//                 Automate.h
//*************************************************************

#include "machineA.h"
#include "machineB.h"
#include "machineC.h"

extern "C" {void evenementSerie(); }

extern void  machineAInitEtats();
extern void  machineBInitEtats();
extern void  machineCInitEtats();

const ms_t sec = 1000;
ms_t debut_process;

class Automate
{
public:
	Automate() {}
	~Automate() {}
	void initialiserLesMachines();
	void actualiserLesMachines();
	void actualiserLesEvenements();
};
Automate automate;

void Automate::initialiserLesMachines()
{
	//----------- initialisation des Machines -------------

	machineAInitEtats();
	machineBInitEtats();
	machineCInitEtats();
	
	//-----------Démarrage des Machines --------------------
	
	machineA.demarrer();
	
}
void Automate::actualiserLesMachines()
{
	//---- actualiser les machines -------------

	machineA.actualiser();
	machineB.actualiser();
	machineC.actualiser();
	
}
void Automate::actualiserLesEvenements()
{
	//surveiller la liaison série:
	evenementSerie();
}


//------ liaison série ---------------
void decodeMessage(char c)
{
	switch (c)
	{
	case 'd':
		Serial.println(F("DEMARRER A"));
		machineA.demarrer(MachineA::Etats::A_1);
		break;
	case 'r':
		Serial.println(F("REPRENDRE A, B, C"));
		machineA.reprendre();
		machineB.reprendre();
		machineC.reprendre();
		break;
	case 's':
		Serial.println(F("SUSPENDRE C, B, A"));
		machineC.suspendre();
		machineB.suspendre();
		machineA.suspendre();
		break;		
	default:
		break;
	}
}
//Place les caractères reçues par le port Serial dans buf_RX[]
// jusqu'à '\n'.
void evenementSerie() {
	char bufRX[16];
	static uint8_t nbRX = 0;
	static bool bRX_complet = false;

	while (Serial.available()) {
		// on lit le nouveau caractère
		char inChar = Serial.read();
		//si on a 'fin de ligne' '\n le message est complet.
		if (inChar == '\n') {
			bufRX[nbRX] = '\0';
			bRX_complet = true;
		}
		//sinon on le place dans bufRX.
		else bufRX[nbRX++] = inChar;
	}
	if (bRX_complet) {
		decodeMessage(bufRX[0]);//code sur une lettre
		bRX_complet = false;
		nbRX = 0; //RAZ du nombre de caractères recus.
	}
}


//--------------------------------------------------------------
// Fonctions de la machineA
//--------------------------------------------------------------

//--------------------------------------------------------------
void A_1_entree()
{
	Serial.println(F("A_1"));
	machineA.startChrono(1 * sec);
}
void A_1_evnmt(evnmt_t ev)
{
	if (ev == Machine::Evenements::FIN_TEMPO) machineA.etatSuivant(MachineA::Etats::A_2);
	if (ev == MachineA::Evenements::a_1)
	{
		Serial.println(F("ON REDEMARRE.."));
		machineA.etatSuivant(MachineA::Etats::A_1);
	}
}

//--------------------------------------------------------------
void A_2_entree()
{
	Serial.println(F("A_2"));
	debut_process = millis();
	machineB.demarrer(MachineB::Etats::B_1);
	machineC.demarrer(MachineC::Etats::C_1);
	machineA.startChrono(23 * sec);
}
void A_2_faire()
{
	if (machineB.enEtatFinal() && machineC.enEtatFinal())
	{
		ms_t t = millis() - debut_process;
		Serial.print(F("DUREE PROCESS: ")); Serial.println(t / 1000);
		machineA.etatSuivant(MachineA::Etats::A_3);
	}
}
void A_2_evnmt(evnmt_t ev)
{
	if (ev == Machine::Evenements::FIN_TEMPO) machineA.etatSuivant(MachineA::Etats::A_erreur);
}

//--------------------------------------------------------------
void A_3_entree()
{
	Serial.println(F("A_3"));
	machineA.startChrono(1 * sec);
}
void A_3_evnmt(evnmt_t ev)
{
	//exemple d'utilisation de 'signaler' directement à un état donné.
	if (ev == Machine::Evenements::FIN_TEMPO) machineA.signaler(&machineA, MachineA::Etats::A_1, MachineA::Evenements::a_1);

}
//------------------------------------------------------------------
void A_erreur_entree()
{
	Serial.println(F("A_erreur"));
	machineA.etatFinal();
}

//--------------------------------------------------------------
void machineAInitEtats()
{
	machineA.initEtat(MachineA::Etats::A_1, A_1_entree, nullptr, nullptr, A_1_evnmt);
	machineA.initEtat(MachineA::Etats::A_2, A_2_entree, A_2_faire, nullptr, A_2_evnmt);
	machineA.initEtat(MachineA::Etats::A_3, A_3_entree, nullptr, nullptr, A_3_evnmt);
	machineA.initEtat(MachineA::Etats::A_erreur, A_erreur_entree, nullptr, nullptr, nullptr);	
}

// --------------------------------------------------------------
// Fonctions de la machineB
//--------------------------------------------------------------


//--------------------------------------------------------------
void B_1_entree()
{
	ms_t t = random(5, 20);
	Serial.print(F("B_1: ")); 	Serial.println(t);
	machineB.startChrono(t * sec);
}
void B_1_evnmt(evnmt_t ev)
{
	if (ev == Machine::Evenements::FIN_TEMPO)
	{
		machineC.synchroB1 = true;
		machineB.etatSuivant(MachineB::Etats::B_2);
	}
}

//--------------------------------------------------------------
void B_2_entree()
{
	Serial.println(F("B_2"));
	machineB.startChrono(1 * sec);
}
void B_2_evnmt(evnmt_t ev)
{
	if (ev == Machine::Evenements::FIN_TEMPO) machineB.etatSuivant(MachineB::Etats::B_synchro);
}
void B_synchro_entree()
{
	Serial.println(F("B_synchro"));
}
void B_synchro_faire()
{
	if (machineB.synchroC2) machineB.etatSuivant(MachineB::Etats::B_3);
}
void B_synchro_sortie()
{
	machineB.synchroC2 = false;
}
//--------------------------------------------------------------
void B_3_entree()
{
	Serial.println(F("B_3"));
	machineB.startChrono(1 * sec);
}
void B_3_evnmt(evnmt_t ev)
{
	if (ev == Machine::Evenements::FIN_TEMPO) machineB.etatFinal();
}

//--------------------------------------------------------------
void machineBInitEtats()
{
	machineB.initEtat(MachineB::Etats::B_1, B_1_entree, nullptr, nullptr, B_1_evnmt);
	machineB.initEtat(MachineB::Etats::B_2, B_2_entree, nullptr, nullptr, B_2_evnmt);
	machineB.initEtat(MachineB::Etats::B_3, B_3_entree, nullptr, nullptr, B_3_evnmt);
	machineB.initEtat(MachineB::Etats::B_synchro, B_synchro_entree, B_synchro_faire, B_synchro_sortie, nullptr);
}

// --------------------------------------------------------------
// Fonctions de la machineC
//--------------------------------------------------------------
//--------------------------------------------------------------
void C_1_entree()
{
	Serial.println(F("C_1"));
	machineC.startChrono(10 * sec);
}
void C_1_evnmt(evnmt_t ev)
{
	if (ev == Machine::Evenements::FIN_TEMPO) machineC.etatSuivant(MachineC::Etats::C_synchro);
}
void C_synchro_entree()
{
	Serial.println(F("C_synchro"));

}
void C_synchro_faire()
{
	if (machineC.synchroB1) machineC.etatSuivant(MachineC::Etats::C_2);

}
void C_synchro_sortie()
{
	machineC.synchroB1 = false;
}

void C_2_entree()
{
	ms_t t = random(1,5);
	Serial.print(F("C_2: ")); 	Serial.println(t);
	machineC.startChrono(t*sec);
}
void C_2_evnmt(evnmt_t ev)
{
	if (ev == Machine::Evenements::FIN_TEMPO)
	{
		machineB.synchroC2 = true;
		machineC.etatSuivant(MachineC::Etats::C_3);
	}
}
void C_3_entree()
{
	Serial.println(F("C_3"));
	machineC.startChrono(5 * sec);
}
void C_3_evnmt(evnmt_t ev)
{
	if (ev == Machine::Evenements::FIN_TEMPO) machineC.etatFinal();
}


void machineCInitEtats()
{
	machineC.initEtat(MachineC::Etats::C_1, C_1_entree, nullptr, nullptr, C_1_evnmt);
	machineC.initEtat(MachineC::Etats::C_2, C_2_entree, nullptr, nullptr, C_2_evnmt);
	machineC.initEtat(MachineC::Etats::C_3, C_3_entree, nullptr, nullptr, C_3_evnmt);
	machineC.initEtat(MachineC::Etats::C_synchro, C_synchro_entree, C_synchro_faire, C_synchro_sortie, nullptr);
}