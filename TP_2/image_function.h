#ifndef IMAGE_FUNCTION_H
#define IMAGE_FUNCTION_H

#include <opencv2/opencv.hpp>

// Déclarations des fonctions
void dilateImage(cv::Mat& img, int kernelSize);
void erodeImage(cv::Mat& img, int kernelSize);
void resizeImage(cv::Mat& img, int width, int height);
void applyCannyEdgeDetection(cv::Mat& img);

#endif
