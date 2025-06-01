#pragma once
#include <opencv2/opencv.hpp>

cv::Mat Dilatation(const cv::Mat& image, int dilationSize);
cv::Mat Erosion(cv::Mat image, int erosionSize);
cv::Mat DimensionResizing(cv::Mat image, int xDimension, int yDimension);
cv::Mat FactorResizing(cv::Mat image, double factor);
cv::Mat LightenDarken(const cv::Mat& image, float factor);
cv::Mat CannyEdgeDetection(const cv::Mat& image, int lowerThreshold, int upperThreshold, float blurIntensity);
cv::Mat FaceDetection(const cv::Mat& image, const std::string& filename);
void VideoManipulation(const std::string& filename);
cv::Mat BackgroundSeparation(const cv::Mat& image);
cv::Mat StitchImages(const cv::Mat& image1, const cv::Mat& image2);