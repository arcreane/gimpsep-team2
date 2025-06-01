#pragma once

#include <opencv2/opencv.hpp>
#include <string>

// Name of the main window 
static const std::string MAIN_WINDOW_NAME = "Interface";

/// Displays directly in the main window without creating a new one.
inline void WindowManagement(const std::string& /*windowTitle*/, const cv::Mat& img) {
    // We always base ourselves on MAIN_WINDOW_NAME
    cv::imshow(MAIN_WINDOW_NAME, img);

    // We allow a very short delay for the image to display,
    // but without blocking (0 ms). The Interface display loop
    // continues to run.
    cv::waitKey(1);
}
