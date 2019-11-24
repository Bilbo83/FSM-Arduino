#pragma once

//*****************************************
//  BLINK
//******************************************
/*
Clignotement de la LED interne (LED_BUILTIN),
à une cadence contrôlée par la valeur de 'dt'.
*/
class Blink : public Machine
{
public:
	Blink() : Machine(size_t(Etats::fin)) {}
	~Blink() {};
	//table des états
	enum Etats : etat_t
	{
		BLINK,
		fin
	};
	//table des évènements.
	enum Evenements : evnmt_t
	{
		//aucun
	};
	ms_t dt=1000ul;
};
//------------------
Blink blink;
//------------------


//--------------------------------------------------------------
// Fonctions de la machine
//--------------------------------------------------------------
void blink_entree()
{
	Serial.println(F("Blink"));
	digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
	blink.startChrono(blink.dt);
}
void blink_evnmt(evnmt_t ev)
{
	if (ev == Machine::Evenements::FIN_TEMPO) blink.etatSuivant(Blink::Etats::BLINK);
}

void blinkInitEtats()
{
	blink.initEtat(Blink::Etats::BLINK, blink_entree, nullptr, nullptr, blink_evnmt);
}