#include "../Node.hpp"
#include "../Node2D.hpp"

class FishNode_S2D : public Node2D{
  public:
  float speed = 120.0f; 
  FishNode_S2D() : Node2D() {
    // Child initialization code here
  }

// Calling a specific parameterized parent constructor via initializer list
  
void update(float dt) {
  Vec2 movementInputs = Engine::GetVector("input_left","input_right","input_up","input_down");
  this->Position += movementInputs * speed * dt; 
  // if(inputs.isActionHeld("input_up")){
  //   this->Position.y -=2;  
  // }
  //
  // if(inputs.isActionHeld("input_down")){
  //   this->Position.y +=2;  
  // }
  //
  // if(inputs.isActionHeld("input_right")){
  //   this->Position.x +=2;  
  // }
  //
  // if(inputs.isActionHeld("input_left")){
  //   this->Position.x -=2;  
  // }

  // find a way to bake this in perhaps
  Node2D::update(dt);
  }
};
