#include "utils.h"

void WindowManagement(std::string windowName, cv::Mat dst) {
    cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
    cv::imshow(windowName, dst);
}