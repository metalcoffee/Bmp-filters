#pragma once
#include "Image.h"
const double M_RED_NUM = 0.299;
const double M_GREEN_NUM = 0.587;
const double M_BLUE_NUM = 0.114;
const int64_t MATRIX_CONST = 5;
const double KERNEL_CONST = 3.0;
struct Filters {
    Filters();
    void Crop(Image& image, int32_t width, int32_t height);
    void Negative(Image& image);
    void Grayscale(Image& image);
    void GaussianBlur(Image& image, double sigma);
    void Sharpening(Image& image);
    void EdgeDetection(Image& image, double threshold);
};
