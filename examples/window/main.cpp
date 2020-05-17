/*
 *	MIT License
 *
 *	Copyright (c) 2020 Gaëtan Dezeiraud and Ribault Paul
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *	SOFTWARE.
 */

#include "Engine.h"
#include "Utils.h"
#include "IModelInstance.h"

#define PI	3.1415926535

using namespace	ExoEngine;
using namespace	ExoRenderer;

int		main(int argc, char **argv)
{
	std::string	path = ExoEngine::getPath(std::string(argv[0]));
	Engine		engine(path + "resources/settings.xml");
	IRenderer*	renderer = engine.getRenderer();
	IWindow*	window;
	IKeyboard*	keyboard;
	bool		run = true;

	renderer->initialize("example window", 1280, 720, WINDOWED, false);
	engine.getResourceManager()->load(path + "resources/resources.xml");
	window = renderer->getWindow();
	keyboard = renderer->getKeyboard();
	window->setVsync(true);

	ICursor* cursor = renderer->createCursor();
	std::shared_ptr<ITexture> cursorTexture = engine.getResourceManager()->get<ITexture>("cursor");
	cursor->setCursorTexture(cursorTexture);
	renderer->setCursor(cursor);

	ICamera*	camera = renderer->createCamera();
	camera->setPos(0, 0.5, 1);
	camera->setDir(0, 0, -1);
	camera->setUp(0, 1, 0);
	renderer->setCurrentCamera(camera);

	std::shared_ptr<Model> model = engine.getResourceManager()->get<Model>("model");
	IModelInstance* instance = renderer->instanciate(model.get());
	IModelInstance* instance2 = renderer->instanciate(model.get());
	renderer->add(instance);
	renderer->add(instance2);
	instance2->translate(glm::vec3(-1, 0, -1));

	while (run)
	{
		if (keyboard->isKeyDown(KEY_ESCAPE))
			run = false;
		if (window->getIsClosing())
			run = false;
		instance->rotate(PI / 300, glm::vec3(0, 1, 0));
		instance2->rotate(-PI / 30, glm::vec3(0, 1, 0));
		cursor->update();
		renderer->swap();
	}
	return (EXIT_SUCCESS);
}
