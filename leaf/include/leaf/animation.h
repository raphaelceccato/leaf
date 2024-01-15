#pragma once
#include "texture.h"
#include "timer.h"
#include "rect.h"
#include <vector>
#include <memory>
#include <exception>
#include <glm/vec2.hpp>

namespace leaf {
	class Animation;
	typedef std::shared_ptr<Animation> AnimationPtr;

	struct AnimFrame {
		Rect<int> rect;
		glm::ivec2 offset;

		AnimFrame(Rect<int> rect, glm::ivec2 offset = glm::ivec2(0, 0)) {
			this->rect = rect;
			this->offset = offset;
		}
	};

	class Animation {
	public:
		static AnimationPtr createStaticAnimation(TexturePtr tex, Rect<int> rect, glm::ivec2 offset = glm::ivec2(0, 0)) {
			auto anim = std::make_shared<Animation>(tex, 1, false);
			anim->addFrame(rect, offset);
			return anim;
		}

		Animation(TexturePtr tex, unsigned int frameTime, bool repeat) {
			this->tex = tex;
			this->frameTime = frameTime;
			this->repeat = repeat;
		}
		void addFrame(Rect<int> rect, glm::ivec2 offset = glm::ivec2(0, 0)) {
			frames.emplace_back(rect, offset);
		}
		AnimFrame& getFrame(int index) {
			if (frames.empty())
				throw std::exception("animation is empty");
			return frames[index];
		}
		int getFrameCount() const { return frames.size(); }
		TexturePtr getTexture() const { return tex; }
		bool isRepeating() const { return repeat; }
		int getDuration() const { return (frameTime * frames.size()); }
		int getFrameTime() const { return frameTime; }
	private:
		TexturePtr tex;
		unsigned int frameTime;
		bool repeat;
		std::vector<AnimFrame> frames;

		friend class Animator;
	};


	class Animator;
	typedef std::shared_ptr<Animator> AnimatorPtr;

	class Animator {
	public:
		Animator(AnimationPtr anim = nullptr) { this->anim = anim; }
		AnimationPtr getAnimation() const { return anim; }
		void setAnimation(AnimationPtr anim) {
			this->anim = anim;
			reset();
		}
		void reset() { timer.reset(); }
		bool hasFinished() const { return (!anim || (!anim->isRepeating() && timer.getElapsedTime() >= anim->getDuration())); }
		AnimFrame& getCurrentFrame() const {
			if (!anim)
				throw std::exception("no animation defined for animator");
			return anim->getFrame((anim->isRepeating() ? (timer.getElapsedTime() / anim->getFrameTime()) % anim->frames.size()
				: std::min(timer.getElapsedTime() / anim->getFrameTime(), (int)anim->frames.size() - 1)));
		}
	private:
		AnimationPtr anim;
		Timer timer;
	};
}
