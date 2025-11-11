#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <chrono>

#include "read.h"
#include "write.h"
#include "spline.h"
#include "others.h"
#define M_TARGET 512

using Clock = std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::chrono::seconds;

ImageGray I0;
ImageRGB I1;

// for block method
int main() {
    I1 = readRGBImage("../rgb1.txt");
    auto t1 = Clock::now();
    // NxN -> NxM
    ImageRGB I2 = allocateImageRGB(M_TARGET, I1.height);
    float step = (I1.height -1)/(M_TARGET -1.0); // 將 [0, M_TARGET-1] 映射到 [0, I1.height-1]
    for(int c=0; c<3; c++) {
        for(int i=0; i<I1.height; i++) {
            std::vector<double> x(I1.width);
            std::vector<double> y(I1.width);
            for(int j=0; j<I1.width; j++) {
                x[j]=j;
                y[j]=I1.data[i][j][c];
            }
            tk::spline s(x, y);
            for(int j=0; j<I2.width; j++) {
                float x_interp = j * step;
                I2.data[i][j][c]=clamp(s(x_interp));
            }
        }
    }
    // NxM -> MxM
    ImageRGB I3 = allocateImageRGB(M_TARGET, M_TARGET);
    for(int c=0; c<3; c++) {
        for(int j=0; j<I2.width; j++) {
            std::vector<double> x(I2.height);
            std::vector<double> y(I2.height);
            for(int i=0; i<I2.height; i++) {
                x[i]=i;
                y[i]=I2.data[i][j][c];
            }
            tk::spline s(x, y);
            for(int i=0; i<I3.height; i++) {
                float x_interp = i * step;
                I3.data[i][j][c]=clamp(s(x_interp));
            }
        }
    }
    auto t2 = Clock::now();
    auto duration = t2 - t1;
// 轉換為毫秒 (milliseconds)
    auto ms = std::chrono::duration_cast<milliseconds>(duration);
    
    // 轉換為微秒 (microseconds)
    auto us = std::chrono::duration_cast<microseconds>(duration);

    // 轉換為秒 (seconds) - 注意：這會截斷小數
    auto s_int = std::chrono::duration_cast<seconds>(duration);

    // 如果需要帶有小數的秒數 (推薦)
    // 將 duration 轉換為 double 類型的秒
    std::chrono::duration<double> s_double = duration;

    printf("執行時間 (毫秒): %lld ms\n", ms.count());
    printf("執行時間 (微秒): %lld ?s\n", us.count());
    printf("執行時間 (秒, 整數): %lld s\n", s_int.count());
    printf("執行時間 (秒, 小數): %.6f s\n", s_double.count());
    

    char output_filename[20];
    sprintf(output_filename, "../raw/interpolation/tt.txt");
    writeRGBImage(output_filename, I3);

    freeImageRGB(&I3);
    freeImageRGB(&I1);
    return 0;
}