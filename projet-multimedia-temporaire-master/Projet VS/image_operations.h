#pragma once
#include <opencv2/opencv.hpp>

void Dilatation(cv::Mat image, int dilationSize);
void Erosion(cv::Mat image, int erosionSize);
void DimensionResizing(cv::Mat image, int xDimension, int yDimension);
void FactorResizing(cv::Mat image, double factor);
void LightenDarken(const cv::Mat& image, float factor);
void CannyEdgeDetection(const cv::Mat& image);
void FaceDetection(const cv::Mat& image, const std::string& filename);
void VideoManipulation(const std::string& filename);
void BackgroundSeparation(const cv::Mat& image);
void StitchImages(const cv::Mat& image1, const cv::Mat& image2);