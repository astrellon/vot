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
        if (!load_texture("bullet_blue", "data/bullet_blue.png"))
        {
            std::cout << "Failed to load bullet blue texture\n";
            return false;
        }
        if (!load_texture("bullet_blue_circle", "data/bullet_blue_circle.png"))
        {
            std::cout << "Failed to load bullet blue circle texture\n";
            return false;
        }
        if (!load_texture("player", "data/player.png"))
        {
            std::cout << "Failed to load player texture\n";
            return false;
        }
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
}
