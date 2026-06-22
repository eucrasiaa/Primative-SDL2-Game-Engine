#include "SystemStruct.hpp"
#include "ansi_control.hpp"

char SystemStruct::ProcessLetter(SDL_KeyboardEvent kevent){
  char res = (kevent.keysym.mod & (KMOD_LSHIFT | KMOD_RSHIFT)) 
    ? std::toupper('a' + kevent.keysym.scancode - 4)
    : ('a' + kevent.keysym.scancode - 4);
  return res;
}

void SystemStruct::pushKeystroke(char c){
  keyBuff.push_back(c);
}
void SystemStruct::processBackspace(){
  keyBuff.pop_back();
}
void SystemStruct::printDebug(uint32_t flags){
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
void SystemStruct::initPrint(){
  std::cout<<"\n\n\n\n\n\n\n\n\n\n\n\n" <<std::flush;
}
void SystemStruct::init(){
  initPrint();
}
