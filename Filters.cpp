
#include "Filters.h"
#include <iostream>
#include <cmath>

void Filters::Crop(Image& image, int32_t width, int32_t height) {
    if (height < image.m_height_) {
        image.m_image_.resize(height);
        for (auto& elem : image.m_image_) {
            if (width < image.m_width_) {
                elem.resize(width);
            } else {
                break;
            }
        }
    } else {
        for (auto& elem : image.m_image_) {
            if (width < image.m_width_) {
                elem.resize(width);
            } else {
                break;
            }
        }
    }
    image.m_width_ = width;
    image.m_height_ = height;
}

void Filters::Negative(Image& image) {
    for (auto& elem : image.m_image_) {
        for (auto& pixel : elem) {
            pixel = Color(1.0 - pixel.m_red_, 1 - pixel.m_green_, 1.0 - pixel.m_blue_);
        }
    }
}
void Filters::Grayscale(Image& image) {
    for (auto& elem : image.m_image_) {
        for (auto& pixel : elem) {
            double new_color = M_RED_NUM * pixel.m_red_ + M_GREEN_NUM * pixel.m_green_ + M_BLUE_NUM * pixel.m_blue_;
            pixel = Color(new_color, new_color, new_color);
        }
    }
}

std::vector<double> GaussianKernel(int k, double sigma) {
    std::vector<double> kernel(2 * k + 1);
    double sum = 0.0;
    for (int i = -k; i <= k; i++) {
        double value = exp(-(i * i) / (2 * sigma * sigma));
        kernel[i + k] = value;
        sum += value;
    }
    for (int i = 0; i < kernel.size(); i++) {
        kernel[i] /= sum;
    }
    return kernel;
}

void Filters::GaussianBlur(Image& input, double sigma) {
    int k = static_cast<int32_t>(KERNEL_CONST * sigma);
    std::vector<double> kernel = GaussianKernel(k, sigma);
    Image temp1(input.m_width_, input.m_height_);
    Image temp2(input.m_width_, input.m_height_);

    // Blur horizontally using kernel
    for (int y = 0; y < input.m_height_; y++) {
        for (int x = 0; x < input.m_width_; x++) {
            double r = 0.0;
            double g = 0.0;
            double b = 0.0;
            double w = 0.0;
            for (int i = -k; i <= k; i++) {
                int j = x + i;
                if (j < 0) {
                    j = 0;
                }
                if (j >= input.m_width_) {
                    j = input.m_width_ - 1;
                }
                double weight = kernel[i + k];
                r += input.m_image_[y][j].m_red_ * weight;
                g += input.m_image_[y][j].m_green_ * weight;
                b += input.m_image_[y][j].m_blue_ * weight;
                w += weight;
            }
            if (w > 0.0) {
                r /= w;
                g /= w;
                b /= w;
            }
            temp1.m_image_[y][x] = Color(r, g, b);
        }
    }

    // Blur vertically using kernel
    for (int y = 0; y < input.m_height_; y++) {
        for (int x = 0; x < input.m_width_; x++) {
            double r = 0.0;
            double g = 0.0;
            double b = 0.0;
            double w = 0.0;
            for (int i = -k; i <= k; i++) {
                int j = y + i;
                if (j < 0) {
                    j = 0;
                }
                if (j >= input.m_height_) {
                    j = input.m_height_ - 1;
                }
                double weight = kernel[i + k];
                r += temp1.m_image_[j][x].m_red_ * weight;
                g += temp1.m_image_[j][x].m_green_ * weight;
                b += temp1.m_image_[j][x].m_blue_ * weight;
                w += weight;
            }
            if (w > 0.0) {
                r /= w;
                g /= w;
                b /= w;
            }
            temp2.m_image_[y][x] = Color(r, g, b);
        }
    }

    for (int y = 0; y < input.m_height_; y++) {
        for (int x = 0; x < input.m_width_; x++) {
            input.m_image_[y][x] = temp2.m_image_[y][x];
        }
    }
}

void Filters::Sharpening(Image& image) {
    std::vector<std::vector<Color>> tmp = image.m_image_;
    for (int i = 0; i < image.m_height_; ++i) {
        for (int j = 0; j < image.m_width_; ++j) {
            tmp[i][j] =
                Color(std::min(1.0, std::max(0.0, image.m_image_[i][j].m_red_ * MATRIX_CONST -
                                                      image.m_image_[std::max(0, i - 1)][j].m_red_ -
                                                      image.m_image_[i][std::max(0, j - 1)].m_red_ -
                                                      image.m_image_[std::min(image.m_height_ - 1, i + 1)][j].m_red_ -
                                                      image.m_image_[i][std::min(image.m_width_ - 1, j + 1)].m_red_)),
                      std::min(1.0, std::max(0.0, image.m_image_[i][j].m_green_ * MATRIX_CONST -
                                                      image.m_image_[std::max(0, i - 1)][j].m_green_ -
                                                      image.m_image_[i][std::max(0, j - 1)].m_green_ -
                                                      image.m_image_[std::min(image.m_height_ - 1, i + 1)][j].m_green_ -
                                                      image.m_image_[i][std::min(image.m_width_ - 1, j + 1)].m_green_)),
                      std::min(1.0, std::max(0.0, image.m_image_[i][j].m_blue_ * MATRIX_CONST -
                                                      image.m_image_[std::max(0, i - 1)][j].m_blue_ -
                                                      image.m_image_[i][std::max(0, j - 1)].m_blue_ -
                                                      image.m_image_[std::min(image.m_height_ - 1, i + 1)][j].m_blue_ -
                                                      image.m_image_[i][std::min(image.m_width_ - 1, j + 1)].m_blue_)));
        }
    }
    image.m_image_ = std::move(tmp);
}

void Filters::EdgeDetection(Image& image, double threshold) {
    Grayscale(image);
    std::vector<std::vector<Color>> tmp = image.m_image_;
    for (int i = 0; i < image.m_height_; ++i) {
        for (int j = 0; j < image.m_width_; ++j) {
            tmp[i][j] = Color(image.m_image_[i][j].m_red_ * 4 - image.m_image_[std::max(0, i - 1)][j].m_red_ -
                                  image.m_image_[i][std::max(0, j - 1)].m_red_ -
                                  image.m_image_[std::min(image.m_height_ - 1, i + 1)][j].m_red_ -
                                  image.m_image_[i][std::min(image.m_width_ - 1, j + 1)].m_red_,
                              image.m_image_[i][j].m_green_ * 4 - image.m_image_[std::max(0, i - 1)][j].m_green_ -
                                  image.m_image_[i][std::max(0, j - 1)].m_green_ -
                                  image.m_image_[std::min(image.m_height_ - 1, i + 1)][j].m_green_ -
                                  image.m_image_[i][std::min(image.m_width_ - 1, j + 1)].m_green_,
                              image.m_image_[i][j].m_blue_ * 4 - image.m_image_[std::max(0, i - 1)][j].m_blue_ -
                                  image.m_image_[i][std::max(0, j - 1)].m_blue_ -
                                  image.m_image_[std::min(image.m_height_ - 1, i + 1)][j].m_blue_ -
                                  image.m_image_[i][std::min(image.m_width_ - 1, j + 1)].m_blue_);
            if (tmp[i][j].m_blue_ > threshold || tmp[i][j].m_red_ > threshold || tmp[i][j].m_green_ > threshold) {
                tmp[i][j] = Color(1, 1, 1);
            } else {
                tmp[i][j] = Color(0, 0, 0);
            }
        }
    }
    image.m_image_ = std::move(tmp);
}
Filters::Filters() {
}
