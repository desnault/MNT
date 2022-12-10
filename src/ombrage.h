/*
Projet: Create_Raster
Auteur: Damien ESNAULT ROB23
Fichier: ombrage.h
Description:
Header de l'objet Ombrage
*/

// Definition Ombrage
#ifndef __OMBRAGE_H__
# define __OMBRAGE_H__

// Classe Ombrage
class Ombrage
{
	public:
		Ombrage(int width, int height, double h_x, double h_y, double **grille); // Constructeur
		/*
		params:
		width = largeur de l'image
		height = hauteur de l'image
		h_x = taille du pixel suivant x
		h_y = taille du pixel suivant y
		**grille = tableau des profondeurs
		*/
		double **m_tab_ombrage; // Stockage des valeurs d'ombrage
		double **m_grille_copy; // Copie des valeurs de profondeur
		int m_width; // Sauvegarde de la largeur
		int m_height; // Sauvegarde de la hauteur
		double m_h_x; // Sauvegarde de la taille selon x
		double m_h_y; // Sauvegarde de la taille selon y
		void destroy(); // Methode de suppression des tableaux dynamiques
		void create_ombrage(); // Methode calculant les valeurs d'ombrage
};		

#endif