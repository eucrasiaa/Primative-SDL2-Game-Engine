#include <SDL2/SDL.h>
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_render.h>
#include <cctype>
#include <cstdint>
#include <format>
#include <iostream>
#include <new>
#include <stdint.h>


#include <string_view>
#include <sys/types.h>
#include <vector>

#include "ansi_control.hpp"
#include "InputManager.hpp"
const int WINR_WIDTH = 1024;
const int WINR_HEIGHT = 768;
const int WINR_WIDTH_MIN = 640;
const int WINR_HEIGHT_MIN = 480;

typedef struct DebugStates{
  struct MouseStateType {
    int_fast32_t x = 0;
    int_fast32_t y = 0;
    int_fast32_t x_rel = 0;
    int_fast32_t y_rel = 0;
  };

  struct KeyboardStateType {
    uint_fast32_t scancode = 0;
    uint_fast32_t mod = 0;
    uint_fast32_t sym = 0;
  };
  MouseStateType mouse;
  KeyboardStateType keyboard;
}DebugStates;


typedef enum DebugPrintStates{
  None = 0x0,
  Mouse = 0x1,
  Keyboard = 0x2,
  TextBuff = 0x4,
  All = 0x1F,
}DebugPrintStates;
class SystemStruct{
  private:
  public:
    DebugStates debugStates;
    InputManager input;
    std::vector<char> keyBuff;
    SystemStruct(){
      keyBuff.reserve(1000);
    }
    ~SystemStruct() = default;

    void initPrint(){
      std::cout<<"\n\n\n\n\n\n\n\n\n\n\n\n" <<std::flush;
    }
    void pushKeystroke(char c){
      keyBuff.push_back(c);
    }
    char ProcessLetter(SDL_KeyboardEvent kevent){
      char res = (kevent.keysym.mod & (KMOD_LSHIFT | KMOD_RSHIFT)) 
        ? std::toupper('a' + kevent.keysym.scancode - 4)
        : ('a' + kevent.keysym.scancode - 4);
      return res;
    }
    void printDebug(uint32_t flags){
      if (flags & DebugPrintStates::Mouse) {
        term::move_up(DebugPrintStates::Mouse, true);
        term::curser_home();
        term::print_labeled_array("Mouse", 
            {debugStates.mouse.x, 
            debugStates.mouse.y, 
            debugStates.mouse.x_rel, 
            debugStates.mouse.y_rel}, 
            {"x", "y", "xrel", "yrel"});
        term::move_down(DebugPrintStates::Mouse);
      }
      if (flags & DebugPrintStates::Keyboard) {
        term::move_up(DebugPrintStates::Keyboard, true);
                term::curser_home();
        term::print_labeled_array("Keydown", 
            {(int32_t)debugStates.keyboard.scancode, 
            (int32_t)debugStates.keyboard.mod,
            (int32_t)debugStates.keyboard.sym},
            {"scancode", "mod", "sym"});
        term::move_down(DebugPrintStates::Keyboard);
      }
      if (flags & DebugPrintStates::TextBuff) {
        term::move_up(DebugPrintStates::TextBuff, true);
                term::curser_home();

        std::cout << std::string_view(keyBuff.data(), keyBuff.size()) << std::flush;
        term::move_down(DebugPrintStates::TextBuff);
      }

      // term::move_up(5, true);
      // term::print_labeled_array("Mouse", 
      //     {event.motion.x, event.motion.y, 
      //     event.motion.xrel, event.motion.yrel,
      //     (Sint32)event.motion.windowID}, 
      //     {"x","y","xrel","yrel", "window"});
      // term::move_down(5);
      //
    }
};

int main(){
  SystemStruct *CoreSysStruct = new SystemStruct();
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
  CoreSysStruct->initPrint();
  while(gameIsRunning){
    SDL_Event event;
    // start loop:
    // while(SDL_PollEvent(&event)){
    if (SDL_WaitEvent(&event)){
      CoreSysStruct->printDebug(DebugPrintStates::All);
      switch (event.type){
        using enum SDL_EventType;
        [[likely]] case SDL_MOUSEMOTION:
          CoreSysStruct->debugStates.mouse = {
            .x = event.motion.x, 
            .y = event.motion.y, 
            .x_rel = event.motion.xrel, 
            .y_rel = event.motion.yrel
          };
          break;
        case SDL_KEYDOWN:
          CoreSysStruct->debugStates.keyboard = {
            .scancode = event.key.keysym.scancode,
            .mod = event.key.keysym.mod,
            .sym = static_cast<uint_fast32_t>(event.key.keysym.sym)
          };

          if (event.key.keysym.scancode >= SDL_SCANCODE_A && event.key.keysym.scancode <= SDL_SCANCODE_Z) {
            CoreSysStruct->pushKeystroke(CoreSysStruct->ProcessLetter(event.key));
          // char res = '_';
          // if(event.key.keysym.mod & (KMOD_LSHIFT | KMOD_RSHIFT)){
          //   res = std::toupper('a'+event.key.keysym.scancode-4);
          // }
          // else{
          //   res = ('a'+event.key.keysym.scancode-4);
          // }
          // CoreSysStruct->keyBuff.push_back(res);
          // term::move_up(3,true);
          // std::cout<< std::string_view(CoreSysStruct->keyBuff)<<std::flush;
          // term::move_down(3);
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
