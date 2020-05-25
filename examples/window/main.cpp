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

#define STEP	0.1
#define ANGLE	0.1

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

	renderer->initialize("example window", 1920, 1080, WINDOWED, false);
	engine.getResourceManager()->load(path + "resources/resources.xml");
	window = renderer->getWindow();
	keyboard = renderer->getKeyboard();
	window->setVsync(true);

	ICursor* cursor = renderer->createCursor();
	std::shared_ptr<ITexture> cursorTexture = engine.getResourceManager()->get<ITexture>("cursor");
	cursor->setCursorTexture(cursorTexture);
	renderer->setCursor(cursor);

	ICamera*	camera = renderer->createCamera();
	camera->setPos(0, 1, 3);
	camera->setDir(0, 0, -1);
	camera->setUp(0, 1, 0);
	renderer->setCurrentCamera(camera);

	// std::shared_ptr<Model> model = engine.getResourceManager()->get<Model>("model");
	std::shared_ptr<Model> model = engine.getResourceManager()->get<Model>("model");
	std::shared_ptr<Model> charizard = engine.getResourceManager()->get<Model>("charizard");
	std::shared_ptr<Model> skybox = engine.getResourceManager()->get<Model>("skybox");
	renderer->instanciate(skybox.get());
	// IModelInstance* instance = renderer->instanciate(model.get());
	// IModelInstance* instance2 = renderer->instanciate(model.get());
	// renderer->add(instance);
	// renderer->add(instance2);
	// instance2->translate(glm::vec3(-1, 0, -1));

	std::vector<IModelInstance*>	instances;
	for (double x = 0; x <= 3; x++)
		for (double y = 0; y <= 3; y++)
		{
			IModelInstance* instance = renderer->instanciate(charizard.get());
			instance->translate(glm::vec3(x * 3, 0, y * 3));
			// instance->rotate((PI * x * y) / 30, glm::vec3(0, 1, 0));
			renderer->add(instance);
			instances.push_back(instance);
		}

	while (run)
	{
		if (keyboard->isKeyDown(KEY_ESCAPE))
			run = false;
		if (window->getIsClosing())
			run = false;

		//	movement
		if (keyboard->isKeyDown(KEY_W))
			camera->moveForward(STEP);
		if (keyboard->isKeyDown(KEY_S))
			camera->moveBackward(STEP);
		if (keyboard->isKeyDown(KEY_A))
			camera->moveLeft(STEP);
		if (keyboard->isKeyDown(KEY_D))
			camera->moveRight(STEP);
		if (keyboard->isKeyDown(KEY_SPACE))
			camera->moveUp(STEP);
		if (keyboard->isKeyDown(KEY_LSHIFT))
			camera->moveDown(STEP);

		//	camera rotation
		if (keyboard->isKeyDown(KEY_LEFT))
			camera->turnLeft(ANGLE);
		if (keyboard->isKeyDown(KEY_RIGHT))
			camera->turnRight(ANGLE);
		if (keyboard->isKeyDown(KEY_UP))
			camera->turnUp(ANGLE);
		if (keyboard->isKeyDown(KEY_DOWN))
			camera->turnDown(ANGLE);

		//	objects modifications
		// instance->rotate(PI / 300, glm::vec3(0, 1, 0));
		// instance2->rotate(-PI / 30, glm::vec3(0, 1, 0));

		for (IModelInstance* instance : instances)
			instance->rotate((PI * (instance->getBody()->getMatrix()[3][0] + 1) * (instance->getBody()->getMatrix()[3][2] + 1)) / 300, glm::vec3(0, 1, 0));

		//	window update / swap
		cursor->update();
		renderer->swap();
	}
	return (EXIT_SUCCESS);
}
