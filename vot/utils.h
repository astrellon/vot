#pragma once

#include <SFML/Graphics.hpp>

#include "ray.h"
#include "circle.h"

#include <random>
#include <math.h>

namespace vot
{
    class AnglePair
    {
        public:
            AnglePair(float to_angle, float delta_angle) :
                _to_angle(to_angle),
                _delta_angle(delta_angle)
            {

            }

            inline float to_angle() const
            {
                return _to_angle;
            }
            inline float delta_angle() const
            {
                return _delta_angle;
            }

        private:
            float _to_angle;
            float _delta_angle;
    };

    class Utils
    {
        public:

            static AnglePair calculate_angles(const sf::Vector2f &pos1, const sf::Vector2f &pos2, float orig_angle, float offset_angle); 

            static inline float lerp(float init, float target, float t)
            {
                return (target - init) * t + init;
            }
            static inline sf::Vector2f lerp(const sf::Vector2f &init, const sf::Vector2f &target, float t)
            {
                float x = lerp(init.x, target.x, t);
                float y = lerp(init.y, target.y, t);
                return sf::Vector2f(x, y);
            }

            static inline float para_lerp(float init, float target, float t)
            {
                auto x = (t - 1);
                x = -x*x + 1;
                return (target - init) * x + init;
            }
            static inline sf::Vector2f para_lerp(const sf::Vector2f &init, const sf::Vector2f &target, float t)
            {

                float x = para_lerp(init.x, target.x, t);
                float y = para_lerp(init.y, target.y, t);
                return sf::Vector2f(x, y);
            }

            static inline float vector_angle(const sf::Vector2f &vec)
            {
                return atan2(vec.y, vec.x) * 57.295779513082;
            }

            static inline float vector_dot(const sf::Vector2f &vec1, const sf::Vector2f &vec2)
            {
                return vec1.x * vec2.x + vec1.y * vec2.y;
            }

            static bool ray_circle_intersect(const Ray &ray, const Circle &circle, 
                sf::Vector2f points[2], sf::Vector2f normals[2]);

        private:

            static std::random_device _rd;
            static std::mt19937 _rand;

        public:
            static inline float randf(float min = 0.0f, float max = 1.0f)
            {
                std::uniform_real_distribution<float> dist(min, max);
                return dist(_rand);
            }
            static inline sf::Vector2f rand_vec(float min = 0.0f, float max = 1.0f)
            {
                return sf::Vector2f(randf(min, max), randf(min, max));
            }
    };
}
