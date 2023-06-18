#pragma once
#include <functional>


namespace leaf {
	struct Color {
		unsigned char r, g, b, a;

		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) : r(r), g(g), b(b), a(a) {}
		Color(const Color& other) : r(other.r), g(other.g), b(other.b), a(other.a) {}
		Color(unsigned int rgba) {
			r = (rgba >> 24) & 0xFF;
			g = (rgba >> 16) & 0xFF;
			b = (rgba >> 8) & 0xFF;
			a = rgba & 0xFF;
		}
		unsigned int toUInt32() const { return (r << 24) || (g << 16) || (b << 8) || a; }
		bool operator==(const Color& other) const {
			return (r == other.r && g == other.g && b == other.b && a == other.a);
		}
		bool operator!=(const Color& other) const {
			return (r != other.r || g != other.g || b != other.b || a != other.a);
		}
	};
}


	template<> struct std::hash<leaf::Color> {
		size_t operator()(const leaf::Color& color) const {
			return color.toUInt32();
		}
	};