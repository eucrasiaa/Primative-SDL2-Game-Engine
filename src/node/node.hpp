#pragma once
#include "../rendering/renderer.hpp"
#include <iostream>
#include <stdint.h>

class Node {
  protected: 
    uint32_t           id;
    char               name[64];
    Node*              parent = nullptr;


    Node**             children = nullptr;
    uint               child_count =0;
    uint               child_cap   =0;

    bool               active = true;
    void*              owner;
    bool               queued_free;
  public:
    // ? will i doubt id ever call it when theres children id travese first. but
    // also does this.. FIXME?
    virtual ~Node() {
      if (children) {
        for (uint32_t i = 0; i < child_count; ++i) {
          delete children[i];
        }
        delete[] children;
      }
    }

    void queueFree() { queued_free = true; }
    bool isQueuedForDeletion() const { return queued_free; }


    virtual void ready() {}

    virtual void update(float dt);
    virtual void render(Renderer& renderer);
    void addChild(Node* child);
};
