/*
 * File: Scene.h
 * Created: 5th April 2020 6:45 pm
 * Author: Ribault Paul (pribault.dev@gmail.com)
 * 
 * Last Modified: 5th April 2020 6:45 pm
 * Modified By: Ribault Paul (pribault.dev@gmail.com)
 * 
 * Copyright © 2020 Ribault Paul. All rights reserved.
 */

#pragma once

#include "State.h"

namespace	ExoEngine
{
	class	SceneStateMachine;
	class	Scene : public State<>
	{
		public:
			Scene(SceneStateMachine &stateMachine, const std::string& name);
			virtual ~Scene(void);

			virtual void	run(void);

			virtual void	draw(void);
			virtual void	handleEvents(void);
		private:
	};
}
