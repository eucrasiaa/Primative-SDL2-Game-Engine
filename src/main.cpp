#include <SDL2/SDL.h>
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_render.h>
#include <SDL_timer.h>
#include <stdint.h>
#include <sys/types.h>
#include "CoreEngine.hpp"




int main(){
  CoreEngine engine;

  if(!engine.init()){
    return 1;
  }
  engine.run();
  return 0;


    SDL_Event event;
    // start loop;

  return 0;
}

