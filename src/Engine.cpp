#include "Engine.hpp"
#include "BindScriptManager.hpp"
#include "SystemStruct.hpp"
#include "WindowManager.hpp"

Engine* Engine::we_globalEngineInstance = nullptr;

bool Engine::InitEngine() {
    if (we_globalEngineInstance) {
        std::cerr << "Engine already initialized!" << std::endl;
        return true;
    }

    we_globalEngineInstance = new Engine();
    return we_globalEngineInstance->wef_initInternal();
}

bool Engine::wef_initInternal() {
  //--sdl
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return false;
  }
  
  //--the systems:
  we_input = std::make_unique<InputManager>();
  we_sysStruct = std::make_unique<SystemStruct>();
  we_gameWindow = std::make_unique<WindowInstance>();
  // we_debugWindow = std::make_unique<WindowInstance>();

  //--rendering
  we_gameWindow->initialize("Game Window");
  // debugWindow.initialize("Debug View", 400, 400);
  auto* ren = we_gameWindow->getRenderer();
  if (ren) {
    SDL_SetRenderDrawColor(ren, 33, 150, 243, 255);
    SDL_RenderClear(ren);
    SDL_RenderPresent(ren);
  }
  we_coreRenderer = std::make_unique<Renderer>(ren);
  
  //--binding fun
  wef_setupBindingsInternal();



  we_sysStruct->init();
  
  /* testing */

  // FishNode_S2D* fish2D = new FishNode_S2D();
  // Node* aNode = new Node();
  // Sprite2D* tex = new Sprite2D(3);
  // aNode->addChild(fish2D);
  // fish2D->addChild(tex);
  // we_activeScene.reset(aNode);

  /* end testing */


  we_activeRunning = true;
  return true;
}

void Engine::Run() {

    if (!we_globalEngineInstance) {
        std::cerr << "Engine not initialized!" << std::endl;
        return;
    }
    we_globalEngineInstance->wef_runInternal();
}


void Engine::wef_runInternal() {
  double accumulator = 0.0;
  const double FIXED_DT = 1.0 / 60.0; 
  const double MAX_DELTA = 0.25; //death spiral cap

  // stages: 
  // get frame and time info
  // update input states by pushing current to past frame
  // handleEvents 
  // fixed scale update for physics primarily
  // gather render queue
  // flush render queue
  // present
  uint64_t frequency = SDL_GetPerformanceFrequency();
  //ensure right before loop for first frame fun
  uint64_t currentTicks = SDL_GetPerformanceCounter();
  while (we_activeRunning) {
    
    // for simulation speed math
    uint64_t newTicks = SDL_GetPerformanceCounter();
    // exact real time elapsed, seconds
    double frameTime = static_cast<double>(newTicks - currentTicks) / frequency;
    currentTicks = newTicks;
    

    //spiral catch
    if (frameTime > MAX_DELTA) {
      frameTime = MAX_DELTA;
    }
    //time as currency #what moron wrote these comments
    accumulator += (frameTime*we_timeScale);
    
    // update events sync
    we_input->updateStartOfFrame();
    
    // handle events
    wef_handleEventsInternal();
    
    // physics and game logic here
    // todo: split phys to thread pool?
    while (accumulator >= FIXED_DT) {
      if (we_activeScene != nullptr) {
        // Physics and vital game logic updates use FIXED_DT, NOT variable frameTime
        we_activeScene->update(FIXED_DT);
      }
      accumulator -= FIXED_DT;
    }

    //double alpha = accumulator / FIXED_DT;
    if (we_activeScene != nullptr) {
      // condider DT for interpolation alpha
      we_activeScene->render();//*we_coreRenderer); //, alpha);
    }
    we_coreRenderer->flushQueue();

    auto *ren = we_gameWindow->getRenderer();
    SDL_RenderPresent(ren);
    // ???
    // const double MIN_FRAME_TIME = 1.0 / 120.0;
    // while ((static_cast<double>(SDL_GetPerformanceCounter() - newTicks) / frequency) < MIN_FRAME_TIME) {
    //   SDL_Delay(0); 
    // }
  }
}

void Engine::Shutdown() {
    if (we_globalEngineInstance) {
        we_globalEngineInstance->wef_shutdownInternal();
        delete we_globalEngineInstance;
        we_globalEngineInstance = nullptr;
    }
}
void Engine::wef_shutdownInternal() {
    we_activeRunning = false;
    // most of these dont exist yet oops
    we_activeScene.reset();
    we_coreRenderer.reset();
    we_input.reset();
    we_sysStruct.reset();
    we_gameWindow.reset();
    // m_debugWindow.reset();
    SDL_Quit();
}
void Engine::KillGame() {
    if (we_globalEngineInstance) {
        we_globalEngineInstance->we_activeRunning = false;
        std::cout << "stopped running" << std::endl;
    }
}

void Engine::SetActiveScene(Node* scene) {
    if (!we_globalEngineInstance) return;
    we_globalEngineInstance->we_activeScene.reset(scene);
}

void Engine::wef_handleEventsInternal(){
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    we_sysStruct->printDebug(DebugPrintStates::All);
    we_input->processEvent(event);
    switch (event.type) {
      [[likely]] case SDL_MOUSEMOTION:
          we_sysStruct->debugStates.mouse = {
            .x = event.motion.x, 
            .y = event.motion.y, 
            .x_rel = event.motion.xrel, 
            .y_rel = event.motion.yrel
          };
        break;
      case SDL_KEYDOWN:
        we_sysStruct->debugStates.keyboard = {
          .scancode = event.key.keysym.scancode,
          .mod = event.key.keysym.mod,
          .sym = static_cast<unsigned int>(event.key.keysym.sym)
        };
        if (event.key.keysym.sym == SDLK_BACKSPACE) {
          we_sysStruct->processBackspace();
        }
        else if (event.key.keysym.scancode >= SDL_SCANCODE_A && event.key.keysym.scancode <= SDL_SCANCODE_Z) {
          we_sysStruct->pushKeystroke(we_sysStruct->ProcessLetter(event.key));
        }
        break;

      [[unlikely]] case SDL_QUIT:
        Shutdown();
        break;
    }
  }
}


void Engine::wef_setupBindingsInternal() {
    loadBindingsCompiled(*we_input);
    
    // Testing bindings
    we_input->addActionFunctMap("input_up", 
        [](void*) {
            printf("hihihiihihihihihih\n\n\nhihihih\n\n\n");
        }, 
        nullptr
    );
    
    we_input->addActionFunctMap("quit_game", 
        [](void* context) {
            if (context != nullptr) {
                auto* engine = static_cast<Engine*>(context);
                engine->KillGame();
            }
        }, 
        this
    );
}

void Engine::processGarbageCollection(Node* root) {
  if (!root) return;
  // POST ORDER!! children up.. NOTE NO INCREMENET, WE SWAP?
  // TODO. validate in connection with the ~ decontrucct to make sure i dont double it LOL
  for (uint32_t i = 0; i < root->child_count; ) {
    if (root->children[i]->isQueuedForDeletion()) {
      // calls ~ ... be aware...
      delete root->children[i];  

      // shift for gap?
      for (uint32_t j = i; j < root->child_count - 1; ++j) {
        root->children[j] = root->children[j + 1];
      }
      root->child_count--;
      // Do not increment i, evaluate the swapped element
    } else {
      processGarbageCollection(root->children[i]);
      ++i;
    }
  }
}

