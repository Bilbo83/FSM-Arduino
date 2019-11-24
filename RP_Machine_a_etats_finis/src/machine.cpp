//****************************************************************************
//                              machine.cpp
//**************************************************************************** 

#include "machine.h"

//*****************************************************************************
//                              Classe Machine
//*****************************************************************************
//D�marrer ou red�marrer la machine � l'�tat courant.
void Machine::demarrer()
{
	_etatFinal = false;
	if (Etats[_etatCourant].entree != nullptr) Etats[_etatCourant].entree();
}

//Forcer le d�marrage de la machine � un �tat donn�.
void Machine::demarrer(etat_t e)
{
	_etatFinal = false;
	_etatCourant = e;
	if (Etats[_etatCourant].entree != nullptr) Etats[_etatCourant].entree();
}

//Suspendre le fonctionnement de la machine avec m�morisation
//de son �tat dans 'historique'
void Machine::suspendre()
{
	historique.etatCourant = _etatCourant;
	historique.etatFinal = _etatFinal;
	historique.tempsRestant = _tempoDebut + _tempoDuree - millis();
	historique.startChrono = _startChrono;
	_pause = true;
}

//Reprendre le fonctionnement de la machine avec restauration
//de son �tat avec 'historique'.
void Machine::reprendre()
{
	_etatCourant = historique.etatCourant;
	_etatFinal = historique.etatFinal;
	_tempoDebut = millis();
	_tempoDuree = historique.tempsRestant;
	_startChrono = historique.startChrono;
	_pause = false;

}

//Actualiser la machine:
void Machine::actualiser(void)
{
	if (!_pause) //si la machine n'est pas en pause, faire:
	{	
		_signauxInternes(); //g�n�re des signaux internes (tempo).
		//on �x�cute la fonction 'faire' si elle existe, sinon rien.
		if (Etats[_etatCourant].faire != nullptr) Etats[_etatCourant].faire();
	}
}

//Signaler un �v�nement � l'�tat courant.
// evt: id de l'�v�nement.
void Machine::signaler(evnmt_t evt)
{	
	//si la machine n'est pas en pause, ex�cuter la fonction �v�nement de l'�tat courant si elle existe.
	if (!_pause && Etats[_etatCourant].evnmt != nullptr) Etats[_etatCourant].evnmt(evt);	
}

//Signaler un �v�nement 'ev' � la fonction 'evnmt' de l'�tat 'e' d'une machine 'm'. 
// (� utiliser avec pr�cautions)
void Machine::signaler(Machine *m, etat_t e , evnmt_t ev) {m->Etats[e].evnmt(ev);}

//Transistion � l'�tat suivant.
void Machine::etatSuivant(etat_t e)
{
	if (!_pause) //si la machine n'est pas en pause, faire:
	{
		//on �x�cute la fonction 'sortie' si elle existe.
		if (Etats[_etatCourant].sortie != nullptr) Etats[_etatCourant].sortie();
		//on passe � l'�tat suivant.
		_etatCourant = e;
		//on �x�cute la fonction 'entree' si elle existe.
		if (Etats[_etatCourant].entree != nullptr) Etats[_etatCourant].entree();
	}
}

//Gestion des temporisations.
void Machine::startChrono(ms_t duree) //lancement d'une tempo.
{
	_tempoDuree = duree;
	_startChrono = true;
	_tempoDebut = millis();
}

void Machine::_signauxInternes()
{
	//si temporisation atteinte, signaler.
	if (_startChrono && (millis() - _tempoDebut > _tempoDuree))	
		this->signaler(Machine::Evenements::FIN_TEMPO);
}
