#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <algorithm>

struct Pixel {
    unsigned char r;
    unsigned char g; 
    unsigned char b;
    
    Pixel(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0): r(r), g(g), b(b) {}
};

class Image {
private:
    std::vector<Pixel> pixels;
    size_t width;
    size_t height;
    
public:
    Image(size_t w = 0, size_t h = 0): width(w), height(h) {
       pixels.reserve(w * h);
    }
    
    void load(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("не удалось открыть файл " + filename);
        }
        
        std::string magic;
        file >> magic;

        if (magic != "P3") {
            throw std::runtime_error("требуется P3");
        }
        
        char c;

        while (file.get(c) && c == '#') {
            file.ignore(10000, '\n');
        }
        file.putback(c);
        
        file >> width >> height;
        
        if (width == 0 || height == 0) {
            throw std::runtime_error("некорректные размеры изображения");
        }
        
        int max_val;
        file >> max_val;

        if (max_val != 255) {
            throw std::runtime_error("поддерживается только 8-битный формат");
        }
        
        pixels.clear();
        pixels.reserve(width * height);
        
        for (size_t i = 0; i < height; ++i) {
            for (size_t j = 0; j < width; ++j) {
                int r, g, b;
                file >> r >> g >> b;

                if (file.fail()) {
                    throw std::runtime_error("ошибка чтения пикселей из файла");
                }

                pixels.emplace_back(
                    static_cast<unsigned char>(r),
                    static_cast<unsigned char>(g),
                    static_cast<unsigned char>(b)
                );
            }
        }
    }
    
    void save(const std::string& filename) const {
        std::ofstream file(filename);

        if (!file.is_open()) {
            throw std::runtime_error("не удалось создать файл " + filename);
        }
        
        file << "P3\n";
        file << width << " " << height << "\n";
        file << "255\n";
        
        for (size_t i = 0; i < height; ++i) {
            for (size_t j = 0; j < width; ++j) {
                const Pixel& p = pixels[i * width + j];
                file << static_cast<int>(p.r) << " "
                     << static_cast<int>(p.g) << " "
                     << static_cast<int>(p.b) << " ";
            }
            
            file << "\n";
        }
    }
    
    Pixel getPixel(size_t x, size_t y) const {
        if (x >= width || y >= height) {
            throw std::out_of_range("координаты пикселя вне диапазона");
        }

        return pixels[y * width + x];
    }
    
    void setPixel(size_t x, size_t y, const Pixel& pixel) {
        if (x >= width || y >= height) {
            throw std::out_of_range("координаты пикселя вне диапазона");
        }

        pixels[y * width + x] = pixel;
    }
    
    size_t getWidth() const { return width; }
    size_t getHeight() const { return height; }
    std::vector<Pixel>& getPixels() { return pixels; }
    const std::vector<Pixel>& getPixels() const { return pixels; }
    
    Image operator+(const Image& other) const {
        size_t new_width = std::min(width, other.width);
        size_t new_height = std::min(height, other.height);
        Image result(new_width, new_height);
        
        for (size_t y = 0; y < new_height; ++y) {
            for (size_t x = 0; x < new_width; ++x) {
                Pixel p1 = getPixel(x, y);
                Pixel p2 = other.getPixel(x, y);
                result.setPixel(x, y, Pixel(
                    std::min(255, static_cast<int>(p1.r) + p2.r),
                    std::min(255, static_cast<int>(p1.g) + p2.g),
                    std::min(255, static_cast<int>(p1.b) + p2.b)
                ));
            }
        }
        
        return result;
    }
};


#endif // IMAGE_H
