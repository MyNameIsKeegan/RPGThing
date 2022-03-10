#include "main.hpp"

class button {
    public:
    sf::Text label;
    sf::RectangleShape btn;
    string id;
    int r = 0 , g = 0, b = 0;
    bool isActive = 0;
    button *upBtn = nullptr, *downBtn = nullptr, *leftBtn = nullptr, *rightBtn = nullptr;
    button (): label(), btn() {}
    button (string id, int w, int h, int x, int y, int r, int g, int b): label(), btn({(float)w, (float)h}), id(id), r(r), g(g), b(b) {
        this->btn.setPosition(x, y);
        this->btn.setFillColor(sf::Color(r, g, b, 155));
        this->label.setString(id);
        this->label.setPosition(x+5.f, y+5.f);
    }
    button (string id, sf::Vector2f size, sf::Vector2f pos, int r, int g, int b): label(), btn(size), id(id), r(r), g(g), b(b) {
        this->btn.setPosition(pos);
        this->btn.setFillColor(sf::Color(r, g, b, 155));
        this->label.setString(id);
        this->label.setPosition(pos.x+5.f, pos.y+5.f);
    }
    void SetActive() { //move to menu and set activeBtn?
        this->isActive = 1;
        this->btn.setFillColor(sf::Color(r, g, b, 255));
    }
    void SetInactive() {
        this->isActive = 0;
        this->btn.setFillColor(sf::Color(r, g, b, 155));
    }
    void link(char d, button* btn) {
        switch (d) {
            case 'u':
            this->upBtn = btn;
            btn->downBtn = this;
            break;
            case 'd':
            this->downBtn = btn;
            btn->upBtn = this;
            break;
            case 'l':
            this->leftBtn = btn;
            btn->rightBtn = this;
            break;
            case 'r':
            this->rightBtn = btn;
            btn->leftBtn = this;
            break;
        }
    }
};

class menu {
    public:
    vector <button*> btnmap;
    button* activeBtn = nullptr;
    void navigate(char dir) {
        switch (dir) {
            case 'u':
            if (activeBtn->upBtn != nullptr) {
                activeBtn->SetInactive();
                activeBtn->upBtn->SetActive();
                activeBtn = activeBtn->upBtn;
            }
            break;
            case 'd':
            if (activeBtn->downBtn != nullptr) {
                activeBtn->SetInactive();
                activeBtn->downBtn->SetActive();
                activeBtn = activeBtn->downBtn;
            }
            break;
            case 'l':
            if (activeBtn->leftBtn != nullptr) {
                activeBtn->SetInactive();
                activeBtn->leftBtn->SetActive();
                activeBtn = activeBtn->leftBtn;
            }
            break;
            case 'r':
            if (activeBtn->rightBtn != nullptr) {
                activeBtn->SetInactive();
                activeBtn->rightBtn->SetActive();
                activeBtn = activeBtn->rightBtn;
            }
            break;
        }
    }
    string actButton () {
        return activeBtn->id;
    }
};

class bar {
    public:
    sf::RectangleShape base, fill;
    sf::Text label;
    bar() : base(), fill(), label() {}
    bar(int w, int h, int br, int bg, int bb, int fr, int fg, int fb): base({(float)w, (float)h}), fill({(float)w, (float)h}), label() {
        this->base.setFillColor(sf::Color(br, bg, bb));
        this->base.setOutlineColor(sf::Color(0, 0, 0, 0));
        this->fill.setFillColor(sf::Color(fr, fg, fb));
        this->fill.setOutlineColor(sf::Color(0, 0, 0, 0));
        this->label.setCharacterSize(25);
        this->label.setOutlineThickness(1);
    }
    void update(auto cur, auto max) {
        this->fill.setScale((float)cur/max, 1.f);
        this->label.setString(to_string(cur)+" / "+to_string(max));
    }
};