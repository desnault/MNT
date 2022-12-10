/*
Projet: Create_Raster
Auteur: Damien ESNAULT ROB23
Fichier: triangle.cpp
Description:

Decrit le fonctionnement d'un objet Triangle. Un objet Triangle permets comme son nom l'indique de definir un triangle mais plus precisement un triangle
en 2.5D. 

Un triangle est definie par 3 sommets p1, p2 et p3 et par une hauteur moyenne mean_z. L'objet Triangle a pour de sauvegarder les triangles obtenues via 
l'algorithme de triangulation afin de pouvoir par la suite attribuer une hauteur aux pixels contenus dans le triangle.

Pour cela un object Triangle possède la methode is_in_triangle() qui permets de  verifier si un point (x,y) est contenu dans le triangle.  
*/


// Librairies
#include "triangle.h"
#include <iostream>

using namespace std;


// Constructeur
Triangle::Triangle(double p1[3], double p2[3], double p3[3])
{
	// Determine la moyenne en z du triangle a partir de ces sommets
	// C'est cette valeur qui sera utilise pour definir la valeur d'un pixel contenus par le triangle
	m_mean_z = (p1[2]+p2[2]+p3[2])/3;
	// Enregistre les coordonnes x et y des sommets
	m_x[0] = p1[0];
	m_x[1] = p2[0];
	m_x[2] = p3[0];
	m_y[0] = p1[1];
	m_y[1] = p2[1];
	m_y[2] = p3[1];
}


// Methode produit scalaire
double Triangle::scalar_prod(double xa, double ya, double xb, double yb)
{
	/*
	Retourne la valeur de xa*yb - ya*xb
	*/
	return(xa*yb - ya*xb);
}


// Methode pertmettant de determiner si un pixel appartient au triangle
bool Triangle::is_in_triangle(double x, double y)
{
	/*
	Cette methode permets de detecter la precense d'un point dans le triangle a l'aide du determinant
	Pour cela:
	- on selectionne 2 sommets du triangles A et B. Le sommet restant est appelle C et le point a tester X
	- on calcule det(AC,AB) et det(AX,AB)
	- si le signe des determinants est le meme alors on peut passer a la suite
	- si le signe est different alors le point est forcement hors du triangle
	=> on reitire le procede pour les 2 autres paires de sommets
	si aucune difference de signe n'est detectee pour les 3 paires alors le point est dans le triangle
	*/
	for (int i=0; i<3;i++) // Parcours des sommets du triangle (on choisit le point A)
	{
		// Pour chaque jeu de 2 sommets verifie si le point (x,y) est dans la zone adequate
		double AB[2] = {m_x[(i+1)%3]-m_x[i],m_y[(i+1)%3]-m_y[i]}; // Point B = sommet i+1%3
		double AC[2] = {m_x[(i+2)%3]-m_x[i],m_y[(i+2)%3]-m_y[i]}; // Point C = sommet i+2%3
		double AX[2] = {x-m_x[i],y-m_y[i]};
		if (this->scalar_prod(AC[0],AC[1],AB[0],AB[1])*this->scalar_prod(AX[0],AX[1],AB[0],AB[1]) < 0) //Verification du signe 
		{	
			return false; // Si le signe est different alors c'est non
		}
	}
	return true; // Si aucunes differences de signe alors c'est oui
} 