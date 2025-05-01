#include "image_function.h"
#include <opencv2/opencv.hpp>
#include <iostream>

// kernelSize nous permet de choisir la taille du noyau dans certaines fonctions

void dilateImage(cv::Mat& img, int kernelSize) {
    cv::Mat result;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelSize, kernelSize));
    cv::dilate(img, result, kernel);
    cv::imshow("Dilatation", result);
    cv::waitKey(0);
}

void erodeImage(cv::Mat& img, int kernelSize) {
    cv::Mat result;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelSize, kernelSize));
    cv::erode(img, result, kernel);
    cv::imshow("Erosion", result);
    cv::waitKey(0);
}

void resizeImage(cv::Mat& img, int width, int height) {
    cv::Mat result;
    cv::resize(img, result, cv::Size(width, height));
    cv::imshow("Image Redimensionnée", result);
    cv::waitKey(0);
}

void applyCannyEdgeDetection(cv::Mat& img) {
    cv::Mat edges;
    cv::Canny(img, edges, 100, 200);
    cv::imshow("Détection des bords (Canny)", edges);
    cv::waitKey(0);
}
