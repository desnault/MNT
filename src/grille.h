/*
Projet: Create_Raster
Auteur: Damien ESNAULT ROB23
Fichier: grille.h
Description:
Header de l'objet Grille
*/

// Definition Grille
#ifndef __GRILLE_H__
# define __GRILLE_H__

// Librairies
#include "triangle.h"

// Classe Grille
class Grille
{
	public:
		Grille(int width, int height, double x_min, double x_max, double y_min, double y_max, double z_min); // Constructeur
		/* Params : 
			- width = largeur de l'image
			- height = hauteur de l'image
			- x_min = valeur min en x
			- x_max = valeur max en x
			- y_min = valeur min en y
			- y_max = valeur max en y
			- z_min = valeur min en z
		*/
		double **m_grille; // Tableau dynamique 2D permettant de stocker les valeurs de hauteurs de chaque pixel
		double m_x_min; // Sauvegarde de x_min
		double m_y_min; // Sauvegarde de y_min
		double m_h_x; // Largeur d'un pixel
		double m_h_y; // Hauteur d'un pixel
		double m_z_min; // Sauvegarde de z_min
		int m_width; // Sauvegarde de la largeur de l'image
		int m_height; // Sauvegarde de la hauteur de l'image
		void add_triangle(Triangle* triangle); // Methode permettant de traiter un triangle pour modifier la valeur des pixels lui appartennant 
		void destroy(); // Methode permettant de supprimer proprement le tableau dynamique pour eviter les fuites memoires
		int check_value(int indice, int lim_basse, int lim_haute); // Methode permettant de limiter un indice entre une lim_basse et une lim_haute
};		

#endif