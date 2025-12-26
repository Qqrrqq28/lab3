#include "image.h"
#include "filters.h"

#include <iostream>
#include <memory>
#include <vector>

int main() {
    try {
        Image image;
        image.load("test.ppm");
        std::cout << "загружено изображение " << image.getWidth() << "x" << image.getHeight() << std::endl;
        
        std::vector<std::unique_ptr<IImageFilter>> pipeline;
        pipeline.reserve(2);
        
        pipeline.push_back(std::make_unique<GrayscaleFilter>());
        pipeline.push_back(std::make_unique<InvertFilter>());
        
        std::cout << "применяем фильтры" << std::endl;
        applyFilterPipeline(pipeline, image);
        
        image.save("result.ppm");
        std::cout << "рез сохр в result.ppm" << std::endl;
        
        Image original;
        original.load("test.ppm");
        
        Image combined = image + original;
        combined.save("combined.ppm");
        std::cout << "результат наложения в combined.ppm" << std::endl;
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "ошибка " << e.what() << std::endl;
        return 1;
    }
}
