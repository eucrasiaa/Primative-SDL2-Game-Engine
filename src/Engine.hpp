#pragma once
// ok rework time. yay.
// fundimentally this rework will be about breaking CoreEngine into its internal processes
// a psudo public Engine with the global static states for things for more universal access
// eg inputs should be global reads not passed to everything?
#include "InputManager.hpp"
#include "bindings/BindingSystem.hpp"
#include "rendering/renderer.hpp"
#include "wtypes/Vec2.hpp"
#include "node/Node.hpp"
#include <memory>

// ive written static so many times my fingers are bleeding
class Node;
class WindowInstance;
struct SystemStruct;

class Engine{
  private: 
    static Engine* we_globalEngineInstance;

    // use of unique_ptr for lifetime management is good practice ive read.
    // TODO follow up on this
    std::unique_ptr<InputManager> we_input;
    std::unique_ptr<Renderer> we_coreRenderer;
    std::unique_ptr<SystemStruct> we_sysStruct;


    std::unique_ptr<WindowInstance> we_gameWindow;

    std::unique_ptr<Node> we_activeScene;

    // runtime info:
    bool we_activeRunning = false;
    Uint32 ugh_frameStart =0;
    double we_timeScale = 1.0;


    //methods:
    // these are private to ensure the static like. works
    Engine() = default;
    ~Engine() = default;

    bool wef_initInternal();
    void wef_runInternal();
    void wef_shutdownInternal();
    void wef_handleEventsInternal();
    void wef_setupBindingsInternal();

    void processGarbageCollection(Node* root);
  public:
    // copy blocking?
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
    Engine(Engine&&) = delete;
    Engine& operator=(Engine&&) = delete;

    //lifecycle, static api access:
    // starts up sdl and inits subsystems
    static bool InitEngine();
    // begins game loop
    static void Run();
    // safe shutdown
    static void Shutdown();
    // stops running loop, but doesnt clean up anything else
    static void KillGame();

    //the global accessors:
    static Engine* Get() { return we_globalEngineInstance; }

    static InputManager* GetInput() { 
      return we_globalEngineInstance ? we_globalEngineInstance->we_input.get() : nullptr; 
    }

    static Renderer* GetRenderer() {
      return we_globalEngineInstance ? we_globalEngineInstance->we_coreRenderer.get() : nullptr;
    }

    static SystemStruct* GetSysStruct() {
      return we_globalEngineInstance ? we_globalEngineInstance->we_sysStruct.get() : nullptr;
    }


    //sceneslop
    static void SetActiveScene(Node* scene);
    static Node* GetActiveScene() {
      return we_globalEngineInstance ? we_globalEngineInstance->we_activeScene.get() : nullptr;
    }


    // -------- INPUT ________________________
    static bool IsActionHeld(std::string_view action) {
      auto* input = GetInput();
      return input && input->isActionHeld(action);
    }
    static bool IsActionPressed(std::string_view action) {
      auto* input = GetInput();
      return input && input->isActionJustPressed(action);
    }
    static bool IsActionReleased(std::string_view action) {
      auto* input = GetInput();
      return input && input->isActionJustReleased(action);
    }

    //godot clones here i just like them
    static float GetAxis(std::string_view negative, std::string_view positive) {
      float value = 0.0f;
      if (IsActionHeld(negative)) value -= 1.0f;
      if (IsActionHeld(positive)) value += 1.0f;
      return value;
    }


    static Vec2 GetVector(std::string_view left, std::string_view right,
        std::string_view up, std::string_view down) {
      // prob good practice to have it throw err if none found lol
      Vec2 vec{0.0f, 0.0f};
      if (IsActionHeld(left)) vec.x -= 1.0f;
      if (IsActionHeld(right)) vec.x += 1.0f;
      if (IsActionHeld(up)) vec.y -= 1.0f;
      if (IsActionHeld(down)) vec.y += 1.0f;
      //TODO:
      vec.normalize(); // is this needed? 
      return vec;
    }

    // === Runtime Status ===
    static bool IsGameRunning() {
      return we_globalEngineInstance && we_globalEngineInstance-> we_activeRunning;
    }
};

#define ENGINE_INPUT Engine::GetInput()
#define ENGINE_RENDERER Engine::GetRenderer()
#define ENGINE_SCENE Engine::GetActiveScene()
#define ENGINE_SYSSTRUCT Engine::GetSysStruct()

