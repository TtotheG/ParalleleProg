#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

//time: 3.4633 seconds

// Include that allows to print result as an image
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// Default size of image
#define X 1280
#define Y 720
#define MAX_ITER 10000

void calc_mandelbrot(uint8_t image[Y][X])

{

    double xmax = 1;
    double xmin = -2.5;
    double ymax = 1;
    double ymin = -1;
    double pixelWidth = (xmax - xmin) / X;
    double pixelHeight = (ymax - ymin) / Y;

#pragma omp for
    for (int yi = 0; yi < Y; yi++)
    {
        for (int xi = 0; xi < X; xi++)
        {
            double x = 0.0;
            double y = 0.0;
            double cx = xmin + xi * pixelWidth;
            double cy = ymax - yi * pixelHeight;
            int iteration = 0;
            while (x * x + y * y <= 2 * 2 && iteration < MAX_ITER)
            {
                double x_tmp = x * x - y * y + cx;
                y = 2 * x * y + cy;
                x = x_tmp;
                iteration++;
            }
            double norm_iteration = iteration / 255;
            image[yi][xi] = norm_iteration;
        }
    }
}
int main()
{
    double startTime = omp_get_wtime();
    uint8_t image[Y][X];
#pragma omp parallel
    calc_mandelbrot(image);

    const int channel_nr = 1, stride_bytes = 0;
    stbi_write_png("mandelbrot.png", X, Y, channel_nr, image, stride_bytes);
    double endTime = omp_get_wtime();
    printf("time: %2.4f seconds\n", endTime - startTime);
    return EXIT_SUCCESS;
}
