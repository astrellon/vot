#include "font_manager.h"

#include <iostream>

namespace vot
{
    FontManager::FontMap FontManager::s_fonts;

    bool FontManager::init()
    {
        return load_default_fonts();
    }
    void FontManager::deinit()
    {
        s_fonts.clear();
    }

    const FontManager::FontMap &FontManager::fonts()
    {
        return s_fonts;
    }

    bool FontManager::load_font(const std::string &name, const std::string &filename)
    {
        if (name.empty())
        {
            return false;
        }

        sf::Font font;
        if (!font.loadFromFile(filename))
        {
            return false;
        }

        s_fonts[name] = font;
        return true;
    }

    const sf::Font *FontManager::font(const std::string &name)
    {
        auto find = s_fonts.find(name);
        if (find == s_fonts.end())
        {
            return nullptr;
        }

        return &find->second;
    }
    
    bool FontManager::load_default_fonts()
    {
        if (!load_font("mono", "data/fonts/LiberationMono-Regular.ttf"))
        {
            std::cout << "Failed to load mono font\n";
            return false;
        }
        if (!load_font("sans", "data/fonts/LiberationSans-Regular.ttf"))
        {
            std::cout << "Failed to load sans font\n";
            return false;
        }
        return true;
    }
}

