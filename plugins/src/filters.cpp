
// #include "menubar.hpp"
// #include "canvas.hpp"
#include "../headers/filters.hpp"

namespace psapi {

    std::unique_ptr<IAction> FiltersButton::createAction(const IRenderWindow *renderWindow_, const Event &event_) {
        return std::make_unique<FiltersButtonAction>(this, renderWindow_, event_);
    }

    std::unique_ptr<IAction> BlurTool::createAction(const IRenderWindow *renderWindow_, const Event &event_) {
        return std::make_unique<BlurToolAction>(this, renderWindow_, event_);
    }

    std::unique_ptr<IAction> BrightnessTool::createAction(const IRenderWindow *renderWindow_, const Event &event_) {
        return std::make_unique<BrightnessToolAction>(this, renderWindow_, event_);
    }

    std::unique_ptr<IAction> NegativeTool::createAction(const IRenderWindow *renderWindow_, const Event &event_) {
        return std::make_unique<NegativeToolAction>(this, renderWindow_, event_);
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                      FiltersAction
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    void FiltersButton::updateState(const IRenderWindow *render_window_, const Event &event_) {
        getActionController()->execute(AMenuButton::createAction(render_window_, event_));
    }

    FiltersButtonAction::FiltersButtonAction(FiltersButton *button_, const IRenderWindow *render_window_, const Event &event_)
    :   AAction(render_window_, event_), button(button_) {}

    bool FiltersButtonAction::isUndoable(const Key &key) {
        return false;
    }

    bool FiltersButtonAction::execute(const Key &key)
    {
        button->updateState(render_window, event);
        if (button->getState() == IBarButton::State::Press) {
            button->activateMenu();
            getActionController()->execute(button->menu->createAction(render_window, event));
        } else if (button->getState() == IBarButton::State::Released){
            button->deactivateMenu();
        }

        return true;
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                      BlurToolAction
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    void BlurTool::updateState(const IRenderWindow *render_window_, const Event &event_) {
        vec2i mouse_pos = sfm::Mouse::getPosition(render_window_);
        vec2i pos = this->getPos();
        vec2u size = this->getSize();

        if (pos.x <= mouse_pos.x && mouse_pos.x <= pos.x + size.x &&
            pos.y <= mouse_pos.y && mouse_pos.y <= pos.y + size.y) {
            if (event_.type == Event::MouseButtonPressed) {
                this->state = IBarButton::State::Press;
            } else if (event_.type == Event::MouseButtonReleased) {
                this->state = IBarButton::State::Released;
            } else {
                this->state = IBarButton::State::Hover;
            }
        } else if (event_.type == Event::MouseButtonReleased) {
            this->state = IBarButton::State::Released;
        } else {
            this->state = IBarButton::State::Normal;
        }
    }

    std::vector<std::vector<double>> BlurToolAction::generateGaussKernel(int radius, double sigma) {
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

    sfm::Color BlurToolAction::applyGaussianBlur(int x, int y, Canvas* canvas, const std::vector<std::vector<double>>& kernel) {
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

    BlurToolAction::BlurToolAction(BlurTool *blur_, const IRenderWindow *render_window_, const Event &event_)
    :   AAction(render_window_, event_), blur(blur_) {}

    bool BlurToolAction::isUndoable(const Key &key) {
        return false;
    }

    bool BlurToolAction::execute(const Key &key) {
        blur->updateState(render_window, event);
        Canvas* canvas = static_cast<Canvas*>(getRootWindow()->getWindowById(kCanvasWindowId));
        ILayer* temp_layer = canvas->getTempLayer();
        Layer layer(canvas->layer_size);
        vec2i mouse_pos    = canvas->getMousePosition();
        vec2i canvas_pos   = canvas->getPos();
        if (blur->getState() == IBarButton::State::Press) {
            int radius = 3;
            double sigma = 1.0;
            std::vector<std::vector<double>> kernel = generateGaussKernel(radius, sigma);
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
        return true;
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                  BrightnessToolAction
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    void BrightnessTool::updateState(const IRenderWindow *render_window_, const Event &event_) {
        vec2i mouse_pos = sfm::Mouse::getPosition(render_window_);
        vec2i pos = this->getPos();
        vec2u size = this->getSize();

        if (pos.x <= mouse_pos.x && mouse_pos.x <= pos.x + size.x &&
            pos.y <= mouse_pos.y && mouse_pos.y <= pos.y + size.y) {
            if (event_.type == Event::MouseButtonPressed) {
                this->state = IBarButton::State::Press;
            } else if (event_.type == Event::MouseButtonReleased) {
                this->state = IBarButton::State::Released;
            } else {
                this->state = IBarButton::State::Hover;
            }
        } else if (event_.type == Event::MouseButtonReleased) {
            this->state = IBarButton::State::Released;
        } else {
            this->state = IBarButton::State::Normal;
        }
    }

    std::vector<std::vector<double>> BrightnessToolAction::generateGaussKernel(int radius, double sigma) {
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

    sfm::Color BrightnessToolAction::applySharpening(int x, int y, Canvas* canvas, const std::vector<std::vector<double>>& kernel, double alpha) {
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

    BrightnessToolAction::BrightnessToolAction(BrightnessTool *brightness_, const IRenderWindow *render_window_, const Event &event_)
    :   AAction(render_window_, event_), brightness(brightness_) {}

    bool BrightnessToolAction::isUndoable(const Key &key) {
        return false;
    }

    bool BrightnessToolAction::execute(const Key &key) {
        brightness->updateState(render_window, event);
        Canvas* canvas = static_cast<Canvas*>(getRootWindow()->getWindowById(kCanvasWindowId));
        ILayer* temp_layer = canvas->getTempLayer();
        Layer layer(canvas->layer_size);
        vec2i canvas_size = {static_cast<int>(canvas->getSize().x), static_cast<int>(canvas->getSize().y)};

        int radius = 3;
        double sigma = 1.0;
        double alpha = 1.5;
        std::vector<std::vector<double>> kernel = generateGaussKernel(radius, sigma);

        if (brightness->getState() == IBarButton::State::Press) {
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
        return true;
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                  NegativeToolAction
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    void NegativeTool::updateState(const IRenderWindow *render_window_, const Event &event_) {
        vec2i mouse_pos = sfm::Mouse::getPosition(render_window_);
        vec2i pos = this->getPos();
        vec2u size = this->getSize();

        if (pos.x <= mouse_pos.x && mouse_pos.x <= pos.x + size.x &&
            pos.y <= mouse_pos.y && mouse_pos.y <= pos.y + size.y) {
            if (event_.type == Event::MouseButtonPressed) {
                this->state = IBarButton::State::Press;
            } else if (event_.type == Event::MouseButtonReleased) {
                this->state = IBarButton::State::Released;
            } else {
                this->state = IBarButton::State::Hover;
            }
        } else if (event_.type == Event::MouseButtonReleased) {
            this->state = IBarButton::State::Released;
        } else {
            this->state = IBarButton::State::Normal;
        }
    }

    NegativeToolAction::NegativeToolAction(NegativeTool *negative_, const IRenderWindow *render_window_, const Event &event_)
    :   AAction(render_window_, event_), negative(negative_) {}

    bool NegativeToolAction::isUndoable(const Key &key) {
        return false;
    }

    bool NegativeToolAction::execute(const Key &key) {
        negative->updateState(render_window, event);
        Canvas* canvas = static_cast<Canvas*>(getRootWindow()->getWindowById(kCanvasWindowId));
        ILayer* temp_layer = canvas->getTempLayer();
        Layer layer(canvas->layer_size);
        vec2i canvas_size = {static_cast<int>(canvas->getSize().x), static_cast<int>(canvas->getSize().y)};

        if (negative->getState() == IBarButton::State::Press) {
            for (int i = 0; i < canvas->layer_size.x; i++) {
                for (int j = 0; j < canvas->layer_size.y; j++) {
                    sfm::Color col = temp_layer->getPixel({i, j});
                    int k = -1;
                    temp_layer->setPixel({i, j}, {static_cast<uint8_t>((col.r - 255) * k),
                                                  static_cast<uint8_t>((col.g - 255) * k),
                                                  static_cast<uint8_t>((col.b - 255) * k),
                                                  255});
                }
            }
        }
        return true;
    }

extern "C" {
        __attribute__((visibility("default"))) bool loadPlugin() {
            auto menubar = static_cast<IBar*>(getRootWindow()->getWindowById(kMenuBarWindowId));
            // ChildInfo info = menubar->getNextChildInfo();
            vec2i pos = {menubar->getPos().x + 100, menubar->getPos().y};
            vec2u size = {100, 50};
            auto button = std::make_unique<FiltersButton>(pos, size, kMenuFilterId);

            pos = {100, 50};
            size = {100, 20};
            auto gaus = std::make_unique<BlurTool>(pos, size, 1);
            button->addMenuItem(std::move(gaus));

            pos = {100, 70};
            auto brightness = std::make_unique<BrightnessTool>(pos, size, 2);
            button->addMenuItem(std::move(brightness));

            pos = {100, 90};
            auto negative = std::make_unique<NegativeTool>(pos, size, 3);
            button->addMenuItem(std::move(negative));

            if (menubar) {
                menubar->addWindow(std::move(button));
                std::cout << "add Window Succsesfull\n";
                return true;
            }
            std::cout << "not add Window Succsesfull\n";
            assert(menubar);
            return false;
        }

        __attribute__((visibility("default"))) void unloadPlugin() {
            IBar* menubar = static_cast<IBar*>(getRootWindow()->getWindowById(kToolBarWindowId));
            if (menubar) {
                menubar->removeWindow(1);
            }
        }
    }
}
