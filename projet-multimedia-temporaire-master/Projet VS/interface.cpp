#include "image_operations.h"
#include "button.h"
#include "interface.h"

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;
using namespace cv;


// Structure to hold the data for the interface
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
	Button** activeField; // point to the currently active text field
};

static void tryFunction(const std::function<cv::Mat(const cv::Mat&, int)>& func, dataPackage* group, const std::string& inputText) {
    try {
        int val = std::stoi(inputText);
		// we replace the current image with the result of the function
        *(group->pCurrentImage) = func(*(group->pCurrentImage), val);
    }
    catch (const std::invalid_argument&) {
        std::cerr << "Invalid value entered in the text field!" << std::endl;
    }
    catch (const std::out_of_range&) {
        std::cerr << "Value out of range!" << std::endl;
    }
}

static void tryFunctionTwoFields(const std::function<Mat(cv::Mat, int, int)>& func, dataPackage* group, const std::string& inputText1, const std::string& inputText2) {
    try {
        int val1 = std::stoi(inputText1);
        int val2 = std::stoi(inputText2);
        *(group->pCurrentImage) = func(*(group->pCurrentImage), val1, val2);
    }
    catch (const std::invalid_argument&) {
        std::cerr << "Invalid value entered in the text field!" << std::endl;
    }
    catch (const std::out_of_range&) {
        std::cerr << "Value out of range!" << std::endl;
    }
}

static void tryCanny(const std::function<Mat(cv::Mat, int, int, float)>& func, dataPackage* group, const std::string& inputText1, const std::string& inputText2, const std::string& inputText3) {
    try {
        int val1 = std::stoi(inputText1);
        int val2 = std::stoi(inputText2);
        float val3 = std::stof(inputText3);

        if (group->pCurrentImage->empty()) {
            std::cerr << "Empty image at the time of CannyEdgeDetection!" << std::endl;
            return;
        }

        *(group->pCurrentImage) = func(*(group->pCurrentImage), val1, val2, val3);
    }
    catch (const std::invalid_argument&) {
        std::cerr << "Invalid value entered in the text field!" << std::endl;
    }
    catch (const std::out_of_range&) {
        std::cerr << "Value out of range!" << std::endl;
    }
}

static void tryFunctionDouble(const std::function<Mat(cv::Mat, double)>& func, dataPackage* group, const std::string& inputText) {
    try {
        double val = std::stod(inputText);
        *(group->pCurrentImage) = func(*(group->pCurrentImage), val);
    }
    catch (const std::invalid_argument&) {
        std::cerr << "Invalid value entered in the text field!" << std::endl;
    }
    catch (const std::out_of_range&) {
        std::cerr << "Value out of range!" << std::endl;
    }
}

static void tryFunctionFloat(const std::function<Mat(cv::Mat, float)>& func, dataPackage* group, const std::string& inputText) {
    try {
        float val = std::stof(inputText);
        *(group->pCurrentImage) = func(*(group->pCurrentImage), val);
    }
    catch (const std::invalid_argument&) {
        std::cerr << "Invalid value entered in the text field!" << std::endl;
    }
    catch (const std::out_of_range&) {
        std::cerr << "Value out of range!" << std::endl;
    }
}

static void tryStitching(const std::function<Mat(const cv::Mat&, const cv::Mat&)>& func, dataPackage* group, const std::string& filename2) {
    try {
        cv::Mat img2 = cv::imread(filename2);

        if (img2.empty()) {
            std::cerr << "Error when loading images." << std::endl;
            return;
        }

        *(group->pCurrentImage) = func(*(group->pCurrentImage), img2);
    }
    catch (const std::invalid_argument&) {
        std::cerr << "Invalid value entered in the text field!" << std::endl;
    }
    catch (const std::out_of_range&) {
        std::cerr << "Value out of range!" << std::endl;
    }
}

static void tryBackgroundSeparation(const std::function<cv::Mat(const cv::Mat&)>& func, dataPackage* group) {
    try {
        if (group->pCurrentImage->empty()) {
            std::cerr << "Empty image at the moment of background separation!" << std::endl;
            return;
        }

        *(group->pCurrentImage) = func(*(group->pCurrentImage));
    }
    catch (const std::exception& error) {
        std::cerr << "Error when background separation: " << error.what() << std::endl;
    }
}





static void InterfaceMouseCallback(int event, int x, int y, int flags, void* userdataAndMore) {
    if (event != EVENT_LBUTTONDOWN) return;

    dataPackage* group = static_cast<dataPackage*>(userdataAndMore);
    const string& filename = *(group->filename);

	// Manage button clicks
    if (group->dilatationBtn->isClicked(x, y)) {
        std::cout << "Dilation button pressed!" << std::endl;
        tryFunction(ImageOperations::Dilatation, group, group->dilatationField->getText());
    }
    if (group->erosionBtn->isClicked(x, y)) {
        std::cout << "Erosion button pressed!" << std::endl;
        tryFunction(ImageOperations::Erosion, group, group->erosionField->getText());
    }
    if (group->dimensionBtn->isClicked(x, y)) {
        std::cout << "Resize button pressed!" << std::endl;
        tryFunctionTwoFields(ImageOperations::DimensionResizing, group, group->dimensionField1->getText(), group->dimensionField2->getText());
    }
    if (group->factorBtn->isClicked(x, y)) {
        std::cout << "Refactor button pressed!" << std::endl;
        tryFunctionDouble(ImageOperations::FactorResizing, group, group->factorField->getText());
    }
    if (group->lightenDarkenBtn->isClicked(x, y)) {
        std::cout << "Luminosity button pressed!" << std::endl;
        tryFunctionFloat(ImageOperations::LightenDarken, group, group->lightenDarkenField->getText());
    }
    if (group->cannyBtn->isClicked(x, y)) {
        std::cout << "Canny edge detection button pressed!" << std::endl;
        tryCanny(ImageOperations::CannyEdgeDetection, group, group->cannyField1->getText(), group->cannyField2->getText(), group->cannyField3->getText());
    }
    if (group->faceDetectionBtn->isClicked(x, y)) {
        std::cout << "Face detection button pressed!" << std::endl;
        ImageOperations::FaceDetection(cv::imread(filename), "haarcascade_frontalface_default.xml");
    }
    if (group->backgroundSeparationBtn->isClicked(x, y)) {
        std::cout << "Background separation button pressed!" << std::endl;
        tryBackgroundSeparation(ImageOperations::BackgroundSeparation, group);
    }
    if (group->stitchingBtn->isClicked(x, y)) {
        std::cout << "Panorama/stitching button pressed!" << std::endl;
        tryStitching(ImageOperations::StitchImages, group, group->stitchingField->getText());
    }

    // Wassim
    if (group->saveBtn->isClicked(x, y)) {
        std::cout << "Save button pressed!" << std::endl;
        std::string savePath = group->saveField->getText();
        bool success = cv::imwrite(savePath, *(group->pCurrentImage));
        if (success) {
            std::cout << "Image saved in " << savePath << std::endl;
        }
        else {
            std::cerr << "Error when saving." << std::endl;
        }
    }



	// Manage text field activation
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

	// Creation of buttons and text fields
    Button dilatationButton(0, 0, 150, btnHeight, "Dilation");
    Button dilatationField(0, btnHeight, 150, btnHeight, ""); dilatationField.setAsTextField(true);

    Button erosionButton(spacing, 0, 150, btnHeight, "Erosion");
    Button erosionField(spacing, btnHeight, 150, btnHeight, ""); erosionField.setAsTextField(true);

    Button dimensionButton(spacing * 2, 0, 150, btnHeight, "Resize");
    Button dimensionField1(spacing * 2, btnHeight, 70, btnHeight, ""); dimensionField1.setAsTextField(true);
    Button dimensionField2(spacing * 2 + spacing / 2, btnHeight, 70, btnHeight, ""); dimensionField2.setAsTextField(true);

    Button factorButton(spacing * 3, 0, 150, btnHeight, "Refactor");
    Button factorField(spacing * 3, btnHeight, 150, btnHeight, ""); factorField.setAsTextField(true);

    Button lightenDarkenButton(spacing * 4, 0, 150, btnHeight, "Luminosity");
    Button lightenDarkenField(spacing * 4, btnHeight, 150, btnHeight, ""); lightenDarkenField.setAsTextField(true);

    Button cannyButton(spacing * 5, 0, 150, btnHeight, "Canny edge");
    Button cannyField1(spacing * 5, btnHeight, 50, btnHeight, ""); cannyField1.setAsTextField(true);
    Button cannyField2(spacing * 5 + spacing / 3, btnHeight, 50, btnHeight, ""); cannyField2.setAsTextField(true);
    Button cannyField3(spacing * 5 + (2 * spacing) / 3, btnHeight, 50, btnHeight, ""); cannyField3.setAsTextField(true);

    Button faceDetection(spacing * 6, 0, 150, btnHeight, "Face detection");

    Button backgroundSeparation(spacing * 7, 0, 150, btnHeight, "BG separation");

    Button stitchingButton(spacing * 8, 0, 150, btnHeight, "Panorama");
    Button stitchingField(spacing * 8, btnHeight, 150, btnHeight, ""); stitchingField.setAsTextField(true);

    // Wassim
    Button saveButton(spacing * 9, 0, 150, btnHeight, "Save");
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

	// Create the window and set its properties
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

        cv::imshow("GimpSEP", canvas);
        cv::setMouseCallback("GimpSEP", InterfaceMouseCallback, &usefulThings);

        int key = waitKey(1);
        if (key == 27) break; // Escape

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

