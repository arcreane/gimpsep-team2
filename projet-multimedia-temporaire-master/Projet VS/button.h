#pragma once
#include <opencv2/opencv.hpp>
#include <string>

class Button {
public:
    Button(int x, int y, int width, int height,
        const std::string& text,
        cv::Scalar bg = cv::Scalar(255, 255, 255),
        cv::Scalar txt = cv::Scalar(0, 0, 0),
        double font = 1.0, int thick = 1);

    void draw(cv::Mat& canvas) const;
    bool isClicked(int x, int y) const;

    void setAsTextField(bool flag) { isTextField = flag; }
    bool isTextFieldMode() const { return isTextField; }

    void setActive(bool flag) { active = flag; }
    bool isActive() const { return active; }

    void appendChar(char c) {
        if (isTextField && active) label += c;
    }

    void removeLastChar() {
        if (isTextField && active && !label.empty()) label.pop_back();
    }

    std::string getText() const { return label; }
    void clearText() { if (isTextField) label.clear(); }




private:
    bool isTextField = false;
    bool active = false;       // pour savoir si l'utilisateur écrit dedans

    cv::Rect area;
    std::string label;
    cv::Scalar bgColor;
    cv::Scalar textColor;
    double fontScale;
    int thickness;
};
