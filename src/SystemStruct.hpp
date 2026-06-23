#pragma once
#include <SDL2/SDL.h>
#include "InputManager.hpp"
typedef struct DebugStates{
  struct MouseStateType {
    int x = 0;
    int y = 0;
    int x_rel = 0;
    int y_rel = 0;
  };

  struct KeyboardStateType {
    unsigned int scancode = 0;
    unsigned int mod = 0;
    unsigned int sym = 0;
  };
  MouseStateType mouse;
  KeyboardStateType keyboard;
}DebugStates;


typedef enum DebugPrintStates{
  None = 0b0,
  Mouse = 0b10,
  Keyboard = 0b100,
  TextBuff = 0b1000,
  InputState = 0x10000,
  All = 0xFFFF,
}DebugPrintStates;

class SystemStruct{
  private:
  public:
    DebugStates debugStates;



    // const InputManager& input;
    std::vector<char> keyBuff;
    // SystemStruct(const InputManager& input) : input(input){
    SystemStruct(){ 
      keyBuff.reserve(1000);
    }
    ~SystemStruct() = default;

    void initPrint();
    
    void pushKeystroke(char c);
    void processBackspace();

    void init();
    // TODO: fix letter handling ? it only does alpha. or natively tie all basic letters to
    //  a bound funct. idk
    char ProcessLetter(SDL_KeyboardEvent kevent);
    void printDebug(uint32_t flags);
};

