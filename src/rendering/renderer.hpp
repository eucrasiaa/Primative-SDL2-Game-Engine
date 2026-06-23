#pragma once
#include <SDL_render.h>
#include <algorithm>
#include <iostream>
#include <stdint.h>
#include <vector>
enum TexturesTMP{
  NONE = 5,
  Square = 0,
  Rectangle = 1,
  R_Triangle = 2,
  Iso_Triangle = 3,
  Circle = 4,
};



// _______________________
// ______________________
//  REALLY DOGSHIT PLACEHOLDER NOT A JUDGE OF CHARATER
//  ITLL GET REPLACED LATER WITH OPENGL
//  ____________________
//
//  ____________________
//
//  
    struct RenderCommand {
      int z_index;          
      uint32_t texture_id;
      float x, y, width, height;
      float rotation;
      float scaleX,scaleY;
      uint8_t r, g, b, a;   // Tint
    };

class Renderer {
  
  public:
  private:
    SDL_Renderer *sdlRenderer;
    std::vector<RenderCommand> renderQueue;

    // void flushQueue() {
    //   std::stable_sort(renderQueue.begin(), renderQueue.end(), [](const RenderCommand& a, const RenderCommand& b) {
    //     return a.z_index < b.z_index;
    //   });
    //   for (RenderCommand cmd : renderQueue ){
    //     // for now just draw a rect on screen based on xywh and rot.
    //   }
    // }
    // void drawSprite(RenderCommand rCmd){
    //
    // }
    void drawRect(float x, float y, float w, float h, uint8_t r, uint8_t g, uint8_t b);
  


    
    void drawSprite(RenderCommand cmd) {
      // Set color tint for this command
      SDL_SetRenderDrawColor(sdlRenderer, cmd.r, cmd.g, cmd.b, cmd.a);
      
      // Calculate final scaled dimensions
      float finalW = cmd.width * cmd.scaleX;
      float finalH = cmd.height * cmd.scaleY;

      // Map texture_id enum to geometric shapes
      switch (static_cast<TexturesTMP>(cmd.texture_id)) {
        case Square:
        case Rectangle: {
          SDL_FRect rect = { cmd.x, cmd.y, finalW, finalH };
          SDL_RenderFillRectF(sdlRenderer, &rect);
          break;
        }
        case R_Triangle: { // Right-angled triangle
          SDL_FPoint points[4] = {
            { cmd.x, cmd.y },                      // Top-left
            { cmd.x + finalW, cmd.y + finalH },    // Bottom-right
            { cmd.x, cmd.y + finalH },             // Bottom-left
            { cmd.x, cmd.y }                       // Close loop
          };
          SDL_RenderDrawLinesF(sdlRenderer, points, 4);
          break;
        }
        case Iso_Triangle: { // Isosceles triangle
          SDL_FPoint points[4] = {
            { cmd.x + (finalW / 2.0f), cmd.y },    // Top peak
            { cmd.x + finalW, cmd.y + finalH },    // Bottom right
            { cmd.x, cmd.y + finalH },             // Bottom left
            { cmd.x + (finalW / 2.0f), cmd.y }     // Close loop
          };
          SDL_RenderDrawLinesF(sdlRenderer, points, 4);
          break;
        }
        case Circle: {
          int radius = static_cast<int>(finalW / 2.0f);
          int centerX = static_cast<int>(cmd.x + radius);
          int centerY = static_cast<int>(cmd.y + radius);
          drawCircleHelper(centerX, centerY, radius);
          break;
        }
      }
    }
void drawCircleHelper(int centreX, int centreY, int radius) {
      for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
          int dx = radius - w;
          int dy = radius - h;
          if ((dx*dx + dy*dy) <= (radius * radius)) {
            SDL_RenderDrawPoint(sdlRenderer, centreX + dx, centreY + dy);
          }
        }
      }
    }



  public:
    virtual ~Renderer() = default;
     
    Renderer(SDL_Renderer* renderer) : sdlRenderer(renderer) {

    }



    void submitCommand(RenderCommand cmd){
      renderQueue.push_back(cmd);
    }
    // Core lifecycle
    // virtual void clear() = 0;
    
    void clear() {
      SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255); // Clear black
      SDL_RenderClear(sdlRenderer);
    }

    void present() {
      flushQueue(); // Process and draw everything
      SDL_RenderPresent(sdlRenderer);
    }

    void flushQueue() {
      // Sort commands by depth layer
      std::stable_sort(renderQueue.begin(), renderQueue.end(), [](const RenderCommand& a, const RenderCommand& b) {
        return a.z_index < b.z_index;
      });

      // Draw all primitives
      for (const auto& cmd : renderQueue) {
        drawSprite(cmd);
      }



      renderQueue.clear();
    }

    // virtual void present() = 0;

    // Game-specific drawing commands (Abstracted!)
    // Notice no SDL_Texture or OpenGL IDs here, just plain data.
    // virtual void drawSprite(uint32_t textureId, float x, float y, float rotation) = 0;
    // virtual void drawRect(float x, float y, float w, float h, uint8_t r, uint8_t g, uint8_t b) = 0;
};
