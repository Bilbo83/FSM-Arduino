#pragma once

//*******************************************************
//					Machine Porte (porte poulailler)
//*******************************************************

//const uint8_t NB_ETATS_PORTE = 7U;
const ms_t T_OUVERTURE_MAX = 17000UL;
const ms_t T_FERMETURE_MAX = 15000UL;

//---------------------------------------------
//				Classe Porte
//---------------------------------------------
class Porte : public Machine
{
public:
	Porte() : Machine(size_t(Etats::fin))	{}
	~Porte() {};
	//table des états
	enum Etats : etat_t
	{
		ARRET,
		OUVERTURE,
		FERMETURE,
		OUVERTE,
		FERMEE,
		ERR_OUVERTURE,
		ERR_FERMETURE,
		fin
	};

	//table des évènements.
	enum Evenements : evnmt_t
	{
		CMDE_ARRET,		//cmde arrêt
		CMDE_OUVERTURE, //cmde  ouverture
		CMDE_FERMETURE, //cmde  fermeture
		FC_BAS,			// fin de course basse
		FC_HAUT			// fin de course haute 
	};

};
//-----------fin classe Porte --------------
Porte porte;
//------------------------------------------

//--------------------------------------------------------------
// Fonctions de la machine
//--------------------------------------------------------------

//------------------------
// fonctions état : ARRET
//------------------------
void arret_entree()
{
	Serial.println(F("arret.."));
	//couper les relais d'ouverture  et fermeture
}
void arret_evt(evnmt_t ev)
{
	if (ev == Porte::Evenements::CMDE_OUVERTURE)	porte.etatSuivant(Porte::Etats::OUVERTURE);
	if (ev == Porte::Evenements::CMDE_FERMETURE)	porte.etatSuivant(Porte::Etats::FERMETURE);
}
//-----------------------------------------------
// fonctions état: OUVERTURE
//-----------------------------------------------
void ouverture_entree()
{
	Serial.println(F("ouverture.."));
	//marche relai ouverture.
	//lancer une tempo de T_OUVERTURE_MAX.
	porte.startChrono(T_OUVERTURE_MAX);
}
void ouverture_sortie()
{
	//arrêt relai ouverture.
	Serial.println(F("fin ouverture"));
}
void ouverture_evt(evnmt_t ev)
{
	if (ev == Machine::Evenements::FIN_TEMPO) porte.etatSuivant(Porte::Etats::ERR_OUVERTURE);
	if (ev == Porte::Evenements::CMDE_FERMETURE)	porte.etatSuivant(Porte::Etats::FERMETURE);
	if (ev == Porte::Evenements::FC_HAUT)	porte.etatSuivant(Porte::Etats::OUVERTE);
	if (ev == Porte::Evenements::CMDE_ARRET)	porte.etatSuivant(Porte::Etats::ARRET);
}
//------------------------------------------
// fonctions état: FERMETURE
//-----------------------------------------------
void fermeture_entree()
{
	Serial.println(F("fermeture.."));
	//marche relai fermeture.
	//lancer une tempo de T_OUVERTURE_MAX.
	porte.startChrono(T_FERMETURE_MAX);
}
void fermeture_sortie()
{
	//arrêt relai fermeture.
	Serial.println(F("fin fermeture"));
}
void fermeture_evt(evnmt_t ev)
{
	if (ev == Machine::Evenements::FIN_TEMPO) porte.etatSuivant(Porte::Etats::ERR_FERMETURE);
	if (ev == Porte::Evenements::CMDE_OUVERTURE)	porte.etatSuivant(Porte::Etats::OUVERTURE);
	if (ev == Porte::Evenements::FC_BAS)	porte.etatSuivant(Porte::Etats::FERMEE);
	if (ev == Porte::Evenements::CMDE_ARRET)	porte.etatSuivant(Porte::Etats::ARRET);
}
//-----------------------------------------------
// fonctions état: FERMEE
//-----------------------------------------------
void fermee_entree()
{
	Serial.println(F("porte fermee"));
}
void fermee_evt(evnmt_t ev)
{
	if (ev == Porte::Evenements::CMDE_OUVERTURE)	porte.etatSuivant(Porte::Etats::OUVERTURE);
}
//-----------------------------------------------
// fonctions état: OUVERTE
//-----------------------------------------------
void ouverte_entree()
{
	Serial.println(F("porte ouverte"));
}
void ouverte_evt(evnmt_t ev)
{
	if (ev == Porte::Evenements::CMDE_FERMETURE)	porte.etatSuivant(Porte::Etats::FERMETURE);
}
//-----------------------------------------------
// fonctions état: ERR_FERMETURE
//-----------------------------------------------
void err_fermeture_entree()
{
	Serial.println(F("erreur fermeture"));
}
void err_fermeture_evt(evnmt_t ev)
{
	if (ev == Porte::Evenements::CMDE_OUVERTURE)	porte.etatSuivant(Porte::Etats::OUVERTURE);
}
//-----------------------------------------------
// fonctions état: ERR_OUVERTURE
//-----------------------------------------------
void err_ouverture_entree()
{
	Serial.println(F("erreur ouverture"));
}
void err_ouverture_evt(evnmt_t ev)
{
	if (ev == Porte::Evenements::CMDE_FERMETURE)	porte.etatSuivant(Porte::Etats::FERMETURE);
}
//-----------------------------------------------
// initialisation des états
 void porteInitEtats()
{
	porte.initEtat(Porte::Etats::ARRET, arret_entree, nullptr, nullptr, arret_evt);
	porte.initEtat(Porte::Etats::OUVERTURE, ouverture_entree, nullptr, ouverture_sortie, ouverture_evt);
	porte.initEtat(Porte::Etats::FERMETURE, fermeture_entree, nullptr, fermeture_sortie, fermeture_evt);
	porte.initEtat(Porte::Etats::OUVERTE, ouverte_entree, nullptr, nullptr, ouverte_evt);
	porte.initEtat(Porte::Etats::FERMEE, fermee_entree, nullptr, nullptr, fermee_evt);
	porte.initEtat(Porte::Etats::ERR_OUVERTURE, err_ouverture_entree, nullptr, nullptr, err_ouverture_evt);
	porte.initEtat(Porte::Etats::ERR_FERMETURE, err_fermeture_entree, nullptr, nullptr, err_fermeture_evt);
}
