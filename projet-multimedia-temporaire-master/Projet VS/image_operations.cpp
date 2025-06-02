#include "image_operations.h"
#include <iostream>

cv::Mat ImageOperations::Dilatation(const cv::Mat& image, int dilationSize) {
    // The kernel is the pattern we use for modification. Let's consider it's the "strength" of the modification
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(dilationSize, dilationSize));
    cv::Mat dst;

    // OpenCV dilation function, we input an image and the kernel, it outputs the modified picture.
    cv::dilate(image, dst, kernel);
    return dst;
}

cv::Mat ImageOperations::Erosion(const cv::Mat& image, int erosionSize) {
    // The kernel is the pattern we use for modification. Let's consider it's the "strength" of the modification
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(erosionSize, erosionSize));
    cv::Mat dst;

    // OpenCV erosion function, we input an image and the kernel, it outputs the modified picture.
    cv::erode(image, dst, kernel);
    return dst;
}

cv::Mat ImageOperations::DimensionResizing(const cv::Mat& image, int xDimension, int yDimension) {
    cv::Mat dst;
    cv::resize(image, dst, cv::Size(xDimension, yDimension));
    return dst;
}

cv::Mat ImageOperations::FactorResizing(const cv::Mat& image, double factor) {
    cv::Mat dst;
    cv::resize(image, dst, cv::Size(), factor, factor);
    return dst;
}

cv::Mat ImageOperations::LightenDarken(const cv::Mat& image, float factor) {
    if (factor < -1.0f || factor > 1.0f) {
        std::cerr << "Invalid factor. Please enter a value between -1.0 and 1.0." << std::endl;
        return image;
    }

    cv::Mat hsvImage;
    cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV); // Convert the image in HSV (easier to work on it here)

    // We change luminosity for every pixel:
    for (int i = 0; i < hsvImage.rows; ++i) {
        for (int j = 0; j < hsvImage.cols; ++j) {
            cv::Vec3b& pixel = hsvImage.at<cv::Vec3b>(i, j);
            int luminosity = pixel[2];
            luminosity = cv::saturate_cast<uchar>(luminosity + factor * 255.0f);
            pixel[2] = luminosity;
        }
    }

    cv::Mat result;
    cv::cvtColor(hsvImage, result, cv::COLOR_HSV2BGR); // Convert the image back to BGR
    return result;
}

cv::Mat ImageOperations::CannyEdgeDetection(const cv::Mat& image, int lowerThreshold, int upperThreshold, float blurIntensity) {
    
    // Convert in greyscale
    cv::Mat gray, blurred, edges;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    // Reduce noise with blur
    cv::GaussianBlur(gray, blurred, cv::Size(5, 5), blurIntensity);

    // Contours detection
    cv::Canny(blurred, edges, lowerThreshold, upperThreshold);
    
    // Convert the image back to BGR for canvas compatibility
    cv::Mat edgesColor;
    cv::cvtColor(edges, edgesColor, cv::COLOR_GRAY2BGR);

    return edgesColor;
}

cv::Mat ImageOperations::FaceDetection(const cv::Mat& image, const std::string& filename) {
    cv::CascadeClassifier face_cascade;
    if (!face_cascade.load(filename)) {
        std::cerr << "Error loading cascade file\n";
        return image;
    }

    std::vector<cv::Rect> faces;
    face_cascade.detectMultiScale(image, faces, 1.1, 3, 0, cv::Size(30, 30));
    cv::Mat result = image.clone();
    for (const auto& face : faces) {
        cv::rectangle(result, face, cv::Scalar(255, 0, 0), 2);
    }
    cv::imshow("Faces detected", result);
    cv::waitKey(0);
    cv::destroyAllWindows();
    return result;
}

void ImageOperations::VideoManipulation(const std::string& filename) {
    cv::VideoCapture cap(filename);
    if (!cap.isOpened()) {
        std::cerr << "Error opening video file" << std::endl;
        return;
    }

    cv::Mat frame;
    while (true) {
        cap >> frame;
        if (frame.empty()) break;
        cv::imshow("Vidéo", frame);
        if (cv::waitKey(30) == 0) break;
    }

    cap.release();
    cv::destroyAllWindows();
}

cv::Mat ImageOperations::BackgroundSeparation(const cv::Mat& image) {
    cv::Mat gris, flou, binaire;
    cv::cvtColor(image, gris, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gris, flou, cv::Size(5, 5), 0);
    cv::adaptiveThreshold(flou, binaire, 255, cv::ADAPTIVE_THRESH_MEAN_C,
        cv::THRESH_BINARY_INV, 11, 2);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binaire, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    cv::Mat mask = cv::Mat::zeros(image.size(), CV_8UC1);
    for (const auto& contour : contours) {
        if (cv::contourArea(contour) > 500) {
            cv::drawContours(mask, std::vector<std::vector<cv::Point>>{contour}, -1, 255, cv::FILLED);
        }
    }

    cv::Mat resultat;
    image.copyTo(resultat, mask);
    return resultat;
}

cv::Mat ImageOperations::StitchImages(const cv::Mat& image1, const cv::Mat& image2) {
    std::vector<cv::Mat> images = { image1, image2 };
    cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(cv::Stitcher::PANORAMA);

    cv::Mat pano;
    cv::Stitcher::Status status = stitcher->stitch(images, pano);

    if (status != cv::Stitcher::OK) {
        std::cerr << "Error when assembling images: error code = " << int(status) << std::endl;
        return image1;
    }

    return pano;
}
