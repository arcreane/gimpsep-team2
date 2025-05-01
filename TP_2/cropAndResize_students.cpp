#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(void)
{
Mat source, scaleD, scaleU;

// Read source image
source = cv::imread("C://Users//wfrio//Downloads//HappyFish.jpg");

// Scaling factors
double scaleX = 0.6;
double scaleY = 0.6;

// Scaling down the image 0.6 times
cv::resize(source, scaleD, cv::Size(), scaleX, scaleY, cv::INTER_LINEAR);
// Scaling up the image 1.8 times
cv::resize(source, scaleU, cv::Size(), 1.8, 1.8, cv::INTER_LINEAR);


//Cropped image
Mat crop = source(cv::Rect(50, 50, 200, 200));



//Display windows and show for all four images
cv::imshow("Original Image", source);
cv::imshow("Scaled Down Image", scaleD);
cv::imshow("Scaled Up Image", scaleU);
cv::imshow("Cropped Image", crop);

// Wait for a key press to close the windows
cv::waitKey(0);
return 0;
}
