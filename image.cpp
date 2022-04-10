#include <iostream>
#include <fstream>
#include "image.h"
#include "exceptions.h"


Image::Image(Arguments& arg) {
    std::ifstream input_stream(arg.input_file_, std::ios::binary);

    if (!input_stream.is_open()) {
        throw WrongInputPath();
    }

    input_stream.read(reinterpret_cast<char *>(&type), sizeof(type));
    if (type != 0x4D42) {
        throw BMPHeaderError();
    }
    input_stream.read(reinterpret_cast<char *>(&size), sizeof(size));
    input_stream.read(reinterpret_cast<char *>(&reserved1), sizeof(reserved1));
    input_stream.read(reinterpret_cast<char *>(&reserved2), sizeof(reserved2));
    input_stream.read(reinterpret_cast<char *>(&offset), sizeof(offset));
    input_stream.read(reinterpret_cast<char *>(&dib_size), sizeof(dib_size));
    input_stream.read(reinterpret_cast<char *>(&width), sizeof(width));

    input_stream.read(reinterpret_cast<char *>(&height), sizeof(height));
    printing_end_y = static_cast<size_t>(height);

    input_stream.read(reinterpret_cast<char *>(&planes), sizeof(planes));
    if (planes != 0x1) {
        throw BMPHeaderError();
    }
    input_stream.read(reinterpret_cast<char *>(&bpp), sizeof(bpp));
    if (bpp != 0x18) {
        throw BMPHeaderError();
    }
    input_stream.read(reinterpret_cast<char *>(&compression), sizeof(compression));
    if (compression != 0x0) {
        throw BMPHeaderError();
    }
    input_stream.read(reinterpret_cast<char *>(&image_size), sizeof(image_size));
    input_stream.read(reinterpret_cast<char *>(&x_ppm), sizeof(x_ppm));
    input_stream.read(reinterpret_cast<char *>(&y_ppm), sizeof(y_ppm));
    input_stream.read(reinterpret_cast<char *>(&num_colors), sizeof(num_colors));
    input_stream.read(reinterpret_cast<char *>(&important_colors), sizeof(important_colors));

    auto padding_size = static_cast<std::streamsize>((width * 3) % 4);

    for (size_t i = 0; i < height; i++) {
        std::vector<Color> temp_vec;
        for (size_t j = 0; j < width; j++) {
            Color temp;
            input_stream.read(reinterpret_cast<char *>(&temp.red), sizeof(Color::red));
            input_stream.read(reinterpret_cast<char *>(&temp.green), sizeof(Color::red));
            input_stream.read(reinterpret_cast<char *>(&temp.blue), sizeof(Color::red));
            temp_vec.push_back(temp);
        }
        if (padding_size) {
            int32_t padding;

            input_stream.read(reinterpret_cast<char *>(&padding), 4 - padding_size);
        }

        data_.push_back(temp_vec);
    }

}

void Image::Print(Arguments& arg) {
    std::ofstream output_stream(arg.output_file_, std::ios::binary);
    std::cout<< arg.output_file_ << std::endl;
    if(!output_stream.is_open()) {
        throw WrongOutputPath();
    }

    output_stream.write(reinterpret_cast<char *>(&type), sizeof(type));
    output_stream.write(reinterpret_cast<char *>(&size), sizeof(size));
    output_stream.write(reinterpret_cast<char *>(&reserved1), sizeof(reserved1));
    output_stream.write(reinterpret_cast<char *>(&reserved2), sizeof(reserved2));
    output_stream.write(reinterpret_cast<char *>(&offset), sizeof(offset));
    output_stream.write(reinterpret_cast<char *>(&dib_size), sizeof(dib_size));
    output_stream.write(reinterpret_cast<char *>(&width), sizeof(width));
    output_stream.write(reinterpret_cast<char *>(&height), sizeof(height));
    output_stream.write(reinterpret_cast<char *>(&planes), sizeof(planes));
    output_stream.write(reinterpret_cast<char *>(&bpp), sizeof(bpp));
    output_stream.write(reinterpret_cast<char *>(&compression), sizeof(compression));
    output_stream.write(reinterpret_cast<char *>(&image_size), sizeof(image_size));
    output_stream.write(reinterpret_cast<char *>(&x_ppm), sizeof(x_ppm));
    output_stream.write(reinterpret_cast<char *>(&y_ppm), sizeof(y_ppm));
    output_stream.write(reinterpret_cast<char *>(&num_colors), sizeof(num_colors));
    output_stream.write(reinterpret_cast<char *>(&important_colors), sizeof(important_colors));

    auto padding_size = static_cast<std::streamsize>((width * 3) % 4);

    for (size_t i = printing_start_y; i < printing_end_y; ++i) {
        for (size_t j = 0; j < width; ++j) {
            output_stream.write(reinterpret_cast<char *>(&data_[i][j].red), sizeof(Color::red));
            output_stream.write(reinterpret_cast<char *>(&data_[i][j].green), sizeof(Color::red));
            output_stream.write(reinterpret_cast<char *>(&data_[i][j].blue), sizeof(Color::red));
        }
        if (padding_size) {
            int32_t padding;
            output_stream.write(reinterpret_cast<char *>(&padding), 4 - padding_size);
        }
    }
}

