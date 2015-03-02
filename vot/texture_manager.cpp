#include "texture_manager.h"

#include <iostream>

namespace vot
{
    TextureManager *TextureManager::s_main = nullptr;

    TextureManager::TextureManager() :
        _num_textures(0u),
        _texture_load_attempt(0u)
    {

    }

    const TextureManager::TextureMap &TextureManager::textures() const
    {
        return _textures;
    }

    bool TextureManager::load_texture(const std::string &name, const std::string &filename)
    {
        _texture_load_attempt++;
        if (name.empty())
        {
            return false;
        }

        sf::Texture texture;
        if (!texture.loadFromFile(filename))
        {
            return false;
        }
        texture.setSmooth(true);

        _textures[name] = texture;
        _num_textures++;
        return true;
    }

    const sf::Texture *TextureManager::find_texture(const std::string &name) const
    {
        auto find = _textures.find(name);
        if (find == _textures.end())
        {
            return nullptr;
        }

        return &find->second;
    }

    uint32_t TextureManager::num_textures() const
    {
        return _num_textures;
    }
    uint32_t TextureManager::count_textures() const
    {
        return _textures.size();
    }
    uint32_t TextureManager::texture_load_attempt() const
    {
        return _texture_load_attempt;
    }
    
    bool TextureManager::load_default_textures()
    {
        if (!load_texture_log("bullet_blue", "data/laserBlue07.png")) return false;
        if (!load_texture_log("bullet_blue_circle", "data/bullet_blue_circle.png")) return false;
        if (!load_texture_log("bullet_red_circle", "data/bullet_red_circle.png")) return false;
        if (!load_texture_log("player", "data/playerShip2_blue.png")) return false;
        if (!load_texture_log("red_star", "data/red_star.png")) return false;
        if (!load_texture_log("blue_star", "data/blue_star.png")) return false;
        if (!load_texture_log("enemy", "data/ufoRed.png")) return false;
        if (!load_texture_log("target", "data/target.png")) return false;
        if (!load_texture_log("powerup_bullet", "data/powerupBlue_bolt.png")) return false;
        if (!load_texture_log("powerup_homing", "data/powerupBlue_star.png")) return false;
        if (!load_texture_log("homing_center", "data/homing_center.png")) return false;
        if (!load_texture_log("homing_outer", "data/homing_outer.png")) return false;
        if (!load_texture_log("beam", "data/beam.png")) return false;
        if (!load_texture_log("beam_top", "data/beam_top.png")) return false;

        return true;
    }

    void TextureManager::display(const std::string &message) const
    {
        std::cout << message << " texture counts: " << num_textures() << ", " << count_textures() << ", " << texture_load_attempt() << "\n";
    }

    void TextureManager::main(TextureManager *manager)
    {
        s_main = manager;
    }
    TextureManager *TextureManager::main()
    {
        return s_main;
    }

    const sf::Texture *TextureManager::texture(const std::string &name)
    {
        return s_main->find_texture(name); 
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
