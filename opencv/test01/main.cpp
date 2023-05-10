#include <opencv2/opencv.hpp>
using namespace cv;

int main() {
    Mat image = imread("D:\\a.jpg");
    imshow("Image", image);
    waitKey(0);
    destroyAllWindows();
    printf("ssssssssssssssssss");
    return 0;
}
