#pragma once


#include <vector>
#include <cstdint>

#include "argparser.h"

struct Color {
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
};


class Image {
public:
    explicit Image(Arguments& arg);

    void Print(Arguments& arg);

    ////header
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
    uint32_t dib_size;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bpp;
    uint32_t compression;
    uint32_t image_size;
    uint32_t x_ppm;
    uint32_t y_ppm;
    uint32_t num_colors;
    uint32_t important_colors;


    ///pixels colors
    std::vector<std::vector<Color>> data_;


    ///variables for crop filter (to print right upper part of picture)
    size_t printing_start_y = 0;
    size_t printing_end_y;
};