#include "image_operations.h"
#include "utils.h"
#include <iostream>

void Dilatation(cv::Mat image, int dilationSize) {
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(dilationSize, dilationSize));
    cv::Mat dst;
    cv::dilate(image, dst, kernel);
    WindowManagement("Image dilatée", dst);
}

void Erosion(cv::Mat image, int erosionSize) {
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(erosionSize, erosionSize));
    cv::Mat dst;
    cv::erode(image, dst, kernel);
    WindowManagement("Image érodée", dst);
}

void DimensionResizing(cv::Mat image, int xDimension, int yDimension) {
    cv::Mat dst;
    cv::resize(image, dst, cv::Size(xDimension, yDimension));
    WindowManagement("Image redimensionnée", dst);
}

void FactorResizing(cv::Mat image, double factor) {
    cv::Mat dst;
    cv::resize(image, dst, cv::Size(), factor, factor);
    WindowManagement("Image redimensionnée", dst);
}

void LightenDarken(const cv::Mat& image, float factor) {
    if (factor < -1.0f || factor > 1.0f) {
        std::cerr << "Facteur invalide. Veuillez entrer une valeur entre -1.0 et 1.0." << std::endl;
        return;
    }

    cv::Mat hsvImage;
    cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);

    for (int i = 0; i < hsvImage.rows; ++i) {
        for (int j = 0; j < hsvImage.cols; ++j) {
            cv::Vec3b& pixel = hsvImage.at<cv::Vec3b>(i, j);
            int luminosity = pixel[2];
            luminosity = cv::saturate_cast<uchar>(luminosity + factor * 255.0f);
            pixel[2] = luminosity;
        }
    }

    cv::Mat result;
    cv::cvtColor(hsvImage, result, cv::COLOR_HSV2BGR);
    WindowManagement("Image éclaircie/assombrie", result);
}


void CannyEdgeDetection(const cv::Mat& image) {
    int lowerThreshold, upperThreshold;
    float blurIntensity;

    std::cout << "Entrez le seuil inférieur : ";
    std::cin >> lowerThreshold;

    std::cout << "Entrez le seuil supérieur : ";
    std::cin >> upperThreshold;

    std::cout << "Entrez l'intensité du flou : ";
    std::cin >> blurIntensity;

    // Convertir en niveaux de gris
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    // Réduire le bruit avec un flou
    cv::Mat blurred;
    cv::GaussianBlur(gray, blurred, cv::Size(5, 5), blurIntensity);

    // Détection des contours
    cv::Mat edges;
    cv::Canny(blurred, edges, lowerThreshold, upperThreshold);

    WindowManagement("Contours (Canny)", edges);
}

void FaceDetection(const cv::Mat& image) {
    cv::CascadeClassifier face_cascade;
	// Todo: Need to be updated to the correct path of the haarcascade file
    if (!face_cascade.load("C:/Users/wfrio/Desktop/Projet VS/x64/Debug/haarcascade_frontalface_default.xml")) {
        std::cerr << "Erreur lors du chargement du fichier cascade\n";
        return;
    }

    std::vector<cv::Rect> faces;
    face_cascade.detectMultiScale(image, faces, 1.1, 3, 0, cv::Size(30, 30));
    cv::Mat result = image.clone();
    for (const auto& face : faces) {
        cv::rectangle(result, face, cv::Scalar(255, 0, 0), 2);
    }
    WindowManagement("Détection de visage", result);
}


void VideoManipulation(const std::string& filename) {
	cv::VideoCapture cap(filename);
	if (!cap.isOpened()) {
		std::cerr << "Erreur lors de l'ouverture de la vidéo." << std::endl;
		return;
	}
	cv::Mat frame;
	while (true) {
		cap >> frame;
		if (frame.empty()) break;
		// Afficher le cadre
		cv::imshow("Vidéo", frame);
		if (cv::waitKey(30) == 0) break;
	}
	cap.release();
	cv::destroyAllWindows();
}

void BackgroundSeparation(const cv::Mat& image) {
   cv::Mat gray, blurred, edges;

   cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

   cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 0);

   cv::Canny(blurred, edges, 50, 150);

   WindowManagement("Séparation d'arrière-plan", edges);
}