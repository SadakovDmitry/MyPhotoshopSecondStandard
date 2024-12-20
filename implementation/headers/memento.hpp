#ifndef MEMENTO_HPP
#define MEMENTO_HPP

// #include "api_photoshop.hpp"
// #include "api_sfm.hpp"
#include "/Users/dima/MIPT/SecondSem/MyPaint2.0/Standard/api_memento.hpp"

#include "toolbar.hpp"
#include "my_sfm.hpp"
#include "buttons.hpp"

#include <cstdint>
#include <memory>
#include <iostream>

namespace psapi {

struct SnapshotType {
    std::vector<sfm::Color> pixels;
    std::vector<std::unique_ptr<sfm::Drawable>>  drawables;
    vec2u size;
};

class LayerSnapshot : public ILayerSnapshot {
public:
    std::vector<sfm::Color> pixels;
    std::vector<std::unique_ptr<sfm::Drawable>> drawables;
    vec2u size;

    LayerSnapshot(const std::vector<sfm::Color>& pixels_,
                  std::vector<std::unique_ptr<sfm::Drawable>>&& drawables_,
                  const vec2u& size_)
        : pixels(pixels_), drawables(std::move(drawables_)), size(size_) {}
};

class CanvasSnapshot : public ICanvasSnapshot {
public:
    std::vector<std::unique_ptr<sfm::Drawable>> drawables;
    vec2u size;

    CanvasSnapshot(std::vector<std::unique_ptr<sfm::Drawable>>&& drawables_,
                  const vec2u& size_)
        : drawables(std::move(drawables_)), size(size_) {}
};

} // namespace

#endif // MEMENTO_HPP

