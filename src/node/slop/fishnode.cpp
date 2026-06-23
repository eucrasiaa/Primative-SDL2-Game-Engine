#include "../Node.hpp"
#include "../Node2D.hpp"

class FishNode_S2D : public Node2D{
  public:
  FishNode_S2D() : Node2D() {
    // Child initialization code here
  }

// Calling a specific parameterized parent constructor via initializer list
  
void update(float dt, const InputManager& inputs) {
  if(inputs.isActionHeld("input_up")){
    this->Position.y -=2;  
  }

  if(inputs.isActionHeld("input_down")){
    this->Position.y +=2;  
  }

  if(inputs.isActionHeld("input_right")){
    this->Position.x +=2;  
  }

  if(inputs.isActionHeld("input_left")){
    this->Position.x -=2;  
  }

  Node2D::update(dt, inputs);
  }
};
