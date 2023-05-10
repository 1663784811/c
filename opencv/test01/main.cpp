#include <iostream>
#include <opencv2/opencv.hpp>
#include "quickDemo.h"

using namespace std;
using namespace cv;
/**
         255   255  255
 8位图片：  B     G    R

 通道：    s:饱和度         h:          v:亮度




 */
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