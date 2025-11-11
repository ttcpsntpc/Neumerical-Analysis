#ifndef BLOCK_METHID_H
#define BLOCK_METHID_H
#include <stdio.h>
#include "read.h"
#include "others.h"

float lagrange_interpolate(float* y_block, int K, float x_interp, int method) {
    float sum = 0.0f;
    if(method == 0) {
        for(int i = 0; i < K; i++) {
            float term = y_block[i];
            for(int j = 0; j < K; j++) {
                if (j != i) {
                    term *= (x_interp - j) / (i - j);
                }
            }
            sum += term;
        }
    }
    else if(method == 1) {
        for(int i = -1; i < K + 1; i++) {
            float term = y_block[i + 1];
            for(int j = -1; j < K + 1; j++) {
                if (j != i) {
                    term *= (x_interp - j) / (i - j);
                }
            }
            sum += term;
        }
    }

    return sum;
}

// method: 0 - K pixels, 1 - 1+K+1 pixels
ImageGray superSample(ImageGray input, int K, int M, int method) {
    int N = input.width; // Assuming square images

    int zoom = M / N; // 放大倍率
    int num_blocks = N / K; // 每行/每列的區塊數量
    int new_block_size = K * zoom; // 每個新區塊的大小
    
    float step = (K-1)/(new_block_size -1.0); // 將 [0, new_block_size-1] 映射到 [0, K-1]

    float *block_pixels;
    if(method == 0) {
        block_pixels = (float*)malloc(K * sizeof(float));
    }
    else if(method == 1) {
        block_pixels = (float*)malloc((K + 2) * sizeof(float));
    }
    // NxN -> NxM
    ImageGray I1 = allocateImage(M, N);
    for (int i = 0; i < N; i++) { 
        for (int b = 0; b < num_blocks; b++) {
            int block_start_col = b * K;

            if(method == 0) {
                // 複製 K 個 point，準備做內插
                for(int k = 0; k < K; k++) {
                    block_pixels[k] = input.data[i][block_start_col + k];
                }

                // 進行內插
                int new_start_col = b * new_block_size;
                for (int j = 0; j < new_block_size; j++) {
                    float x_interp = j * step;
                    float interp_val = lagrange_interpolate(block_pixels, K, x_interp, 0);
                    I1.data[i][new_start_col + j] = clamp(interp_val);
                }
            }
            else if(method == 1) {
                // 複製 K+2 個 point，準備做內插 (包含邊界的情況)
                for(int k = 0; k < K + 2; k++) {
                    int idx = block_start_col + k - 1;
                    if (idx < 0) idx = 0;
                    if (idx >= N) idx = N - 1;
                    block_pixels[k] = input.data[i][idx];
                }

                // 進行內插
                int new_start_col = b * new_block_size;
                for (int j = 0; j < new_block_size; j++) {
                    float x_interp = j * step;
                    float interp_val = lagrange_interpolate(block_pixels, K, x_interp, 1);
                    I1.data[i][new_start_col + j] = clamp(interp_val);
                }
            }

        }
    }
    
    // NxM -> MxM
    ImageGray I2 = allocateImage(M, M);
    for(int j = 0; j < M; j++) {
        for (int b = 0; b < num_blocks; b++) {
            int block_start_row = b * K;

            if(method == 0) {
                // 複製 block 的 K 個 pixel
                for(int k = 0; k < K; k++) {
                    block_pixels[k] = I1.data[block_start_row + k][j];
                }

                // 進行內插
                int new_start_row = b * new_block_size;
                for (int i = 0; i < new_block_size; i++) {
                    float x_interp = i * step;
                    float interp_val = lagrange_interpolate(block_pixels, K, x_interp, 0);
                    I2.data[new_start_row + i][j] = clamp(interp_val);
                }
            }
            else if(method == 1) {
                // 複製 block 的 K+2 個 pixel (包含邊界的情況)
                for(int k = 0; k < K + 2; k++) {
                    int idx = block_start_row + k - 1;
                    if (idx < 0) idx = 0;
                    if (idx >= N) idx = N - 1;
                    block_pixels[k] = I1.data[idx][j];
                }

                // 進行內插
                int new_start_row = b * new_block_size;
                for (int i = 0; i < new_block_size; i++) {
                    float x_interp = i * step;
                    float interp_val = lagrange_interpolate(block_pixels, K, x_interp, 1);
                    I2.data[new_start_row + i][j] = clamp(interp_val);
                }
            }
        }
    }

    free(block_pixels);
    freeImage(&I1);

    return I2;
}

ImageRGB superSampleRGB(ImageRGB input, int K, int M, int method) {
    int N = input.width; // Assuming square images

    int zoom = M / N; // 放大倍率
    int num_blocks = N / K; // 每行/每列的區塊數量
    int new_block_size = K * zoom; // 每個新區塊的大小
    
    float step = (K-1)/(new_block_size -1.0); // 將 [0, new_block_size-1] 映射到 [0, K-1]

    float *block_pixels;
    if(method == 0) {
        block_pixels = (float*)malloc(K * sizeof(float));
    }
    else if(method == 1) {
        block_pixels = (float*)malloc((K + 2) * sizeof(float));
    }
    // NxN -> NxM
    ImageRGB I1 = allocateImageRGB(M, N);
    for(int c = 0; c < 3; c++) { // 對每個 color channel 分別處理
        for (int i = 0; i < N; i++) { 
            for (int b = 0; b < num_blocks; b++) {
                int block_start_col = b * K;

                if(method == 0) {
                    // 複製 K 個 point，準備做內插
                    for(int k = 0; k < K; k++) {
                        block_pixels[k] = input.data[i][block_start_col + k][c];
                    }

                    // 進行內插
                    int new_start_col = b * new_block_size;
                    for (int j = 0; j < new_block_size; j++) {
                        float x_interp = j * step;
                        float interp_val = lagrange_interpolate(block_pixels, K, x_interp, 0);
                        I1.data[i][new_start_col + j][c] = clamp(interp_val);
                    }
                }
                else if(method == 1) {
                    // 複製 K+2 個 point，準備做內插 (包含邊界的情況)
                    for(int k = 0; k < K + 2; k++) {
                        int idx = block_start_col + k - 1;
                        if (idx < 0) idx = 0;
                        if (idx >= N) idx = N - 1;
                        block_pixels[k] = input.data[i][idx][c];
                    }

                    // 進行內插
                    int new_start_col = b * new_block_size;
                    for (int j = 0; j < new_block_size; j++) {
                        float x_interp = j * step;
                        float interp_val = lagrange_interpolate(block_pixels, K, x_interp, 1);
                        I1.data[i][new_start_col + j][c] = clamp(interp_val);
                    }
                }

            }
        }
    }
    
    // NxM -> MxM
    ImageRGB I2 = allocateImageRGB(M, M);
    for(int c = 0; c < 3; c++) { // 對每個 color channel 分別處理
        for(int j = 0; j < M; j++) {
            for (int b = 0; b < num_blocks; b++) {
                int block_start_row = b * K;

                if(method == 0) {
                    // 複製 block 的 K 個 pixel
                    for(int k = 0; k < K; k++) {
                        block_pixels[k] = I1.data[block_start_row + k][j][c];
                    }

                    // 進行內插
                    int new_start_row = b * new_block_size;
                    for (int i = 0; i < new_block_size; i++) {
                        float x_interp = i * step;
                        float interp_val = lagrange_interpolate(block_pixels, K, x_interp, 0);
                        I2.data[new_start_row + i][j][c] = clamp(interp_val);
                    }
                }
                else if(method == 1) {
                    // 複製 block 的 K+2 個 pixel (包含邊界的情況)
                    for(int k = 0; k < K + 2; k++) {
                        int idx = block_start_row + k - 1;
                        if (idx < 0) idx = 0;
                        if (idx >= N) idx = N - 1;
                        block_pixels[k] = I1.data[idx][j][c];
                    }

                    // 進行內插
                    int new_start_row = b * new_block_size;
                    for (int i = 0; i < new_block_size; i++) {
                        float x_interp = i * step;
                        float interp_val = lagrange_interpolate(block_pixels, K, x_interp, 1);
                        I2.data[new_start_row + i][j][c] = clamp(interp_val);
                    }
                }
            }
        }
    }

    free(block_pixels);
    freeImageRGB(&I1);

    return I2;
}

#endif