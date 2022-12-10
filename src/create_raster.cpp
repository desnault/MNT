/*
Projet: Create_Raster
Auteur: Damien ESNAULT ROB23
Fichier: create_raster.cpp
Description:
Programme permettant de realiser une MNT a partir d'un fichier donne en argument
*/

// Librairies
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <vector>

#include "triangle.h"
#include "fichier.h"
#include "grille.h"
#include "ombrage.h"
#include "delaunator.hpp"

using namespace std;


// Colormap Haxby en 256 valeurs
// Il suffit de se servir de la valeur pgm comme indice pour recuper le bon tuple rgb dans cette table
double tab_colomap_haxby [256][3]=
{{0.1451,0.22353,0.68627},{0.14556,0.23429,0.69796},{0.14602,0.24506,0.70965},{0.14648,0.25582,0.72134},{0.14694,0.26659,0.73303},{0.1474,0.27735,0.74471},{0.14787,0.28812,0.7564},{0.14833,0.29889,0.76809},{0.14879,0.30965,0.77978},{0.14925,0.32042,0.79146},{0.14971,0.33118,0.80315},{0.15017,0.34195,0.81484},{0.15063,0.35271,0.82653},{0.1511,0.36348,0.83822},{0.15156,0.37424,0.8499},{0.15202,0.38501,0.86159},{0.15248,0.39577,0.87328},{0.15294,0.40654,0.88497},{0.1534,0.4173,0.89666},{0.15386,0.42807,0.90834},{0.15433,0.43883,0.92003},{0.15479,0.4496,0.93172},{0.15525,0.46036,0.94341},{0.15571,0.47113,0.95509},{0.15617,0.48189,0.96678},{0.15663,0.49266,0.97847},{0.15763,0.50288,0.98462},{0.15917,0.51257,0.98524},{0.16071,0.52226,0.98585},{0.16225,0.53195,0.98647},{0.16378,0.54164,0.98708},{0.16532,0.55133,0.9877},
{0.16686,0.56101,0.98831},{0.1684,0.5707,0.98893},{0.16993,0.58039,0.98954},{0.17147,0.59008,0.99016},{0.17301,0.59977,0.99077},{0.17455,0.60946,0.99139},{0.17609,0.61915,0.992},{0.17762,0.62884,0.99262},{0.17916,0.63852,0.99323},{0.1807,0.64821,0.99385},{0.18224,0.6579,0.99446},{0.18378,0.66759,0.99508},{0.18531,0.67728,0.99569},{0.18685,0.68697,0.99631},{0.18839,0.69666,0.99692},{0.18993,0.70634,0.99754},{0.19146,0.71603,0.99815},{0.193,0.72572,0.99877},{0.19454,0.73541,0.99938},{0.19608,0.7451,1},{0.20469,0.75202,1.},{0.2133,0.75894,1.},{0.22191,0.76586,1.},{0.23053,0.77278,1.},{0.23914,0.7797,1.},{0.24775,0.78662,1.},{0.25636,0.79354,1.},{0.26498,0.80046,1.},{0.27359,0.80738,1.},{0.2822,0.8143,1.},{0.29081,0.82122,1.},{0.29942,0.82814,1.},{0.30804,0.83506,1.},{0.31665,0.84198,1.},{0.32526,0.8489,1.},{0.33387,0.85582,1.},
{0.34248,0.86275,1.},{0.3511,0.86967,1.},{0.35971,0.87659,1.},{0.36832,0.88351,1.},{0.37693,0.89043,1.},{0.38554,0.89735,1.},{0.39416,0.90427,1.},{0.40277,0.91119,1.},{0.41138,0.91811,1.},{0.41815,0.92165,0.99377},{0.42307,0.9218,0.98131},{0.42799,0.92195,0.96886},{0.43291,0.92211,0.9564},{0.43783,0.92226,0.94394},{0.44275,0.92241,0.93149},{0.44767,0.92257,0.91903},{0.4526,0.92272,0.90657},{0.45752,0.92288,0.89412},{0.46244,0.92303,0.88166},{0.46736,0.92318,0.8692},{0.47228,0.92334,0.85675},{0.4772,0.92349,0.84429},{0.48212,0.92364,0.83183},{0.48704,0.9238,0.81938},{0.49196,0.92395,0.80692},{0.49689,0.92411,0.79446},{0.50181,0.92426,0.78201},{0.50673,0.92441,0.76955},{0.51165,0.92457,0.75709},{0.51657,0.92472,0.74464},{0.52149,0.92488,0.73218},{0.52641,0.92503,0.71972},{0.53133,0.92518,0.70727},{0.53626,0.92534,0.69481},
{0.54118,0.92549,0.68235},{0.55148,0.92841,0.68051},{0.56178,0.93133,0.67866},{0.57209,0.93426,0.67682},{0.58239,0.93718,0.67497},{0.5927,0.9401,0.67313},{0.603,0.94302,0.67128},{0.6133,0.94594,0.66943},{0.62361,0.94887,0.66759},{0.63391,0.95179,0.66574},{0.64421,0.95471,0.6639},{0.65452,0.95763,0.66205},{0.66482,0.96055,0.66021},{0.67512,0.96348,0.65836},{0.68543,0.9664,0.65652},{0.69573,0.96932,0.65467},{0.70604,0.97224,0.65283},{0.71634,0.97516,0.65098},{0.72664,0.97809,0.64913},{0.73695,0.98101,0.64729},{0.74725,0.98393,0.6454},{0.75755,0.98685,0.6436},{0.76786,0.98977,0.64175},{0.77816,0.9927,0.63991},{0.78847,0.99562,0.63806},{0.79877,0.99854,0.63622},{0.80661,0.99854,0.63214},{0.812,0.99562,0.62584},{0.81738,0.9927,0.61953},{0.82276,0.98977,0.61323},{0.82814,0.98685,0.60692},{0.83353,0.98393,0.60062},{0.83891,0.98101,0.59431},
{0.84429,0.97809,0.588},{0.84967,0.97516,0.5817},{0.85506,0.97224,0.57539},{0.86044,0.96932,0.56909},{0.86582,0.9664,0.56278},{0.8712,0.96348,0.55648},{0.87659,0.96055,0.55017},{0.88197,0.95763,0.54387},{0.88735,0.95471,0.53756},{0.89273,0.95179,0.53126},{0.89812,0.94887,0.52495},{0.9035,0.94594,0.51865},{0.90888,0.94302,0.51234},{0.91426,0.9401,0.50604},{0.91965,0.93718,0.49973},{0.92503,0.93426,0.49343},{0.93041,0.93133,0.48712},{0.93579,0.92841,0.48082},{0.94118,0.92549,0.47451},{0.94348,0.91826,0.46928},{0.94579,0.91103,0.46405},{0.9481,0.90381,0.45882},{0.9504,0.89658,0.45359},{0.95271,0.88935,0.44837},{0.95502,0.88212,0.44314},{0.95732,0.87489,0.43791},{0.95963,0.86767,0.43268},{0.96194,0.86044,0.42745},{0.96424,0.85321,0.42222},{0.96655,0.84598,0.41699},{0.96886,0.83875,0.41176},{0.97116,0.83153,0.40654},{0.97347,0.8243,0.40131},
{0.97578,0.81707,0.39608},{0.97809,0.80984,0.39085},{0.98039,0.80261,0.38562},{0.9827,0.79539,0.38039},{0.98501,0.78816,0.37516},{0.98731,0.78093,0.36993},{0.98962,0.7737,0.36471},{0.99193,0.76647,0.35948},{0.99423,0.75925,0.35425},{0.99654,0.75202,0.34902},{0.99885,0.74479,0.34379},{1.,0.73902,0.33972},{1.,0.73472,0.33679},{1.,0.73041,0.33387},{1.,0.72611,0.33095},{1.,0.7218,0.32803},{1.,0.71749,0.32511},{1.,0.71319,0.32218},{1.,0.70888,0.31926},{1.,0.70458,0.31634},{1.,0.70027,0.31342},{1.,0.69596,0.3105},{1.,0.69166,0.30757},{1.,0.68735,0.30465},{1.,0.68304,0.30173},{1.,0.67874,0.29881},{1.,0.67443,0.29589},{1.,0.67013,0.29296},{1.,0.66582,0.29004},{1.,0.66151,0.28712},{1.,0.65721,0.2842},{1.,0.6529,0.28128},{1.,0.6486,0.27835},{1.,0.64429,0.27543},{1.,0.63998,0.27251},{1.,0.63568,0.26959},{1.,0.63137,0.26667},{1.,0.63522,0.27666},{1.,0.63906,0.28666},
{1.,0.64291,0.29666},{1.,0.64675,0.30665},{1.,0.6506,0.31665},{1.,0.65444,0.32664},{1.,0.65829,0.33664},{1.,0.66213,0.34664},{1.,0.66597,0.35663},{1.,0.66982,0.36663},{1.,0.67366,0.37662},{1.,0.67751,0.38662},{1.,0.68135,0.39662},{1.,0.6852,0.40661},{1.,0.68904,0.41661},{1.,0.69289,0.42661},{1.,0.69673,0.4366},{1.,0.70058,0.4466},{1.,0.70442,0.45659},{1.,0.70827,0.46659},{1.,0.71211,0.47659},{1.,0.71596,0.48658},{1.,0.7198,0.49658},{1.,0.72364,0.50657},{1.,0.72749,0.51657},{1.,0.73472,0.53095},{1.,0.74533,0.54971},{1.,0.75594,0.56847},{1.,0.76655,0.58724},{1.,0.77716,0.606},{1.,0.78777,0.62476},{1.,0.79839,0.64352},{1.,0.809,0.66228},{1.,0.81961,0.68105},{1.,0.83022,0.69981},{1.,0.84083,0.71857},{1.,0.85144,0.73733},{1.,0.86205,0.75609},{1.,0.87266,0.77486},{1.,0.88328,0.79362},{1.,0.89389,0.81238},{1.,0.9045,0.83114},{1.,0.91511,0.8499},{1.,0.92572,0.86867},
{1.,0.93633,0.88743},{1.,0.94694,0.90619},{1.,0.95755,0.92495},{1.,0.96817,0.94371},{1.,0.97878,0.96248},{1.,0.98939,0.98124},{1.,1.,1.}};


// Programme principal
int main(int argc, char *argv[]){

	// Recuperation des arguments donnes en entree
	string input_filename = argv[1];
	int image_width = stoi(argv[2]);
	
	// Affichage a l'utilisateur
	cout<<"Input filename: "<<input_filename<<endl;
	cout <<"Input image width: "<<image_width<<endl<<endl;;


	// ------------------------- Lecture et Projection des donnees ------------------------
	
	Fichier file = Fichier(input_filename); // Creation d'un objet Fichier
	file.parse_and_convert(); // Lancement du parsing

	// Stockage des valeurs min/max de chaque variable
	double x_min = *min_element(begin(file.tab_x), end(file.tab_x));
	double x_max = *max_element(begin(file.tab_x), end(file.tab_x));
	double y_min = *min_element(begin(file.tab_y), end(file.tab_y));
	double y_max = *max_element(begin(file.tab_y), end(file.tab_y));
	double z_min = *min_element(begin(file.tab_z), end(file.tab_z));
	double z_max = *max_element(begin(file.tab_z), end(file.tab_z));
	
	// Calcul de la hauteur du fichier en pixel
	int image_height = (image_width*(y_max-y_min))/(x_max-x_min);
	// Affichage a l'utilisateur
	cout<<"Calculated image height: "<<image_height<<endl<<endl;
	
	// ------------------------------------------------------------------------------------


	// --------------------------------   Triangulation   -------------------------------

	// Creation d'un vecteur pour mettre les coordonnees au bon format: (x0 y0 x1 y1 ...)
	vector<double> coords;
	for (size_t i=0;i<file.tab_x.size();i++)
	{
		coords.push_back(file.tab_x[i]);
		coords.push_back(file.tab_y[i]);
		cout<<"\rData Reading: "<<(i*100)/(file.tab_x.size()-1)<<"%";
	}
	cout<<endl;

	// Algorithme de triangulation
	cout<<"Delaunator algorithm ..."<<endl;
	delaunator::Delaunator d(coords);
	cout<<"Completed"<<endl;

	// Affichage du nombre de triangles crees a l'utilisateur
	cout<<"Number of triangle calculated: "<<d.triangles.size()/3<<endl;

	// ----------------------------------------------------------------------------------
	

	// --------------------------------   Passage sous forme d'objet   -------------------------------
	/*
	Passage des triangles issus de la triangulation en objet Triangle qui seront plus facile a utiliser
	*/

	int compteur = 0;

	// Creation d'un vecteur pour stocker les triangles
	vector<Triangle> conteneur;
	for(size_t i = 0; i < d.triangles.size(); i+=3) // pour chaque triangle
	{	
		// Recuperation des coordonnes
		double point1[3] = {d.coords[2*d.triangles[i]],d.coords[2*d.triangles[i] + 1],file.tab_z[(d.triangles[i])]};
		double point2[3] = {d.coords[2*d.triangles[i+1]],d.coords[2*d.triangles[i+1] + 1],file.tab_z[(d.triangles[i+1])]};
		double point3[3] = {d.coords[2*d.triangles[i+2]],d.coords[2*d.triangles[i+2] + 1],file.tab_z[(d.triangles[i+2])]};

		// Verification de la taille du triangle (solutions aux envellopes convexes)
		double temp_tab_x[3] = {point1[0],point2[0],point3[0]};
		double temp_tab_y[3] = {point1[1],point2[1],point3[1]};
		double temp_x_min = *min_element(begin(temp_tab_x), end(temp_tab_x));
		double temp_x_max = *max_element(begin(temp_tab_x), end(temp_tab_x));
		double temp_y_min = *min_element(begin(temp_tab_y), end(temp_tab_y));
		double temp_y_max = *max_element(begin(temp_tab_y), end(temp_tab_y));
		if (!((temp_y_max-temp_y_min >= (y_max-y_min)/50) or (temp_x_max-temp_x_min >= (x_max-x_min)/50))) // si la taille est satisfaisante
		{
			conteneur.push_back(Triangle(point1,point2,point3)); // Creation de Triangle
			compteur++;
		}
        cout<<"\rCreating triangles : "<<(i*100/3)/((d.triangles.size()/3)-1)<<"%";
    }

    // Affichage du nombre de Triangle crees a l'utilisateur 
    cout<<endl;
    cout<<"Triangle created: "<<compteur<<endl<<endl;

    // ----------------------------------------------------------------------------------------------


    // --------------------------------  Recuperation des profondeurs  -------------------------------

    // Creation d'un objet Grille
   	cout<<"Creating height matrix ...";
   	Grille grille = Grille(image_width,image_height,x_min,x_max,y_min,y_max,z_min);
   	cout<<"\r"<<"Height matrix created successfuly"<<endl;

   	// la grille est alimentee avec les Triangles pour actualiser les valeurs de profondeurs
   	for (size_t i=0;i<conteneur.size();i++)
   	{
   		grille.add_triangle(&conteneur[i]);
   		cout<<"\rFulfil height matrix: "<<100*i/(conteneur.size()-1)<<"%";
   	}
   	cout<<endl<<endl;

   	// ----------------------------------------------------------------------------------------------


   	// --------------------------------  Creation de l'ombrage  -------------------------------

   	// Creation d'un objet ombrage
   	cout<<"Creating Hillshade ...";
   	Ombrage hillshade = Ombrage(image_width,image_height,grille.m_h_x,grille.m_h_y,grille.m_grille);
   	cout<<"\rHillshade Created Successfuly"<<endl;
   	// Calcul des valeurs d'ombrage
   	hillshade.create_ombrage();
   	cout<<endl;

   	// -------------------------------------------------------------------------------------------


   	// --------------------------------  Creation de l'image  -------------------------------
   	/*
	Pour transformer une profondeur en un pixel de couleur, la profondeur est d'abord transformee en valeur pgm [0,255].
	Cette valeur permet de recuperer les composantes RGB dans la table haxby et ces valeurs sont couple avec un ombrage
	avec un facteur 1-hillshade_factor
   	*/

   	// Ouverture d'un fichier ppm en binaire
   	cout<<"Creating image ..."<<endl;
   	string new_filename = input_filename.substr(0,input_filename.size()-4) + ".ppm";
   	cout<<"Output filename: "<< new_filename<<endl;
    ofstream ofs(new_filename.c_str(), ios_base::out | ios_base::binary);
    // Ecriture des parametres du fichier
    ofs << "P6" << endl << image_width << ' ' << image_height << endl << "255" << endl; 

    // Coefficients et variables
    double a = 255/(z_max-z_min),b = -1*(z_min/(z_max-z_min))*255; // Coefficient pour la conversion profondeur vers pgm
    int compteur_pixel = 0;
    int pixel_r_value; // Varaible pour la valeur r du pixel
    int pixel_g_value; // Varaible pour la valeur g du pixel
    int pixel_b_value; // Varaible pour la valeur b du pixel
    int pixel_value; // variable pour la valeur pgm du pixel
    float hillshade_factor = 0.7; // coefficient d'ombrage

    // Parcours des pixels
    for (int j=image_height-1; j>=0; j--)
    {
        for (int i=0; i<image_width; i++) 
        {	
        	// Si la valeur de profondeur est incorrecte (pixel hors des triangles)
        	if (grille.m_grille[i][j]<z_min)
        	{
        		// Le pixel est en noir
        		pixel_value = 0;
        		pixel_r_value = 0;
    			pixel_g_value = 0;
    			pixel_b_value = 0;
        	}
        	else // Sinon
        	{
        		pixel_value = a*grille.m_grille[i][j] + b; // on transforme la profondeur en pgm
        		// Correction de la valeur
        		if (pixel_value<0)
        		{
        			pixel_value = 0;
        		}
        		if (pixel_value>255)
        		{
        			pixel_value = 255;
        		}
        		// Calcul des composantes RGB avec l'ombrage 
        		pixel_r_value = (hillshade_factor*tab_colomap_haxby[pixel_value][0]*255) + ((1 - hillshade_factor)*hillshade.m_tab_ombrage[i][j]);
    			pixel_g_value = (hillshade_factor*tab_colomap_haxby[pixel_value][1]*255) + ((1 - hillshade_factor)*hillshade.m_tab_ombrage[i][j]);
    			pixel_b_value = (hillshade_factor*tab_colomap_haxby[pixel_value][2]*255) + ((1 - hillshade_factor)*hillshade.m_tab_ombrage[i][j]);
        	}
        	ofs << (char) pixel_r_value << (char) pixel_g_value << (char) pixel_b_value; // Ecriture dans le fichier
        	compteur_pixel++; 
        	// Affichage utilisateur
            cout<<"\rProcess: "<<100*compteur_pixel/(image_width*image_height)<<"%";
        }
    }
    // Fermeture propre du fichier
    ofs.close(); 
    cout<<endl;
    cout<<"Image created"<<endl<<endl;

    // -------------------------------------------------------------------------------------------

    // Suppresion des tableaux dynamiques
   	grille.destroy();
   	hillshade.destroy();

    cout<<"Programm is completed"<<endl;
	return EXIT_SUCCESS;
}