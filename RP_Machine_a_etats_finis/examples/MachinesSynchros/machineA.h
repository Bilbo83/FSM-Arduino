#pragma once

//*******************************************************
//					MachineA 
//*******************************************************
/*                    
				
			a1            a21             a3
*---->|A1|-------->|A2|------------>|A3|--------->||finA||
                    |     a22
					|-------------->||Aerreur||
	  
*/

#include <machine.h>

class MachineA : public Machine
{
public:
	MachineA() : Machine(size_t(Etats::fin)) {}
	~MachineA() {};
	

	//table des états
	enum Etats : etat_t
	{
		A_1,
		A_2,
		A_3,
		A_erreur,
		fin
	};
	//table des évènements.
	enum Evenements : evnmt_t
	{
		a_1,	//apres 1s
		a_21,  //apres finB et finC
		a_22,	//après 20s
		a_3  //apres '1s'		
	};
		
};
MachineA machineA;





