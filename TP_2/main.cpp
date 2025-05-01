#include <iostream>
#include "image_function.h"

int main() {
    std::string imagePath;
	std::cout << "Entrez le chemin de l'image (sans guillemets)"; // On recupere le chemin de l'image
	std::getline(std::cin, imagePath);  // Permet de lire l'image meme si il y a des espaces

    // Chargement de l'image
	cv::Mat img = cv::imread(imagePath); // On charge l'image puis verifie si elle est vide
    if (img.empty()) {
        std::cerr << "Impossible de charger l'image. Vérifiez le chemin" << std::endl;
        return -1;
    }

    int choice; // On met en place un menu avec les fonctions disponibles pour nos images
    std::cout << "Choisissez une opération:\n";
    std::cout << "1. Dilatation\n";
    std::cout << "2. Erosion\n";
    std::cout << "3. Redimensionner\n";
    std::cout << "4. Détection des bords (Canny)\n";
    std::cout << "Votre choix: ";
    std::cin >> choice;

    int kernelSize;
	switch (choice) { // Et ici on va switcher entre les choix pour choisir la fonction souhaitée par l'utilisateur
    case 1:
        std::cout << "Entrez la taille du noyau de dilatation: ";
        std::cin >> kernelSize;
        dilateImage(img, kernelSize);
        break;
    case 2:
        std::cout << "Entrez la taille du noyau d'érosion: ";
        std::cin >> kernelSize;
        erodeImage(img, kernelSize);
        break;
    case 3:
        int width, height;
        std::cout << "Entrez la nouvelle largeur: ";
        std::cin >> width;
        std::cout << "Entrez la nouvelle hauteur: ";
        std::cin >> height;
        resizeImage(img, width, height);
        break;
    case 4:
        applyCannyEdgeDetection(img);
        break;
	default: // Si l'utilisateur ne rentre pas un choix parmi les propositions, alors on lui affiche un message d'erreur
        std::cerr << "Choix invalide." << std::endl;
        break;
    }

    return 0;
}
