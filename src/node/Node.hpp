#pragma once
#include "../Engine.hpp"

class Node;


constexpr size_t MAX_CONNECTIONS = 16;

struct Connection {
    Node* receiver;
    void (*callback)(Node* receiver, Node* emitter, void* data);
};

struct Signal {
    Connection connections[MAX_CONNECTIONS];
    int count = 0; 
};

class Node {
  protected: 
    uint32_t           id;
    char               name[64];
    Node*              parent = nullptr;


    Node**             children = nullptr;
    unsigned int               child_count =0;
    unsigned int               child_cap   =0;

    bool               active = true;
    void*              owner;
    bool               queued_free;
  public:
    friend class Engine;
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
    virtual void render();

    // some lifecycle helpers maybe
    virtual void onEnter() {}
    virtual void onExit() {}
    virtual void onPause() {}
    virtual void onResume() {}

    // child and stuff mgmt. #maternity
    // TODO: THIS SEEMS LIKE A THING TO FIX LATER!!
    // consider reworking to unique_ptrs because yea. i doubt itll ever not be true ownership?
    void addChild(Node* child);
    void removeChild(Node* child);
    
    template<typename T>
    T* addChild(T* child) {
        addChild(static_cast<Node*>(child)); 
        return child; 
    }
    
    // possibly in passing down mouse clicks, tbd
    virtual void onInputEvent(const SDL_Event& event) {};
};


/* handle me later plz ty
void processGarbageCollection(Node* root) {
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

 */

