////////////////////////////////////////////////////////////////////////////////
/// TD4 Automne 2025, INF1005C.
/// \file    TD4.cpp
/// \authors Minh Beaulieu et Emma Métivier
////////////////////////////////////////////////////////////////////////////////

#pragma region "Inclusions" //{

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <span>  // Noter que si votre compilateur ne supporte pas bien C++20, on peut inclure "gsl/span" en C++17.
#include "cppitertools/range.hpp"

using namespace std;
#ifdef ITERBASE_HPP_
using namespace iter;
#endif
#ifdef GSL_SPAN_H
using namespace gsl;
#endif

#pragma endregion //}



#pragma region "Déclaration" //{

#define JUSTE_POUR_QUE_CA_COMPILE return {}; //TODO: Toutes les lignes qui indiquent JUSTE_POUR_QUE_CA_COMPILE sont à éliminer, à mesure que vous écrivez vos fonctions; elles sont là uniquement pour que le programme compile avant d'avoir écrit les fonctions qui doivent retourner une valeur.

#pragma region "Constantes" //{

static const unsigned tailleMaxImage = 30;         ///< La taille maximale d'une image, en pixels, dans chaque dimension.
static const unsigned tailleMaxGroupeImages = 20; ///< Le nombre maximum d'images dans un groupe.

static const int minConcentrationCouleurPrimare = 0;   ///< Valeur minimale que peut prendre une concentration de couleur dans un pixel d'image.
static const int maxConcentrationCouleurPrimare = 255; ///< Valeur maximale que peut prendre une concentration de couleur dans un pixel d'image.

static const unsigned tailleEntete = 50;        ///< Taille, en nombre de caractères, des lignes de séparation dans les affichages d'entêtes; voir exemple dans l'énoncé. 
static const char caractereEnteteGroupe = '*'; ///< Le caractère à afficher pour les entêtes de groupe.
static const char caractereEnteteImage  = '='; ///< Le caractère à afficher pour les entêtes d'image.

static const string nomFichierImages = "Images.txt"; ///< Le nom du fichier à charger.

static const int aucun = -1; ///< Constante à utiliser lorsqu'un indice n'existe pas, par exemple lorsqu'une recherche ne trouve pas.

// Constantes pour indiquer sur quelle dimension s'applique certaines fonctions (i.e. doublerTaille).
// Devrait être un enum mais est seulement matière au chapitre 8 du cours.
static const int dimensionLargeur = 1; ///< Choisit d'appliquer sur la largeur.
static const int dimensionHauteur = 2; ///< Choisit d'appliquer sur la hauteur.

#pragma endregion //}


#pragma region "Structures" //{

/// La couleur composée d'un pixel.
/// Les concentrations de chaque couleur primaire sont entre minConcentrationCouleurPrimare et maxConcentrationCouleurPrimare dans une image.
/// Ce type est aussi utilisé pour la différence entre deux compositions de couleur, et à ce moment les valeurs peuvent être aussi bien positives que négatives.
struct Pixel {
	int tauxRouge;           ///< La concentration de la couleur rouge dans le pixel.
	int tauxVert;            ///< La concentration de la couleur verte dans le pixel.
	int tauxBleu;            ///< La concentration de la couleur bleu  dans le pixel.
};


/// Pour la taille des images, en nombre de pixels.
struct Taille2D {
	unsigned largeur;
	unsigned hauteur;
};


/// Les informations et données d'une image.
/// Utilise actuellement l'allocation statique puisque l'allocation dynamique est vue seulement au chapitre 7 du cours.
struct Image {
	string nomImage;
	Taille2D taille;         ///< Le nombre de pixels de l'image en largeur et hauteur; chaque dimension doit toujours être entre 0 et tailleMaxImage.
	Pixel pixels[tailleMaxImage][tailleMaxImage];   ///< L'ensemble des pixels de l'image dans l'ordre pixels[positionEnHauteur][positionEnLargeur]. Attention que le pixel de coordonnées (x,y) est donc à pixels[y][x].
};


/// Ensemble d'images appartenant au meme type.
/// Si une même image doit être dans plus d'un groupe, les données doivent actuellement être copiées car les pointeurs sont vus au chapitre 7 du cours.
struct GroupeImages {
	string type;                          ///< Le type des images appartenant à ce groupe.
	unsigned nImages;                     ///< Le nombre d'images dans ce groupe; doit être entre 0 et tailleMaxGroupeImages.
	Image images[tailleMaxGroupeImages];  ///< L'ensemble des images du groupe.
};

#pragma endregion //}


#pragma region "Prototypes de fonctions" //{

//  Fonctions pour les pixels 

Pixel creerPixel(int tauxRouge, int tauxVert, int tauxBleu);

void ajouterConcentrationCouleurPrimaire(int& concentrationAModifier, int increment);

void ajouterCouleurPixel(Pixel& pixelAModifier, const Pixel& increment);

char retournerCouleurPixel(const Pixel& pixel);


// Fonctions pour les images

Image creerImage(const string& nomImage, unsigned tailleEnLargeur, unsigned tailleEnHauteur);

void doublerTaille(Image& image, int doitDoublerQuelleDimension, const Pixel& couleur);

void affecterPixel(Image& image, unsigned positionEnLargeur, unsigned positionEnHauteur, const Pixel& pixel);

void afficherImage(const Image& image);


// Fonctions pour les groupes d'images

void ajouterImage(GroupeImages& groupeImages, const Image& image);

void modifierType(GroupeImages& groupeImages, const string& type);

int chercherImageParNom(const GroupeImages& groupeImages, const string& nomImage);

void afficherGroupeImages(const GroupeImages& groupeImages);

GroupeImages lireFichier(const string& nomFichier);


#pragma endregion //}

#pragma endregion //}

void autresTests()
{
	//TODO: 0 Tester les fonctions à mesure qu'elles sont écrites.
	// Par exemple:
	// Créer un pixel d'une certaine couleur.  Vérifier dans le débogueur que la lettre associée avec cette couleur est la bonne.
	// Ajouter une couleur au pixel.  Vérifier dans le débogueur que ça donne bien la bonne somme pour les trois composantes de la couleur du pixel.
	// Creer une petite image.  Affecter une valeur à un des pixels.  Afficher l'image pour voir qu'il y a bien un pixel avec la valeur voulue.
	// Creer un groupe vide d'images.  Ajouter l'image précédente deux fois, en modifiant son nom entre les deux.  Vérifier qu'il indique bien qu'il y a une image après le premier ajout, qu'il y en a deux après le deuxième ajout, et que les images aux indices 0 et 1 sont bien l'image crée.
	// Chercher dans le groupe d'images les deux noms d'images et un nom autre, et vérifier que les indices retournés sont bien 0, 1 et 'aucun' (constante qui vaut -1).
}

int main()
{
	autresTests();

	//TODO: 1 Recuperer (lire) le group d'images à partir du fichier fourni, vérifier qu'il y a le bon nombre d'images, et que la première image a la bonne taille.
	GroupeImages groupeImages = lireFichier(nomFichierImages);
	if (groupeImages.nImages > tailleMaxGroupeImages && groupeImages.images[0].taille.hauteur > tailleMaxImage && groupeImages.images[0].taille.largeur > tailleMaxImage) { //si supérieurs aux constantes alors erreur
		exit(1);
	}

	//TODO: 2 Modifier le type de ce groupe d'images par "Images de tests".
	modifierType(groupeImages, "Images de tests");

	//TODO: 3 Chercher, dans ce groupe, les images ayant les nom "Image_Verte", "Image_Rouge", et "Blabla".
	int indiceImageVerte = chercherImageParNom(groupeImages, "Image_Verte");
	int indiceImageRouge = chercherImageParNom(groupeImages, "Image_Rouge");
	int indiceImageBlabla = chercherImageParNom(groupeImages, "Blabla");

	//TODO: 4 Doubler la taille de Image_Verte en hauteur, en ajoutant des pixels bleus.
	doublerTaille(groupeImages.images[indiceImageVerte], groupeImages.images[indiceImageVerte].taille.hauteur, groupeImages.images[indiceImageVerte].pixels[(groupeImages.images[indiceImageVerte].taille.hauteur)*2][(groupeImages.images[indiceImageVerte].taille.largeur) * 2]);

	//TODO: 5 Doublee la taille des Image_Rouge en largeur, en ajoutant des pixels rouges.
	doublerTaille(groupeImages.images[indiceImageRouge], groupeImages.images[indiceImageRouge].taille.largeur, groupeImages.images[indiceImageRouge].pixels[tailleMaxImage][tailleMaxImage]);

	//TODO: 6 Modifier la couleur du pixel (1,1) de l'Image_Rouge en augmentant la concetration du bleu de 50 unités et en diminuant la concentration du rouge de 255 unités.
	
	
	//TODO: 7 Modifier la couleur du pixel (2,1) de l'Image_Verte en augementant la concetration du bleu de 100 unités.
	

	//TODO: 8 Afficher le groupe d'images.
	afficherGroupeImages(groupeImages);
}

#pragma region "Définitions" //{

#pragma region "Fonctions" //{

#pragma region "Fonctions pour les pixels" //{

Pixel creerPixel(int tauxRouge, int tauxVert, int tauxBleu)
{
	int rouge = tauxRouge;
	int vert = tauxVert;
	int bleu = tauxBleu;
	return { rouge, vert, bleu };
}


void ajouterConcentrationCouleurPrimaire(int& concentrationAModifier, int increment)
{
	//TODO: Augmenter ou diminuer ( suivant le signe de increment ) la concentration de la couleur primaire passée en paramètre. La concentration finale doit être entre minConcentrationCouleurPrimare et maxConcentrationCouleurPrimare .
	concentrationAModifier += increment;
}


void ajouterCouleurPixel(Pixel& pixelAModifier, const Pixel& increment)
{
	//TODO: Ajoute, composante par composante, l'increment au pixelAModifier, en s'assurant que chaque concentration résultante est dans les bornes.
	if (pixelAModifier.tauxBleu + increment.tauxBleu >= minConcentrationCouleurPrimare && pixelAModifier.tauxBleu + increment.tauxBleu <= maxConcentrationCouleurPrimare) {
		ajouterConcentrationCouleurPrimaire(pixelAModifier.tauxBleu, increment.tauxBleu);
	}
	if (pixelAModifier.tauxVert + increment.tauxVert >= minConcentrationCouleurPrimare && pixelAModifier.tauxVert + increment.tauxVert <= maxConcentrationCouleurPrimare) {
		ajouterConcentrationCouleurPrimaire(pixelAModifier.tauxVert, increment.tauxVert);
	}
	if (pixelAModifier.tauxRouge + increment.tauxRouge >= minConcentrationCouleurPrimare && pixelAModifier.tauxRouge + increment.tauxRouge <= maxConcentrationCouleurPrimare) {
		ajouterConcentrationCouleurPrimaire(pixelAModifier.tauxRouge, increment.tauxRouge);
	}
}


char retournerCouleurPixel(const Pixel& pixel)
{
	//TODO: Retourner  'R' (respectivement 'V' et 'B') pour un pixel contenant uniquement du rouge (respectivement uniquement du vert et uniquement du bleu); ' ' pour un pixel contenant aucun rouge, ni vert, ni bleu; 'Q' pour tout autre couleur de pixel.
	if (pixel.tauxRouge == 0 && pixel.tauxVert != 0 && pixel.tauxBleu != 0) return 'R';
	if (pixel.tauxRouge != 0 && pixel.tauxVert == 0 && pixel.tauxBleu != 0) return 'V';
	if (pixel.tauxRouge != 0 && pixel.tauxVert != 0 && pixel.tauxBleu == 0) return 'B';
	if (pixel.tauxRouge == 0 && pixel.tauxVert == 0 && pixel.tauxBleu == 0) return ' ';
	if (pixel.tauxRouge != 0 && pixel.tauxVert != 0 && pixel.tauxBleu != 0) return 'Q';
}

#pragma endregion //}


#pragma region "Fonctions pour les images" //{

Image creerImage(const string& nomImage, unsigned tailleEnLargeur, unsigned tailleEnHauteur)
{
	//TODO: Creer une image à partir des informations en paramètres et la retourner.
	JUSTE_POUR_QUE_CA_COMPILE
}


void doublerTaille(Image& image, int doitDoublerQuelleDimension, const Pixel& couleur)
{
	//TODO: Doubler la taille de l'image suivant doitDoublerQuelleDimension en paramètre (voir les définitions de constantes pour les valeurs possibles de ce paramètre) en respectant la taille maximale de l'image et en ajoutant des pixels de la couleur spécifiée en paramètre.
}


void affecterPixel(Image& image, unsigned positionEnLargeur, unsigned positionEnHauteur, const Pixel& pixel)
{
	//TODO: Remplacer le pixel de l'image à la position indiquée par positionEnLargeur et positionEnHauteur par la valeur du pixel passé en paramètre.
}


void afficherImage(const Image& image)
{
	//TODO: Afficher l'image au complet, avec entête pour son nom, chaque pixel étant représenté par un caractère; le caractère à utiliser est indiqué dans la fonction retournerCouleurPixel.  Vous trouverez un exemple d'affichage dans l'enoncé.
	//TODO: Cette fonction ne doit avoir aucun ancien "for".
}

#pragma endregion //}


#pragma region "Fonctions pour les groupes d'images" //{

void ajouterImage(GroupeImages& groupeImages, const Image& image)
{
	//TODO: Si il y a encore de l'espace dans groupeImages, ajouter l'image en paramètre.
}


void modifierType(GroupeImages& groupeImages, const string& type)
{
	//TODO: Modifier le type du groupeImages par le type en paramètre.
}


int chercherImageParNom(const GroupeImages& groupeImages, const string& nomImage)
{
	//TODO: Retourner l'indice de l'image du groupeImages dont le nom est celui donné par le paramètre nomImage.  Retourner la constante 'aucun' si ce nom n'est pas trouvé.
	JUSTE_POUR_QUE_CA_COMPILE
}


void afficherGroupeImages(const GroupeImages& groupeImages)
{
	//TODO: Afficher le groupe d'images, soit l'entête pour le type d'images et toutes les images du groupe. (Vous trouverez dans l'énoncé un exemple)
	//TODO: Cette fonction ne doit avoir aucun ancien "for".
}


GroupeImages lireFichier(const string& nomFichier)
{
	//TODO: Récuperer (lire), à partir du fichier dont le nom est donné en paramètre, l'ensemble des images stockés, puis retourner un groupe d'images contenant ces images. Le type de ce groupe d'images n'est pas spécifié.
	JUSTE_POUR_QUE_CA_COMPILE
}

#pragma endregion //}

#pragma endregion //}

#pragma endregion //}
