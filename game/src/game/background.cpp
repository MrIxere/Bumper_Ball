#include <game/background.h>
#include <SFML/Graphics/RenderTarget.hpp>
#include "maths/basic.h"
#include <engine/globals.h>

namespace game
{
    void Background::Init()
    {
    }

    void Background::Draw(sf::RenderTarget& window)
    {
    	sf::CircleShape ring;
    	ring.setRadius(250);
        ring.setOrigin(ring.getRadius(), ring.getRadius());
    	ring.setPosition(window.getSize().x/2, window.getSize().y/2);
        ring.setFillColor(sf::Color::White);
        window.draw(ring);
        window.draw(vertexArray_);
    }
}
