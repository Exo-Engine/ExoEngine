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

using namespace	ExoEngine;
using namespace	ExoRenderer;

int		main(void)
{
	Engine		engine("resources/settings.xml");
	IRenderer*	renderer = engine.getRenderer();
	IWindow*	window;
	IKeyboard*	keyboard;
	bool		run = true;

	renderer->initialize("example window", 1280, 720, WINDOWED, false);
	engine.getResourceManager()->load("resources/resources.xml");
	window = renderer->getWindow();
	keyboard = renderer->getKeyboard();
	window->setVsync(true);

	ICursor* cursor = renderer->createCursor();
	std::shared_ptr<ITexture> cursorTexture = engine.getResourceManager()->get<ITexture>("cursor");
	cursor->setCursorTexture(cursorTexture);
	renderer->setCursor(cursor);

	IImage* image = renderer->createImage(cursorTexture);
	image->setAnchor(AnchorPoint::CENTER);
	image->setSize(100, 100);
	renderer->add(image);

	while (run)
	{
		if (keyboard->isKeyDown(KEY_ESCAPE))
			run = false;
		if (window->getIsClosing())
			run = false;
		cursor->update();
		renderer->swap();
	}
	return (EXIT_SUCCESS);
}
