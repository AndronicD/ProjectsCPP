#include "lab_m1/Tema2/PlayerCar.h"

PlayerCar::PlayerCar(){};
PlayerCar::~PlayerCar(){};

glm::vec3 PlayerCar::getPos() { return position; };
void PlayerCar::setPos(glm::vec3 pos) { position = pos; }

glm::vec3 PlayerCar::getVel() { return velocity; };
void PlayerCar::setVel(glm::vec3 vel) { velocity = vel; }

float PlayerCar::getRot() { return rotY; }
void PlayerCar::setRot(float rot) { rotY = rot; }

float PlayerCar::getCarSpeed() { return carSpeed; }
void PlayerCar::setCarSpeed(float carS) { carSpeed = carS; };

float PlayerCar::getTurnSpeed() { return turnSpeed; }
void PlayerCar::setTurnSpeed(float turnS) { turnSpeed = turnS; }