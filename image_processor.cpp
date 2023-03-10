#include "Image.h"
#include "Parser.h"
#include <unordered_map>
#include "Filters.h"

int main(int argc, char** argv) {
    Parser parser(argc, argv);
    Filters application;
    Image image;
    image.ReadBmp(parser.input_path_);
    for (const auto& elem : parser.GetFiltersArgs()) {
        if (elem.first == "-crop") {
            application.Crop(image, static_cast<int32_t>(elem.second.first), static_cast<int32_t>(elem.second.second));
        } else if (elem.first == "-neg") {
            application.Negative(image);
        } else if (elem.first == "-gs") {
            application.Grayscale(image);
        } else if (elem.first == "-sharp") {
            application.Sharpening(image);
        } else if (elem.first == "-edge") {
            application.EdgeDetection(image, elem.second.first);
        } else if (elem.first == "-blur") {
            application.GaussianBlur(image, elem.second.first);
        }
    }
    image.WriteBmp(parser.output_path_);
    return 0;
}
