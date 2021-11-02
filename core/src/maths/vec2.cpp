#include <maths/vec2.h>
#include <cmath>

namespace core
{
    Vec2f::Vec2f(sf::Vector2f v) : x(v.x), y(v.y)
    {
    }

    sf::Vector2f Vec2f::toSf() const
    {
        return sf::Vector2f(x, y);
    }

    Vec2f Vec2f::operator+(Vec2f v) const
    {
        return {x + v.x, y + v.y};
    }

    Vec2f& Vec2f::operator+=(Vec2f v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vec2f Vec2f::operator-(Vec2f v) const
    {
        return {x - v.x, y - v.y};
    }

    Vec2f& Vec2f::operator-=(Vec2f v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    Vec2f Vec2f::operator*(float f) const
    {
        return {x * f, y * f};
    }

    Vec2f Vec2f::operator/(float f) const
    {
        return {x / f, y / f};
    }

    Vec2f operator*(float f, Vec2f v)
    {
        return v*f;
    }

    float Vec2f::GetMagnitude() const
{
    return std::sqrt(GetSqrMagnitude());
}

void Vec2f::Normalize()
{
    const auto magnitude = GetMagnitude();
    x /= magnitude;
    y /= magnitude;
}

Vec2f Vec2f::GetNormalized() const
{
    const auto magnitude = GetMagnitude();
    return (*this) / magnitude;
}

float Vec2f::GetSqrMagnitude() const
{
    return x * x + y * y;
}

float Vec2f::Dot(Vec2f a, Vec2f b)
{
    return a.x * b.x + a.y * b.y;
}

Vec2f Vec2f::Lerp(Vec2f a, Vec2f b, float t)
{
    return a + (b - a) * t;
}

Vec2f inv(const Vec2f& v1, const Vec2f& v2)
{
    return Vec2f{ -v2.y + v1.y, -v2.x + v1.x };
}

Vec2f ComputeNormal(core::Vec2f center, core::Vec2f i)
{
    return(i - center).GetNormalized();
}

Vec2f ComputeTangent(core::Vec2f center, core::Vec2f i)
{
    const Vec2f tangent = ComputeNormal(center, i);
    return{ tangent.y, -tangent.x };
}

float CalculateDistance(Vec2f a, Vec2f b)
{
    const float dx = b.x - a.x;
    const float dy = b.y - a.y;
    return std::sqrt(dx * dx + dy + dy);
}

}
