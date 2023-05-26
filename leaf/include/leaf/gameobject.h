#pragma once
#include "export.h"
#include "texture.h"
#include "animation.h"
#include "rendertarget.h"


namespace leaf {
	class EXPORT GameObject {
	public:
		GameObject();
		unsigned int getGameObjectId() const { return gameObjectId; }
		glm::ivec2 getPosition() const { return pos; }
		void setPosition(const glm::ivec2 pos) { this->pos = pos; }
		AnimatorPtr getAnimator() const { return animator; }
		void drawOnTarget(RenderTargetPtr target);

	protected:
		glm::ivec2 pos;
		AnimatorPtr animator;

	private:
		unsigned int gameObjectId;
		static unsigned int lastGameObjectId;
	};


	typedef std::shared_ptr<GameObject> GameObjectPtr;
}