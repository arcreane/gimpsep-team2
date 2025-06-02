#ifndef IMAGE_OPERATIONS_H
#define IMAGE_OPERATIONS_H

#include <opencv2/opencv.hpp>
#include <string>

class ImageOperations {
public:
    // Arnaud
    static cv::Mat Dilatation(const cv::Mat& image, int dilationSize);
    static cv::Mat Erosion(const cv::Mat& image, int erosionSize);

    // Wassim
    static cv::Mat DimensionResizing(const cv::Mat& image, int xDimension, int yDimension);
    static cv::Mat FactorResizing(const cv::Mat& image, double factor);

    // Alexandre
    static cv::Mat LightenDarken(const cv::Mat& image, float factor);
    static cv::Mat CannyEdgeDetection(const cv::Mat& image, int lowerThreshold, int upperThreshold, float blurIntensity);
    static cv::Mat BackgroundSeparation(const cv::Mat& image);

    // Wassim
    static cv::Mat FaceDetection(const cv::Mat& image, const std::string& cascadeFilename);

    // Rayane
    static void VideoManipulation(const std::string& filename);
    static cv::Mat StitchImages(const cv::Mat& image1, const cv::Mat& image2);
};

#endif // IMAGE_OPERATIONS_H
