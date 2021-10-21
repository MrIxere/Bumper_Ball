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
    	sf::CircleShape ring;
        //ring.setRadius(250);
    	ring.setRadius(250);
        ring.setOrigin(ring.getRadius(), ring.getRadius());
        //ring.setPosition(sf::Vector2f(window.getSize().x/2, window.getSize().y/2));
        //ring.setPosition(sf::Vector2f{ window.getSize() / 4u});
    	ring.setPosition(window.getSize().x/2, window.getSize().y/2);
        ring.setFillColor(sf::Color::White);
        window.draw(ring);
        window.draw(vertexArray_);
    }
}
