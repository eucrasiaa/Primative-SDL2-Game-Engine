#include <SDL2/SDL.h>
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_render.h>
#include <cctype>
#include <cstdint>
#include <format>
#include <iostream>
#include <stdint.h>


#include <string_view>
#include <sys/types.h>
#include <vector>

#include "ansi_control.hpp"
const int WINR_WIDTH = 1024;
const int WINR_HEIGHT = 768;
const int WINR_WIDTH_MIN = 640;
const int WINR_HEIGHT_MIN = 480;


std::vector<char> keyBuff;
int main(){
  keyBuff.reserve(1000);
  // 2 structs: sdlwindow and sdl renderer, then initialize it!
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  // initialize win + render
  //SDL_CreateWindowAndRenderer(640, 640, 0, &window, &renderer);
  window = SDL_CreateWindow("Will\'s first window", 
                            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            WINR_WIDTH, WINR_HEIGHT, SDL_WINDOW_SHOWN);
  if(window == nullptr){
    std::cerr << std::format("window creation failure, see {}\n", SDL_GetError());
    SDL_Quit();
    return 1;
  } 
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if(renderer == nullptr){
    std::cerr << std::format("renderer creation failure, see {}\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  } 
  // SDL_RenderSetScale(renderer, 4,4);
  SDL_RenderSetLogicalSize(renderer, WINR_WIDTH, WINR_HEIGHT);
  SDL_SetWindowMinimumSize(window, WINR_WIDTH_MIN, WINR_HEIGHT_MIN);
  
  SDL_SetRenderDrawColor(renderer, 33, 150, 243, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
  // loop
  bool gameIsRunning = true;
  std::cout<<"\n\n\n\n"<<std::endl;
  while(gameIsRunning){
    SDL_Event event;
    // start loop:
    // while(SDL_PollEvent(&event)){
    if (SDL_WaitEvent(&event)){
    switch (event.type){
        using enum SDL_EventType;
        case SDL_MOUSEMOTION:
        term::move_up(5, true);
          term::print_labeled_array("Mouse", 
              {event.motion.x, event.motion.y, 
              event.motion.xrel, event.motion.yrel,
              (Sint32)event.motion.windowID}, 
              {"x","y","xrel","yrel", "window"});
        term::move_down(5);
              
          break;
        case SDL_KEYDOWN:
          term::move_up(4, true);
          term::print_labeled_array("keydown: ", 
              {(int32_t) event.key.keysym.scancode, (int32_t) event.key.keysym.mod,
              (int32_t) event.key.keysym.sym},
              {"scancode", "mod", "sym"});
          term::move_down(4);
          if (event.key.keysym.scancode >= SDL_SCANCODE_A && event.key.keysym.scancode <= SDL_SCANCODE_Z) {
            char res = '_';
            if(event.key.keysym.mod & (KMOD_LSHIFT | KMOD_RSHIFT)){
              res = std::toupper('a'+event.key.keysym.scancode-4);
            }
            else{
              res = ('a'+event.key.keysym.scancode-4);
            }
            keyBuff.push_back(res);
            term::move_up(3,true);
            std::cout<< std::string_view(keyBuff)<<std::flush;
            term::move_down(3);
          }
          break;
        [[unlikely]] case SDL_QUIT:
          gameIsRunning = false;
          break;
      }
    }

  }
  // SDL_Delay(3000);
  


  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();


  return 0;
}
