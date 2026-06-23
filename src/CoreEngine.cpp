#include "CoreEngine.hpp"
#include "BindScriptManager.hpp"
#include "node/Sprite2D.hpp"
#include "node/Node2D.hpp"
#include "node/slop/fishnode.cpp"
#include <cstdint>


bool CoreEngine::init() {
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
  
  coreRenderer = new Renderer(ren);
  setupBindings();
  sysStruct->init();
  running = true;
  

  /* testing */

  FishNode_S2D *Fish2D = new FishNode_S2D();
  Node *aNode = new Node();
  Sprite2D *tex = new Sprite2D(3);
  aNode->addChild(Fish2D);
  Fish2D->addChild(tex);
  this->activeScene=aNode;

  /* end testing */

  return true;
}


CoreEngine::CoreEngine() {
  input = new InputManager();
  sysStruct = std::make_unique<SystemStruct>(*input);
}
void CoreEngine::killgame(){
  this->running=false;
}

void CoreEngine::handleEvents(){
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    sysStruct->printDebug(DebugPrintStates::All);
    input->processEvent(event);
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
  
  uint64_t currentTicks = SDL_GetPerformanceCounter();
  uint64_t frequency = SDL_GetPerformanceFrequency();
 
  double accumulator = 0.0;
  const double FIXED_DT = 1.0 / 60.0; 
  const double MAX_DELTA = 0.25; //death spiral cap

  while (running) {
    
    uint64_t newTicks = SDL_GetPerformanceCounter();
    // exact real time elapsed, seconds
    double frameTime = static_cast<double>(newTicks - currentTicks) / frequency;
    currentTicks = newTicks;
    

    //spiral catch
    if (frameTime > MAX_DELTA) {
      frameTime = MAX_DELTA;
    }
    //time as currency #what moron wrote these comments
    accumulator += frameTime;

    input->updateStartOfFrame();
    // sysStruct->frameStart = SDL_GetTicks();

    handleEvents();
    
    while (accumulator >= FIXED_DT) {
      if (activeScene != nullptr) {
        // Physics and vital game logic updates use FIXED_DT, NOT variable frameTime
        activeScene->update(FIXED_DT, *input);
      }
      accumulator -= FIXED_DT;
    }

    // if(activeScene !=nullptr){
    //   activeScene->update(dt, *inputs);
    //   activeScene->render(*coreRenderer);
    // }
    // GAME LOGIC & UI STATE UPDATES HERE
    // RENDER FRAME HERE
    if (activeScene != nullptr) {
      // condider DT for interpolation alpha
      activeScene->render(*coreRenderer);
    }
    coreRenderer->flushQueue();

    auto *ren =    gameWindow.getRenderer();
    SDL_RenderPresent(ren);
    // const double MIN_FRAME_TIME = 1.0 / 120.0;
    // while ((static_cast<double>(SDL_GetPerformanceCounter() - newTicks) / frequency) < MIN_FRAME_TIME) {
    //   SDL_Delay(0); 
    // }
    // uint64_t frameTime = SDL_GetTicks() - frameStart;
    // if (FIXED_TIME_STEP > frameTime) {
    //   SDL_Delay(FIXED_TIME_STEP - frameTime);
    // }
  }
}

CoreEngine::~CoreEngine() {
  delete coreRenderer;
  delete input;
  SDL_Quit();
}

void PrintA(void*){
  printf("hihihiihihihihihih\n\n\nhihihih\n\n\n");
}

struct and_self{
  void * first;
  CoreEngine * self;
};
void QuitGameBind(void *game){
  static_cast<CoreEngine*>(game)->killgame();
}
void ResizeGame(int8_t direction){
  if (direction > 0){

  }
}
void CoreEngine::setupBindings(){
  loadBindingsCompiled(*input);

  /* testing */
  input->addActionFunctMap("input_up", *PrintA, nullptr);
  input->addActionFunctMap("quit_game", 
      [](void* context) {
      if (context != nullptr) {
      auto* engine = static_cast<CoreEngine*>(context);
      engine->killgame();
      }
      }, 
      this
      );

  /* end testing */

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


