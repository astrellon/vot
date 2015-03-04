#pragma once

#include <SFML/Graphics.hpp>
#include <stdint.h>
#include <vector>
#include <memory>

#include "circle.h"
#include "hardpoint.h"

namespace vot
{
    class Character : public sf::Drawable, public sf::Transformable
    {
        public:
            Character(const sf::Texture &texture);
            Character(const Character &clone);

            virtual void update(float dt);
            virtual void draw(sf::RenderTarget &target, sf::RenderStates state) const;

            void translate(const sf::Vector2f &vector);
            void location(const sf::Vector2f &vector);
            sf::Vector2f location() const;
            sf::Vector2f center() const;

            void rotateBy(float angle);
            void rotation(float angle);
            float rotation() const;

            Circle &hitbox();
            void take_damage(float damage);

            void is_dead(bool value);
            bool is_dead() const;

            void life(float value);
            float life() const;

            void max_life(float value);
            float max_life() const;

            void id(uint16_t value);
            uint16_t id() const;

            sf::Sprite &sprite();
            const sf::Sprite &sprite() const;

            sf::Transform forward_center_trans(bool rotate = true) const;

            typedef std::vector<std::unique_ptr<Hardpoint> > HardpointList; 
            const HardpointList *hardpoints() const;
            
            void add_hardpoint(Hardpoint *point);

        private:
            sf::Sprite _sprite;
            Circle _hitbox;
            float _life;
            float _max_life;
            bool _is_dead;
            uint16_t _id;

            HardpointList _hardpoints;
    };
}
