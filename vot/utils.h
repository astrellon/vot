#pragma once

#include <SFML/Graphics.hpp>

#include "ray.h"
#include "circle.h"

#include <random>
#include <math.h>
#include <stdint.h>

namespace vot
{
    class AnglePair
    {
    public:
        inline AnglePair(float to_angle, float delta_angle) :
            _to_angle(to_angle),
            _delta_angle(delta_angle)
        {

        }
        inline AnglePair(double to_angle, double delta_angle) :
            _to_angle(static_cast<float>(to_angle)),
            _delta_angle(static_cast<float>(delta_angle))
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

            static float degrees_to_radians;
            static float radians_to_degrees;
            static float pi;

            static uint32_t max_uint;
            static int32_t max_int;
            static int32_t min_int;

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

            static inline float vector_degrees(const sf::Vector2f &vec)
            {
                return atan2(vec.y, vec.x) * radians_to_degrees;
            }
            static inline float vector_radians(const sf::Vector2f &vec)
            {
                return atan2(vec.y, vec.x);
            }

            static inline sf::Vector2f degrees_vector(float degrees)
            {
                auto radians = degrees * degrees_to_radians;
                return sf::Vector2f(cos(radians), sin(radians));
            }
            static inline sf::Vector2f radians_vector(float radians)
            {
                return sf::Vector2f(cos(radians), sin(radians));
            }

            static inline float vector_dot(const sf::Vector2f &vec1, const sf::Vector2f &vec2)
            {
                return vec1.x * vec2.x + vec1.y * vec2.y;
            }

            static bool ray_circle_intersect(const Ray &ray, const Circle &circle, 
                sf::Vector2f points[2], sf::Vector2f normals[2]);

            static inline float abs(float value)
            {
                if (value < 0.0f)
                {
                    return -value;
                }
                return value;
            }

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
