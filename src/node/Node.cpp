#include "Node.hpp"

void Node::update(float dt, const InputManager& inputs) {
  if (!active) return;
  for (uint i =0; i< child_count; ++i) {
    // TODO verify if this check is needed. 
    if (children[i] != nullptr){
      children[i]->update(dt, inputs);
    }
  }
}

// base only signals down a render chain start
void Node::render(Renderer& renderer) {
  // std::cout<<"call Node Parent Render " <<active << std::endl;
  if (!active) return;
  for (uint32_t i = 0; i < child_count; ++i) {
    if (children[i] != nullptr){
      children[i]->render(renderer);
    }
  }
}

void Node::addChild(Node* child) {
  child->parent = this;
  // TODO: vectors... assuming i really dont have too many children (or if its child spawning i'd manage a different way)
  //    will i ever be concerned abt capping children?
  std::cerr<<" we really shouldnt need to reallocate, check status of " << this->name << " and reeval" <<std::endl;
  if (child_count >= child_cap) {
    child_cap = (child_cap == 0) ? 4 : child_cap * 2;
    Node** new_children = new Node*[child_cap];
    // copy
    if (children) {
      for (uint32_t i = 0; i < child_count; ++i) {
        new_children[i] = children[i];
      }
      delete[] children;
    }
    children = new_children;
  }

  children[child_count++] = child;
}


