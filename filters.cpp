#include "filters.h"

#include <cmath>
#include <map>

void Grayscale::Filter(Image &my_image) {
    for (size_t i = 0; i < my_image.height; i++) {
        for (size_t j = 0; j < my_image.width; j++) {
            my_image.data_[i][j].red =
                static_cast<uint8_t>(my_image.data_[i][j].red * 0.299 + 0.587 * my_image.data_[i][j].green +
                                     0.114 * my_image.data_[i][j].blue);
            my_image.data_[i][j].green = my_image.data_[i][j].red;
            my_image.data_[i][j].blue = my_image.data_[i][j].red;
        }
    }
}

void Negative::Filter(Image &my_image) {
    for (size_t i = 0; i < my_image.height; i++) {
        for (size_t j = 0; j < my_image.width; j++) {
            my_image.data_[i][j].red = 255 - my_image.data_[i][j].red;
            my_image.data_[i][j].green = 255 - my_image.data_[i][j].green;
            my_image.data_[i][j].blue = 255 - my_image.data_[i][j].blue;
        }
    }
}

void Sharpening::Filter(Image &my_image) {
    std::vector<std::vector<Color>> temp = my_image.data_;
    for (size_t i = 0; i < my_image.height; i++) {
        for (size_t j = 0; j < my_image.width; j++) {
            int red = 0;
            int green = 0;
            int blue = 0;

            size_t new_i = i;
            if (i != 0) {
                --new_i;
            }
            red -= my_image.data_[new_i][j].red;
            green -= my_image.data_[new_i][j].green;
            blue -= my_image.data_[new_i][j].blue;

            size_t new_j = j;
            if (j != 0) {
                --new_j;
            }
            red -= my_image.data_[i][new_j].red;
            green -= my_image.data_[i][new_j].green;
            blue -= my_image.data_[i][new_j].blue;

            new_i = i;
            if (i != my_image.height - 1) {
                ++new_i;
            }
            red -= my_image.data_[new_i][j].red;
            green -= my_image.data_[new_i][j].green;
            blue -= my_image.data_[new_i][j].blue;

            new_j = j;
            if (j != my_image.width - 1) {
                ++new_j;
            }
            red -= my_image.data_[i][new_j].red;
            green -= my_image.data_[i][new_j].green;
            blue -= my_image.data_[i][new_j].blue;

            red += 5 * my_image.data_[i][j].red;
            green += 5 * my_image.data_[i][j].green;
            blue += 5 * my_image.data_[i][j].blue;

            temp[i][j].red = std::clamp(red, 0, UINT8_MAX);
            temp[i][j].green = std::clamp(green, 0, UINT8_MAX);
            temp[i][j].blue = std::clamp(blue, 0, UINT8_MAX);
        }
    }
    my_image.data_ = temp;
}

EdgeDetection::EdgeDetection(int thr) {
    threshold_ = thr;
}

void EdgeDetection::Filter(Image &my_image) {
    Grayscale to_gr;
    to_gr.Filter(my_image);
    std::vector<std::vector<Color>> temp = my_image.data_;
    for (size_t i = 0; i < my_image.height; i++) {
        for (size_t j = 0; j < my_image.width; j++) {
            int red = 0;

            if (i == 0) {
                red -= my_image.data_[i][j].red;
            } else {
                red -= my_image.data_[i - 1][j].red;
            }

            if (j == 0) {
                red -= my_image.data_[i][j].red;
            } else {
                red -= my_image.data_[i][j - 1].red;
            }

            if (i == my_image.height - 1) {
                red -= my_image.data_[i][j].red;
            } else {
                red -= my_image.data_[i + 1][j].red;
            }

            if (j == my_image.width - 1) {
                red -= my_image.data_[i][j].red;
            } else {
                red -= my_image.data_[i][j + 1].red;
            }
            red += 4 * my_image.data_[i][j].red;
            if (red > threshold_) {
                temp[i][j].red = 255;
                temp[i][j].green = 255;
                temp[i][j].blue = 255;
            } else {
                temp[i][j].red = 0;
                temp[i][j].green = 0;
                temp[i][j].blue = 0;
            }
        }
    }
    my_image.data_ = temp;
}

void Crop::Filter(Image &my_image) {
    if (static_cast<int>(my_image.height) > height_) {
        my_image.printing_start_y = my_image.height - height_;
        my_image.height = height_;
    }
    if (static_cast<int>(my_image.width) > width_) {
        my_image.width = width_;
    }
}

void GaussianBlur::Filter(Image &my_image) {
    //std::cout << "start of GaussianBlur" << std::endl;
    std::vector<std::vector<Color>> temp = my_image.data_;
    auto accuracy = static_cast<int>(std::ceil(3 * sigma_));
    std::map<int, double> koef;
    for (int k = -accuracy; k <= accuracy; ++k) {
        koef[k] =
            (1. / (std::sqrt(2. * M_PI) * sigma_)) * exp(-1. * static_cast<double>(k * k) / (2 * sigma_ * sigma_));
    }
    for (size_t i = 0; i < my_image.height; ++i) {
        for (size_t j = 0; j < my_image.width; ++j) {
            double red = 0;
            double green = 0;
            double blue = 0;
            for (int k = -accuracy; k <= accuracy; ++k) {
                size_t new_j = 0;
                if (k + j >= 0 && j + k < my_image.width) {
                    new_j = k + j;
                } else if (j + k >= my_image.width) {
                    new_j = my_image.width - 1;
                }
                red += koef[k] * my_image.data_[i][new_j].red;
                green += koef[k] * my_image.data_[i][new_j].green;
                blue += koef[k] * my_image.data_[i][new_j].blue;
            }
            temp[i][j].red = static_cast<uint8_t>(red);
            temp[i][j].green = static_cast<uint8_t>(green);
            temp[i][j].blue = static_cast<uint8_t>(blue);
        }
    }
    //std::cout << " second part " << std::endl;
    for (size_t i = 0; i < my_image.height; i++) {
        for (size_t j = 0; j < my_image.width; j++) {
            double red = 0;
            double green = 0;
            double blue = 0;
            for (int k = -accuracy; k <= accuracy; ++k) {
                size_t new_i = 0;
                if (k + i >= 0 && k + i < my_image.height) {
                    new_i = k + i;
                } else if (k + i >= my_image.height) {
                    new_i = my_image.height - 1;
                }
                red += koef[k] * temp[new_i][j].red;
                green += koef[k] * temp[new_i][j].green;
                blue += koef[k] * temp[new_i][j].blue;
            }
            my_image.data_[i][j].red = static_cast<uint8_t>(red);
            my_image.data_[i][j].green = static_cast<uint8_t>(green);
            my_image.data_[i][j].blue = static_cast<uint8_t>(blue);
        }
    }
    //std::cout << "end of GaussianBlur" << std::endl;
}

void ApplyFilters(Arguments &arg, Image &my_image) {
    if (arg.gr_) {
        Grayscale gr;
        gr.Filter(my_image);
    }
    if (arg.neg_) {
        Negative ng;
        ng.Filter(my_image);
    }
    if (arg.sharp_) {
        Sharpening sh;
        sh.Filter(my_image);
    }
    if (get<0>(arg.edge_)) {
        EdgeDetection edge(get<1>(arg.edge_));
        edge.Filter(my_image);
    }
    if (get<0>(arg.my_filter_)) {
        MyFilter my_filter(get<1>(arg.my_filter_));
        my_filter.Filter(my_image);
    }
    if (get<0>(arg.blur_)) {
        GaussianBlur blur(get<1>(arg.blur_));
        blur.Filter(my_image);
    }
    if (get<0>(arg.crop_)) {
        Crop crop(get<1>(arg.crop_), get<2>(arg.crop_));
        crop.Filter(my_image);
    }
}

void MyFilter::Filter(Image &my_image) {
    for (size_t i = 0; i < my_image.height; i += h_) {
        for (size_t j = 0; j < my_image.width; j += h_) {
            int red = 0;
            int green = 0;
            int blue = 0;
            for (size_t i1 = i; i1 < i + h_; ++i1) {
                for (size_t j1 = j; j1 < j + h_; ++j1) {
                    if (i1 >= 0 && i1 < my_image.height && j1 >= 0 && j1 < my_image.width) {
                        red += my_image.data_[i1][j1].red;
                        green += my_image.data_[i1][j1].green;
                        blue += my_image.data_[i1][j1].blue;
                    }
                }
            }
            auto new_red = static_cast<uint8_t>(red / (h_ * h_));
            auto new_green = static_cast<uint8_t>(green / (h_ * h_));
            auto new_blue = static_cast<uint8_t>(blue / (h_ * h_));
            for (size_t i1 = i; i1 < i + h_; ++i1) {
                for (size_t j1 = j; j1 < j + h_; ++j1) {
                    if (i1 >= 0 && i1 < my_image.height && j1 >= 0 && j1 < my_image.width) {
                        my_image.data_[i1][j1].red = new_red;
                        my_image.data_[i1][j1].green = new_green;
                        my_image.data_[i1][j1].blue = new_blue;
                    }
                }
            }
        }
    }
}