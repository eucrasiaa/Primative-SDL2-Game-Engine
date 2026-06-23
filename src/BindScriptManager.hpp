#pragma once
#include <SDL_scancode.h>
#include <iostream>
#include <string_view>
#include <ranges>
#include <fstream>
#include <sstream>
#include <SDL2/SDL.h>
#include "InputManager.hpp"



constexpr std::string_view script_source = R"(
    input_up:    W, UP
    input_down:  S, DOWN
    input_left:  A, LEFT
    input_right: D, RIGHT
    quit_game:   ESC
    resize_down: [
    resize_up:   ]
)";



struct KeyBind {
    std::string_view action;
    SDL_Scancode key;
};

constexpr SDL_Scancode string_to_scancode(std::string_view token) {
    if (token == "W")     return SDL_SCANCODE_W;
    if (token == "UP")    return SDL_SCANCODE_UP;
    if (token == "S")     return SDL_SCANCODE_S;
    if (token == "DOWN")  return SDL_SCANCODE_DOWN;
    if (token == "A")     return SDL_SCANCODE_A;
    if (token == "LEFT")  return SDL_SCANCODE_LEFT;
    if (token == "D")     return SDL_SCANCODE_D;
    if (token == "RIGHT") return SDL_SCANCODE_RIGHT;
    if (token == "ESC")   return SDL_SCANCODE_ESCAPE;
    if (token == "[")     return SDL_SCANCODE_LEFTBRACKET;
    if (token == "]")     return SDL_SCANCODE_RIGHTBRACKET;
    return SDL_SCANCODE_UNKNOWN; // fallback
}

struct FixedBindingList {
    std::array<KeyBind, 64> data{};
    size_t count = 0;

    constexpr void push_back(KeyBind bind) {
        if (count < data.size()) {
            data[count] = bind;
            count++;
        }
    }
};


constexpr std::string_view trim(std::string_view str) {
    auto start = str.find_first_not_of(" \t\n\r");
    if (start == std::string_view::npos) return "";
    auto end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

constexpr FixedBindingList parse_script(std::string_view text) {
    FixedBindingList list;
    size_t start = 0;
    while (start < text.size()) {
        size_t end = text.find('\n', start);
        if (end == std::string_view::npos) {
            end = text.size();
        }
        std::string_view line = trim(text.substr(start, end - start));
        start = end + 1;
        if (line.empty()) continue;
        size_t colon = line.find(':');
        if (colon == std::string_view::npos) continue;
        std::string_view action = trim(line.substr(0, colon));
        std::string_view keys_blob = line.substr(colon + 1);
        size_t key_start = 0;
        while (key_start < keys_blob.size()) {
            size_t comma = keys_blob.find(',', key_start);
            if (comma == std::string_view::npos) {
                comma = keys_blob.size();
            }
            std::string_view key_token = trim(keys_blob.substr(key_start, comma - key_start));
            key_start = comma + 1;
            if (key_token.empty()) continue;
            list.push_back({ action, string_to_scancode(key_token) });
        }
    }
    return list;
}

constexpr FixedBindingList compiled_bindings = parse_script(script_source);


inline void loadBindingsCompiled(InputManager& input) {
    for (size_t i = 0; i < compiled_bindings.count; ++i) {
        const auto& bind = compiled_bindings.data[i];
        input.addKeyMapping(std::string(bind.action), bind.key);
    }
}
