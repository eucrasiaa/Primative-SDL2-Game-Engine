#pragma once
#include <SDL2/SDL.h>
#include <SDL_render.h>
#include <string>
const int WINR_WIDTH = 1024;
const int WINR_HEIGHT = 768;
const int WINR_WIDTH_MIN = 640;
const int WINR_HEIGHT_MIN = 480;
typedef struct InitWinRenConfig {
  int x = SDL_WINDOWPOS_CENTERED;
  int y = SDL_WINDOWPOS_CENTERED;
  int w = WINR_WIDTH;
  int h = WINR_HEIGHT;
  int w_min = WINR_WIDTH_MIN;
  int h_min = WINR_HEIGHT_MIN;
  const char* title = "wintitle";
  Uint32 wflags = SDL_WINDOW_SHOWN;
  int index = -1;
  Uint32 rflags = SDL_RENDERER_ACCELERATED;
  bool default_conf = true;
} InitWinRenConfig;



class WindowInstance{
  private:
    SDL_Window* win = nullptr;
    SDL_Renderer* ren = nullptr;

  public:
    WindowInstance() = default;
    WindowInstance(const WindowInstance&) = delete;
    WindowInstance& operator=(const WindowInstance&) = delete;
    WindowInstance(WindowInstance&& other) noexcept 
      : win(other.win), ren(other.ren) {
        other.win = nullptr;
        other.ren = nullptr;
      }
    WindowInstance& operator=(WindowInstance&& other) noexcept {
      if (this != &other) {
        cleanup();
        win = other.win;
        ren = other.ren;
        other.win = nullptr;
        other.ren = nullptr;
      }
      return *this;
    }

    void cleanup() {
      if (ren) { SDL_DestroyRenderer(ren); ren = nullptr; }
      if (win) { SDL_DestroyWindow(win); win = nullptr; }
    }

    SDL_Renderer* getRenderer() const { return ren; }
    SDL_Window* getWindow() const { return win; }


    int initialize(const char* title){
      return initialize_Conf({.title = title});
    }
    int initialize_Conf(const InitWinRenConfig conf);
};
