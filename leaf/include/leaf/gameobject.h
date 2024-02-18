#ifndef __LEAF_GAMEOBJECT__
#define __LEAF_GAMEOBJECT__

#include "rendertarget.h"
#include "texture.h"
#include "animation.h"


namespace leaf {
	class GameObject {
	public:
		GameObject() {
			static unsigned int lastGameObjectId = 0;
			animator = new Animator(nullptr);
			gameObjectId = ++lastGameObjectId;
			flipMode = FlipMode::None;
		}

		~GameObject() {
			delete animator;
		}


		unsigned int getGameObjectId() const { return gameObjectId; }
		glm::ivec2 getPosition() const { return pos; }
		void setPosition(const glm::ivec2 pos) { this->pos = pos; }
		Animator* getAnimator() const { return animator; }
		FlipMode getFlipMode() const { return flipMode; }
		void setFlipMode(FlipMode flipMode) { this->flipMode = flipMode; }


		void drawOnTarget(RenderTarget* target) {
			if (auto anim = animator->getAnimation()) {
				auto frame = animator->getCurrentFrame();
				target->drawEx(anim->getTexture(), pos.x - frame.offset.x, pos.y - frame.offset.y, frame.rect.w, frame.rect.h, frame.rect, flipMode, 0);
			}
		}


	protected:
		glm::ivec2 pos;
		Animator* animator;
		FlipMode flipMode;

	private:
		unsigned int gameObjectId;
	};
}

#endif
