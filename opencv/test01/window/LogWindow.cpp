#include <opencv2/opencv.hpp>
#include <Windows.h>
#include <wingdi.h>


#include "LogWindow.h"


using namespace std;


void LogWindow::init() {




}

void LogWindow::log(char *log) {


}


Mat LogWindow::getScreen() {
    // 获取显示器句柄
    HDC hdcScreen = GetDC(NULL);
    // 计算屏幕的实际宽度和高度
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    printf("高度  %d  ",screenHeight);
    // 创建内存设备上下文
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    // 创建位图
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, screenWidth, screenHeight);
    // 将位图选入内存设备上下文
    SelectObject(hdcMem, hBitmap);
    // 将屏幕内容拷贝到位图中
    BitBlt(hdcMem, 0, 0, screenWidth, screenHeight, hdcScreen, 0, 0, SRCCOPY);
    // 从位图中创建OpenCV图像
    BITMAPINFOHEADER bi = { sizeof(BITMAPINFOHEADER), screenWidth, -screenHeight, 1, 32, BI_RGB, 0, 0, 0, 0, 0 };
    Mat img(screenHeight, screenWidth, CV_8UC4);
    GetDIBits(hdcMem, hBitmap, 0, screenHeight, img.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    cv::imwrite("d:\\sss.png", img);
    return img;
}
