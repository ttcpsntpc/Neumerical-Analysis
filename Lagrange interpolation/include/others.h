#ifndef OTHERS_H
#define OTHERS_H
#include <stdio.h>
#include <stdlib.h>
#include "read.h"

ImageGray allocateImage(int width, int height) {
    ImageGray image;
    image.width = width;
    image.height = height;
    image.name = NULL;
    image.data = (float**)malloc(height * sizeof(float*));
    
    for (int i = 0; i < height; i++) {
        image.data[i] = (float*)malloc(width * sizeof(float));
    }
    return image;
}
ImageRGB allocateImageRGB(int width, int height) {
    ImageRGB image;
    image.width = width;
    image.height = height;
    image.name = NULL;
    image.data = (float***)malloc(height * sizeof(float**));
    for (int i = 0; i < height; i++) {
        image.data[i] = (float**)malloc(width * sizeof(float*));
        for (int j = 0; j < width; j++) {
            image.data[i][j] = (float*)malloc(3 * sizeof(float));
        }
    }
    return image;
}

void freeImage(ImageGray* image) {
    if (image && image->data) {
        for (int i = 0; i < image->height; i++) {
            free(image->data[i]);
        }
        free(image->data);
        image->data = NULL;
        image->width = 0;
        image->height = 0;
    }
}
void freeImageRGB(ImageRGB* image) {
    if (image && image->data) {
        for (int i = 0; i < image->height; i++) {
            for (int j = 0; j < image->width; j++) {
                free(image->data[i][j]);
            }
            free(image->data[i]);
        }
        free(image->data);
        image->data = NULL;
        image->width = 0;
        image->height = 0;
    }
}

float clamp(float val) {
    if (val > 1.0f) return 1.0f;
    if (val < 0.0f) return 0.0f;
    return val;
}

#endif