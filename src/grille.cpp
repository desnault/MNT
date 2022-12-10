/*
Projet: Create_Raster
Auteur: Damien ESNAULT ROB23
Fichier: grille.cpp
Description:

Decrit le fonctionnement d'un objet Grile. Un objet Grille permets de stocker les hauteurs associees a chaque pixel. Pour mettre a jout ces valeurs il faut
renseigner a la grille l'adresse d'un objet triangle pour modifier les hauteurs des pixels contenus dans le triangle.

Une Grille contient les caractéristiques de l'image (largeur, hauteur, xmin, xmax, ...) mais surtout un tableau avec les correspondances pixels hauteurs.

Après initialisation, il suffit d'appeler add_triangle() pour chaque triangle cree lors de la triangulation pour mettre a jour le tableau hauteur/pixel.  
*/

// Librairies
#include "grille.h"
#include "triangle.h"
#include <iostream>
#include <algorithm>
#include <math.h>  

using namespace std;

// Constructeur
Grille::Grille(int width, int height, double x_min, double x_max, double y_min, double y_max, double z_min)
{
	// Sauvegarde des parametres de l'image
	m_x_min = x_min;
	m_y_min = y_min;
	m_width = width;
	m_height = height;
	m_z_min = z_min;
	// Calcul de la hauteur/largeur d'un pixel
	m_h_x = (x_max-x_min)/width;
	m_h_y = (y_max-y_min)/height;

	// Creation du tableau
	m_grille = new double*[width];
	for (int i=0;i<width;i++)
	{
		m_grille[i] = new double[height];
		for (int j=0; j<height;j++)
		{
			m_grille[i][j] = z_min/2; // Cellule initialise a z_min/2
			/*
			Si une cellule reste a z_min/2 c'est que aucun actualisation n'a eu lieu sur cette cellule
			A l'affichage il suffit donc de tester si la valeur de la cellule est inferieure a z_min pour savoir si il faut afficher la valeur
			de la cellule ou non
			*/
		}
	}
}


// Methode permettant de supprimer le tableau dynamique m_grille
void Grille::destroy()
{
	delete [] m_grille;
}


// Methode permettant de limiter la valeur indice entre lim_basse et lim_haute
int Grille::check_value(int indice, int lim_basse, int lim_haute)
{
	if (indice<lim_basse) // Si inferieure a lim_basse 
	{
		return(lim_basse); // Renvoie lim_basse
	}
	if (indice>=lim_haute) // Si superieure ou egale a lim_haute
	{
		return(lim_haute-1); // Renvoie lim_haute-1
	}
	return(indice); // Sinon renvoie la valeur donnee (comprise donc en lim_basse et lim_haute)
}


// Methode permettant d'actualiser la grille avec un triangle
void Grille::add_triangle(Triangle* triangle)
{
	/*
	La methode va entourer le triangle d'un rectangle et isoler les pixels presents dans le rectangle. On testera alors pour chaque pixel
	si ce dernier (son centre) est dans le triangle. Si c'est le cas alors on actualise la grille pour ce pixel avec la valeur mean_z du triangle.
	*/

	// Rectangle entourant le triangle
	double temp_x_min = *min_element(begin(triangle->m_x), end(triangle->m_x));
	double temp_x_max = *max_element(begin(triangle->m_x), end(triangle->m_x));
	double temp_y_min = *min_element(begin(triangle->m_y), end(triangle->m_y));
	double temp_y_max = *max_element(begin(triangle->m_y), end(triangle->m_y));

	// Isolation des pixels contenus dans le rectangle (par leurs indices)
	// +2 a chaque indice pour rajouter une marge d'erreur du au floor()
	// note: +1 serait suffisant mais je laisse +2 par securite
	int i_min = this->check_value(floor((temp_x_min - m_x_min)/m_h_x)-2,0,m_width);
	int i_max = this->check_value(floor((temp_x_max - m_x_min)/m_h_x)+2,0,m_width);
	int j_min = this->check_value(floor((temp_y_min - m_y_min)/m_h_y)-2,0,m_height);
	int j_max = this->check_value(floor((temp_y_max - m_y_min)/m_h_y)+2,0,m_height);

	// Pour chaque pixels isoles
	for (int i=i_min;i<=i_max;i++)
	{
		for (int j=j_min;j<=j_max;j++)
		{
			if (triangle->is_in_triangle(m_x_min + i*m_h_x + 0.5*m_h_x, m_y_min + j*m_h_y + 0.5*m_h_y)) // Si le centre est dans le triangle
			{
				m_grille[i][j] = triangle->m_mean_z; // Actualisation 
			}
		}
	}
}