#include "image_operations.h"
#include "utils.h"
#include "button.h"
#include "interface.h"
#include "image_operations.h"

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;
using namespace cv;

std::string filename;

// Pour détecter s'il s'agit d'une vidéo
bool isVideoFile(const std::string& path) {
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
    std::cout << "Entrez le chemin du fichier (image ou vidéo) : ";
    std::getline(std::cin, filename);

    if (isVideoFile(filename)) {
        std::cout << "Fichier vidéo détecté. Ouverture de l’interface vidéo..." << std::endl;
        VideoManipulation(filename);
    } else {
        cv::Mat image = cv::imread(filename);
        if (image.empty()) {
            std::cerr << "Erreur : le fichier n’a pas pu être chargé comme image." << std::endl;
            return -1;
        }
        std::cout << "Image détectée. Ouverture de l’interface image..." << std::endl;
        Interface(filename);
    }

    return 0;
}
