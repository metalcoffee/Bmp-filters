#pragma once
#include <iostream>
#include <vector>
const double CONST_PIXEL = 255.0;
struct Color {
    double m_red_, m_green_, m_blue_;
    Color();
    Color(double r, double g, double b);
};

struct Image {
    Image();
    Image(int32_t width, int32_t height);
    void ReadBmp(const char* path);
    void WriteBmp(const char* path);
    int32_t m_width_;
    int32_t m_height_;
    std::vector<std::vector<Color>> m_image_;
};
