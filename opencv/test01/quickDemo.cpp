//
// Created by why on 2023/5/11.
//
#include "quickDemo.h"


/**
 *
 */
void QuickDemo::colorSpace_Demo(Mat &image) {
    Mat gray, hsv;
    cvtColor(image, hsv, COLOR_BGR2HSV);
    cvtColor(image, gray, COLOR_BGR2GRAY);
    imshow("hsv", hsv);
    imshow("灰度", gray);
    imwrite("D:/hsv.png", hsv);
    imwrite("D:/灰度.png", gray);

}