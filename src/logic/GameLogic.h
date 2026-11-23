#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

#include "logic/GameState.h"
#include "resources/ResourceManager.h"
#include "logic/TitlePage.h"
#include "logic/LevelSelect.h"
#include "logic/FinishButton.h"
#include "logic/EventManager.h"
#include "logic/LevelFactory.h"
#include "logic/Level.h"
#include "logic/Dialogue.h"

namespace escape {
	class GameLogic {

		private:
			b2World* World;

			void changeToLoading();

		public:
			GameLogic();
			GameState state;

			LevelFactory factory;

			Level level;
			TitlePage titlePage;
			LevelSelect levelSelect;
			Dialogue dialogue;

			EventManager eventManager;

			//Resources (fonts, sprites)
			ResourceManager resources;

			void pollEvent(sf::RenderWindow *App, sf::Clock gameTime, double targetMs);

			void makeNextLevelDot();
			void loadLevel(int level);

			void progressSimluation();
			void partialProgressSimluation(float portion);
	};
}
#endif
