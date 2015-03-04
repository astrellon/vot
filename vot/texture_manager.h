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
            TextureManager();

            typedef std::map<std::string, sf::Texture> TextureMap;
            const TextureMap &textures() const;

            bool load_texture(const std::string &name, const std::string &filename);
            const sf::Texture *find_texture(const std::string &name) const;

            uint32_t num_textures() const;
            uint32_t count_textures() const;
            uint32_t texture_load_attempt() const;

            void display(const std::string &message) const;

            bool load_default_textures();

            static void main(TextureManager *manager);
            static TextureManager *main();

            static const sf::Texture *texture(const std::string &name);

        private:

            TextureMap _textures;
            uint32_t _num_textures;
            uint32_t _texture_load_attempt;

            static TextureManager *s_main;

            bool load_texture_log(const std::string &name, const std::string &filename);
            void try_load(const std::string &name, const std::string &from_data);
    };
}
