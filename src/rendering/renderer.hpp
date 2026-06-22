#pragma once
#include <stdint.h>
class Renderer {
public:
    virtual ~Renderer() = default;

    // Core lifecycle
    virtual void clear() = 0;
    virtual void present() = 0;

    // Game-specific drawing commands (Abstracted!)
    // Notice no SDL_Texture or OpenGL IDs here, just plain data.
    virtual void drawSprite(uint32_t textureId, float x, float y, float rotation) = 0;
    virtual void drawRect(float x, float y, float w, float h, uint8_t r, uint8_t g, uint8_t b) = 0;
};
