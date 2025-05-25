#include "image_operations.h"
#include "utils.h"
#include "button.h"
#include "interface.h"

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;
using namespace cv;

std::string filename;



int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    
    std::cout << "Entrez le chemin de l’image : ";
    std::getline(std::cin, filename);

    cv::Mat image = cv::imread(filename);
    if (image.empty()) {
        std::cerr << "Erreur : l’image n’a pas pu être chargée." << std::endl;
        return -1;
    }

    Interface(image, filename);
    
    return 0;
}
