//***************************************************************************
//                         machine.h
//***************************************************************************

#ifndef _MACHINE_h
#define _MACHINE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

//*****************************************************************************
//                        Classe Machine
//*****************************************************************************

typedef uint8_t etat_t; // identifiant d'un état.
typedef uint8_t evnmt_t; // identifiant d'un évènement.
typedef uint32_t ms_t;	// temps en ms.

class Machine
{
public:
	// constructeur:
	// nbEtats: nbre d'états de la machine.
	// etatDepart: id de l'état de départ.
	Machine(uint8_t nbEtats, etat_t etatDepart=0) : 
		Etats(new Etat[nbEtats]),
		_etatCourant(etatDepart)	
	{
		_etatFinal = false;
		_pause = false;
	}
	// destructeur:
	~Machine() {};
	//----------------- Stucture des Etats ------------------------------	
	struct Etat
	{
		//fonctions associées à l'état.
		void(*entree)();	//sera exécutée à l'entrée de l'état par la fonction 'etatSuivant'.
		void(*faire)();		//sera exécutée par la fonction 'actualiser'.
		void(*sortie)();	//sera exécutée à la sortie de l'état, lors de la transition par la fonction 'etatSuivant'.
		void(*evnmt)(evnmt_t); //sera exécutée par appel de la fonction 'signaler(ev)'.
		//NB: les 'gardes' conditionnelles seront gérées dans la fonction 'evnmt'.
	};		
	Etat *Etats; //pointeur sur le tableau des états.
	//-----------------------------------------------------------------		

	//initialisation d'un état par attribution des fonctions associées. 
	void initEtat(etat_t e, void(*fn_entree)(), void(*fn_faire)(), void(*fn_sortie)(), void(*fn_evnmt)(evnmt_t))
	{
		Etats[e].entree = fn_entree;
		Etats[e].faire = fn_faire;
		Etats[e].sortie = fn_sortie;
		Etats[e].evnmt = fn_evnmt;
	}

	//----------------- Stucture Historique ------------------------------	
	struct Historique
	{
		etat_t etatCourant;
		bool etatFinal;
		ms_t tempsRestant; // = millis() - tempoDebut + tempoDuree
		ms_t repetitionCpt;
		bool tempoPermanente;
		bool startChrono;

	};
	Historique historique; // mémoire historique.
	//---------------------------------------------------------------------
	void demarrer();						//démarrer sur l'état initial.
	void demarrer(etat_t);					//force démarrage sur un état donné.
	void suspendre();						//Suspendre le fonctionnement
	void reprendre();						//Reprendre le fonctionnement
	inline bool enPause() { return _pause; }  //retourne vrai si la machine est en pause.
	void actualiser();						//exécute la fonction 'faire' de l'état courant.
	void signaler(evnmt_t);					//signale un évènement à la fonction 'evnmt' de l'état courant.
	void signaler(Machine*, etat_t, evnmt_t); //signale un évènement à la fonction 'evnmt' de l'état d'une machine. 
	void etatSuivant(etat_t);				 //transition à l'état donné en paramètre.
	inline void etatFinal() { _etatFinal = true; } //la machine à atteint son état final.
	inline bool enEtatFinal() { return _etatFinal; } // retourne vrai si état final.
	inline etat_t quelEtat() { return _etatCourant; } //retourne l'Id de l'état courant.
	//méthodes publiques de temporisation.
	void startChrono(ms_t);//lancement d'une tempo
	inline void stopChrono() { _startChrono = false; } //arrêt tempo en cours.

	//table des évènement réservés et générés par la machine.
	enum Evenements : evnmt_t
	{
		FIN_TEMPO = 0xFF,
	};
protected:
	etat_t _etatCourant;	//Id état courant.
	bool _etatFinal; //la machine est à l'état final.
	bool _pause = false; //vrai si machine en pause.
	//-------------temporisation-----------------------
	ms_t _tempoDebut; //mémorise le début de la tempo.
	ms_t _tempoDuree; //durée de la tempo.
	bool _startChrono=false;	//vrai si le chrono est déclenché.
	void _signauxInternes(); //génère les signaux internes (chrono).
};

#endif

