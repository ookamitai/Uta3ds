#ifndef _UI_H_
#define _UI_H_
#include <algorithm>
#include <cmath>
#include <fstream>
#include "screen.h"

typedef struct ColorText {
    std::string prefix;
    std::string content;

    std::vector<Character> output() const {
        std::vector<Character> ret = std::vector<Character>(content.length());
        for (size_t i = 0; i < content.length(); i++)
            ret[i] = Character(content[i], prefix);
        return ret;
    }

    ColorText() = default;

    ColorText(const std::string &content, const std::string &prefix)
        : prefix(prefix), content(content) {}
} ColorText;

typedef struct UI {
public:
    void render_log(const std::vector<Character> &text) {
        size_t x = 0;
        for (; x < screen->size().x; x++) {
            screen->set(Coord(x, screen->size().y - 1), Character(0));
        }
        x = 0;
        for (auto &&s : text) {
            screen->set(Coord(x, screen->size().y - 1), s);
            x++;
        }
    }
    void update() { screen->show(); }
    void clear() { screen->clear(); }
    const Coord &size() const noexcept { return screen->size(); }
    explicit UI(Screen *screen) : screen(screen) {}

private:
    void render_text(size_t *x, size_t y, const ColorText &text) {
        for (auto &&s : text.output()) {
            screen->set(Coord(*x, y), s);
            (*x)++;
        }
    }

    Screen *screen;
} UI;

#endif