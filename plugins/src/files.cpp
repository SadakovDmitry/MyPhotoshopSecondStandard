
// #include "menubar.hpp"
// #include "canvas.hpp"
#include "../headers/files.hpp"
#include <QFileDialog>
#include <QDir>
#include <QString>
#include <fstream>

namespace psapi {

    FileButton::FileButton(vec2i pos_, vec2u size_, wid_t id_, const std::string& file)
        : AMenuButton(pos_, size_, id_) {
        if (!texture.loadFromFile(file)) {
             std::cerr << "Error opening file\n";
        }
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        sprite.setScale(1, 1);
        sprite.setColor(sfm::Color(255, 255, 255, 255));
        sprite.setPosition(pos.x, pos.y);
        vec2i menu_pos = {pos.x + static_cast<int>(getSize().x), pos.y + static_cast<int>(getSize().y)};
        vec2u menu_size = {size.x, 50};
        menu = new AMenu(menu_pos, menu_size);
    }

    SaveTool::SaveTool(vec2i pos_, vec2u size_, wid_t id_, const std::string& file)
        : ABarButton(pos_, size_, id_) {
        if (!texture.loadFromFile(file)) {
            std::cerr << "Error opening file\n";
        }
        hovered_sprite.setTexture(&texture);
        hovered_sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        hovered_sprite.setScale(1.5, 1.5);
        hovered_sprite.setColor(sfm::Color(200, 200, 200, 255));
        hovered_sprite.setPosition(pos.x, pos.y);

        if (!texture.loadFromFile(file)) {
            std::cerr << "Error opening file\n";
        }
        pressed_sprite.setTexture(&texture);
        pressed_sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        pressed_sprite.setScale(1.5, 1.5);
        pressed_sprite.setColor(sfm::Color(150, 150, 150, 255));
        pressed_sprite.setPosition(pos.x, pos.y);

        if (!texture.loadFromFile(file)) {
            std::cerr << "Error opening file\n";
        }
        released_sprite.setTexture(&texture);
        released_sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        released_sprite.setScale(1.5, 1.5);
        released_sprite.setColor(sfm::Color(255, 255, 255, 255));
        released_sprite.setPosition(pos.x, pos.y);

        if (!texture.loadFromFile(file)) {
            std::cerr << "Error opening file\n";
        }
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        sprite.setScale(1.5, 1.5);
        sprite.setColor(sfm::Color(255, 255, 255, 255));
        sprite.setPosition(pos.x, pos.y);
    }

    LoadTool::LoadTool(vec2i pos_, vec2u size_, wid_t id_, const std::string& file)
        : ABarButton(pos_, size_, id_) {
        if (!texture.loadFromFile(file)) {
            std::cerr << "Error opening file\n";
        }
        hovered_sprite.setTexture(&texture);
        hovered_sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        hovered_sprite.setScale(1.5, 1.5);
        hovered_sprite.setColor(sfm::Color(200, 200, 200, 255));
        hovered_sprite.setPosition(pos.x, pos.y);

        if (!texture.loadFromFile(file)) {
            std::cerr << "Error opening file\n";
        }
        pressed_sprite.setTexture(&texture);
        pressed_sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        pressed_sprite.setScale(1.5, 1.5);
        pressed_sprite.setColor(sfm::Color(150, 150, 150, 255));
        pressed_sprite.setPosition(pos.x, pos.y);

        if (!texture.loadFromFile(file)) {
            std::cerr << "Error opening file\n";
        }
        released_sprite.setTexture(&texture);
        released_sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        released_sprite.setScale(1.5, 1.5);
        released_sprite.setColor(sfm::Color(255, 255, 255, 255));
        released_sprite.setPosition(pos.x, pos.y);

        if (!texture.loadFromFile(file)) {
            std::cerr << "Error opening file\n";
        }
        sprite.setTexture(&texture);
        sprite.setTextureRect(sfm::IntRect({0, 0}, size));
        sprite.setScale(1.5, 1.5);
        sprite.setColor(sfm::Color(255, 255, 255, 255));
        sprite.setPosition(pos.x, pos.y);
    }

    std::unique_ptr<IAction> FileButton::createAction(const IRenderWindow *renderWindow_, const Event &event_) {
        return std::make_unique<FileButtonAction>(this, renderWindow_, event_);
    }

    std::unique_ptr<IAction> LoadTool::createAction(const IRenderWindow *renderWindow_, const Event &event_) {
        return std::make_unique<LoadToolAction>(this, renderWindow_, event_);
    }

    std::unique_ptr<IAction> SaveTool::createAction(const IRenderWindow *renderWindow_, const Event &event_) {
        return std::make_unique<SaveToolAction>(this, renderWindow_, event_);
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                      FiltersAction
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    void FileButton::updateState(const IRenderWindow *render_window_, const Event &event_) {
        getActionController()->execute(AMenuButton::createAction(render_window_, event_));
    }

    FileButtonAction::FileButtonAction(FileButton *button_, const IRenderWindow *render_window_, const Event &event_)
    :   AAction(render_window_, event_), button(button_) {}

    bool FileButtonAction::isUndoable(const Key &key) {
        return false;
    }

    bool FileButtonAction::execute(const Key &key)
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
//                                                                      LoadToolAction
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    void LoadTool::updateState(const IRenderWindow *render_window_, const Event &event_) {
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

    LoadToolAction::LoadToolAction(LoadTool *load_, const IRenderWindow *render_window_, const Event &event_)
    :   AAction(render_window_, event_), load(load_) {}

    bool LoadToolAction::isUndoable(const Key &key) {
        return false;
    }

    void LoadToolAction::drawImage(ILayer* temp_layer) {
        for (uint32_t i = 0; i < load->loaded_image.getSize().x; i++) {
            for (uint32_t j = 0; j < load->loaded_image.getSize().y; j++) {
                temp_layer->setPixel({static_cast<int>(i),
                                      static_cast<int>(j)},
                                      load->loaded_image.getPixel({i, j}));
            }
        }
    }

    bool LoadToolAction::execute(const Key &key) {
        load->updateState(render_window, event);

        if (load->getState() == IBarButton::State::Press) {
            Canvas* canvas = static_cast<Canvas*>(getRootWindow()->getWindowById(kCanvasWindowId));
            ILayer* temp_layer = canvas->getTempLayer();

            QFileDialog fileDialog(nullptr, "Выберите изображение", QDir::homePath(),
                                   "Изображения (*.png *.jpg *.bmp);;Все файлы (*.*)");

            fileDialog.setOption(QFileDialog::DontUseNativeDialog, true);
            fileDialog.setModal(true);

            if (fileDialog.exec() == QDialog::Accepted) {
                QString fileName = fileDialog.selectedFiles().first();
                if (!fileName.isEmpty()) {
                    if (!load->loaded_image.loadFromFile(fileName.toStdString())) {
                        std::cerr << "\033[31mОшибка загрузки файла: \033[0m" << fileName.toStdString() << "\n";
                    }
                    drawImage(temp_layer);
                }
            } else {
                std::cerr << "Файл не выбран.\n";
            }
        }

        return true;
    }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                  SaveToolAction
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    void SaveTool::updateState(const IRenderWindow *render_window_, const Event &event_) {
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

    SaveToolAction::SaveToolAction(SaveTool *save_tool_, const IRenderWindow *render_window_, const Event &event_)
    :   AAction(render_window_, event_), save_tool(save_tool_) {}

    bool SaveToolAction::isUndoable(const Key &key) {
        return false;
    }

    bool SaveToolAction::execute(const Key &key) {
        save_tool->updateState(render_window, event);

        if (save_tool->getState() == IBarButton::State::Press) {
            Canvas* canvas = static_cast<Canvas*>(getRootWindow()->getWindowById(kCanvasWindowId));
            ILayer* temp_layer = canvas->getTempLayer();

            QString fileName = QFileDialog::getSaveFileName(nullptr,
                                                            "Сохранить изображение",
                                                            QDir::homePath(),
                                                            "Изображения (*.png *.jpg *.bmp);;Все файлы (*.*)");

            sf::Image image;
            sf::Texture save_texture;
            save_texture.create(temp_layer->getSize().x, temp_layer->getSize().y);
            sf::Sprite save_sprite;

            sf::RenderTexture render_texture;
            if (!render_texture.create(temp_layer->getSize().x, temp_layer->getSize().y)) {
                std::cerr << "\033[31mОшибка создания RenderTexture: \033[0m\n";
                return false;
            }

            render_texture.clear(sf::Color::Transparent);

            for (size_t i = 0; i < canvas->getNumLayers(); i++) {
                image.create(temp_layer->getSize().x, temp_layer->getSize().y);

                for (uint32_t x = 0; x < temp_layer->getSize().x; x++) {
                    for (uint32_t y = 0; y < temp_layer->getSize().y; y++) {
                        sfm::Color sfm_color = temp_layer->getPixel({static_cast<int>(x), static_cast<int>(y)});
                        image.setPixel(x, y, {sfm_color.r, sfm_color.g, sfm_color.b, sfm_color.a});
                    }
                }

                save_texture.update(image);
                save_sprite.setTexture(save_texture);
                render_texture.draw(save_sprite);
            }

            render_texture.display();

            sf::Image canvasImage = render_texture.getTexture().copyToImage();

            if (!fileName.isEmpty()) {
                if (!canvasImage.saveToFile(fileName.toStdString())) {
                    std::cerr << "\033[31mОшибка сохранения файла: \033[0m"
                              << fileName.toStdString() << "\n";
                    return false;
                }
                std::cout << "Файл успешно сохранён: " << fileName.toStdString() << "\n";
            } else {
                std::cerr << "Сохранение отменено.\n";
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
            auto button = std::make_unique<FileButton>(pos, size, kMenuFileId, "/Users/dima/MIPT/SecondSem/MyPaint2.0/images/Files.png");

            pos = {104, 50};
            size = {96, 20};
            auto gaus = std::make_unique<LoadTool>(pos, size, 1, "/Users/dima/MIPT/SecondSem/MyPaint2.0/images/load.png");
            button->addMenuItem(std::move(gaus));

            pos = {104, 80};
            auto save_tool = std::make_unique<SaveTool>(pos, size, 2, "/Users/dima/MIPT/SecondSem/MyPaint2.0/images/save.png");
            button->addMenuItem(std::move(save_tool));


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
