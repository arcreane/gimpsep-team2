#include "image_operations.h"
#include "utils.h"
#include "button.h"
#include "interface.h"

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;
using namespace cv;


// Structure pour regrouper plusieurs boutons
struct dataPackage {
    Button* dilatationBtn;
    Button* dilatationField;
    Button* erosionBtn;
    Button* erosionField;
    Button* dimensionBtn;
    Button* dimensionField1;
    Button* dimensionField2;
    Button* factorBtn;
    Button* factorField;
    Button* lightenDarkenBtn;
    Button* lightenDarkenField;
    Button* cannyBtn;
    Button* cannyField1;
    Button* cannyField2;
    Button* cannyField3;
    Button* faceDetectionBtn;
    Button* backgroundSeparationBtn;
    Button* stitchingBtn;
    Button* stitchingField;
    Button* saveBtn;
    Button* saveField;


    string* filename;
    Mat* pCurrentImage;
    Button** activeField; // pointeur vers le champ actif
};

static void tryFunction(const std::function<cv::Mat(const cv::Mat&, int)>& func, dataPackage* group, const std::string& inputText) {
    try {
        int val = std::stoi(inputText);
        // On remplace l’image actuelle par celle résultant de l'opération
        *(group->pCurrentImage) = func(*(group->pCurrentImage), val);
    }
    catch (const std::invalid_argument&) {
        std::cerr << "Valeur invalide saisie dans le champ de texte !" << std::endl;
    }
    catch (const std::out_of_range&) {
        std::cerr << "Valeur hors plage !" << std::endl;
    }
}

static void tryFunctionTwoFields(const std::function<Mat(cv::Mat, int, int)>& func, dataPackage* group, const std::string& inputText1, const std::string& inputText2) {
    try {
        int val1 = std::stoi(inputText1);
        int val2 = std::stoi(inputText2);
        *(group->pCurrentImage) = func(*(group->pCurrentImage), val1, val2);
    }
    catch (const std::invalid_argument&) {
        std::cerr << "Valeur invalide saisie dans le champ de texte !" << std::endl;
    }
    catch (const std::out_of_range&) {
        std::cerr << "Valeur hors plage !" << std::endl;
    }
}

static void tryCanny(const std::function<Mat(cv::Mat, int, int, float)>& func, dataPackage* group, const std::string& inputText1, const std::string& inputText2, const std::string& inputText3) {
    try {
        int val1 = std::stoi(inputText1);
        int val2 = std::stoi(inputText2);
        float val3 = std::stof(inputText3);

        std::cout << "bliiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiip";
        if (group->pCurrentImage->empty()) {
            std::cerr << "Image vide au moment de CannyEdgeDetection !" << std::endl;
            return;
        }

        *(group->pCurrentImage) = func(*(group->pCurrentImage), val1, val2, val3);
    }
    catch (const std::invalid_argument&) {
        std::cerr << "Valeur invalide saisie dans le champ de texte !" << std::endl;
    }
    catch (const std::out_of_range&) {
        std::cerr << "Valeur hors plage !" << std::endl;
    }
}

void tryFunctionDouble(const std::function<Mat(cv::Mat, double)>& func, dataPackage* group, const std::string& inputText) {
    try {
        double val = std::stod(inputText);
        *(group->pCurrentImage) = func(*(group->pCurrentImage), val);
    }
    catch (const std::invalid_argument&) {
        std::cerr << "Valeur invalide saisie dans le champ de texte !" << std::endl;
    }
    catch (const std::out_of_range&) {
        std::cerr << "Valeur hors plage !" << std::endl;
    }
}

static void tryFunctionFloat(const std::function<Mat(cv::Mat, float)>& func, dataPackage* group, const std::string& inputText) {
    try {
        float val = std::stof(inputText);
        *(group->pCurrentImage) = func(*(group->pCurrentImage), val);
    }
    catch (const std::invalid_argument&) {
        std::cerr << "Valeur invalide saisie dans le champ de texte !" << std::endl;
    }
    catch (const std::out_of_range&) {
        std::cerr << "Valeur hors plage !" << std::endl;
    }
}

static void tryStitching(const std::function<Mat(const cv::Mat&, const cv::Mat&)>& func, dataPackage* group, const std::string& filename2) {
    try {
        cv::Mat img2 = cv::imread(filename2);

        if (img2.empty()) {
            std::cerr << "Erreur lors du chargement des images." << std::endl;
            return;
        }

        *(group->pCurrentImage) = func(*(group->pCurrentImage), img2);
    }
    catch (const std::invalid_argument&) {
        std::cerr << "Valeur invalide saisie dans le champ de texte !" << std::endl;
    }
    catch (const std::out_of_range&) {
        std::cerr << "Valeur hors plage !" << std::endl;
    }
}

static void tryBackgroundSeparation(const std::function<cv::Mat(const cv::Mat&)>& func, dataPackage* group) {
    try {
        if (group->pCurrentImage->empty()) {
            std::cerr << "Image vide au moment de la séparation de fond !" << std::endl;
            return;
        }

        *(group->pCurrentImage) = func(*(group->pCurrentImage));
    }
    catch (const std::exception& e) {
        std::cerr << "Erreur lors de la séparation de fond : " << e.what() << std::endl;
    }
}





static void InterfaceMouseCallback(int event, int x, int y, int flags, void* userdataAndMore) {
    if (event != EVENT_LBUTTONDOWN) return;

    dataPackage* group = static_cast<dataPackage*>(userdataAndMore);
    const string& filename = *(group->filename);

    // Gestion des clics sur les boutons
    if (group->dilatationBtn->isClicked(x, y)) {
        std::cout << "Bouton dilatation cliqué !" << std::endl;
        tryFunction(Dilatation, group, group->dilatationField->getText());
    }
    if (group->erosionBtn->isClicked(x, y)) {
        std::cout << "Bouton érosion cliqué !" << std::endl;
        tryFunction(Erosion, group, group->erosionField->getText());
    }
    if (group->dimensionBtn->isClicked(x, y)) {
        std::cout << "Bouton redimensionner cliqué !" << std::endl;
        tryFunctionTwoFields(DimensionResizing, group, group->dimensionField1->getText(), group->dimensionField2->getText());
    }
    if (group->factorBtn->isClicked(x, y)) {
        std::cout << "Bouton refactoriser cliqué !" << std::endl;
        tryFunctionDouble(FactorResizing, group, group->factorField->getText());
    }
    if (group->lightenDarkenBtn->isClicked(x, y)) {
        std::cout << "Bouton luminosité cliqué !" << std::endl;
        tryFunctionFloat(LightenDarken, group, group->lightenDarkenField->getText());
    }
    if (group->cannyBtn->isClicked(x, y)) {
        std::cout << "Bouton détection des bords de canny cliqué !" << std::endl;
        tryCanny(CannyEdgeDetection, group, group->cannyField1->getText(), group->cannyField2->getText(), group->cannyField3->getText());
    }
	if (group->faceDetectionBtn->isClicked(x, y)) {
		std::cout << "Bouton détection de visage cliqué !" << std::endl;
        
        FaceDetection(cv::imread(filename), "haarcascade_frontalface_default.xml");
	}
    if (group->backgroundSeparationBtn->isClicked(x, y)) {
        std::cout << "Bouton séparation de fond cliqué !" << std::endl;
        tryBackgroundSeparation(BackgroundSeparation, group);
    }
    if (group->stitchingBtn->isClicked(x, y)) {
        std::cout << "Bouton panorama cliqué !" << std::endl;
        tryStitching(StitchImages, group, group->stitchingField->getText());
    }
    if (group->saveBtn->isClicked(x, y)) {
        std::cout << "Bouton Sauvegarder cliqué !" << std::endl;
        std::string savePath = group->saveField->getText();
        bool success = cv::imwrite(savePath, *(group->pCurrentImage));
        if (success) {
            std::cout << "Image sauvegardée dans " << savePath << std::endl;
        }
        else {
            std::cerr << "Erreur lors de la sauvegarde." << std::endl;
        }
    }



    // Gestion des clics sur champs texte (activation/désactivation)
    Button* fields[] = {
        group->dilatationField, group->erosionField, group->dimensionField1, group->dimensionField2, group->factorField, group->lightenDarkenField, group->stitchingField, group->cannyField1, group->cannyField2, group->cannyField3, group->saveField
    };

    bool fieldActivated = false;
    for (Button* field : fields) {
        if (field->isClicked(x, y)) {
            field->setActive(true);
            *group->activeField = field;
            fieldActivated = true;
        }
        else {
            field->setActive(false);
        }
    }

    if (!fieldActivated) {
        *group->activeField = nullptr;
    }
}




void Interface(string filename) {
    cv::Mat image = cv::imread(filename);
        
    int btnHeight = 50;
    int spacing = 160;

    auto maxInt = [](int a, int b) { return (a > b) ? a : b; };

    int minWidth = 1920;
    int minHeight = 1080;

    int canvasWidth = maxInt(image.cols, minWidth);
    int canvasHeight = maxInt(image.rows + 2 * btnHeight, minHeight);

    cv::Mat canvas(canvasHeight, canvasWidth, CV_8UC3, cv::Scalar(0, 0, 0));
    image.copyTo(canvas(cv::Rect(0, 2 * btnHeight, image.cols, image.rows)));

    // Création des boutons et champs texte
    Button dilatationButton(0, 0, 150, btnHeight, "Dilatation");
    Button dilatationField(0, btnHeight, 150, btnHeight, ""); dilatationField.setAsTextField(true);

    Button erosionButton(spacing, 0, 150, btnHeight, "Erosion");
    Button erosionField(spacing, btnHeight, 150, btnHeight, ""); erosionField.setAsTextField(true);

    Button dimensionButton(spacing * 2, 0, 150, btnHeight, "Redimensionner");
    Button dimensionField1(spacing * 2, btnHeight, 70, btnHeight, ""); dimensionField1.setAsTextField(true);
    Button dimensionField2(spacing * 2 + spacing/2, btnHeight, 70, btnHeight, ""); dimensionField2.setAsTextField(true);

    Button factorButton(spacing * 3, 0, 150, btnHeight, "Refactoriser");
    Button factorField(spacing * 3, btnHeight, 150, btnHeight, ""); factorField.setAsTextField(true);

    Button lightenDarkenButton(spacing * 4, 0, 150, btnHeight, "Luminosite");
    Button lightenDarkenField(spacing * 4, btnHeight, 150, btnHeight, ""); lightenDarkenField.setAsTextField(true);

    Button cannyButton(spacing * 5, 0, 150, btnHeight, "Canny edge detection");
    Button cannyField1(spacing * 5, btnHeight, 50, btnHeight, ""); cannyField1.setAsTextField(true);
    Button cannyField2(spacing * 5 + spacing / 3, btnHeight, 50, btnHeight, ""); cannyField2.setAsTextField(true);
    Button cannyField3(spacing * 5 + (2*spacing) / 3, btnHeight, 50, btnHeight, ""); cannyField3.setAsTextField(true);

    Button faceDetection(spacing * 6, 0, 150, btnHeight, "Detection visage");

    Button backgroundSeparation(spacing * 7, 0, 150, btnHeight, "Separation fond");

    Button stitchingButton(spacing * 8, 0, 150, btnHeight, "Panorama");
    Button stitchingField(spacing * 8, btnHeight, 150, btnHeight, ""); stitchingField.setAsTextField(true);

    Button saveButton(spacing * 9, 0, 150, btnHeight, "Sauvegarder");
    Button saveField(spacing * 9, btnHeight, 150, btnHeight, ""); saveField.setAsTextField(true);



    Button* activeField = nullptr;

    dataPackage usefulThings = {
        &dilatationButton, &dilatationField,
        &erosionButton, &erosionField,
        &dimensionButton, &dimensionField1, &dimensionField2,
        &factorButton, &factorField,
        &lightenDarkenButton, &lightenDarkenField,
        &cannyButton, &cannyField1, &cannyField2, &cannyField3,
        &faceDetection,
        &backgroundSeparation,
        &stitchingButton, &stitchingField,
        &saveButton, &saveField,
        &filename,
        &image,
        &activeField
    };

    // Affichage initial
    cv::namedWindow("GimpSEP", cv::WINDOW_NORMAL);
    cv::resizeWindow("GimpSEP", canvasWidth, canvasHeight);

    while (true) {
        image.copyTo(canvas(cv::Rect(0, 2 * btnHeight, image.cols, image.rows)));

        dilatationButton.draw(canvas); dilatationField.draw(canvas);
        erosionButton.draw(canvas); erosionField.draw(canvas);
        dimensionButton.draw(canvas); dimensionField1.draw(canvas); dimensionField2.draw(canvas);
        factorButton.draw(canvas); factorField.draw(canvas);
        lightenDarkenButton.draw(canvas); lightenDarkenField.draw(canvas);
        cannyButton.draw(canvas); cannyField1.draw(canvas); cannyField2.draw(canvas); cannyField3.draw(canvas);
        faceDetection.draw(canvas);
        backgroundSeparation.draw(canvas);
        stitchingButton.draw(canvas); stitchingField.draw(canvas);
        saveButton.draw(canvas); saveField.draw(canvas);

        imshow("GimpSEP", canvas);
        setMouseCallback("GimpSEP", InterfaceMouseCallback, &usefulThings);

        int key = waitKey(1);
        if (key == 27) break; // Échap

        if (activeField != nullptr && activeField->isActive()) {
            if (key == 8 || key == 255) {
                activeField->removeLastChar();
            }
            else if (key >= 32 && key <= 126) {
                activeField->appendChar(static_cast<char>(key));
            }
        }
    }

    destroyAllWindows();
}

