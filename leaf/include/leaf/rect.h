#ifndef __LEAF_RECT__
#define __LEAF_RECT__

#include <algorithm>


namespace leaf {
	template <typename T>
	struct Rect {
		T x, y, w, h;

		Rect(T x = 0, T y = 0, T w = 0, T h = 0) : x(x), y(y), w(w), h(h) {}
		bool isInside(int x, int y) const {
			return (x >= this->x && y >= this->y && x < (this->x + this->w) && y < (this->y + this->h));
		}
		int getArea() const {
			return w * h;
		}
		bool isOverlapping(const Rect& other) const {
			return (other.isInside(x, y) || other.isInside(x + w, y) || other.isInside(x, y + h) || other.isInside(x + w, y + h));
		}
		bool operator==(const Rect& other) const {
			return (x == other.x && y == other.y && w == other.w && h == other.h);
		}
		bool operator!=(const Rect& other) const {
			return (x != other.x || y != other.y || w != other.w || h != other.h);
		}
	};
}

#endif
