#include "Button.h"

Button::Button(int x, int y, int width, int height,
    const std::string& text,
    cv::Scalar bg, cv::Scalar txt,
    double font, int thick)
    : area(x, y, width, height), label(text),
    bgColor(bg), textColor(txt),
    fontScale(font), thickness(thick)
{
}

void Button::draw(cv::Mat& canvas) const {
    cv::Scalar drawColor = isTextField && active ? cv::Scalar(255, 255, 255) : bgColor;
    cv::rectangle(canvas, area, drawColor, cv::FILLED);
    cv::rectangle(canvas, area, cv::Scalar(0, 0, 0), 1); // Bordure

    int baseline = 0;
    cv::Size textSize = cv::getTextSize(label, cv::FONT_HERSHEY_PLAIN, fontScale, thickness, &baseline);
    cv::Point textOrg(
        area.x + 5, // Pour champs texte : aligné à gauche
        area.y + (area.height + textSize.height) / 2
    );
    cv::putText(canvas, label, textOrg, cv::FONT_HERSHEY_PLAIN, fontScale, textColor, thickness);
}


bool Button::isClicked(int x, int y) const {
    return area.contains(cv::Point(x, y));
}
