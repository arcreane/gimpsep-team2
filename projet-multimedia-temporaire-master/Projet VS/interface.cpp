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
    Button* FaceDetectionBtn;
    Button* BackgroundSeparationBtn;

    string* filename;
    Button** activeField; // pointeur vers le champ actif
};

void tryFunction(const std::function<void(cv::Mat, int)>& func, const std::string& filename, const std::string& inputText) {
    try {
        int val = std::stoi(inputText);
        func(cv::imread(filename), val);
    }
    catch (const std::invalid_argument&) {
        std::cerr << "Valeur invalide saisie dans le champ de texte !" << std::endl;
    }
    catch (const std::out_of_range&) {
        std::cerr << "Valeur hors plage !" << std::endl;
    }
}

void tryFunctionTwoFields(const std::function<void(cv::Mat, int, int)>& func, const std::string& filename, const std::string& inputText1, const std::string& inputText2) {
    try {
        int val1 = std::stoi(inputText1);
        int val2 = std::stoi(inputText2);
        func(cv::imread(filename), val1, val2);
    }
    catch (const std::invalid_argument&) {
        std::cerr << "Valeur invalide saisie dans le champ de texte !" << std::endl;
    }
    catch (const std::out_of_range&) {
        std::cerr << "Valeur hors plage !" << std::endl;
    }
}

void tryFunctionDouble(const std::function<void(cv::Mat, double)>& func, const std::string& filename, const std::string& inputText) {
    try {
        double val = std::stod(inputText);
        func(cv::imread(filename), val);
    }
    catch (const std::invalid_argument&) {
        std::cerr << "Valeur invalide saisie dans le champ de texte !" << std::endl;
    }
    catch (const std::out_of_range&) {
        std::cerr << "Valeur hors plage !" << std::endl;
    }
}

void tryFunctionFloat(const std::function<void(cv::Mat, float)>& func, const std::string& filename, const std::string& inputText) {
    try {
        float val = std::stof(inputText);
        func(cv::imread(filename), val);
    }
    catch (const std::invalid_argument&) {
        std::cerr << "Valeur invalide saisie dans le champ de texte !" << std::endl;
    }
    catch (const std::out_of_range&) {
        std::cerr << "Valeur hors plage !" << std::endl;
    }
}



void InterfaceMouseCallback(int event, int x, int y, int flags, void* userdataAndMore) {
    if (event != EVENT_LBUTTONDOWN) return;

    dataPackage* group = static_cast<dataPackage*>(userdataAndMore);
    const string& filename = *(group->filename);

    // Gestion des clics sur les boutons
    if (group->dilatationBtn->isClicked(x, y)) {
        std::cout << "Bouton dilatation cliqué !" << std::endl;
        tryFunction(Dilatation, filename, group->dilatationField->getText());
    }
    if (group->erosionBtn->isClicked(x, y)) {
        std::cout << "Bouton érosion cliqué !" << std::endl;
        tryFunction(Erosion, filename, group->erosionField->getText());
    }
    if (group->dimensionBtn->isClicked(x, y)) {
        std::cout << "Bouton redimensionner cliqué !" << std::endl;
        tryFunctionTwoFields(DimensionResizing, filename, group->dimensionField1->getText(), group->dimensionField2->getText());
    }
    if (group->factorBtn->isClicked(x, y)) {
        std::cout << "Bouton refactoriser cliqué !" << std::endl;
        tryFunctionDouble(FactorResizing, filename, group->factorField->getText());
    }
    if (group->lightenDarkenBtn->isClicked(x, y)) {
        std::cout << "Bouton luminosité cliqué !" << std::endl;
        tryFunctionFloat(LightenDarken, filename, group->lightenDarkenField->getText());
    }
	if (group->FaceDetectionBtn->isClicked(x, y)) {
		std::cout << "Bouton détection de visage cliqué !" << std::endl;
		FaceDetection(cv::imread(filename));
	}
    if (group->BackgroundSeparationBtn->isClicked(x, y)) {
        std::cout << "Bouton séparation de fond cliqué !" << std::endl;
        BackgroundSeparation(cv::imread(filename));
    }


    // Gestion des clics sur champs texte (activation/désactivation)
    Button* fields[] = {
        group->dilatationField, group->erosionField, group->dimensionField1, group->dimensionField2, group->factorField, group->lightenDarkenField
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

void Interface(Mat image, string filename) {
    int btnHeight = 50;

    
    int totalWidth = 800; 
    int minCanvasWidth = std::max<int>(image.cols, 960);

    cv::Mat canvas(image.rows + 2 * btnHeight, minCanvasWidth, CV_8UC3, cv::Scalar(0, 0, 0));
    image.copyTo(canvas(cv::Rect(0, 2 * btnHeight, image.cols, image.rows)));

    // Création des boutons et champs texte
    Button dilatationButton(0, 0, 150, btnHeight, "Dilatation");
    Button dilatationField(0, btnHeight, 150, btnHeight, ""); dilatationField.setAsTextField(true);

    Button erosionButton(160, 0, 150, btnHeight, "Érosion");
    Button erosionField(160, btnHeight, 150, btnHeight, ""); erosionField.setAsTextField(true);

    Button dimensionButton(320, 0, 150, btnHeight, "Redimensionner");
    Button dimensionField1(320, btnHeight, 70, btnHeight, ""); dimensionField1.setAsTextField(true);
    Button dimensionField2(400, btnHeight, 70, btnHeight, ""); dimensionField2.setAsTextField(true);

    Button factorButton(480, 0, 150, btnHeight, "Refactoriser");
    Button factorField(480, btnHeight, 150, btnHeight, ""); factorField.setAsTextField(true);

    Button lightenDarkenButton(640, 0, 150, btnHeight, "Luminosité");
    Button lightenDarkenField(640, btnHeight, 150, btnHeight, ""); lightenDarkenField.setAsTextField(true);

    int spacing = 160;
    Button FaceDetection(spacing * 5, 0, 150, btnHeight, "Détection visage");

	Button BackgroundSeparation(spacing * 6, 0, 150, btnHeight, "Séparation fond");


    // Pointeur vers le champ actif
    Button* activeField = nullptr;

    // Création du groupe de données
    dataPackage group = {
        &dilatationButton, &dilatationField,
        &erosionButton, &erosionField,
        &dimensionButton, &dimensionField1, &dimensionField2,
        &factorButton, &factorField,
        &lightenDarkenButton, &lightenDarkenField,
		& FaceDetection,
		& BackgroundSeparation,
        &filename, &activeField
    };

    // Affichage initial
    while (true) {
        // Redessiner interface après chaque opération
        image.copyTo(canvas(cv::Rect(0, 2 * btnHeight, image.cols, image.rows)));
        dilatationButton.draw(canvas); dilatationField.draw(canvas);
        erosionButton.draw(canvas); erosionField.draw(canvas);
        dimensionButton.draw(canvas); dimensionField1.draw(canvas); dimensionField2.draw(canvas);
        factorButton.draw(canvas); factorField.draw(canvas);
        lightenDarkenButton.draw(canvas); lightenDarkenField.draw(canvas);
		FaceDetection.draw(canvas);
		BackgroundSeparation.draw(canvas);

        imshow("Interface", canvas);
        setMouseCallback("Interface", InterfaceMouseCallback, &group);

        int key = waitKey(0);

        if (key == 27) break; // Bouton échap

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
