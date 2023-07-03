//
// Created by why on 2023/5/17.
//
/**
 *
 * 日志打印窗口
 *
 */
#include <opencv2/opencv.hpp>

#ifndef TEST01_LOGWINDOW_H
#define TEST01_LOGWINDOW_H

using namespace cv;

class LogWindow {

public:
    // 初始化
    static void init();
    // 打印日志
    static void log(char log[]);
    // 获取显示器图像
    static Mat getScreen();

};


#endif //TEST01_LOGWINDOW_H
