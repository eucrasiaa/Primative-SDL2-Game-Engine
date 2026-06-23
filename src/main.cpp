#include <SDL2/SDL.h>
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_render.h>
#include <SDL_timer.h>
#include <stdint.h>
#include <sys/types.h>
#include "Engine.hpp"

#include "./node/devNode.hpp"


int main(){
  // CoreEngine engine;
    
  if(!Engine::InitEngine()){
    return 1;
  }

    /* testing */

  FishNode_S2D* fish2D = new FishNode_S2D();
  Node* aNode = new Node();
  Sprite2D* tex = new Sprite2D(3);
  aNode->addChild(fish2D);
  fish2D->addChild(tex);
  Engine::SetActiveScene(aNode);

  /* end testing */
  Engine::Run();
  return 0;


    SDL_Event event;
    // start loop;

  return 0;
}

