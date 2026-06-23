#pragma once
#include <iostream>
#include <string_view>



namespace Config {
  // add binds here
  constexpr std::string_view BINDINGS = R"(
        input_up:    W, UP
        input_down:  S, DOWN
        input_left:  A, LEFT
        input_right: D, RIGHT
        quit_game:   ESC
        resize_down: [
        resize_up:   ]
    )";

}
