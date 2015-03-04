#pragma once

#include <SFML/Graphics.hpp>

#include "character.h"
#include "powerup.h"
#include "beam.h"
#include "hardpoint.h"

#include <map>

namespace vot
{
    class PatternBullet;
    class HomingBullet;
    class Enemy;

    class Player : public Character
    {
        public:
            Player(const sf::Texture &texture);
            virtual void update(float dt);

            void init();
            
            void target(Enemy *value);
            Enemy *target() const;

            bool looking_at_target() const;
            bool auto_target() const;

            Circle &powerup_hitbox();

            void add_powerup(const Powerup &powerup);

        private:
            float _cooldown;
            float _homing_cooldown;
            bool _look_at_target;
            bool _auto_target;
            Enemy *_target;

            std::map<Powerup::Type, int32_t> _powerups;
            Circle _powerup_hitbox;
            
            PatternBullet *spawn_pattern_bullet(const std::string &name, uint32_t pattern_type);
            void spawn_pattern_bullet_pair(const std::string &name, uint32_t pattern_type, float x, float y);
            HomingBullet *spawn_homing_bullet();
            void spawn_homing_bullet_pair(float offset_angle);
            
            Beam *_middle_beam;
            Beam *_left_beam;
            Beam *_right_beam;

            //std::unique_ptr<PatternBulletHardpoint> _left_turret;
    };
}
