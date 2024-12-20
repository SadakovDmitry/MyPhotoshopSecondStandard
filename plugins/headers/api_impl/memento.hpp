#ifndef PHOTOSHOP_MEMENTO_IMPLEMENTATION
#define PHOTOSHOP_MEMENTO_IMPLEMENTATION


#include <api/api_memento.hpp>
#include <api_impl/canvas.hpp>


class ALayerSnapshot : public psapi::ILayerSnapshot
{
    std::vector<Color> pixels_;
    vec2i offset_;
public:
    ALayerSnapshot(const std::vector<Color> pixels, vec2i offset);
    vec2i getOffset();
    Color operator[](size_t index);
};


class ACanvasSnapshot : public psapi::ICanvasSnapshot
{
public:
};


template<typename SnapShot>
class AMementable : public psapi::IMementable<SnapShot>
{
public:
    virtual std::unique_ptr<SnapShot> save() override;
    virtual void restore(SnapShot* snapshot) override;
};


#endif // PHOTOSHOP_MEMENTO_IMPLEMENTATION
