/*
 * File: SceneStateMachine.h
 * Created: 5th April 2020 6:45 pm
 * Author: Ribault Paul (pribault.dev@gmail.com)
 * 
 * Last Modified: 5th April 2020 6:45 pm
 * Modified By: Ribault Paul (pribault.dev@gmail.com)
 * 
 * Copyright © 2020 Ribault Paul. All rights reserved.
 */

#pragma once

#include "StateMachine.h"
#include "ResourceManager.h"

namespace	ExoEngine
{
	class	Scene;
	class	SceneStateMachine : public StateMachine<>
	{
		public:
			SceneStateMachine(ResourceManager* resourceManager);
			virtual ~SceneStateMachine(void);

			ResourceManager*	getResourceManager(void) const;

			void	addScene(int id, Scene* scene);
			void	removeScene(int id);
			void	removeScene(Scene* scene);
		private:
			ResourceManager*	_resourceManager;
	};
}
