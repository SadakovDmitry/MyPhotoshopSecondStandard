
// #include "toolbar.hpp"
// #include "canvas.hpp"
#include "brightness.hpp"
#include <vector>
#include <iostream>
#include <dlfcn.h>
#include <cmath>

namespace psapi {

    std::vector<std::vector<double>> BrightnessTool::generateGaussKernel(int radius, double sigma) {
        int size = 2 * radius + 1;
        std::vector<std::vector<double>> kernel(size, std::vector<double>(size));
        double sum = 0.0;

        for (int i = -radius; i <= radius; i++) {
            for (int j = -radius; j <= radius; j++) {
                double value = (1.0 / (2.0 * M_PI * sigma * sigma)) * exp(-(i * i + j * j) / (2 * sigma * sigma));
                kernel[i + radius][j + radius] = value;
                sum += value;
            }
        }

        for (int i = 0; i < size; i++) {
         for (int j = 0; j < size; j++) {
                kernel[i][j] /= sum;
            }
        }

        return kernel;
    }

    sfm::Color BrightnessTool::applySharpening(int x, int y, Canvas* canvas, const std::vector<std::vector<double>>& kernel, double alpha) {
        ILayer* temp_layer = canvas->getTempLayer();
        int radius = kernel.size() / 2;
        int width = canvas->layer_size.x;
        int height = canvas->layer_size.y;

        sfm::Color original = temp_layer->getPixel({x, y});

        double sum_r = 0.0, sum_g = 0.0, sum_b = 0.0, sum_a = 0.0;
        for (int i = -radius; i <= radius; i++) {
            for (int j = -radius; j <= radius; j++) {
                int nx = x + i;
                int ny = y + j;

                if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                    sfm::Color pixel = temp_layer->getPixel({nx, ny});
                    double weight = kernel[i + radius][j + radius];
                    sum_r += pixel.r * weight;
                    sum_g += pixel.g * weight;
                    sum_b += pixel.b * weight;
                    sum_a += pixel.a * weight;
                }
            }
        }

        sfm::Color blurred = {
            static_cast<unsigned char>(std::clamp(sum_r, 0.0, 255.0)),
            static_cast<unsigned char>(std::clamp(sum_g, 0.0, 255.0)),
            static_cast<unsigned char>(std::clamp(sum_b, 0.0, 255.0)),
            static_cast<unsigned char>(std::clamp(sum_a, 0.0, 255.0))
        };

        sfm::Color sharpened = {
            static_cast<unsigned char>(std::clamp(original.r + alpha * (original.r - blurred.r), 0.0, 255.0)),
            static_cast<unsigned char>(std::clamp(original.g + alpha * (original.g - blurred.g), 0.0, 255.0)),
            static_cast<unsigned char>(std::clamp(original.b + alpha * (original.b - blurred.b), 0.0, 255.0)),
            static_cast<unsigned char>(original.a)
        };

        return sharpened;
    }

    void BrightnessTool::action() {
        Canvas* canvas = static_cast<Canvas*>(getRootWindow()->getWindowById(kCanvasWindowId));
        ILayer* temp_layer = canvas->getTempLayer();
        Layer layer(canvas->layer_size);
        vec2i canvas_size = {static_cast<int>(canvas->getSize().x), static_cast<int>(canvas->getSize().y)};

        int radius = 3;
        double sigma = 1.0;
        double alpha = 1.5;
        std::vector<std::vector<double>> kernel = generateGaussKernel(radius, sigma);

        if (canvas->isPressed()) {
            for (int i = 0; i < canvas->layer_size.x; i++) {
                for (int j = 0; j < canvas->layer_size.y; j++) {
                    sfm::Color sharpened = applySharpening(i, j, canvas, kernel, alpha);
                    layer.setPixel({i, j}, sharpened);
                }
            }
            for (int i = 0; i < canvas->layer_size.x; i++) {
                for (int j = 0; j < canvas->layer_size.y; j++) {
                    sfm::Color res_col = layer.getPixel({i, j});
                    temp_layer->setPixel({i, j}, res_col);
                }
            }
        }
    }


    extern "C" {
        __attribute__((visibility("default"))) bool loadPlugin() {
            auto toolbar = static_cast<IBar*>(getRootWindow()->getWindowById(kToolBarWindowId));
            ChildInfo info = toolbar->getNextChildInfo();
            auto brightness = std::make_unique<BrightnessTool>(info.pos, info.size, 5);

            if (toolbar) {
                toolbar->addWindow(std::move(brightness));
                return true;
            }
            assert(toolbar);
            return false;
        }

        __attribute__((visibility("default"))) void unloadPlugin() {
            IBar* toolbar = static_cast<IBar*>(getRootWindow()->getWindowById(kToolBarWindowId));
            if (toolbar) {
                toolbar->removeWindow(1);
            }
        }
    }
}
