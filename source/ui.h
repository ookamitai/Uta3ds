#ifndef _UI_H_
#define _UI_H_
#include <algorithm>
#include <cmath>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <functional>
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

typedef std::pair<ColorText, std::function<void(void)>> MENU_PAIR;
typedef struct Menu {
    std::vector<MENU_PAIR> items;
    ColorText name;
    Menu(const ColorText &n, const std::vector<MENU_PAIR> &m) : items(m), name(n) {}
} Menu;

typedef struct UI {
public:
    void draw_menu(const Menu &p, size_t sel) {
        size_t x = 0, y = 0;
        render_text(&x, y, p.name);
        y += 2; x = 0;
        for (size_t a = 0; a < p.items.size(); a++) {
            render_text(&x, y, ColorText(((sel == a) ? " >" : "  ") + p.items[a].first.content, p.items[a].first.prefix));
            // render_text(&x, y, p.items[a].first);
            y++; x = 0;
        }
    }
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
    void render_text(size_t *x, size_t y, const ColorText &text) {
        for (auto &&s : text.output()) {
            screen->set(Coord(*x, y), s);
            (*x)++;
        }
    }
    Screen *screen;
} UI;

#endif