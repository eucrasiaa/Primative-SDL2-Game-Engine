
# Will's SDL2 Game Engine

A modular 2D game engine experiment built with SDL2 and C++23.
Designed around a Godot-inspired scene tree architecture, hopefully with flexibility and use in my various projects.

> _Currently in active development - missing really any rendering at all, but eyeing OpenGL rendering pipeline and physics system are the next major milestones...

its VERY much in development. more a proof of concept.
needs basically a whole rewrite i have really inconsistent use of std:: functionality and im not really using C++ properly, ive written basically only C and Rust in the past 3ish years its so weird to come back to C++. also all my design notes were for C. ill upload those later sometime
## overview:
### Core Engine
- Game state and input state tracking
- SDL window/renderer lifecycle
- Root scene node ownership
- Main game loop with fixed timestep physics
- Event polling and input state propagation

The engine drives the update() and render() passes through the scene tree each frame. Separates it all! 


### Node System
Inspired by Godot's nodes. Will provide a structured and compositional(?) approach to game objects:
#### World-Space Nodes (Node2D)
- `Node2D` - Loc Rot Scale
- `Sprite2D` - Graphical elements(?)... semi working...
#### **coming soon:**
- `Collision2D` - Primitive shape definitions for physics
- `RigidBody`, `CharacterBody`, `StaticBody` - Physics bodies with collision responses
- `Area2D` - Non-physics collision detection zones
- `Camera` - Scene viewport control
Screen-Space Nodes (Overlay), UI elements (boxes, labels), Interactive elements (buttons, sliders), Timer - Time-based callbacks, Paths

### Scene Recipe System (WIP. IN DEVELOPMENT)
 scenes are defined declaratively
 - Signal Connection safety ensured recipe level
- Runtime scene instantiation via Object Factory
- Easy scene composition and reuse
#### in the end, the ideal pipeline will be (WIP)
1. user makes
- Custom node classes inheriting from base nodes, internal logic for them
- Scene recipes defining node hierarchies and connections
- Signal/method bindings and input mappings
- im not making a scripting language

1. Build Process:
- recipe parsed & nodes allocated via factory
- scene tree constructed, parent-child relationships etc
- connections bound using string-based dispatch system
- ready() called depth-first on all nodes

3. Runtime Loop:
- Events processed --> input states updated
- Fixed timestep physics update (?) idk terminology
- update(dt) called on all active nodes
- Render pass collects draw commands, sorting heiarhcy + layer
- GPU rendering (SOOONN!!!! )


### Input Management
flexible key binding system via `InputManager`

options:
```cpp
// connect physical key to abstract action strings
input.addKeyMapping("jump", SDL_SCANCODE_SPACE);
input.addKeyMapping("move_left", SDL_SCANCODE_A);

// then! 

// Query input state
if (input.isActionHeld("move_right")) { /*...*/ }
if (input.isActionJustPressed("jump")) { /*...*/ }

// Or bind actions to callbacks
input.addActionFunctMap("quit_game", quitCallback, &engine);
```
Stored well for fast lookup, im pretty sure :<
OR! a mini script in `BindScriptManager` in format of:
```
input_up:    W, UP
input_down:  S, DOWN
quit_game:   ESC
```
to auto bind at compile time (playing around with constexpr)

## Status
**done**
- Core engine loop with fixed timestep
- InputManager- action binding and querying
- Node hierarchy - parent/child relationships
- BindScriptManager - compile-time key bindings
    - SDL2 rendering (INCREDIBLY BASIC. more a Proof of Concept LOL)

**In Progress**
- Scene recipe parsing and tree construction
- Object Factory for runtime scene instantiation
- Physics system (collision detection/resolution)
- OpenGL rendering backend
- lots
**Planned**
- many new node types, but you can add your own right now :3 !
- Camera system with viewport transforms
- Path following nodes
- UI system with interactive elements
- memory safety checking. i think its fine?
# Build

Requirements:
- a C++23 supporting Compiler
- SDL2 libraries
- CMake recommended 
a minimal build scipt helper is included, as i can never remeber how to use CMake LOL
run the bash script in `b` for help, but 
`./b d` for debug build
`./b f` for release
`./b r` for running the build

CMake included was my little SDL2 + also Raylib usable so it actually works for both. but ignore that part :3 

or maybe: idk i haven't tried
```bash
mkdir build && cd build
cmake ..
make
./main
```
