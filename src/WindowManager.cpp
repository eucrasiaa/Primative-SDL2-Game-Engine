#include "WindowManager.hpp"
#include <iostream>

int WindowInstance::initialize_Conf(const InitWinRenConfig conf = {}){
      // backup?
      if (ren != nullptr){
        std::cerr<<"Initiate passed active renderer, safely killing";
        SDL_DestroyRenderer(ren);
        ren = nullptr;
        SDL_DestroyWindow(win);
        win = nullptr;  
      }
      if (win != nullptr){
        std::cerr<<"Initiate passed active window, safely killing";
        SDL_DestroyWindow(win);
        win = nullptr;
      }
      // run
      win = SDL_CreateWindow(conf.title, 
          conf.x, conf.y,
          conf.w, conf.h, conf.wflags);
      if(win == nullptr){
        std::cerr << std::format("window creation failure, see {}\n", SDL_GetError());
        SDL_Quit();
        return 1;
      } 
      ren = SDL_CreateRenderer(win, conf.index, conf.rflags);
      if(ren == nullptr){
        std::cerr << std::format("renderer creation failure, see {}\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
      } 
      if (conf.default_conf){
        SDL_RenderSetLogicalSize(ren, conf.w, conf.h);
        SDL_SetWindowMinimumSize(win, conf.w_min, conf.h_min);
      } 

      return 0;
    }
