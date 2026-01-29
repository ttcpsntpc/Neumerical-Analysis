#ifndef READ_H
#define READ_H
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int width;
    int height;
    float** data;
    char* name;
} Image;

static Image readImage(const char* filename) {
    Image image;
    image.name = NULL;   // 預設空指標
    image.data = NULL;
    image.width = image.height = 0;

    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return image; 
    }

    // 讀寬與高
    if (fscanf(file, "%d %d", &image.width, &image.height) != 2) {
        fprintf(stderr, "Invalid file format: %s\n", filename);
        fclose(file);
        return image;
    }

    // 配置二維陣列
    image.data = (float**)malloc(image.height * sizeof(float*));
    for (int i = 0; i < image.height; i++) {
        image.data[i] = (float*)malloc(image.width * sizeof(float));
        for (int j = 0; j < image.width; j++) {
            if (fscanf(file, "%f", &image.data[i][j]) != 1) {
                fprintf(stderr, "Invalid pixel data at row %d, col %d\n", i, j);
                // 清理並返回空影像
                for (int k = 0; k <= i; k++) free(image.data[k]);
                free(image.data);
                fclose(file);
                image.data = NULL;
                image.width = image.height = 0;
                return image;
            }
        }
    }

    fclose(file);
    return image; //  直接傳值回傳，不用 malloc Image 結構
}

#endif
