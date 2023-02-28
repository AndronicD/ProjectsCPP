#pragma once
#include "components/simple_scene.h"

class PlayerCar {
	public:
		PlayerCar();
		~PlayerCar();
		const float RUN_SPEED = 2.0f;
		const float TURN_SPEED = 2.0f;
	
		glm::vec3 getPos();
		void setPos(glm::vec3 pos);

		glm::vec3 getVel();
		void setVel(glm::vec3 pos);

		float getRot();
		void setRot(float rot);

		float getCarSpeed();
		void setCarSpeed(float carS);

		float getTurnSpeed();
		void setTurnSpeed(float turnS);
	private:
		glm::vec3 position;
		glm::vec3 velocity;
		float rotY;
		float carSpeed;
		float turnSpeed;
};