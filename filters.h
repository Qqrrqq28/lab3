#ifndef FILTERS_H
#define FILTERS_H

#include "image.h"

#include <string>
#include <memory>
#include <vector>
#include <iostream>

class IImageFilter {
public:
    virtual void process(Image& image) = 0;
    virtual std::string describe() const = 0;
    virtual ~IImageFilter() = default;
};

class BasicFilter: public IImageFilter {
protected:
    std::string name;
    
public:
    explicit BasicFilter(const std::string& filterName) : name(filterName) {}
    
    std::string describe() const override {
        return name;
    }
};

class GrayscaleFilter: public BasicFilter {
public:
    GrayscaleFilter(): BasicFilter("Grayscale Filter") {}
    
    void process(Image& image) override {
        auto& pixels = image.getPixels();
        for (auto& p: pixels) {
            unsigned char gray = static_cast<unsigned char>(
                0.299 * p.r + 0.587 * p.g + 0.114 * p.b
            );

            p.r = p.g = p.b = gray;
        }
    }
};

class InvertFilter: public BasicFilter {
public:
    InvertFilter(): BasicFilter("Invert Filter") {}
    
    void process(Image& image) override {
        auto& pixels = image.getPixels();
        for (auto& p: pixels) {
            p.r = 255 - p.r;
            p.g = 255 - p.g;
            p.b = 255 - p.b;
        }
    }
};

std::ostream& operator<<(std::ostream& os, const IImageFilter& filter) {
    os << "Filter: " << filter.describe();
    return os;
}

void applyFilterPipeline(const std::vector<std::unique_ptr<IImageFilter>>& pipeline, Image& image) {
    for (const auto& filter : pipeline) {
        if (dynamic_cast<const GrayscaleFilter*>(filter.get())) {
            std::cout << "Grayscale\n";
        } 
        else if (dynamic_cast<const InvertFilter*>(filter.get())) {
            std::cout << "Invert\n";
        }
        else {
            std::cout << "ХЗ фильтр\n";
        }
        
        filter->process(image);
    }
}

#endif // FILTERS_H
