#pragma once

#include <SFML/Graphics.hpp>
#include <stdint.h>
#include <vector>
#include <memory>

#include "utils/circle.h"
#include "hardpoint.h"
#include "thruster.h"
#include "common.h"

namespace vot
{
    class Character : public sf::Drawable, public sf::Transformable
    {
        public:
            Character(const sf::Texture &texture);
            Character(const Character &clone);

            virtual void update(float dt);
            virtual void draw(sf::RenderTarget &target, sf::RenderStates state) const;

            virtual void translate(const sf::Vector2f &vector);
            virtual void location(const sf::Vector2f &vector);
            virtual sf::Vector2f location() const;
            sf::Vector2f center() const;

            void rotateBy(float angle);
            void rotation(float angle);
            float rotation() const;

            utils::Circle &hitbox();
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

            typedef std::vector<std::unique_ptr<Hardpoint> > HardpointList; 
            const HardpointList *hardpoints() const;
            
            void add_hardpoint(Hardpoint *point);
            void remove_hardpoint(Hardpoint *point);
            virtual void clear_hardpoints();

            typedef std::vector<std::unique_ptr<Thruster> > ThrusterList;
            const ThrusterList *thrusters() const;

            void add_thruster(Thruster *thruster);

            void acceleration(const sf::Vector2f &acc);
            sf::Vector2f acceleration() const;

            sf::Vector2f velocity() const;

            void max_speed(float speed);
            float max_speed() const;

            void strafe_speed(float speed);
            float strafe_speed() const;

            void forwards_speed(float speed);
            float forwards_speed() const;

            void backwards_speed(float speed);
            float backwards_speed() const;

            void rot_acceleration(float acc);
            float rot_acceleration() const;

            float rot_velocity() const;

            void rot_speed(float speed);
            float rot_speed() const;
            
            void max_rot_speed(float speed);
            float max_rot_speed() const;

            void translate_assist(bool assist);
            bool translate_assist() const;

            void rotation_assist(bool assist);
            bool rotation_assist() const;

            void group(Group::Type value);
            Group::Type group() const;

        private:
            sf::Sprite _sprite;
            utils::Circle _hitbox;
            float _life;
            float _max_life;
            bool _is_dead;
            uint16_t _id;

            HardpointList _hardpoints;
            ThrusterList _thrusters;

            sf::Vector2f _acceleration;
            sf::Vector2f _velocity;
            float _max_speed;
            float _strafe_speed;
            float _forwards_speed;
            float _backwards_speed;

            float _rot_acceleration;
            float _rot_velocity;
            float _rot_speed;
            float _max_rot_speed;
            
            bool _translate_assist;
            bool _rotation_assist;

            Group::Type _group;
    };
}
