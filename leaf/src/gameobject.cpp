#include "../include/leaf/gameobject.h"

namespace leaf {
	unsigned int GameObject::lastGameObjectId = 0;


	GameObject::GameObject() {
		animator = std::make_shared<Animator>(nullptr);
		gameObjectId = ++lastGameObjectId;
	}


	void GameObject::drawOnTarget(RenderTargetPtr target) {
		if (auto anim = animator->getAnimation()) {
			auto rect = animator->getCurrentFrame();
			target->drawEx(anim->getTexture(), pos.x, pos.y, rect.w, rect.h, rect, 0);
		}
	}
}