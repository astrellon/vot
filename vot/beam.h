#pragma once

#include <SFML/Graphics.hpp>
#include <stdint.h>
#include <array>
#include <memory>

#include "object_manager.h"
#include "ray.h"
#include "common.h"

namespace vot
{
    // Beam {{{
    class Beam : public sf::Drawable
    {
        public:
            Beam();

            Ray &hitbox();
            const Ray &hitbox() const;

            void max_length(float value);
            float max_length() const;

            void width(float value);
            float width() const;

            void index(uint32_t value);
            uint32_t index() const;

            void group(Group::Type value);
            Group::Type group() const;

            void hitting_target_length(float value);
            float hitting_target_length() const;

            void update(float dt);
            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        private:
            Ray _hitbox;
            uint32_t _index;
            Group::Type _group;
            float _max_length;
            float _width;
            float _hitting_target_length;

            sf::RectangleShape _shape;
            sf::RectangleShape _shape_top;
    };
    // }}}

    // BeamManager {{{
    class BeamManager : public sf::Drawable, public ObjectManager<Beam, 256>
    {
        public:
            void remove_beam(Beam *beam);
            Beam *spawn_beam(const std::string &name, Group::Type group);

            void add_src_beam(const std::string &name, Beam *beam);

            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        private:
            std::map<std::string, std::unique_ptr<Beam> > _src_beams;
    };
    // }}}
}
