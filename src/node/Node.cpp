#include "Node.hpp"

void Node::update(float dt) {
  if (!active) return;
  for (uint i =0; i< child_count; ++i) {
    // TODO verify if this check is needed. 
    if (children[i] != nullptr){
      children[i]->update(dt);
    }
  }
}

// base only signals down a render chain start
void Node::render() {
  // std::cout<<"call Node Parent Render " <<active << std::endl;
  if (!active) return;
  for (uint32_t i = 0; i < child_count; ++i) {
    if (children[i] != nullptr){
      children[i]->render();
    }
  }
}

void Node::addChild(Node* child) {
  child->parent = this;
  // TODO: vectors... assuming i really dont have too many children (or if its child spawning i'd manage a different way)
  //    will i ever be concerned abt capping children?
  if (child_count >= child_cap) {
    std::cerr<<" we really shouldnt need to reallocate, check status of " << this->name << " and reeval" <<std::endl;
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

void Node::removeChild(Node* child) {
  for (uint32_t i = 0; i < child_count; ++i) {
    if (children[i] == child) {
      // gap plug shift 
      for (uint32_t j = i; j < child_count - 1; ++j) {
        children[j] = children[j + 1];
      }
      child_count--;
      child->parent = nullptr; 
      return;
    }
  }
}


