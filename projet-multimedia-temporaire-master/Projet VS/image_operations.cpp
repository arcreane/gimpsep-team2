#include "image_operations.h"
#include "utils.h"
#include <iostream>

//Arnaud
cv::Mat Dilatation(const cv::Mat& image, int dilationSize) {
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(dilationSize, dilationSize));
    cv::Mat dst;
    cv::dilate(image, dst, kernel);
    return dst;
}

//Arnaud
cv::Mat Erosion(cv::Mat image, int erosionSize) {
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(erosionSize, erosionSize));
    cv::Mat dst;
    cv::erode(image, dst, kernel);
    return dst;
}

//Rayane
cv::Mat DimensionResizing(cv::Mat image, int xDimension, int yDimension) {
    cv::Mat dst;
    cv::resize(image, dst, cv::Size(xDimension, yDimension));
    return dst;
}

//Wassim
cv::Mat FactorResizing(cv::Mat image, double factor) {
    cv::Mat dst;
    cv::resize(image, dst, cv::Size(), factor, factor);
    return dst;
}

//Alexandre
cv::Mat LightenDarken(const cv::Mat& image, float factor) {
    if (factor < -1.0f || factor > 1.0f) {
        std::cerr << "Facteur invalide. Veuillez entrer une valeur entre -1.0 et 1.0." << std::endl;
        return image;
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
    return result;
}

//Alexandre
cv::Mat CannyEdgeDetection(const cv::Mat& image, int lowerThreshold, int upperThreshold, float blurIntensity) {
    
    // Convertir en niveaux de gris
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    // Réduire le bruit avec un flou
    cv::Mat blurred;
    cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 2);

    // Détection des contours
    cv::Mat edges;
    cv::Canny(blurred, edges, 20, 80);

    // Re-conversion en BGR pour compatibilité avec le canvas
    cv::Mat edgesColor;
    cv::cvtColor(edges, edgesColor, cv::COLOR_GRAY2BGR);

    return edgesColor;
}

//Wassim
cv::Mat FaceDetection(const cv::Mat& image, const std::string& filename) {
    if (image.empty()) {
        return image;
    }

    cv::CascadeClassifier face_cascade;
    if (!face_cascade.load(filename)) {
        return image;
    }

    std::vector<cv::Rect> faces;
    face_cascade.detectMultiScale(image, faces, 1.1, 3, 0, cv::Size(30, 30));

    cv::Mat result = image.clone();
    for (const auto& face : faces) {
        cv::rectangle(result, face, cv::Scalar(255, 0, 0), 2);
    }

    cv::imshow("Visages détectés", result);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return result;
}


//Rayane
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

//Alexandre
cv::Mat BackgroundSeparation(const cv::Mat& image) {
    // Convert in grey
    cv::Mat gris;
    cv::cvtColor(image, gris, cv::COLOR_BGR2GRAY);

    // Blur to limit noise
    cv::Mat flou;
    cv::GaussianBlur(gris, flou, cv::Size(5, 5), 0);

    // Adaptive threshold
    cv::Mat binaire;
    cv::adaptiveThreshold(flou, binaire, 255, cv::ADAPTIVE_THRESH_MEAN_C,
        cv::THRESH_BINARY_INV, 11, 2);

    // Detect contours
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binaire, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Empty mask
    cv::Mat masque = cv::Mat::zeros(image.size(), CV_8UC1);

    // Draw bigger contours
    for (const auto& contour : contours) {
        if (cv::contourArea(contour) > 500) { // Adjust threshold here
            cv::drawContours(masque, std::vector<std::vector<cv::Point>>{contour}, -1, 255, cv::FILLED);
        }
    }

    // Aplpy mask to initial image
    cv::Mat resultat;
    image.copyTo(resultat, masque);

    return resultat;
}




//Rayane
cv::Mat StitchImages(const cv::Mat& image1, const cv::Mat& image2) {
    std::vector<cv::Mat> images = { image1, image2 };

    cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(cv::Stitcher::PANORAMA);

    cv::Mat pano;
    cv::Stitcher::Status status = stitcher->stitch(images, pano);

    if (status != cv::Stitcher::OK) {
        std::cerr << "Erreur lors de l'assemblage des images : code d'erreur = " << int(status) << std::endl;
        return image1;
    }

    return pano;
}



