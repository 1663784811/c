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
    Mat src = imread("D:\\a.jpg");
    //===========================================
//    if (!src.data) {
//        std::cout << "Image not loaded";
//        return -1;
//    }
//    imshow("image", src);
    //===========================================
    QuickDemo qd;
    //===========================================
//    qd.colorSpace_Demo(src);
    qd.mat_creation_demo(src);



    //===========================================
    waitKey(0);
    printf("sssssssssssssssssssssssss");
    return 0;
}