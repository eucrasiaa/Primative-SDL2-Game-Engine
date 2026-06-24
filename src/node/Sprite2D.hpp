#pragma once
#include "Node.hpp"
#include "Node2D.hpp"
#include "../rendering/renderer.hpp"
class Sprite2D : public Node{
  private:
    uint32_t tex = TexturesTMP::NONE;
    int z_index = 10;
    float x =0;
    float y=0;
    float width=100;
    float height=100;
    float rotation = 0;
    float scaleX=1;
    float scaleY=1;
    uint8_t r=255;
    uint8_t g=0;
    uint8_t b=0;
    uint8_t a=255;

  public:
    Sprite2D() = default;
    Sprite2D(uint32_t textureId) 
        : tex(textureId) {}

    void render(){
      x = static_cast<Node2D*>(parent)->Position.x;
      y = static_cast<Node2D*>(parent)->Position.y;
      RenderCommand cmd = {
        .z_index = z_index,
        .texture_id=tex,
        .x = x, .y=y, .width=width, .height=height,
        .rotation=rotation, .scaleX=scaleX, .scaleY=scaleY,
        .r=r,.g=g,.b=b,.a=a

      };
      Engine::GetRenderer()->submitCommand(cmd);

    }
};
