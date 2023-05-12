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

/**
 * 创建图像：
 *
 *  在数字图像中，图像通道（Channel）是指图像中用于表示不同颜色或亮度分量的不同平面。
 *  通常情况下，彩色图像由三个基本颜色通道（红色、绿色和蓝色）组成，也称为RGB通道。
 *  每个通道包含图像的一部分信息，它们合并在一起形成完整的彩色图像。
 *  在灰度图像中，只有一个通道，该通道表示图像中每个像素的亮度值。
 *
 */
void QuickDemo::mat_creation_demo(Mat &image) {
    Mat m1, m2;
    m1 = image.clone();
    image.copyTo(m2);
    // 创建空白
//    Mat m3 = Mat::zeros(Size(8, 8), CV_8UC1);
    Mat m3 = Mat::ones(Size(50, 50), CV_8UC3);
    m3 = Scalar(0, 0, 255);
    std::cout << "宽度：" << m3.cols << "  高度：" << m3.rows << "  通道：" << m3.channels() << std::endl;
    std::cout << m3 << std::endl;
    imshow("创建图像", m3);
}


