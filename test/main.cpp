#include <leaf/leaf.h>
#include <iostream>

using namespace leaf;

int main(int argc, char** argv) {
	try {
		Engine engine;
		auto win = engine.createWindow("leaf test", 640, 480);
		auto tex = Texture::create("leaf.png");
		auto rt = RenderTexture::create(640, 480);
		bool running = true;
		SDL_Event ev;
		while (running) {
			while (win->pollEvent(&ev)) {
				switch (ev.type) {
				default:
					break;
				case SDL_QUIT:
					running = false;
					break;
				}
			}
			win->clear(Color(0, 0, 0));
			auto size = win->getSize();
			rt->clear(Color(0, 255, 0));
			rt->drawEx(tex, 64, 32, 128, 128, Rect<int>(0, 0, tex->getWidth(), tex->getHeight())
				, 6.28f * 0.001f * engine.getTicks());
			win->draw(rt, 0, 0, size.x/2, size.y/2);
			win->redraw();
		}
	}
	catch (const std::exception& ex) {
		std::cout << ex.what() << std::endl;
	}
	return 0;
}