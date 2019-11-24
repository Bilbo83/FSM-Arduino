/*
 Name:		MachinesSynchros.ino
 Created:	21/11/2019 10:45:59
 Author:	Roger2
*/

//*******************************************************************************************
// Programme exemple de Machines à états finis synchronisées.
//*******************************************************************************************

#include "Automate.h"
//---------------------------------------------------------
void setup()
{
	Serial.begin(115200);
	randomSeed(analogRead(0));
	pinMode(LED_BUILTIN, OUTPUT);
	Serial.println(F("---- setup -----"));
	automate.initialiserLesMachines();

}

//---------------------------------------------------------
void loop()
{
	automate.actualiserLesEvenements();
	automate.actualiserLesMachines();
	delay(10);
}