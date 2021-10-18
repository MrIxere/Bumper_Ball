#include <game/background.h>
#include <SFML/Graphics/RenderTarget.hpp>
#include "maths/basic.h"
#include <engine/globals.h>

namespace game
{
    void Background::Init()
    {
        //ring.setPosition(0, 0);
        /*vertexArray_ = sf::VertexArray(sf::Points, starCount);

        for (std::size_t i = 0; i < starCount; i++)
        {
            auto& vertex = vertexArray_[i];
            vertex.color = sf::Color::White;
            vertex.position = sf::Vector2f(
                core::RandomRange(-50.0f, 50.0f) * core::pixelPerMeter,
                core::RandomRange(-50.0f, 50.0f) * core::pixelPerMeter);
        }*/
    }

    void Background::Draw(sf::RenderTarget& window)
    {
    	sf::CircleShape ring(250);
        ring.setPosition(70, 100);
        ring.setFillColor(sf::Color::White);
        window.draw(ring);
        window.draw(vertexArray_);
    }
}
