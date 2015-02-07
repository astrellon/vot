#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <map>
#include <memory>

namespace vot
{
    class FontManager
    {
        public:
            typedef std::map<std::string, sf::Font> FontMap;
            const FontMap &fonts() const;

            bool load_font(const std::string &name, const std::string &filename);
            const sf::Font *font(const std::string &name) const;

            bool load_default_fonts();

            static void main(FontManager *manager);
            static FontManager *main();

        private:

            FontMap _fonts;

            static FontManager *s_main;
    };
}

