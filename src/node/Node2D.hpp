#pragma once
#include "Node.hpp"
#include "wtypes/Vec2.hpp"
class Node2D : public Node{
  public:
    Vec2 Position = Vec2::ZERO;
    Vec2 Rotation = Vec2::ZERO;
    Vec2 Scale = Vec2::ZERO;

};
