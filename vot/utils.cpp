#include "utils.h"

#include <math.h>

namespace vot
{
    std::random_device Utils::_rd;
    std::mt19937 Utils::_rand(Utils::_rd());

    float Utils::degrees_to_radians = 0.01745329251994329576923690768489f;
    float Utils::radians_to_degrees = 57.295779513082320876798154814105f;

    AnglePair Utils::calculate_angles(const sf::Vector2f &pos1, const sf::Vector2f &pos2, float orig_angle, float offset_angle)
    {
        auto d_pos = pos1 - pos2;
        auto rlength = 1.0f / sqrt(d_pos.x * d_pos.x + d_pos.y * d_pos.y);

        d_pos.x *= rlength;
        d_pos.y *= rlength;

        auto to_angle = atan2(d_pos.y, d_pos.x) * 180.0f / 3.14159;
        to_angle += offset_angle;
        auto d_angle = orig_angle - to_angle;
        while (d_angle >= 180.0f) d_angle -= 360.0f;
        while (d_angle <= -180.0f) d_angle += 360.0f;

        return AnglePair(to_angle, d_angle);
    }

    bool Utils::ray_circle_intersect(const Ray &ray, const Circle &circle,
        sf::Vector2f points[2], sf::Vector2f normals[2])
    {
        sf::Vector2f d = ray.origin() - circle.location();
        float a = Utils::vector_dot(ray.direction(), ray.direction());
        float b = Utils::vector_dot(d, ray.direction());
        float c = Utils::vector_dot(d, d) - circle.radius() * circle.radius();

        float disc = b * b - a * c;
        if (disc < 0.0f)
        {
            return false;
        }

        float sqrtDisc = sqrt(disc);
        float invA = 1.0f / a;

        float t[2];
        t[0] = (-b - sqrtDisc) * invA;
        t[1] = (-b + sqrtDisc) * invA;

        float invRadius = 1.0f / circle.radius();

        for (int i = 0; i < 2; ++i)
        {
            points[i] = ray.origin() + t[i] * ray.direction();
            normals[i] = (points[i] - circle.location()) * invRadius;
        }

        return true;
    }
}
