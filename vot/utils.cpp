#include "utils.h"

#include <math.h>

namespace vot
{
    std::random_device Utils::_rd;
    std::mt19937 Utils::_rand(Utils::_rd());

    float Utils::degrees_to_radians = 0.01745329251994329576923690768489f;
    float Utils::radians_to_degrees = 57.295779513082320876798154814105f;
    float Utils::pi = 3.141592654f;

    uint32_t Utils::max_uint = std::numeric_limits<uint32_t>::max();
    int32_t Utils::max_int = std::numeric_limits<int32_t>::max();
    int32_t Utils::min_int = std::numeric_limits<int32_t>::min();

    AnglePair Utils::calculate_angles(const sf::Vector2f &pos1, const sf::Vector2f &pos2, float orig_angle, float offset_angle)
    {
        auto d_pos = pos1 - pos2;
        auto rlength_sqrd = d_pos.x * d_pos.x + d_pos.y * d_pos.y;
        if (rlength_sqrd != 1.0f)
        {
            auto rlength = 1.0f / sqrt(rlength_sqrd);

            d_pos.x *= rlength;
            d_pos.y *= rlength;
        }

        auto to_angle = vector_degrees(d_pos);
        to_angle += offset_angle;
        auto d_angle = orig_angle - to_angle;
        while (d_angle >= 180.0f) d_angle -= 360.0f;
        while (d_angle <= -180.0f) d_angle += 360.0f;

        return AnglePair(to_angle, d_angle);
    }

    bool Utils::ray_circle_intersect(const Ray &ray, const Circle &circle,
        sf::Vector2f points[2], sf::Vector2f normals[2])
    {
        auto d = ray.origin() - circle.location();
        auto b = Utils::vector_dot(d, ray.direction());
        auto c = Utils::vector_dot(d, d) - circle.radius_squared();
        if (b > 0.0f)
        {
            return false;
        }

        auto disc = b * b - c;
        if (disc < 0.0f)
        {
            return false;
        }

        auto sqrt_disc = sqrt(disc);

        float t[2];
        t[0] = (-b - sqrt_disc);
        t[1] = (-b + sqrt_disc);

        for (auto i = 0; i < 2; ++i)
        {
            points[i] = ray.origin() + t[i] * ray.direction();
            normals[i] = (points[i] - circle.location()) * circle.radius_inv();
        }

        return true;
    }
}
