#pragma once
//*******************************************************
//					MachineC
//*******************************************************
/*

			c1                    cSynchro       c2         c3
*---->|C1|------------>|Csynchro|--------->|C2|------->|C3|----->||finC||
	 b1Synchro ------->|		              |--->c2Synchro

*/

#include <machine.h>

class MachineC : public Machine
{
public:
	MachineC() : Machine(size_t(Etats::fin)) {}
	~MachineC() {};
	//table des états
	enum Etats : etat_t
	{
		C_1,
		C_synchro,
		C_2,
		C_3,
		fin
	};
	//table des évènements.
	enum Evenements : evnmt_t
	{
		c_1,	//apres 10s
		c_synchro,	//c2 & b1Synchro
		c_2,  //apres random(5s)	 
		c_3  //apres 1s
	};
	bool synchroB1 = false;

};
MachineC machineC;


