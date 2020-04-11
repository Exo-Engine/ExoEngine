/*
 * File: Scene.cpp
 * Created: 5th April 2020 6:45 pm
 * Author: Ribault Paul (pribault.dev@gmail.com)
 * 
 * Last Modified: 5th April 2020 6:45 pm
 * Modified By: Ribault Paul (pribault.dev@gmail.com)
 * 
 * Copyright © 2020 Ribault Paul. All rights reserved.
 */

#include "Scene.h"

#include "SceneStateMachine.h"

using namespace	ExoEngine;

Scene::Scene(SceneStateMachine &stateMachine, const std::string& name) :
	State<>(stateMachine, name)
{
}

Scene::~Scene(void)
{
}

void	Scene::run(void)
{
	handleEvents();
	draw();
}

void	Scene::draw(void)
{
}

void	Scene::handleEvents(void)
{
}
