/*
Projet: Create_Raster
Auteur: Damien ESNAULT ROB23
Fichier: fichier.h
Description:
Header de l'objet Fichier
*/

// Definition Fichier
#ifndef __FICHIER_H__
# define __FICHIER_H__

// Librairies
#include <string>
#include <vector>

using namespace std;

// Classe Fichier
class Fichier
{
	public:
		Fichier(string filename); // Constructeur (param filename: nom du fichier a traiter)
		void parse_and_convert(); // Methode principale : lance la lecture et la projection des donnees
		string m_filename; // Nom du fichier a traiter
		vector<double> tab_x; // Conteneur des coordonnes x
		vector<double> tab_y; // Conteneur des coordonnes y
		vector<double> tab_z; // Conteneur des hauteurs z
 
};		

#endif