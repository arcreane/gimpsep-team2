#include "image_operations.h"
#include "utils.h"
#include <iostream>

cv::Mat Dilatation(const cv::Mat& image, int dilationSize) {
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(dilationSize, dilationSize));
    cv::Mat dst;
    cv::dilate(image, dst, kernel);
    return dst;
}


cv::Mat Erosion(cv::Mat image, int erosionSize) {
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(erosionSize, erosionSize));
    cv::Mat dst;
    cv::erode(image, dst, kernel);
    return dst;
}

cv::Mat DimensionResizing(cv::Mat image, int xDimension, int yDimension) {
    cv::Mat dst;
    cv::resize(image, dst, cv::Size(xDimension, yDimension));
    return dst;
}

cv::Mat FactorResizing(cv::Mat image, double factor) {
    cv::Mat dst;
    cv::resize(image, dst, cv::Size(), factor, factor);
    return dst;
}

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


cv::Mat FaceDetection(const cv::Mat& image, const std::string& filename) {
    cv::CascadeClassifier face_cascade;
    if (!face_cascade.load(filename)) {
        std::cerr << "Erreur lors du chargement du fichier cascade\n";
        return image;
    }

    std::vector<cv::Rect> faces;
    face_cascade.detectMultiScale(image, faces, 1.1, 3, 0, cv::Size(30, 30));
    cv::Mat result = image.clone();
    for (const auto& face : faces) {
        cv::rectangle(result, face, cv::Scalar(255, 0, 0), 2);
    }

    return result;
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

cv::Mat BackgroundSeparation(const cv::Mat& image) {
   cv::Mat gray, blurred, edges;

   cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

   cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 0);

   cv::Canny(blurred, edges, 50, 150);

   return edges;
}


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



