/*
Projet: Create_Raster
Auteur: Damien ESNAULT ROB23
Fichier: triangle.h
Description:
Header de l'objet Triangle
*/

// Definition Triangle
#ifndef __TRIANGLE_H__
# define __TRIANGLE_H__

// Classe Triangle
class Triangle
{
	public:
		Triangle(double p1[3], double p2[3], double p3[3]); // Constructeur (param: p1, p2, p3 coordonnes des sommets du triangle (x,y,z))
		double m_mean_z; // Moyenne des hauteurs des sommets
		double m_x[3] = {0,0,0}; // Sauvegarde des coordonnes en x des sommets
		double m_y[3] = {0,0,0}; // Sauvegarde des coordonnes en y des sommets
		double scalar_prod(double xa, double ya, double xb, double yb); // Methode produit scalaire 
		bool is_in_triangle(double x, double y); // Methode verifiant si un point (x,y) est dans le triangle a l'aide du produit scalaire
};		

#endif