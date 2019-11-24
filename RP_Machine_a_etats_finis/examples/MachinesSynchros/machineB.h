#pragma once
//*******************************************************
//					MachineB 
//*******************************************************
/*

			b1            b2                   bSynchro       b3
*---->|B1|-------->|B2|------------>|Bsynchro|--------->|B3|-------||finB||
					|    
					|------>b1Synchro

*/

#include <machine.h>

class MachineB : public Machine
{
public:
	MachineB() : Machine(size_t(Etats::fin)) {}
	~MachineB() {};
	//table des états
	enum Etats : etat_t
	{
		B_1,
		B_2,
		B_synchro,
		B_3,
		fin
	};
	//table des évènements.
	enum Evenements : evnmt_t
	{
		b_1,	//apres random(5s, 20s)
		b_2,  //apres 1s
		b_synchro,	//b2 & c2Synchro
		b_3  //apres '1s'
	};
	bool synchroC2 = false;
};
MachineB machineB;


