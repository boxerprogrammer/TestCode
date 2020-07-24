#include "Equipment.h"


Equipment::Equipment(std::shared_ptr<CollisionManager> cm, std::shared_ptr<Camera> camera):collisionManager_(cm),
camera_(camera){
}