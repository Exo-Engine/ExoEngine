/*
 * File: SceneStateMachine.cpp
 * Created: 5th April 2020 6:45 pm
 * Author: Ribault Paul (pribault.dev@gmail.com)
 * 
 * Last Modified: 5th April 2020 6:45 pm
 * Modified By: Ribault Paul (pribault.dev@gmail.com)
 * 
 * Copyright © 2020 Ribault Paul. All rights reserved.
 */

#include "SceneStateMachine.h"

#include "Scene.h"

using namespace	ExoEngine;

SceneStateMachine::SceneStateMachine(ResourceManager* resourceManager) :
	StateMachine<>({}),
	_resourceManager(resourceManager)
{
}

SceneStateMachine::~SceneStateMachine(void)
{
}

ResourceManager*	SceneStateMachine::getResourceManager(void) const
{
	return (_resourceManager);
}

void	SceneStateMachine::addScene(int id, Scene* scene)
{
	addState(id, scene);
}

void	SceneStateMachine::removeScene(int id)
{
	removeState(id);
}

void	SceneStateMachine::removeScene(Scene* scene)
{
	removeState(scene);
}
