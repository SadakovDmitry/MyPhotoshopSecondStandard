
// #include "toolbar.hpp"
// #include "canvas.hpp"
#include "blur.hpp"
#include <vector>
#include <iostream>
#include <dlfcn.h>
#include <cmath>

namespace psapi {

    void BlurTool::action() {
        Canvas* canvas = static_cast<Canvas*>(getRootWindow()->getWindowById(kCanvasWindowId));
        ILayer* temp_layer = canvas->getTempLayer();
        //canvas->insertEmptyLayer();
        Layer layer(canvas->layer_size);
        vec2i mouse_pos    = canvas->getMousePosition();
        vec2i canvas_pos   = canvas->getPos();

            // if (points_arr.size() < 4) {
            //     points_arr.push_back(mouse_pos);
            // } else {
            //     points_arr.erase(points_arr.begin());
            //     points_arr.push_back(mouse_pos);
    	    //     for (double t = 0; t <= 1; t += 0.001) {
            //         vec2i point = (*this)(t + 1);
            //         if (canvas->isPressed()) {
            //             //temp_layer->setPixel(point, {0, 255, 0, 255});
            //             //for (int )
            //         }
    	    //     }
            // }
        if (canvas->isPressed()) {
            int radius = 3;
            double sigma = 1.0;
            std::vector<std::vector<double>> kernel = generateGaussKernel(radius, sigma);
            // for (int i = -1; i <= 1; i++) {
            //     for (int j = -1; j <= 1; j++) {
            //         sfm::Color col = temp_layer->getPixel({mouse_pos.x + i, mouse_pos.y + j});
            //         res_col = res_col + col;
            //         // размытие квардата 3 на 3
            //         //temp_layer->setPixel({mouse_pos + i, mouse_pos + j}, {0, 255, 0, 255});
            //     }
            // }
            // sfm::Color res_col = applyGaussianBlur(mouse_pos.x, mouse_pos.y, canvas, kernel);
            // temp_layer->setPixel(mouse_pos, res_col);
            // std::cout << canvas->layer_size.x <<  " : " << canvas->layer_size.y << "\n";
            for (int i = 0; i < canvas->layer_size.x; i++) {
                for (int j = 0; j < canvas->layer_size.y; j++) {
                    sfm::Color res_col = applyGaussianBlur(i, j, canvas, kernel);
                    layer.setPixel({i, j}, res_col);
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

    std::vector<std::vector<double>> BlurTool::generateGaussKernel(int radius, double sigma) {
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

    sfm::Color BlurTool::applyGaussianBlur(int x, int y, Canvas* canvas, const std::vector<std::vector<double>>& kernel) {
    ILayer* temp_layer = canvas->getTempLayer();
    int radius = kernel.size() / 2;
    int width = canvas->layer_size.x;
    int height = canvas->layer_size.y;
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

    return {
        static_cast<unsigned char>(std::clamp(sum_r, 0.0, 255.0)),
        static_cast<unsigned char>(std::clamp(sum_g, 0.0, 255.0)),
        static_cast<unsigned char>(std::clamp(sum_b, 0.0, 255.0)),
        static_cast<unsigned char>(std::clamp(sum_a, 0.0, 255.0))
    };
    }

    double BlurTool::CatmullRom(double p0, double p1, double p2, double p3, double t){
        double t2   = t * t;
        double t3   = t2 * t;
        double t3_2 = t3 * 2;
        double t2_3 = t2 * 3;

        double hermite00 =  t3_2 - t2_3 + 1;
        double hermite10 =  t3   - t2 * 2 + t;
        double hermite01 = -t3_2 + t2_3;
        double hermite11 =  t3   - t2;

        double m0 = (p2 - p0) / 2;
        double m1 = (p3 - p1) / 2;

        return std::max(hermite00 * p1 + hermite10 * m0 * 1.0 +
               hermite01 * p2 + hermite11 * m1 * 1.0, 0.0);
    }

    vec2i BlurTool::operator() (double t){
        std::vector<vec2i>::iterator iter = points_arr.begin();
        int i = t / 1;
        iter += i;
        t -= i;
        vec2i temp_point {0, 0};

        temp_point.x = CatmullRom((*(iter - 1)).x, (*iter).x, (*(iter + 1)).x, (*(iter + 2)).x, t);
        temp_point.y = CatmullRom((*(iter - 1)).y, (*iter).y, (*(iter + 1)).y, (*(iter + 2)).y, t);
        return temp_point;
    }


    extern "C" {
        __attribute__((visibility("default"))) bool loadPlugin() {
            auto toolbar = static_cast<IBar*>(getRootWindow()->getWindowById(kToolBarWindowId));
            ChildInfo info = toolbar->getNextChildInfo();
            auto pencil = std::make_unique<BlurTool>(info.pos, info.size, 4);

            if (toolbar) {
                toolbar->addWindow(std::move(pencil));
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
