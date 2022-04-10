#pragma once

#include "image.h"


class FilterClass {
public:
    virtual void Filter(Image &my_image) {};
};


class Grayscale : public FilterClass {
public:
    void Filter(Image &my_image) override;
};


class Negative : public FilterClass {
public:
    void Filter(Image &my_image) override;
};


class Sharpening : public FilterClass {
public:
    void Filter(Image &my_image) override;
};


class EdgeDetection : public FilterClass {
public:
    explicit EdgeDetection(int thr);

    void Filter(Image &my_image) override;

private:
    int threshold_ = 100;
};

class Crop : public FilterClass {
public:
    Crop(int width, int height) : width_(width), height_(height) {};

    void Filter(Image &my_image) override;

private:
    int width_;
    int height_;
};

class GaussianBlur : public FilterClass {
public:
    explicit GaussianBlur(double sigma) : sigma_(sigma) {};
    void Filter(Image &my_image) override;

private:
    double sigma_ = 0;
};

class MyFilter : public FilterClass {
public:
    explicit MyFilter(int h) : h_(h) {};
    void Filter(Image &my_image) override;

private:
    int h_ = 0;
};

void ApplyFilters(Arguments& arg, Image& my_image);