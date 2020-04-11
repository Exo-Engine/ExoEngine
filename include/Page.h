/*
 * File: Page.h
 * Created: 4th April 2020 7:10 pm
 * Author: Ribault Paul (pribault.dev@gmail.com)
 * 
 * Last Modified: 4th April 2020 7:10 pm
 * Modified By: Ribault Paul (pribault.dev@gmail.com)
 * 
 * Copyright © 2020 Ribault Paul. All rights reserved.
 */

#pragma once

#include "Scene.h"
#include "Dynamic.h"
#include "IWidget.h"

#include <libxml/parser.h>

namespace	ExoEngine
{
	class	ResourceManager;
	class	Page : public Scene
	{
		public:
			Page(SceneStateMachine &stateMachine, const std::string& name, const std::string& file);
			~Page(void);

			virtual void	load(void);
			virtual void	unload(void);

			virtual void	draw(void);
			virtual void	handleEvents(void);
		private:

			typedef ExoRenderer::IWidget*	(Page::*t_widget_loader)(const std::map<std::string, std::string>&);

			ExoRenderer::IWidget*	loadImage(const std::map<std::string, std::string>& properties);
			ExoRenderer::IWidget*	loadButton(const std::map<std::string, std::string>& properties);
			ExoRenderer::IWidget*	loadCheckbox(const std::map<std::string, std::string>& properties);
			ExoRenderer::IWidget*	loadInput(const std::map<std::string, std::string>& properties);
			ExoRenderer::IWidget*	loadSelect(const std::map<std::string, std::string>& properties);
			ExoRenderer::IWidget*	loadSlider(const std::map<std::string, std::string>& properties);
			ExoRenderer::IWidget*	loadSpinner(const std::map<std::string, std::string>& properties);
			ExoRenderer::IWidget*	loadView(const std::map<std::string, std::string>& properties);

			void					load(xmlNodePtr node, ExoRenderer::IWidget* parentWidget);
			void					getProperties(xmlNodePtr node, std::map<std::string, std::string>& properties);

			std::map<std::string, ExoRenderer::IWidget*>	_widgets;
			ResourceManager*								_resourceManager;
	};
}
