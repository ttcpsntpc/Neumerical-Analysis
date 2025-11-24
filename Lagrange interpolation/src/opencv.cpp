#include <stdio.h>
#include <stdlib.h>
#include <chrono>

// 包含 C++ 的 OpenCV 函式庫
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include "read.h"
#include "write.h"
#include "block_method.h"
#include "others.h"
#define M_TARGET 256

using Clock = std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::chrono::seconds;

ImageGray I0;
ImageRGB I1;
int K[4] = {2, 4, 8, 16};
int num_K = 4;

static cv::Mat imageRgbToCvMat(const ImageRGB& image) {
    if (image.width == 0 || image.height == 0) {
        return cv::Mat();
    }
    // 建立一個連續記憶體的 OpenCV Mat
    cv::Mat mat(image.height, image.width, CV_32FC3); 
    
    // 手動複製資料
    for (int i = 0; i < image.height; ++i) {
        for (int j = 0; j < image.width; ++j) {
            // cv::Mat 存取方式: mat.at<type>(row, col)[channel]
            // 注意: OpenCV 預設通道順序是 BGR，但 resize 演算法
            // 只是對 3 個通道分別運算，所以 R-G-B 順序進去，
            // 也會是 R-G-B 順序出來，我們不用管它。
            mat.at<cv::Vec3f>(i, j)[0] = image.data[i][j][0]; // 存 R
            mat.at<cv::Vec3f>(i, j)[1] = image.data[i][j][1]; // 存 G
            mat.at<cv::Vec3f>(i, j)[2] = image.data[i][j][2]; // 存 B
        }
    }
    printf("已將 ImageRGB 轉換為 cv::Mat\n");
    return mat;
}
static ImageRGB cvMatToImageRgb(const cv::Mat& mat) {
    if (mat.empty() || mat.type() != CV_32FC3) {
        fprintf(stderr, "錯誤: Mat 為空或格式不符 (需要 CV_32FC3)\n");
        return allocateImageRGB(0, 0);
    }
    
    ImageRGB image = allocateImageRGB(mat.cols, mat.rows);

    for (int i = 0; i < mat.rows; ++i) {
        for (int j = 0; j < mat.cols; ++j) {
            image.data[i][j][0] = clamp(mat.at<cv::Vec3f>(i, j)[0]); // 取 R
            image.data[i][j][1] = clamp(mat.at<cv::Vec3f>(i, j)[1]); // 取 G
            image.data[i][j][2] = clamp(mat.at<cv::Vec3f>(i, j)[2]); // 取 B
        }
    }
    printf("已將 cv::Mat 轉換回 ImageRGB\n");
    return image;
}
int main() {
    char output_filename[20];

    I1 = readRGBImage("../rgb1.txt");

     // 2. (橋樑 1) 將你的結構轉為 cv::Mat
    cv::Mat mat_in = imageRgbToCvMat(I1);

    // 3. (核心) 呼叫 OpenCV 的 Resize
    cv::Mat mat_out;
    cv::Size target_size(M_TARGET, M_TARGET);
    
    // linear interpolation
    auto t1 = Clock::now();
    cv::resize(mat_in, mat_out, target_size, 0, 0, cv::INTER_LINEAR); 
    auto t2 = Clock::now();
    auto duration = t2 - t1;
    std::chrono::duration<double> s_double = duration;
    printf("Linear 執行時間 (秒, 小數): %.6f s\n", s_double.count());

    ImageRGB I_out = cvMatToImageRgb(mat_out);
    sprintf(output_filename, "../raw/interpolation 256/linear.txt");
    writeRGBImage(output_filename, I_out);

    // cubic interpolation
    t1 = Clock::now();
    cv::resize(mat_in, mat_out, target_size, 0, 0, cv::INTER_CUBIC); 
    t2 = Clock::now();
    duration = t2 - t1;
    s_double = duration;
    printf("Cubic 執行時間 (秒, 小數): %.6f s\n", s_double.count());

    I_out = cvMatToImageRgb(mat_out);
    sprintf(output_filename, "../raw/interpolation 256/cubic.txt");
    writeRGBImage(output_filename, I_out);
    
    // lanczos interpolation
    t1 = Clock::now();
    cv::resize(mat_in, mat_out, target_size, 0, 0, cv::INTER_LANCZOS4); 
    t2 = Clock::now();
    duration = t2 - t1;
    s_double = duration;
    printf("Lanczos 執行時間 (秒, 小數): %.6f s\n", s_double.count());

    I_out = cvMatToImageRgb(mat_out);
    sprintf(output_filename, "../raw/interpolation 256/lanczos.txt");
    writeRGBImage(output_filename, I_out);
}