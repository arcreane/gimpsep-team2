#include "image_operations.h"
#include "utils.h"
#include <iostream>

void Dilatation(cv::Mat image, int dilationSize) {
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(dilationSize, dilationSize));
    cv::Mat dst;
    cv::dilate(image, dst, kernel);
    WindowManagement("Image dilat�e", dst);
}

void Erosion(cv::Mat image, int erosionSize) {
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(erosionSize, erosionSize));
    cv::Mat dst;
    cv::erode(image, dst, kernel);
    WindowManagement("Image �rod�e", dst);
}

void DimensionResizing(cv::Mat image, int xDimension, int yDimension) {
    cv::Mat dst;
    cv::resize(image, dst, cv::Size(xDimension, yDimension));
    WindowManagement("Image redimensionn�e", dst);
}

void FactorResizing(cv::Mat image, double factor) {
    cv::Mat dst;
    cv::resize(image, dst, cv::Size(), factor, factor);
    WindowManagement("Image redimensionn�e", dst);
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
    WindowManagement("Image �claircie/assombrie", result);
}


void CannyEdgeDetection(const cv::Mat& image) {
    int lowerThreshold, upperThreshold;
    float blurIntensity;

    std::cout << "Entrez le seuil inf�rieur : ";
    std::cin >> lowerThreshold;

    std::cout << "Entrez le seuil sup�rieur : ";
    std::cin >> upperThreshold;

    std::cout << "Entrez l'intensit� du flou : ";
    std::cin >> blurIntensity;

    // Convertir en niveaux de gris
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    // R�duire le bruit avec un flou
    cv::Mat blurred;
    cv::GaussianBlur(gray, blurred, cv::Size(5, 5), blurIntensity);

    // D�tection des contours
    cv::Mat edges;
    cv::Canny(blurred, edges, lowerThreshold, upperThreshold);

    WindowManagement("Contours (Canny)", edges);
}
