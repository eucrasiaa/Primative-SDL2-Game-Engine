#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include "InputManager.hpp"
#include "SystemStruct.hpp"
#include "WindowManager.hpp"
#include "BindScriptManager.hpp"
const Uint32 FIXED_TIME_STEP = 1000/60; // 60hz, or 0.01666 seconds

class CoreEngine {
  private:
    bool running = false;

  public:
    std::unique_ptr<SystemStruct> sysStruct;
    InputManager input;

    // Visual / UI Subsystems
    WindowInstance gameWindow;
    WindowInstance debugWindow;

    CoreEngine() {
      sysStruct = std::make_unique<SystemStruct>();
    }
    void killgame(){
      this->running=false;
    }
    bool init() {
      if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
      }
      gameWindow.initialize("Game Window");
      // debugWindow.initialize("Debug View", 400, 400);

      auto* ren = gameWindow.getRenderer();
      if (ren) {
        SDL_SetRenderDrawColor(ren, 33, 150, 243, 255);
        SDL_RenderClear(ren);
        SDL_RenderPresent(ren);
      }

      setupBindings();
      sysStruct->init();
      running = true;
      return true;
    }

    void setupBindings();
    void handleEvents();
    void run();
    ~CoreEngine();
};
