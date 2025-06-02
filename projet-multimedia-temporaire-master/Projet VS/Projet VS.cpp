#include "image_operations.h"
#include "button.h"
#include "interface.h"

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;
using namespace cv;

std::string filename;

// For checking if the file is a video
static bool isVideoFile(const std::string& path) {
    std::vector<std::string> videoExtensions = {".mp4", ".avi", ".mov", ".mkv", ".webm", ".flv"};
    std::string lowerPath = path;
    std::transform(lowerPath.begin(), lowerPath.end(), lowerPath.begin(), ::tolower);

    for (const auto& ext : videoExtensions) {
        if (lowerPath.size() >= ext.size() &&
            lowerPath.compare(lowerPath.size() - ext.size(), ext.size(), ext) == 0) {
            return true;
        }
    }
    return false;
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    std::cout << "Type in the path of the file (image or video): ";
    std::getline(std::cin, filename);

    if (isVideoFile(filename)) {
        std::cout << "Video file detected. Video interface opening..." << std::endl;
        ImageOperations::VideoManipulation(filename);
    } else {
        cv::Mat image = cv::imread(filename);
        if (image.empty()) {
            std::cerr << "Error: the file could not be loaded as an image." << std::endl;
            return -1;
        }
        std::cout << "Image detected. Opening the image interface..." << std::endl;
        Interface(filename);
    }

    return 0;
}
