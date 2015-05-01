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
            static bool init();
            static void deinit();

            typedef std::map<std::string, std::unique_ptr<sf::Texture> > TextureMap;
            static const TextureMap &textures();

            static bool load_texture(const std::string &name, const std::string &filename);
            static const sf::Texture *texture(const std::string &name);

            static std::string texture_name(const sf::Texture *texture);

            static uint32_t num_textures();
            static uint32_t count_textures();
            static uint32_t texture_load_attempt();

            static void display(const std::string &message);

        private:

            TextureManager();

            static TextureMap s_textures;
            static uint32_t s_num_textures;
            static uint32_t s_texture_load_attempt;

            static bool load_default_textures();

            static bool load_texture_log(const std::string &name, const std::string &filename);
            static void try_load(const std::string &name, const std::string &from_data);
    };
}
