#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <map>

namespace vot
{
    class FontManager
    {
        public:
            static bool init();
            static void deinit();

            typedef std::map<std::string, sf::Font> FontMap;
            static const FontMap &fonts();

            static bool load_font(const std::string &name, const std::string &filename);
            static const sf::Font *font(const std::string &name);

            static bool load_default_fonts();

        private:

            static FontMap s_fonts;
    };
}

