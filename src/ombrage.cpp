/*
Projet: Create_Raster
Auteur: Damien ESNAULT ROB23
Fichier: ombrage.cpp
Description:

Decrit le fonctionnement d'un objet Ombrage. Un objet Ombrage permets de stocker les valeurs d'ombrage associees a chaque pixel. Pour mettre a jour 
ces valeurs il faut executer la methode create_ombrage().

Un Ombrage est notamment constitue d'un tableau m_tab_ombrage qui contient les valeurs d'ombrage pour chaque pixel. Après initialisation, il suffit 
d'appeler create_ombrage() pour actualiser les valeurs d'ombrage.  
*/

// Librairies
#include "ombrage.h"
#include <iostream>
#include <math.h>  

using namespace std;


// Constructeur
Ombrage::Ombrage(int width, int height, double h_x, double h_y, double **grille)
{
	// Sauvegarde des valeurs
	m_width = width;
	m_height = height;
	m_h_x = h_x;
	m_h_y = h_y;
	// Creation du tableau d'ombrage
	m_tab_ombrage = new double*[width];
	for (int i=0;i<width;i++)
	{
		m_tab_ombrage[i] = new double[height];
		for (int j=0;j<height;j++)
		{
			m_tab_ombrage[i][j] = 0;
		}
	}
	// Copie du tableau des profondeurs
	m_grille_copy = new double*[width];
	for (int i=0;i<width;i++)
	{
		m_grille_copy[i] = new double[height];
		for (int j=0;j<height;j++)
		{
			m_grille_copy[i][j] = grille[i][j];
		}
	}

}


// Methode de suppression propre des tableaux dynamiques
void Ombrage::destroy()
{
	delete [] m_grille_copy;
	delete [] m_tab_ombrage;
}


// Methode actualisant le tableau des valeurs d'ombrage
void Ombrage::create_ombrage()
{
	/*
	Pour la theorie : https://pro.arcgis.com/fr/pro-app/latest/tool-reference/3d-analyst/how-hillshade-works.htm
	*/
	int compteur = 0;

	// Parametres de l'ombrage
	double azimuth = 315.0; //315.0
	double altitude = 45.0; //45.0

	// Calcul du zenith
	double zenith_rad = (90.0-altitude)*M_PI/180;
	// Calcul de l'azimut
	double azimuth_rad = 360.0 - azimuth + 90.0;
	if (azimuth_rad >=360)
	{
		azimuth_rad = azimuth_rad - 360;
	}
	azimuth_rad = azimuth_rad*M_PI/180;
	// Calcul du slope_rad
	for (int k=1; k<m_width-1; k++)
	{
		for (int j=1; j<m_height-1;j++)
		{
			double a = m_grille_copy[k-1][j-1];
			double b = m_grille_copy[k][j-1];
			double c = m_grille_copy[k+1][j-1];
			double d = m_grille_copy[k-1][j];
			double e = m_grille_copy[k][j];
			double f = m_grille_copy[k+1][j];
			double g = m_grille_copy[k-1][j+1];
			double h = m_grille_copy[k][j+1];
			double i = m_grille_copy[k+1][j+1];

			double dz_dx = ((c+i+(2*f))-(a+g+(2*d)))/(8*m_h_x);
			double dz_dy = ((g+i+(2*h))-(a+c+(2*b)))/(8*m_h_y);

			double slope_rad = atan(e*sqrt((dz_dx*dz_dx)+(dz_dy*dz_dy)));
			double aspect_rad;
			if (dz_dx!=0)
			{
				aspect_rad = atan2(dz_dy,-1*dz_dx);
				if(aspect_rad<0)
				{
					aspect_rad = 2*M_PI + aspect_rad;
				}
			}
			else
			{
				if (dz_dy>0)
				{
					aspect_rad = M_PI/2;
				}
				else
				{
					if (dz_dy<0)
					{
						aspect_rad = 2*M_PI - (M_PI/2);
					}
					else{
						aspect_rad = 0;
					}
				}
			}
			// Calcul de la valeur d'ombrage
			m_tab_ombrage[k][j] = 255.0 * ((cos(zenith_rad) * cos(slope_rad)) + (sin(zenith_rad) * sin(slope_rad) * cos(azimuth_rad - aspect_rad)));
			// Correction
			if (m_tab_ombrage[k][j]<0)
			{
				m_tab_ombrage[k][j] = 0;
			}
			if (m_tab_ombrage[k][j]>255)
			{
				m_tab_ombrage[k][j] = 255;
			}
			compteur++; // Un pixel de traite en plus
			cout<<"\rFulfil hillshade: "<< 100*(compteur)/((m_width-3)*(m_height-3))<<"%"; // Avancement du processus
		}
	}
	cout<<endl;
}