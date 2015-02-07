#include "font_manager.h"

#include <iostream>

namespace vot
{
    FontManager *FontManager::s_main = nullptr;

    const FontManager::FontMap &FontManager::fonts() const
    {
        return _fonts;
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

        _fonts[name] = font;
        return true;
    }

    const sf::Font *FontManager::font(const std::string &name) const
    {
        auto find = _fonts.find(name);
        if (find == _fonts.end())
        {
            return nullptr;
        }

        return &find->second;
    }
    
    bool FontManager::load_default_fonts()
    {
        if (!load_font("mono", "data/LiberationMono-Regular.ttf"))
        {
            std::cout << "Failed to load mono font\n";
            return false;
        }
        if (!load_font("sans", "data/LiberationSans-Regular.ttf"))
        {
            std::cout << "Failed to load sans font\n";
            return false;
        }
        return true;
    }

    void FontManager::main(FontManager *manager)
    {
        s_main = manager;
    }
    FontManager *FontManager::main()
    {
        return s_main;
    }
}

