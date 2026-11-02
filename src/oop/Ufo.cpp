#include "Ufo.hpp"
#include "core/CircleTexture.hpp"

void Ufo::render() const {
    if (!isActive()) return;

    CircleTexture::draw(posX, posY, radius, color);
}
