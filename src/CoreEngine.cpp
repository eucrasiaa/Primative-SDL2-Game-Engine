#include "CoreEngine.hpp"
#include "BindScriptManager.hpp"


void CoreEngine::handleEvents(){
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    sysStruct->printDebug(DebugPrintStates::All);
    input.processEvent(event);
    switch (event.type) {
      [[likely]] case SDL_MOUSEMOTION:
        sysStruct->debugStates.mouse = {
          .x = event.motion.x, 
          .y = event.motion.y, 
          .x_rel = event.motion.xrel, 
          .y_rel = event.motion.yrel
        };
      break;
      case SDL_KEYDOWN:
      sysStruct->debugStates.keyboard = {
        .scancode = event.key.keysym.scancode,
        .mod = event.key.keysym.mod,
        .sym = static_cast<unsigned int>(event.key.keysym.sym)
      };
      if (event.key.keysym.sym == SDLK_BACKSPACE) {
        sysStruct->processBackspace();
      }
      else if (event.key.keysym.scancode >= SDL_SCANCODE_A && event.key.keysym.scancode <= SDL_SCANCODE_Z) {
        sysStruct->pushKeystroke(sysStruct->ProcessLetter(event.key));
      }
      break;

      case SDL_QUIT:
      running = false;
      break;
    }
  }
}

void CoreEngine::run() {
  while (running) {
    input.updateStartOfFrame();
    sysStruct->frameStart = SDL_GetTicks();

    handleEvents();

    // GAME LOGIC & UI STATE UPDATES HERE
    // RENDER FRAME HERE

    Uint32 frameTime = SDL_GetTicks() - sysStruct->frameStart;
    if (FIXED_TIME_STEP > frameTime) {
      SDL_Delay(FIXED_TIME_STEP - frameTime);
    }
  }
}

CoreEngine::~CoreEngine() {
  SDL_Quit();
}

void PrintA(void*){
  printf("hihihiihihihihihih\n\n\nhihihih");
}

struct and_self{
  void * first;
  CoreEngine * self;
};
void QuitGameBind(void *game){
  static_cast<CoreEngine*>(game)->killgame();
}
void CoreEngine::setupBindings(){
  loadBindingsCompiled(input);
  input.addActionFunctMap("input_up", *PrintA);
  and_self* args = new and_self();
  args->first = nullptr;
  args->self = this;

  input.addActionFunctMap("quit_game", 
      [](void* context) {
          if (context != nullptr) {
              auto* engine = static_cast<CoreEngine*>(context);
              engine->killgame();
          }
      }, 
      this
  );

      // input.addKeyMapping("input_up", SDL_SCANCODE_W);
      // input.addKeyMapping("input_up", SDL_SCANCODE_UP);
      // input.addKeyMapping("input_down", SDL_SCANCODE_S); 
      // input.addKeyMapping("input_down", SDL_SCANCODE_DOWN);
      //
      //
      // input.addKeyMapping("input_left", SDL_SCANCODE_A); 
      // input.addKeyMapping("input_left", SDL_SCANCODE_LEFT); 
      //
      // input.addKeyMapping("input_right", SDL_SCANCODE_D); 
      // input.addKeyMapping("input_right", SDL_SCANCODE_RIGHT); 
    }
