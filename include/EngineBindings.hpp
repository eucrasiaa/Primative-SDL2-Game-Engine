#pragma once
#include "EngineConfig.hpp"
#include "../src/BindScriptManager.hpp"
#include "../src/Engine.hpp"

namespace Config {

      inline void onInputUp(void*) {
        printf("hihihiihihihihihih\n\n\nhihihih\n\n\n");
    }
    
    inline void onQuitGame(void* context) {
        if (context) {
            static_cast<Engine*>(context)->KillGame();
        }
    }
    inline void setupBindings(Engine* engine, InputManager* input) {
        loadBindingsCompiled(*input);
        input->addActionFunctMap("input_up", onInputUp, nullptr);
        input->addActionFunctMap("quit_game", onQuitGame, engine);
        
        // input->addActionFunctMap("input_up", 
        //     [](void*) { printf("hihihi...\n"); }, 
        //     nullptr
        // );
        //
        // input->addActionFunctMap("quit_game", 
        //     [](void* ctx) { 
        //         if (ctx) static_cast<Engine*>(ctx)->KillGame(); 
        //     }, 
        //     engine
        // );
    }
}
