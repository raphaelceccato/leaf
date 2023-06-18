#pragma once
#include "texture.h"
#include "animation.h"
#include "rendertarget.h"


namespace leaf {
	class GameObject;
	typedef std::shared_ptr<GameObject> GameObjectPtr;

	class GameObject {
	public:
		GameObject() {
			animator = std::make_shared<Animator>(nullptr);
			gameObjectId = ++lastGameObjectId;
		}


		unsigned int getGameObjectId() const { return gameObjectId; }
		glm::ivec2 getPosition() const { return pos; }
		void setPosition(const glm::ivec2 pos) { this->pos = pos; }
		AnimatorPtr getAnimator() const { return animator; }


		void drawOnTarget(RenderTargetPtr target) {
			if (auto anim = animator->getAnimation()) {
				auto rect = animator->getCurrentFrame();
				target->drawEx(anim->getTexture(), pos.x, pos.y, rect.w, rect.h, rect, 0);
			}
		}


	protected:
		glm::ivec2 pos;
		AnimatorPtr animator;

	private:
		unsigned int gameObjectId;
		static unsigned int lastGameObjectId;

		void prepareStatic() {
			unsigned int lastGameObjectId = 0;
		}
	};
}