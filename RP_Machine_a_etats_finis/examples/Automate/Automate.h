#pragma once
//*************************************************************
//                 Automate.h
//*************************************************************


#include <machine.h>
#include "Porte.h"
#include "MachineABCD.h"
#include "Blink.h"

extern "C" {void evenementSerie();}

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
	
	porteInitEtats();
	machineABCDinitEtats();
	blinkInitEtats();
	//-----------Démarrage des Machines --------------------
	//porte.demarrer(Porte::Etats::ARRET);
	machineABCD.demarrer();
	blink.demarrer();
}
void Automate::actualiserLesMachines()
{
	//---- actualiser les machines -------------
	//porte.actualiser();
	machineABCD.actualiser();
	blink.actualiser();
}
void Automate::actualiserLesEvenements()
{
	//ex: surveiller la tension batterie
	const float TBATTERIE_MIN = 9.5;
	float tBatterie = analogRead(A0) * 1024.0f / 5.0f;
	if (tBatterie < TBATTERIE_MIN)
	{
		porte.signaler(Porte::Evenements::CMDE_OUVERTURE);
		machineABCD.suspendre();
	}
	//surveiller la liaison série:
	evenementSerie();

}


//------ liaison série ---------------
void decodeMessage(char c)
{
	switch (c)
	{
		//porte
	case 'O':
		Serial.println(F("PORTE: CMDE_OUVERTURE"));
		porte.signaler(Porte::Evenements::CMDE_OUVERTURE);
		break;
	case 'F':
		Serial.println(F("PORTE: CMDE_FERMETURE"));
		porte.signaler(Porte::Evenements::CMDE_FERMETURE);
		break;
	case 'H':
		Serial.println(F("PORTE: FC_HAUT"));
		porte.signaler(Porte::Evenements::FC_HAUT);
		break;
	case 'B':
		Serial.println(F("PORTE: FC_BAS"));
		porte.signaler(Porte::Evenements::FC_BAS);
		break;
	case 'A':
		Serial.println(F("PORTE: CMDE_ARRET"));
		porte.signaler(Porte::Evenements::CMDE_ARRET);
		break;
	case 'P':
		Serial.println(F("PORTE: PAUSE "));
		porte.suspendre();
		break;
	case 'M':
		Serial.println(F("PORTE: MARCHE "));
		porte.reprendre();
		break;

		//----------------------------------
	//machineABCD
	case 'r':
		Serial.println(F("REPRENDRE ABCD"));
		machineABCD.reprendre();
		break;
	case 's':
		Serial.println(F("SUSPENDRE ABCD"));
		machineABCD.suspendre();
		break;
	default:
		break;
	}
}
//-----------------------------------------------------
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




