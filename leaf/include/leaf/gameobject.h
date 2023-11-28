#pragma once
#include "rendertarget.h"
#include "texture.h"
#include "animation.h"


namespace leaf {
	class GameObject;
	typedef std::shared_ptr<GameObject> GameObjectPtr;

	class GameObject {
	public:
		GameObject() {
			static unsigned int lastGameObjectId = 0;
			animator = std::make_shared<Animator>(nullptr);
			gameObjectId = ++lastGameObjectId;
		}


		unsigned int getGameObjectId() const { return gameObjectId; }
		glm::ivec2 getPosition() const { return pos; }
		void setPosition(const glm::ivec2 pos) { this->pos = pos; }
		AnimatorPtr getAnimator() const { return animator; }


		void drawOnTarget(RenderTargetPtr target) {
			if (auto anim = animator->getAnimation()) {
				auto frame = animator->getCurrentFrame();
				target->drawEx(anim->getTexture(), pos.x - frame.offset.x, pos.y - frame.offset.y, frame.rect.w, frame.rect.h, frame.rect, 0);
			}
		}


	protected:
		glm::ivec2 pos;
		AnimatorPtr animator;

	private:
		unsigned int gameObjectId;
	};
}
