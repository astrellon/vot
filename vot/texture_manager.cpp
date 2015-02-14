#include "texture_manager.h"

#include <iostream>

namespace vot
{
    TextureManager *TextureManager::s_main = nullptr;

    const TextureManager::TextureMap &TextureManager::textures() const
    {
        return _textures;
    }

    bool TextureManager::load_texture(const std::string &name, const std::string &filename)
    {
        if (name.empty())
        {
            return false;
        }

        sf::Texture texture;
        if (!texture.loadFromFile(filename))
        {
            return false;
        }

        _textures[name] = texture;
        return true;
    }

    const sf::Texture *TextureManager::texture(const std::string &name) const
    {
        auto find = _textures.find(name);
        if (find == _textures.end())
        {
            return nullptr;
        }

        return &find->second;
    }
    
    bool TextureManager::load_default_textures()
    {
        if (!load_texture_log("bullet_blue", "data/bullet_blue.png")) return false;
        if (!load_texture_log("bullet_blue_circle", "data/bullet_blue_circle.png")) return false;
        if (!load_texture_log("player", "data/player.png")) return false;
        if (!load_texture_log("red_star", "data/red_star.png")) return false;
        if (!load_texture_log("blue_star", "data/blue_star.png")) return false;
        if (!load_texture_log("enemy", "data/enemy.png")) return false;
        if (!load_texture_log("target", "data/target.png")) return false;

        return true;
    }

    void TextureManager::main(TextureManager *manager)
    {
        s_main = manager;
    }
    TextureManager *TextureManager::main()
    {
        return s_main;
    }

    bool TextureManager::load_texture_log(const std::string &name, const std::string &filename)
    {
        auto result = load_texture(name, filename); 
        if (!result)
        {
            std::cout << "Failed to load " << name << " texture\n";
        }
        return result;
    }
}
