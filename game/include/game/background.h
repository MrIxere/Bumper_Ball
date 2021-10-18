#pragma once
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics.hpp>

#include "graphics/graphics.h"

namespace sf
{
class RenderTarget;
}

namespace game
{

class Background : public core::DrawInterface
{
public:
    void Init();
    void Draw(sf::RenderTarget& window) override;
private:
    //static constexpr std::size_t starCount = 1024;
    
	sf::CircleShape ring();
    sf::VertexArray vertexArray_;
};

}
