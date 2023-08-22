#include <iostream>
#include <opencv2/opencv.hpp>
#include <Windows.h>

#include "quickDemo.h"
#include "window/LogWindow.h"


using namespace std;
using namespace cv;

/**
         255   255  255
 8位图片：  B     G    R

 通道：    s:饱和度         h:          v:亮度




 */
int main() {

//    LogWindow::init();
//    // ===========================================
//
//    LogWindow::getScreen();


    // ===========================================




    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);

    printf("Screen Resolution: %dx%d\n", screen_width, screen_height);


    cv::namedWindow("Screen Capture", cv::WINDOW_NORMAL);
    cv::resizeWindow("Screen Capture", screen_width, screen_height);


    while (true) {
        // 捕获屏幕图像
        cv::Mat screenshot;

        // 获取屏幕设备上下文
        HDC hdcScreen = GetDC(NULL);

        // 创建与屏幕兼容的设备上下文和位图
        HDC hdcCapture = CreateCompatibleDC(hdcScreen);
        HBITMAP hbmCapture = CreateCompatibleBitmap(hdcScreen, screen_width, screen_height);

        // 将位图与设备上下文关联
        SelectObject(hdcCapture, hbmCapture);

        // 使用 BitBlt 函数捕获屏幕图像到位图上下文
        BitBlt(hdcCapture, 0, 0, screen_width, screen_height, hdcScreen, 0, 0, SRCCOPY);

        // 获取位图数据到 OpenCV Mat 对象中
        screenshot.create(screen_height, screen_width, CV_8UC3);
        GetDIBits(hdcCapture, hbmCapture, 0, screen_height, screenshot.data, (BITMAPINFO*)&screenshot, DIB_RGB_COLORS);
        cv::cvtColor(screenshot, screenshot, cv::COLOR_BGR2RGB);

        // 显示图像
        cv::imshow("Screen Capture", screenshot);

        // 按下 'q' 键退出循环
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    // 释放资源
    cv::destroyAllWindows();


    Mat src = imread("D:\\a.jpeg");


    //===========================================
//    if (!src.data) {
//        std::cout << "Image not loaded";
//        return -1;
//    }
//    imshow("image", src);
    //===========================================
    QuickDemo qd;
    //===========================================
    qd.colorSpace_Demo(src);
    qd.mat_creation_demo(src);
//    qd.pixel_visi_demo(src);



    //===========================================
    waitKey(0);
    printf("\n\nsssssssssssssssssssssssss");
    return 0;
}