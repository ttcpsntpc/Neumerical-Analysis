#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
int MIN(int a, int b)
{
    return a < b ? a : b;
}
int main() {
    int raw_width, raw_height, nrChannels;
    unsigned char *raw_data = stbi_load("chest.png", &raw_width, &raw_height, &nrChannels, 0);
    
    int width = 512, height = 512;
    unsigned char *scaled_data = (unsigned char *)malloc(4 * width * height * sizeof(unsigned char)); // RGBA
    
    int scale = width / raw_width; // 放大倍率

    for(int j = 0; j < height; j++)
    {
        float y_in_raw = (float)j / scale;
        int y0 = (int)y_in_raw;
        int y1 = MIN(y0 + 1, raw_height - 1);
        float dy = y_in_raw - y0;

        for(int i = 0; i < width; i++)
        {
            float x_in_raw = (float)i / scale;
            int x0 = (int)x_in_raw;
            int x1 = MIN(x0 + 1, raw_width - 1);
            float dx = x_in_raw - x0;

            for(int c = 0; c < 3; c++)
            {
                int raw00 = (y0 * raw_width + x0) * 4 + c;
                int raw10 = (y0 * raw_width + x1) * 4 + c;
                int raw01 = (y1 * raw_width + x0) * 4 + c;
                int raw11 = (y1 * raw_width + x1) * 4 + c;
                
                int index = (j * width + i) * 4 + c;
                scaled_data[index] = (unsigned char)
                    (raw_data[raw00] * (1 - dx) * (1 - dy) +
                    raw_data[raw10] * dx * (1 - dy) +
                    raw_data[raw01] * (1 - dx) * dy +
                    raw_data[raw11] * dx * dy);
            }
            int index = (j * width + i) * 4 + 3;
            scaled_data[index] = 255;
        }
    }

    stbi_write_png("bilinear_chest.png", width, height, 4, scaled_data, width * 4);
    
    return 0;
}