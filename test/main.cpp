#include <leaf/leaf.h>
#include <iostream>

using namespace leaf;

int main(int argc, char** argv) {
	try {
		Engine engine;
		Window& win = engine.createWindow("leaf test", 640, 480, false);
		auto tex = Texture::create("leaf.png");
		bool running = true;
		SDL_Event ev;
		while (running) {
			while (win.pollEvent(&ev)) {
				switch (ev.type) {
				default:
					break;
				case SDL_QUIT:
					running = false;
					break;
				}
			}
			win.clear(Color(0, 0, 0));
			win.drawEx(tex, 64, 32, 128, 128, Rect<int>(0, 0, tex->getWidth(), tex->getHeight()), FlipMode::None
				, 6.28f * 0.001f * engine.getTicks());
			win.redraw();
		}
	}
	catch (const std::exception& ex) {
		std::cout << ex.what() << std::endl;
	}
	return 0;
}
