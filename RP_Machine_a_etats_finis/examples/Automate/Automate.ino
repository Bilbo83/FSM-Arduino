/*
    Name:       Automate.ino
    Created:	11/11/2019 11:24:25
    Author:     PC-MONSTER\Roger2
*/


#include "Automate.h"

//*******************************************************************************************
// Programme test Machine � �tats finis
//*******************************************************************************************

//-----------------------------------------------------

void setup()
{
	Serial.begin(115200);
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
