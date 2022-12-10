/*
Projet: Create_Raster
Auteur: Damien ESNAULT ROB23
Fichier: fichier.cpp
Description:
Definie le fonctionnement d'un objet fichier. Un objet Fichier a pour but d'ouvrir, de lire et de projeter les coordonnees 
du fichier donne en argument (filename). Ces donnees sont ensuite stockees dans des tableaux contenus par l'objet Fichier.

Cette classe permet notamment de recuperer un jeu de coordonnes planaires x et y a partir d'un jeu de coordonnes GPS. Les hauteurs z sont 
elles simplement stockees. 

Apres la creation de l'object Fichier il suffit d'appeller la methode parse_and_convert() pour lire et projeter
les donnees a la vole.
*/


// Librairies
#include "fichier.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <proj.h>

using namespace std;


// Constructeur de Fichier
Fichier::Fichier(string filename)
{
	m_filename = filename; // Recupere le nom du fichier a traiter
}


// Methode principale 
void Fichier::parse_and_convert()
{
	/*
	Cette methode permet d'ouvrir, de lire et de convertir les donnees contenues dans filename (latitude longitude hauteur) sous la forme : x y z .
	Pour cela la methode utilise la projection Lambert 93 et stocke les nouvelles coordonnes dans tab_x, tab_y et tab_z.
	*/

	// Ouverture du fichier
	cout<<"Opening File ...";
	ifstream file(m_filename);

	// Ouverture reussi
	if (file.is_open())
	{
		cout<<"\rFile opened successfuly"<<endl; // Informe l'utilisateur

		// Variable permettant de recuperer les informations sur la ligne
		double lat;
		double lon;
		double z;

		// Initialisation du projecteur Lambert 93
		PJ* Proj = proj_create_crs_to_crs(PJ_DEFAULT_CTX,"+proj=longlat +datum=WGS84", "+proj=lcc +lat_1=49 +lat_2=44 +lat_0=46.5 +lon_0=3 +x_0=700000 +y_0=6600000 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs", NULL);
		PJ_COORD geo_coord, cartesian_coord;
		geo_coord.lpzt.z = 0;

		// Debut de la lecture et de la conversion
		cout<<"Parsing and Converting: ";

		int i = 0; // Compteur

		// Tant que il reste une ligne a lire
		while (!file.eof())
		{
			 // Recuperation des valeurs sur la ligne
			file >> lat >> lon >> z;
			// Sauvegarde de z
			tab_z.push_back(abs(z));
			// Conversion 
			geo_coord.lpzt.lam = lon; // longitude
  			geo_coord.lpzt.phi = lat; // latitude
  			cartesian_coord = proj_trans(Proj, PJ_FWD, geo_coord);
  			// Sauvegarde de x et de y
  			tab_x.push_back(cartesian_coord.xy.x);
  			tab_y.push_back(cartesian_coord.xy.y);
  			
  			i++; // Incremente le compteur
  			cout<<"\rParsing and Converting: "<<i; // Affiche la valeur compteur
		}

		// Fin de la lecture et de la conversion
		cout<<"\rParsing and Converting successful"<<endl;
		// Informe l'utilisateur du nombre de lignes
		cout<<"Number of data converted: "<<tab_x.size()<<endl;
	}
	// Ouverture impossible
	else
	{
		cout<<"\rError when opening the file"<<endl; // Informe l'utilisateur
	}

	// Fermeture du fichier
	file.close();
}