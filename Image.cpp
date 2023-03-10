
#include "Image.h"
#include <sstream>
#include <fstream>

#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t bfType;
    [[maybe_unused]] uint32_t bfSize;
    [[maybe_unused]] uint16_t bfReserved1;
    [[maybe_unused]] uint16_t bfReserved2;
    [[maybe_unused]] uint32_t bfOffBits;
};

struct BMPInfoHeader {
    [[maybe_unused]] uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    [[maybe_unused]] uint16_t biPlanes;
    uint16_t biBitCount;
    [[maybe_unused]] uint32_t biCompression;
    uint32_t biSizeImage;
    [[maybe_unused]] int32_t biXPelsPerMeter;
    [[maybe_unused]] int32_t biYPelsPerMeter;
    [[maybe_unused]] uint32_t biClrUsed;
    [[maybe_unused]] uint32_t biClrImportant;
};
#pragma pack(pop)

void Image::ReadBmp(const char* path) {

    std::ifstream bmp_file(path, std::ios::binary);
    if (!bmp_file) {
        std::cerr << "Error: Unable to open file " << path << std::endl;
        return;
    }

    BMPFileHeader file_header;
    bmp_file.read(reinterpret_cast<char*>(&file_header), sizeof(BMPFileHeader));
    // If first are BM
    if (file_header.bfType != 0x4D42) {  // NOLINT
        std::cerr << "Error: File is not a BMP" << std::endl;
        return;
    }

    BMPInfoHeader info_header;
    bmp_file.read(reinterpret_cast<char*>(&info_header), sizeof(BMPInfoHeader));
    if (info_header.biBitCount != 24) {  // NOLINT
        std::cerr << "Error: File is not a 24-bit BMP" << std::endl;
        return;
    }

    m_width_ = info_header.biWidth;
    m_height_ = info_header.biHeight;

    m_image_.resize(m_height_);
    for (auto& elem : m_image_) {
        elem.resize(m_width_);
    }

    // Read the pixel data
    for (int i = 0; i < m_height_; i++) {
        for (int j = 0; j < m_width_; j++) {
            unsigned char data[3];
            bmp_file.read(reinterpret_cast<char*>(data), 3);
            m_image_[m_height_ - i - 1][j] =
                Color(static_cast<double>(data[2] / CONST_PIXEL), static_cast<double>(data[1] / CONST_PIXEL),
                      static_cast<double>(data[0] / CONST_PIXEL));
        }
        // Skip over any padding bytes
        bmp_file.seekg((4 - ((m_width_ * 3) % 4)) % 4, std::ios::cur);
    }
    std::cout << "Read from " << path << std::endl;
}

void Image::WriteBmp(const char* path) {
    std::ofstream bmp_file(path, std::ios::binary);

    BMPFileHeader file_header;
    file_header.bfType = 0x4D42;  // NOLINT
    file_header.bfReserved1 = 0;
    file_header.bfReserved2 = 0;
    file_header.bfOffBits = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

    BMPInfoHeader info_header;
    info_header.biSize = sizeof(BMPInfoHeader);
    info_header.biWidth = m_width_;
    info_header.biHeight = m_height_;
    info_header.biPlanes = 1;
    info_header.biBitCount = 24;  // NOLINT
    info_header.biCompression = 0;
    info_header.biSizeImage = ((24 * m_width_ + 31) / 32) * 4 * m_height_;  // NOLINT
    info_header.biXPelsPerMeter = 0;
    info_header.biYPelsPerMeter = 0;
    info_header.biClrUsed = 0;
    info_header.biClrImportant = 0;

    file_header.bfSize = info_header.biSizeImage + sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

    bmp_file.write(reinterpret_cast<char*>(&file_header), sizeof(BMPFileHeader));
    bmp_file.write(reinterpret_cast<char*>(&info_header), sizeof(BMPInfoHeader));

    for (int i = 0; i < m_height_; i++) {
        for (int j = 0; j < m_width_; j++) {
            Color color = m_image_[(m_height_ - i - 1)][j];
            unsigned char color2[] = {static_cast<unsigned char>(color.m_blue_ * CONST_PIXEL),
                                      static_cast<unsigned char>(color.m_green_ * CONST_PIXEL),
                                      static_cast<unsigned char>(color.m_red_ * CONST_PIXEL)};
            bmp_file.write(reinterpret_cast<char*>(color2), 3);
        }
        // Add padding
        for (int j = 0; j < (4 - ((m_width_ * 3) % 4)) % 4; j++) {
            bmp_file.write("\0", sizeof(uint8_t));
        }
    }
    std::cout << "Wrote into " << path << std::endl;
}

Image::Image() {
}
Color::Color() : m_red_(0), m_green_(0), m_blue_(0) {
}

Color::Color(double r, double g, double b) : m_red_(r), m_green_(g), m_blue_(b) {
}

Image::Image(int32_t width, int32_t height) : m_width_(width), m_height_(height) {
    std::vector<std::vector<Color>> tmp(height, std::vector<Color>(width));
    m_image_ = tmp;
}
