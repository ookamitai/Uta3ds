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

typedef std::pair<std::string, std::function<void(void)>> menu_pair;
typedef struct Menu {
    std::vector<menu_pair> items;
    std::string name;
    Menu(const std::string& n, const std::vector<menu_pair>& m) : items(m), name(n) {}
} Menu;

typedef struct UI {
public:
    void draw_menu(const Menu &p, size_t sel) {
        size_t x = 0, y = 0;
        render_text(&x, y, ColorText(p.name, ""));
        y += 2; x = 0;
        for (size_t a = 0; a < p.items.size(); a++) {
            render_text(&x, y, ColorText(((sel == a) ? " >" : "  ") + p.items[a].first, ""));
            // render_text(&x, y, p.items[a].first);
            y++; x = 0;
        }
    }
    
    bool ask_dialogue(std::string&& question, bool comformation=false) {
        if (question.length() > 200) {
            question = "String too long to display.";
        }
        auto vtext = split_by_length(question);

        size_t x = 5, y = 13 - vtext.size() / 2;
        if (!comformation)
            render_text(&x, y, ColorText(" Ask Dialogue                 ", "\x1b[30;47m"));
        else render_text(&x, y, ColorText(" Comformation Dialogue        ", "\x1b[30;47m"));
        x = 5; y++;
        render_text(&x, y, ColorText());
        for (const auto& i : vtext) {
            x = 5; y++;
            render_text(&x, y, ColorText(i, ""));
        }
        x = 5; y++;
        render_text(&x, y, ColorText());
        x = 5; y++;
        if (!comformation) 
            render_text(&x, y, ColorText("      [A] OK  [B] Cancel      ", "\x1b[30;47m"));
        else render_text(&x, y, ColorText("         [A] Dismiss          ", "\x1b[30;47m"));
        update();

        while(true) {
            hidScanInput();
            u32 kDown = hidKeysDown();
            if (kDown & KEY_A) {
                return true;
            }
            if (kDown & KEY_B) {
                return false;
            }
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
    void render_menubar() {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream oss, sec;

        sec << std::put_time(&tm, "%S");
        
        if (std::stoi(sec.str()) % 2 == 0) {
            oss << std::put_time(&tm, "%m/%d %H:%M");
        } else {
            oss << std::put_time(&tm, "%m/%d %H %M");
        }

        auto str = oss.str();

        u8 level = 0;
        PTMU_GetBatteryLevel(&level);
        bool adapt;
        PTMU_GetAdapterState(&adapt);

        size_t x = 0;

        std::string i = (level == 1 ? "    " : (level == 2 ? "#   " : (level == 3 ? "##  " : (level == 4 ? "### " : "####"))));

        render_text(&x, 0, ColorText(" Uta3ds                " + str + " Battery:[", "\x1b[30;47m"));
        render_text(&x, 0, ColorText(i, level == 1 ? "\x1b[31:47m" : "\x1b[32;47m"));
        render_text(&x, 0, ColorText((adapt ? "]+" : "] "), "\x1b[30;47m"));
    }

private:
    std::vector<std::string> split_by_length(const std::string& str, int splitLength=30) {
        int NumSubstrings = str.length() / splitLength;
        std::vector<std::string> ret;

        for (auto i = 0; i < NumSubstrings; i++)
        {
                ret.push_back(str.substr(i * splitLength, splitLength));
        }

        // If there are leftover characters, create a shorter item at the end.
        if (str.length() % splitLength != 0)
        {
                ret.push_back(str.substr(splitLength * NumSubstrings));
        }


        return ret;
    }

    Screen *screen;
} UI;

#endif