#ifndef TOOLBAR_HPP
#define TOOLBAR_HPP

//#include "api_photoshop.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint/Standard/api_bar.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint/implementation/my_sfm.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint/Standard/api_sfm.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint/Standard/api_canvas.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint/Standard/api_photoshop.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint/implementation/api_system.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint/implementation/canvas.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint/implementation/photoshop.hpp"

#include <cstdint>
#include <memory>
#include <iostream>
#include <assert.h>

namespace psapi {

// const wid_t kToolBarWindowId    = 101;
// const wid_t kOptionsBarWindowId = 102;

class BrightnessTool : public ABarButton {
private:
    std::vector<vec2i> points_arr;
public:
    BrightnessTool(vec2i pos_, vec2u size_, wid_t id_)
        : ABarButton(pos_, size_, id_),  points_arr() {
        if (!texture.loadFromFile("/Users/dima/MIPT/SecondSem/MyPaint/source/Button_3.png")) {
             std::cerr << "Error opening file\n";
        }
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect(0, 0, size.x, size.y));
        sprite.setScale(1, 1);
        sprite.setColor(sfm::Color(255, 255, 255, 255));
        sprite.setPosition(pos.x, pos.y);
    }

    virtual void action() override ;

    double CatmullRom(double p0, double p1, double p2, double p3, double t);
    vec2i operator() (double t);
    std::vector<std::vector<double>> generateGaussKernel(int radius, double sigma);
    sfm::Color applySharpening(int x, int y, Canvas* canvas, const std::vector<std::vector<double>>& kernel, double alpha);

    virtual ~BrightnessTool() = default;
};

extern "C" {
    bool loadPlugin();
    void unloadPlugin();
}

}

#endif
