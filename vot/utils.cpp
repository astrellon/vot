#include "utils.h"

namespace vot
{
    AnglePair Utils::calculate_angles(const sf::Vector2f &pos1, const sf::Vector2f &pos2, float orig_angle, float offset_angle)
    {
        auto d_pos = pos1 - pos2;
        auto rlength = 1.0f / sqrt(d_pos.x * d_pos.x + d_pos.y * d_pos.y);

        d_pos.x *= rlength;
        d_pos.y *= rlength;

        auto to_angle = atan2(d_pos.y, d_pos.x) * 180.0f / M_PI;
        to_angle += offset_angle;
        auto d_angle = orig_angle - to_angle;
        while (d_angle >= 180.0f) d_angle -= 360.0f;
        while (d_angle <= -180.0f) d_angle += 360.0f;

        return AnglePair(to_angle, d_angle);
    }
}
