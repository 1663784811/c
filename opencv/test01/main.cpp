#include <iostream>
#include <opencv2/opencv.hpp>
#include "quickDemo.h"

using namespace std;
using namespace cv;

int main() {

    //===========================================
//    Mat srcImage = imread("D:\\a.jpg");
//    if (!srcImage.data) {
//        std::cout << "Image not loaded";
//        return -1;
//    }
//    imshow("image", srcImage);
    //===========================================

    QuickDemo qd;
    Mat src = imread("D:\\a.jpg");
    qd.colorSpace_Demo(src);


    //===========================================
    waitKey(0);
    printf("sssssssssssssssssssssssss");
    return 0;
}