#ifndef READ_H
#define READ_H
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int width;
    int height;
    float** data;
    char* name;
} ImageGray;
typedef struct {
    int width;
    int height;
    float*** data;
    char* name;
} ImageRGB;

static ImageGray readGrayImage(const char* filename) {
    ImageGray image;
    image.name = NULL;   // ?w?]?????
    image.data = NULL;
    image.width = image.height = 0;

    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return image; 
    }

    // ??e?P??
    if (fscanf(file, "%d %d", &image.width, &image.height) != 2) {
        fprintf(stderr, "Invalid file format: %s\n", filename);
        fclose(file);
        return image;
    }

    // ?t?m?G???}?C
    image.data = (float**)malloc(image.height * sizeof(float*));
    for (int i = 0; i < image.height; i++) {
        image.data[i] = (float*)malloc(image.width * sizeof(float));
        for (int j = 0; j < image.width; j++) {
            if (fscanf(file, "%f", &image.data[i][j]) != 1) {
                fprintf(stderr, "Invalid pixel data at row %d, col %d\n", i, j);
                // ?M?z???^??v??
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
    return image; //  ???????^??A???? malloc Image ???c
}
static ImageRGB readRGBImage(const char* filename) {
    ImageRGB image;
    image.name = NULL;  
    image.data = NULL;
    image.width = image.height = 0;

    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return image;
    }

    // ??e?P??
    if (fscanf(file, "%d %d", &image.width, &image.height) != 2) {
        fprintf(stderr, "Invalid file format: %s\n", filename);
        fclose(file);
        return image;
    }

    // ?t?m?G???}?C
    image.data = (float***)malloc(image.height * sizeof(float**));
    for (int i = 0; i < image.height; i++) {
        image.data[i] = (float**)malloc(image.width * sizeof(float*));
        for (int j = 0; j < image.width; j++) {
            for(int c = 0; c < 3; c++) {
                image.data[i][j] = (float*)malloc(3 * sizeof(float));
				if (fscanf(file, "%f", &image.data[i][j][c])!= 1) {
                    fprintf(stderr, "Invalid pixel data at row %d, col %d, channel %d\n", i, j, c);
                    // ?M?z???^??v??
                    for (int m = 0; m <= i; m++) {
                        for (int n = 0; n < (m == i ? j + 1 : image.width); n++) {
                            free(image.data[m][n]);
                        }
                        free(image.data[m]);
                    }
                    free(image.data);
                    fclose(file);
                    image.data = NULL;
                    image.width = image.height = 0;
                    return image;
				}
            }
        }
    }

    fclose(file);
    return image; //  ???????^??A???? malloc Image ???c
}

#endif
