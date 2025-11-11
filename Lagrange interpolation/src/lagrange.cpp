#include <stdio.h>
#include <stdlib.h>
#include <chrono>

#include "read.h"
#include "write.h"
#include "block_method.h"
#include "others.h"
#define M_TARGET 512

using Clock = std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::chrono::seconds;

ImageGray I0;
ImageRGB I1;
int K[4] = {2, 4, 8, 16};
int num_K = 4;

// for block method
int main() {
    I0 = readGrayImage("../gray1.txt");
    I1 = readRGBImage("../rgb1.txt");
    // gray image I0
    printf("Gray Image Lagrange Interpolation:\n");
    for(int i = 0; i < num_K; i++) {
        char output_filename[20];
        
        // Method 0: Using K pixels
        auto t1 = Clock::now();
        ImageGray I2 = superSample(I0, K[i], M_TARGET, 0);
        auto t2 = Clock::now();
        auto duration = t2 - t1;
        std::chrono::duration<double> s_double = duration;
        printf("K = %d 執行時間 (秒): %.6f s\n", K[i], s_double.count());

        sprintf(output_filename, "../raw/lagrange/%d.txt", K[i]);
        writeGrayImage(output_filename, I2);
        freeImage(&I2);

        // Method 1: Using 1+K+1 pixels
        t1 = Clock::now();
        I2 = superSample(I0, K[i], M_TARGET, 1);
        t2 = Clock::now();
        duration = t2 - t1;
        s_double = duration;
        printf("shared K = %d 執行時間 (秒): %.6f s\n", K[i], s_double.count());

        sprintf(output_filename, "../raw/lagrange/%d.txt", K[i] + 1);
        writeGrayImage(output_filename, I2);
        freeImage(&I2);
    }
    // RGB image I1
    printf("\nRGB Image Lagrange Interpolation:\n");
    for(int i = 0; i < num_K; i++) {
        char output_filename[20];
        // Method 0: Using K pixels
        auto t1 = Clock::now();
        ImageRGB I3 = superSampleRGB(I1, K[i], M_TARGET, 0);
        auto t2 = Clock::now();
        auto duration = t2 - t1;
        std::chrono::duration<double> s_double = duration;
        printf("K = %d 執行時間 (秒): %.6f s\n", K[i], s_double.count());
        
        sprintf(output_filename, "../raw/lagrange/%dc.txt", K[i]);
        writeRGBImage(output_filename, I3);

        freeImageRGB(&I3);

        // Method 1: Using 1+K+1 pixels
        t1 = Clock::now();
        I3 = superSampleRGB(I1, K[i], M_TARGET, 1);
        t2 = Clock::now();
        duration = t2 - t1;
        s_double = duration;
        printf("shared K = %d 執行時間 (秒): %.6f s\n", K[i], s_double.count());

        sprintf(output_filename, "../raw/lagrange/%dc.txt", K[i] + 1);
        writeRGBImage(output_filename, I3);

        freeImageRGB(&I3);
    }

    freeImage(&I0);
    freeImageRGB(&I1);
    return 0;
}