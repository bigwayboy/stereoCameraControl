#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h> // Required for CreateDirectory on Windows
#include <io.h> // For _access on Windows

int main() {
    // 定义相机的分辨率
    int imageWidth = 1280 * 2;
    int imageHeight = 720;

    // 打开相机
    cv::VideoCapture cap(1);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera." << std::endl;
        return -1;
    }

    // 设置相机分辨率
    cap.set(cv::CAP_PROP_FRAME_WIDTH, imageWidth);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, imageHeight);

    cv::Mat frame;
    cv::Mat frameLeft;
    cv::Mat frameRight;

    // 创建img文件夹路径
    std::string img_folder_path = "img";

    // 检查文件夹是否存在，如果不存在则创建
    if (_access(img_folder_path.c_str(), 0) == -1) {
        // _access returns -1 if the path does not exist
        if (!CreateDirectoryA(img_folder_path.c_str(), NULL)) {
            // If directory creation fails, print an error and exit
            std::cerr << "Error: Could not create directory." << std::endl;
            return -1;
        }
    }

    while (true) {
        // 从相机读取一帧图像
        cap >> frame;

        // 检查是否成功读取了帧
        if (frame.empty()) {
            std::cerr << "Error: Could not read frame from camera." << std::endl;
            break;
        }

        // 将图像分为左右两个部分
       // 假设图像的宽度是双目相机的总宽度
        frameLeft = frame(cv::Rect(0, 0, imageWidth / 2, imageHeight));
        frameRight = frame(cv::Rect(imageWidth / 2, 0, imageWidth / 2, imageHeight));

        // 显示左目和右目图像
        cv::namedWindow("Left Eye", cv::WINDOW_NORMAL);
        cv::resizeWindow("Left Eye", imageWidth / 2 / 2, imageHeight / 2);
        cv::imshow("Left Eye", frameLeft);

        cv::namedWindow("Right Eye", cv::WINDOW_NORMAL);
        cv::resizeWindow("Right Eye", imageWidth / 2 / 2, imageHeight / 2);
        cv::imshow("Right Eye", frameRight);

        // 按 ' '（空格键）拍照
        if (cv::waitKey(1) == ' ') {
            // 定义文件名，使用cv::getTickCount()生成时间戳
            std::string left_filename = img_folder_path + "/left_frame_" + std::to_string(cv::getTickCount()) + ".jpg";
            std::string right_filename = img_folder_path + "/right_frame_" + std::to_string(cv::getTickCount()) + ".jpg";

            // 保存图像
            cv::imwrite(left_filename, frameLeft);
            cv::imwrite(right_filename, frameRight);

            // 打印信息
            std::cout << "左目图像存储路径: " << left_filename << std::endl;
            std::cout << "右目图像存储路径: " << right_filename << std::endl;
        }

        // 按 'q' 退出循环
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    // 释放相机资源
    cap.release();
    // 关闭所有OpenCV窗口
    cv::destroyAllWindows();

    return 0;
}