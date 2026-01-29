#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// SSIM constants
#define C1 (0.01 * 1.0 * 0.01 * 1.0)  // (K1 * L)^2, L=1.0 for normalized values
#define C2 (0.03 * 1.0 * 0.03 * 1.0)  // (K2 * L)^2

// Structure to hold image data
typedef struct {
    int N;              // Image size (N x N)
    double **data;      // 2D array to store pixel values
} Image;

// Allocate memory for image
Image* create_image(int N) {
    Image *img = (Image*)malloc(sizeof(Image));
    img->N = N;
    img->data = (double**)malloc(N * sizeof(double*));
    for (int i = 0; i < N; i++) {
        img->data[i] = (double*)malloc(N * sizeof(double));
    }
    return img;
}

// Free image memory
void free_image(Image *img) {
    if (img) {
        for (int i = 0; i < img->N; i++) {
            free(img->data[i]);
        }
        free(img->data);
        free(img);
    }
}

// Read image from text file
Image* read_image(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return NULL;
    }
    
    int N, N2;
    if (fscanf(file, "%d %d", &N, &N2) != 2) {
        printf("Error: Invalid file format (cannot read dimensions)\n");
        fclose(file);
        return NULL;
    }
    
    if (N != N2) {
        printf("Error: Image must be square (N x N)\n");
        fclose(file);
        return NULL;
    }
    
    Image *img = create_image(N);
    
    // Read pixel values
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (fscanf(file, "%lf", &img->data[i][j]) != 1) {
                printf("Error: Invalid file format at position (%d, %d)\n", i, j);
                free_image(img);
                fclose(file);
                return NULL;
            }
            
            // Validate range [0, 1]
            if (img->data[i][j] < 0.0 || img->data[i][j] > 1.0) {
                printf("Warning: Pixel value at (%d, %d) = %f is out of range [0, 1]\n", 
                       i, j, img->data[i][j]);
            }
        }
    }
    
    fclose(file);
    return img;
}

// Calculate MSE (Mean Squared Error)
double calculate_mse(Image *img1, Image *img2) {
    double mse = 0.0;
    int N = img1->N;
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double diff = img1->data[i][j] - img2->data[i][j];
            mse += diff * diff;
        }
    }
    
    mse /= (N * N);
    return mse;
}

// Calculate PSNR
double calculate_psnr(double mse, double max_pixel_value) {
    if (mse == 0) {
        return INFINITY; // Images are identical
    }
    
    double psnr = 10.0 * log10((max_pixel_value * max_pixel_value) / mse);
    return psnr;
}

// Calculate local region mean
double calculate_mean(Image *img, int x, int y, int window_size) {
    double sum = 0.0;
    int count = 0;
    int half_window = window_size / 2;
    int N = img->N;
    
    for (int j = -half_window; j <= half_window; j++) {
        for (int i = -half_window; i <= half_window; i++) {
            int nx = x + i;
            int ny = y + j;
            
            // Boundary check
            if (nx >= 0 && nx < N && ny >= 0 && ny < N) {
                sum += img->data[ny][nx];
                count++;
            }
        }
    }
    
    return sum / count;
}

// Calculate local region variance
double calculate_variance(Image *img, int x, int y, int window_size, double mean) {
    double sum = 0.0;
    int count = 0;
    int half_window = window_size / 2;
    int N = img->N;
    
    for (int j = -half_window; j <= half_window; j++) {
        for (int i = -half_window; i <= half_window; i++) {
            int nx = x + i;
            int ny = y + j;
            
            if (nx >= 0 && nx < N && ny >= 0 && ny < N) {
                double diff = img->data[ny][nx] - mean;
                sum += diff * diff;
                count++;
            }
        }
    }
    
    return sum / count;
}

// Calculate local region covariance
double calculate_covariance(Image *img1, Image *img2, int x, int y, 
                           int window_size, double mean1, double mean2) {
    double sum = 0.0;
    int count = 0;
    int half_window = window_size / 2;
    int N = img1->N;
    
    for (int j = -half_window; j <= half_window; j++) {
        for (int i = -half_window; i <= half_window; i++) {
            int nx = x + i;
            int ny = y + j;
            
            if (nx >= 0 && nx < N && ny >= 0 && ny < N) {
                double diff1 = img1->data[ny][nx] - mean1;
                double diff2 = img2->data[ny][nx] - mean2;
                sum += diff1 * diff2;
                count++;
            }
        }
    }
    
    return sum / count;
}

// Calculate SSIM
double calculate_ssim(Image *img1, Image *img2) {
    int window_size = 11; // Common window size
    double ssim_sum = 0.0;
    int count = 0;
    int N = img1->N;
    
    // Sliding window across the image
    for (int y = 0; y < N; y += window_size / 2) {
        for (int x = 0; x < N; x += window_size / 2) {
            // Calculate local statistics
            double mean1 = calculate_mean(img1, x, y, window_size);
            double mean2 = calculate_mean(img2, x, y, window_size);
            
            double var1 = calculate_variance(img1, x, y, window_size, mean1);
            double var2 = calculate_variance(img2, x, y, window_size, mean2);
            
            double cov = calculate_covariance(img1, img2, x, y, window_size, mean1, mean2);
            
            // SSIM formula
            double numerator = (2 * mean1 * mean2 + C1) * (2 * cov + C2);
            double denominator = (mean1 * mean1 + mean2 * mean2 + C1) * (var1 + var2 + C2);
            
            double ssim_value = numerator / denominator;
            ssim_sum += ssim_value;
            count++;
        }
    }
    
    return ssim_sum / count;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <image1.txt> <image2.txt>\n", argv[0]);
        printf("Example: %s original.txt compressed.txt\n", argv[0]);
        printf("\nInput file format:\n");
        printf("  Line 1: N N (image dimensions)\n");
        printf("  Next N lines: N real numbers per line (range [0, 1])\n");
        return 1;
    }
    
    // Load first image
    printf("Loading image 1: %s\n", argv[1]);
    Image *img1 = read_image(argv[1]);
    if (!img1) {
        return 1;
    }
    
    // Load second image
    printf("Loading image 2: %s\n", argv[2]);
    Image *img2 = read_image(argv[2]);
    if (!img2) {
        free_image(img1);
        return 1;
    }
    
    // Check if image dimensions match
    if (img1->N != img2->N) {
        printf("Error: Image dimensions do not match\n");
        printf("Image1: %dx%d, Image2: %dx%d\n", img1->N, img1->N, img2->N, img2->N);
        free_image(img1);
        free_image(img2);
        return 1;
    }
    
    printf("\n=================================================\n");
    printf("Image Quality Comparison Tool\n");
    printf("=================================================\n\n");
    
    printf("Image Information:\n");
    printf("  Dimensions: %d x %d\n", img1->N, img1->N);
    printf("  Total pixels: %d\n", img1->N * img1->N);
    printf("\n");
    
    // Calculate MSE
    printf("Computing...\n");
    double mse = calculate_mse(img1, img2);
    
    // Calculate PSNR (max pixel value is 1.0 for normalized images)
    double psnr = calculate_psnr(mse, 1.0);
    
    // Calculate SSIM
    double ssim = calculate_ssim(img1, img2);
    
    printf("\n-------------------------------------------------\n");
    printf("Results:\n");
    printf("-------------------------------------------------\n");
    printf("  MSE:   %.6f\n", mse);
    printf("  PSNR:  %.2f dB\n", psnr);
    printf("  SSIM:  %.6f\n", ssim);
    printf("-------------------------------------------------\n");
    
    printf("\n");
    printf("Notes:\n");
    printf("  • Input values are normalized to [0, 1] range\n");
    printf("  • PSNR is based on pixel differences, higher is better\n");
    printf("  • SSIM is based on structural similarity, range 0-1, closer to 1 is better\n");
    printf("  • SSIM better matches human visual perception\n");
    printf("=================================================\n");
    
    // Free memory
    free_image(img1);
    free_image(img2);
    
    return 0;
}