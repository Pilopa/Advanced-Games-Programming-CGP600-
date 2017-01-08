#pragma once

#include "TankBehaviorScript.h"
#include "ClassDef.h"

class EnemyBehaviorScript : public TankBehaviorScript {

	public:
		EnemyBehaviorScript();
		EnemyBehaviorScript(Transform* playerTransform);

		virtual void update(); // Overrides CustomScript::update
		void setPlayerTransform(Transform* playerTransform);

	private:
		Transform* playerTransform;

};