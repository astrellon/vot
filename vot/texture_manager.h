#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <map>
#include <memory>

namespace vot
{
    class TextureManager
    {
        public:
            typedef std::map<std::string, sf::Texture> TextureMap;
            const TextureMap &textures() const;

            bool load_texture(const std::string &name, const std::string &filename);
            const sf::Texture *find_texture(const std::string &name) const;

            bool load_default_textures();

            static void main(TextureManager *manager);
            static TextureManager *main();

            static const sf::Texture *texture(const std::string &name);

        private:

            TextureMap _textures;

            static TextureManager *s_main;

            bool load_texture_log(const std::string &name, const std::string &filename);
    };
}
