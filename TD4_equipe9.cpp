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

//#define GSL_SPAN_H return {};
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
	Pixel pixelTest0 = creerPixel(10, 30, 140);
	char couleurTest0 = retournerCouleurPixel(pixelTest0); //doit retourner 'Q'
	cout << boolalpha << "test TODO 0 creer pixel: " << (couleurTest0 == 'Q') << endl;

	Pixel pixelTest1 = creerPixel(0, 0, 0);
	char couleurTest1 = retournerCouleurPixel(pixelTest1); //doit retourner ' '
	cout << "test TODO 0 creer pixel: " << (couleurTest1 == ' ') << endl;

	Pixel pixelTest2 = creerPixel(255, 0, 0);
	char couleurTest2 = retournerCouleurPixel(pixelTest2); //doit retourner 'R'
	cout << "test TODO 0 creer pixel: " << (couleurTest2 == 'R') << endl;

	// Ajouter une couleur au pixel.  Vérifier dans le débogueur que ça donne bien la bonne somme pour les trois composantes de la couleur du pixel.
	ajouterCouleurPixel(pixelTest2, Pixel(1, 30, 140));
	int sommeComposantesTest = pixelTest2.tauxBleu + pixelTest2.tauxRouge + pixelTest2.tauxVert;
	cout << "test TODO 0 ajouter couleur pixel: " << (sommeComposantesTest == 255 + 30 + 140) << endl;
	
	// Creer une petite image.  Affecter une valeur à un des pixels.  Afficher l'image pour voir qu'il y a bien un pixel avec la valeur voulue.
	Image imageTest = creerImage("imageTest", 2, 2);
	ajouterConcentrationCouleurPrimaire(imageTest.pixels[1][1].tauxBleu, 100);
	cout << "test TODO 0 afficher image: " << endl;
	afficherImage(imageTest);
	
	// Creer un groupe vide d'images. 
	GroupeImages groupeImagesTest;
	groupeImagesTest.nImages = 0;
	//Ajouter l'image précédente deux fois, en modifiant son nom entre les deux. Vérifier qu'il indique bien qu'il y a une image après le premier ajout, qu'il y en a deux après le deuxième ajout, et que les images aux indices 0 et 1 sont bien l'image crée.
	ajouterImage(groupeImagesTest, imageTest);
	cout << "test TODO 0 ajouter image: " << (groupeImagesTest.nImages == 1) << endl;
	imageTest.nomImage = "imageTest2";
	ajouterImage(groupeImagesTest, imageTest);
	cout << "test TODO 0 ajouter image: " << (groupeImagesTest.nImages == 2) << endl;
	cout << "test TODO 0 ajouter image: " << (groupeImagesTest.images[0].nomImage == "imageTest") << endl;
	cout << "test TODO 0 ajouter image: " << (groupeImagesTest.images[1].nomImage == "imageTest2") << endl;


	// Chercher dans le groupe d'images les deux noms d'images et un nom autre, et vérifier que les indices retournés sont bien 0, 1 et 'aucun' (constante qui vaut -1).
	cout << "test TODO 0 chercher image par nom: " << (chercherImageParNom(groupeImagesTest, "imageTest") == 0) << endl;
	cout << "test TODO 0 chercher image par nom: " << (chercherImageParNom(groupeImagesTest, "imageTest2") == 1) << endl;
	cout << "test TODO 0 chercher image par nom: " << (chercherImageParNom(groupeImagesTest, "imageAutre") == aucun) << endl;
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
	Image imageVerte = groupeImages.images[indiceImageVerte];
	Pixel pixelBleu = creerPixel(0, 0, 255);
	doublerTaille(imageVerte, imageVerte.taille.largeur, pixelBleu);

	//TODO: 5 Doublee la taille des Image_Rouge en largeur, en ajoutant des pixels rouges.
	Image imageRouge = groupeImages.images[indiceImageRouge];
	Pixel pixelRouge = creerPixel(255, 0, 0);
	doublerTaille(imageRouge, imageRouge.taille.largeur, pixelRouge);

	//TODO: 6 Modifier la couleur du pixel (1,1) de l'Image_Rouge en augmentant la concetration du bleu de 50 unités et en diminuant la concentration du rouge de 255 unités.
	ajouterConcentrationCouleurPrimaire(imageRouge.pixels[1][1].tauxBleu, 50);
	ajouterConcentrationCouleurPrimaire(imageRouge.pixels[1][1].tauxRouge, -255);
	
	//TODO: 7 Modifier la couleur du pixel (2,1) de l'Image_Verte en augementant la concetration du bleu de 100 unités.
	ajouterConcentrationCouleurPrimaire(imageVerte.pixels[1][2].tauxBleu, 100);

	//TODO: 8 Afficher le groupe d'images.
	afficherGroupeImages(groupeImages);

	return 0;
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
	if (concentrationAModifier < minConcentrationCouleurPrimare)
		concentrationAModifier = minConcentrationCouleurPrimare;
	else if (concentrationAModifier > maxConcentrationCouleurPrimare)
		concentrationAModifier = maxConcentrationCouleurPrimare;
}


void ajouterCouleurPixel(Pixel& pixelAModifier, const Pixel& increment)
{
	//TODO: Ajoute, composante par composante, l'increment au pixelAModifier, en s'assurant que chaque concentration résultante est dans les bornes.
	ajouterConcentrationCouleurPrimaire(pixelAModifier.tauxRouge, increment.tauxRouge);
	ajouterConcentrationCouleurPrimaire(pixelAModifier.tauxVert, increment.tauxVert);
	ajouterConcentrationCouleurPrimaire(pixelAModifier.tauxBleu, increment.tauxBleu);
}


char retournerCouleurPixel(const Pixel& pixel)
{
	//TODO: Retourner  'R' (respectivement 'V' et 'B') pour un pixel contenant uniquement du rouge (respectivement uniquement du vert et uniquement du bleu); ' ' pour un pixel contenant aucun rouge, ni vert, ni bleu; 'Q' pour tout autre couleur de pixel.
	if (pixel.tauxRouge !=0 && pixel.tauxVert == 0 && pixel.tauxBleu == 0) return 'R';
	if (pixel.tauxRouge == 0 && pixel.tauxVert != 0 && pixel.tauxBleu == 0) return 'V';
	if (pixel.tauxRouge == 0 && pixel.tauxVert == 0 && pixel.tauxBleu != 0) return 'B';
	if (pixel.tauxRouge == 0 && pixel.tauxVert == 0 && pixel.tauxBleu == 0) return ' ';
	if (pixel.tauxRouge != 0 && pixel.tauxVert != 0 && pixel.tauxBleu != 0) return 'Q';
}

#pragma endregion //}


#pragma region "Fonctions pour les images" //{

Image creerImage(const string& nomImage, unsigned tailleEnLargeur, unsigned tailleEnHauteur)
{
	//TODO: Creer une image à partir des informations en paramètres et la retourner.
	Image image;
	image.nomImage = nomImage;
	image.taille.hauteur = tailleEnHauteur;
	image.taille.largeur = tailleEnLargeur;

	for (unsigned y = 0; y < tailleMaxImage; ++y) {
		for (unsigned x = 0; x < tailleMaxImage; ++x) {
			image.pixels[y][x] = { 0, 0, 0 }; // Noir par défaut
		}
	}

	return image;
}


void doublerTaille(Image& image, int doitDoublerQuelleDimension, const Pixel& couleur)
{
	// TODO: Ajoute, composante par composante, l'increment au pixelAModifier, en s'assurant que chaque concentration résultante est dans les bornes.
	if (doitDoublerQuelleDimension == dimensionLargeur) {
		unsigned nouvelleLargeur = image.taille.largeur * 2;
		if (nouvelleLargeur > tailleMaxImage)
			nouvelleLargeur = tailleMaxImage;

		for (unsigned y = 0; y < image.taille.largeur; ++y) {
			for (unsigned x = image.taille.largeur; x < nouvelleLargeur; ++x) {
				image.pixels[y][x] = couleur;
			}
		}
		image.taille.largeur = nouvelleLargeur;
	}

	else if (doitDoublerQuelleDimension == dimensionHauteur) {
		unsigned nouvelleHauteur = image.taille.hauteur * 2;
		if (nouvelleHauteur > tailleMaxImage)
			nouvelleHauteur = tailleMaxImage;

		for (unsigned y = image.taille.hauteur; y < nouvelleHauteur; ++y) {
			for (unsigned x = 0; x < image.taille.largeur; ++x) {
				image.pixels[y][x] = couleur;
			}
		}
		image.taille.hauteur = nouvelleHauteur;
	}
}



void affecterPixel(Image& image, unsigned positionEnLargeur, unsigned positionEnHauteur, const Pixel& pixel)
{
	//TODO: Remplacer le pixel de l'image à la position indiquée par positionEnLargeur et positionEnHauteur par la valeur du pixel passé en paramètre.
	image.pixels[positionEnHauteur][positionEnHauteur].tauxRouge = pixel.tauxRouge;
	image.pixels[positionEnHauteur][positionEnHauteur].tauxVert = pixel.tauxVert;
	image.pixels[positionEnHauteur][positionEnHauteur].tauxBleu = pixel.tauxBleu;
}


void afficherImage(const Image& image)
{
	//TODO: Afficher l'image au complet, avec entête pour son nom, chaque pixel étant représenté par un caractère; le caractère à utiliser est indiqué dans la fonction retournerCouleurPixel.  Vous trouverez un exemple d'affichage dans l'enoncé.
	//TODO: Cette fonction ne doit avoir aucun ancien "for".
	cout << string(tailleEntete, caractereEnteteImage) << endl;
	cout << "Nom de l'image: " << image.nomImage << endl;
	cout << string(tailleEntete, caractereEnteteImage) << endl;

	span<const Pixel[tailleMaxImage]> lignes(image.pixels, image.taille.hauteur);

	for (const auto& ligne : lignes) {
		span<const Pixel> pixelsLigne(ligne, image.taille.largeur);
		for (const auto& pixel : pixelsLigne) {
			cout << retournerCouleurPixel(pixel);
		}
		cout << endl;
	}
	
	cout << string(tailleEntete, caractereEnteteImage) << endl;
	
}

#pragma endregion //}


#pragma region "Fonctions pour les groupes d'images" //{

void ajouterImage(GroupeImages& groupeImages, const Image& image)
{
	//TODO: Si il y a encore de l'espace dans groupeImages, ajouter l'image en paramètre.
	if (groupeImages.nImages + 1 < tailleMaxGroupeImages) {
		groupeImages.images[groupeImages.nImages] = image;
		groupeImages.nImages += 1;
	}
}


void modifierType(GroupeImages& groupeImages, const string& type)
{
	//TODO: Modifier le type du groupeImages par le type en paramètre.
	groupeImages.type = type;
}


int chercherImageParNom(const GroupeImages& groupeImages, const string& nomImage)
{
	//TODO: Retourner l'indice de l'image du groupeImages dont le nom est celui donné par le paramètre nomImage.  Retourner la constante 'aucun' si ce nom n'est pas trouvé.
	for (int i = 0; i < groupeImages.nImages; ++i) {
		if (groupeImages.images[i].nomImage == nomImage) {
			return i;
		}
	}
	return aucun;
}


void afficherGroupeImages(const GroupeImages& groupeImages)
{
	//TODO: Afficher le groupe d'images, soit l'entête pour le type d'images et toutes les images du groupe. (Vous trouverez dans l'énoncé un exemple)
	//TODO: Cette fonction ne doit avoir aucun ancien "for".
	cout << string(tailleEntete, caractereEnteteGroupe) << endl;
	cout << "Type du groupe d'images : " << groupeImages.type << endl;
	cout << string(tailleEntete, caractereEnteteGroupe) << endl;

	span<const Image> images(groupeImages.images, groupeImages.nImages);
	for (const auto& image : images)
		afficherImage(image);
}


GroupeImages lireFichier(const string& nomFichier)
{
	//TODO: Récuperer (lire), à partir du fichier dont le nom est donné en paramètre, l'ensemble des images stockés, puis retourner un groupe d'images contenant ces images. Le type de ce groupe d'images n'est pas spécifié.
	GroupeImages groupe;
	groupe.nImages = 0;
	groupe.type = "";

	ifstream fichier(nomFichier);
	if (!fichier)
		return groupe;

	while (true) {
		string nomImage;
		unsigned largeur, hauteur;
		if (!(fichier >> nomImage >> largeur >> hauteur))
			break;

		Image image = creerImage(nomImage, largeur, hauteur);

		for (unsigned y = 0; y < hauteur; ++y) {
			for (unsigned x = 0; x < largeur; ++x) {
				int r, v, b;
				fichier >> r >> v >> b;
				image.pixels[y][x] = creerPixel(r, v, b);
			}
		}

		if (groupe.nImages < tailleMaxGroupeImages)
			groupe.images[groupe.nImages++] = image;
	}

	return groupe;
}

#pragma endregion //}

#pragma endregion //}

#pragma endregion //}
