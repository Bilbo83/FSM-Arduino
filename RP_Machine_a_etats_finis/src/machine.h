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

typedef uint8_t etat_t; // identifiant d'un �tat.
typedef uint8_t evnmt_t; // identifiant d'un �v�nement.
typedef uint32_t ms_t;	// temps en ms.

class Machine
{
public:
	// constructeur:
	// nbEtats: nbre d'�tats de la machine.
	// etatDepart: id de l'�tat de d�part.
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
		//fonctions associ�es � l'�tat.
		void(*entree)();	//sera ex�cut�e � l'entr�e de l'�tat par la fonction 'etatSuivant'.
		void(*faire)();		//sera ex�cut�e par la fonction 'actualiser'.
		void(*sortie)();	//sera ex�cut�e � la sortie de l'�tat, lors de la transition par la fonction 'etatSuivant'.
		void(*evnmt)(evnmt_t); //sera ex�cut�e par appel de la fonction 'signaler(ev)'.
		//NB: les 'gardes' conditionnelles seront g�r�es dans la fonction 'evnmt'.
	};		
	Etat *Etats; //pointeur sur le tableau des �tats.
	//-----------------------------------------------------------------		

	//initialisation d'un �tat par attribution des fonctions associ�es. 
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
	Historique historique; // m�moire historique.
	//---------------------------------------------------------------------
	void demarrer();						//d�marrer sur l'�tat initial.
	void demarrer(etat_t);					//force d�marrage sur un �tat donn�.
	void suspendre();						//Suspendre le fonctionnement
	void reprendre();						//Reprendre le fonctionnement
	inline bool enPause() { return _pause; }  //retourne vrai si la machine est en pause.
	void actualiser();						//ex�cute la fonction 'faire' de l'�tat courant.
	void signaler(evnmt_t);					//signale un �v�nement � la fonction 'evnmt' de l'�tat courant.
	void signaler(Machine*, etat_t, evnmt_t); //signale un �v�nement � la fonction 'evnmt' de l'�tat d'une machine. 
	void etatSuivant(etat_t);				 //transition � l'�tat donn� en param�tre.
	inline void etatFinal() { _etatFinal = true; } //la machine � atteint son �tat final.
	inline bool enEtatFinal() { return _etatFinal; } // retourne vrai si �tat final.
	inline etat_t quelEtat() { return _etatCourant; } //retourne l'Id de l'�tat courant.
	//m�thodes publiques de temporisation.
	void startChrono(ms_t);//lancement d'une tempo
	inline void stopChrono() { _startChrono = false; } //arr�t tempo en cours.

	//table des �v�nement r�serv�s et g�n�r�s par la machine.
	enum Evenements : evnmt_t
	{
		FIN_TEMPO = 0xFF,
	};
protected:
	etat_t _etatCourant;	//Id �tat courant.
	bool _etatFinal; //la machine est � l'�tat final.
	bool _pause = false; //vrai si machine en pause.
	//-------------temporisation-----------------------
	ms_t _tempoDebut; //m�morise le d�but de la tempo.
	ms_t _tempoDuree; //dur�e de la tempo.
	bool _startChrono=false;	//vrai si le chrono est d�clench�.
	void _signauxInternes(); //g�n�re les signaux internes (chrono).
};

#endif

