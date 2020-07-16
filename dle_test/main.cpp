
#include "Framework.h"
#include "ActorComponent.h"
#include <time.h>
#include <random>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>

/* Test Framework realization */
class MyFramework : public Framework {

private:

	float					ATime = 0;

	Sprite*					hero_sprite;
	Sprite*					enemy_sprite;
	Sprite*					sight_sprite;
	Sprite*					bullet_sprite;

	std::vector<Enemy>		enemy_pool;

	ActorPosition			sight_pos;

	Player*					mainCharacter;
	BackgroundInfo			backGround;

	void SpawnEnemy()
	{
		int t = getTickCount();
		if (t % 1000 < 5 && enemy_pool.size() < backGround.enemy_max)
		{
			srand(time(0));
			int tx;
			int ty;
			
			if ( mainCharacter->x > backGround.screen_width / 2) 
				tx = rand() % backGround.screen_width / 2 - backGround.heroSpriteSize.x *2;
			else 
				tx = backGround.screen_width / 2 + rand() % backGround.screen_width / 2 + backGround.heroSpriteSize.x * 2;


			if (mainCharacter->x > backGround.screen_height / 2) 
				ty = rand() % backGround.screen_height / 2 - backGround.heroSpriteSize.y * 2;
			else 
				ty = backGround.screen_height / 2 + rand() % backGround.screen_height / 2 + backGround.heroSpriteSize.y * 2;


			Enemy en(tx, ty);
			enemy_pool.push_back(en);
		}
	}

public:

	virtual void PreInit(int& width, int& height, bool& fullscreen)
	{
		parse();
		width						= backGround.screen_width;
		height						= backGround.screen_height;
		fullscreen					= false;

		/*backGround.screen_width		= width;
		backGround.screen_height	= height;
		backGround.enemy_max		= 3;*/

	}

	virtual bool Init() 
	{
		hero_sprite		= createSprite("data\\avatar.jpg");
		enemy_sprite	= createSprite("data\\enemy.png");
		sight_sprite	= createSprite("data\\aim_icon.png");
		bullet_sprite	= createSprite("data\\bullet.png");

		mainCharacter		= new Player(backGround.screen_width / 2, 
										backGround.screen_height / 2, 
										backGround.max_bullets);

		getSpriteSize	(hero_sprite, 
						backGround.heroSpriteSize.x, 
						backGround.heroSpriteSize.y);

		getSpriteSize	(enemy_sprite, 
						backGround.enemySpriteSize.x, 
						backGround.enemySpriteSize.y);

		getSpriteSize	(bullet_sprite, 
						backGround.bulletSpriteSize.x, 
						backGround.bulletSpriteSize.y);

		getSpriteSize	(sight_sprite,
						backGround.sightSpriteSize.x,
						backGround.sightSpriteSize.y);

		return true;
	}

	virtual void Close() 
	{
		destroySprite(hero_sprite);
		destroySprite(bullet_sprite);
		destroySprite(enemy_sprite);
		destroySprite(sight_sprite);

		delete mainCharacter;
	}

	virtual bool Tick() 
	{
		float DeltaTime = getTickCount() - ATime;
		ATime			= getTickCount();

		drawTestBackground();

		drawSprite	(hero_sprite, 
					mainCharacter->x - (backGround.heroSpriteSize.x / 2),
					mainCharacter->y - (backGround.heroSpriteSize.y / 2));

		drawSprite	(sight_sprite, 
					sight_pos.x - (backGround.sightSpriteSize.x / 2),
					sight_pos.y - (backGround.sightSpriteSize.y / 2));
		
		SpawnEnemy();

		for (auto& enemy : enemy_pool)
		{
			drawSprite	(enemy_sprite, 
						enemy.x - (backGround.enemySpriteSize.x / 2),
						enemy.y - (backGround.enemySpriteSize.y / 2));

			enemy.move(mainCharacter->x, mainCharacter->y);
		}

		for (auto& bullet : mainCharacter->bulletPool)
		{
			drawSprite	(bullet_sprite, 
						bullet.x - (backGround.bulletSpriteSize.x / 2),
						bullet.y - (backGround.bulletSpriteSize.y / 2));

			bullet.move();
			bullet.collisionWithEnemy	(enemy_pool, 
										backGround.enemySpriteSize, 
										backGround.bulletSpriteSize);
		}

		mainCharacter->reload();
		mainCharacter->collisionWithEnemy	(enemy_pool, 
										backGround.enemySpriteSize, 
										backGround.heroSpriteSize);

		if (mainCharacter->dead) return true;

		showCursor(false);

		return false;
	}

	virtual void onMouseMove(int x, int y, int xrelative, int yrelative)
	{
		sight_pos.x = x;
		sight_pos.y = y;
	}

	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) 
	{
		if (isReleased)
		{
			switch (button)
			{
			case FRMouseButton::LEFT:
			{
				if (mainCharacter->bulletPool.size() == mainCharacter->maxBullets) 
					mainCharacter->bulletPool.erase(mainCharacter->bulletPool.begin());
				mainCharacter->shoot(sight_pos.x, sight_pos.y);
				break;
			}
			default:
				break;
			}
		}
	}

	virtual void onKeyPressed(FRKey k) {
		switch (k)
		{
		case FRKey::RIGHT:
		{
			int w, h;
			getScreenSize(w, h);
			if (mainCharacter->x <= w - 30) mainCharacter->move(RIGHT);
			break;
		}
		case FRKey::LEFT:
		{
			if (mainCharacter->x >= 30) mainCharacter->move(LEFT);
			break; 
		}
		case FRKey::DOWN:
		{
			int w, h;
			getScreenSize(w, h);
			if (mainCharacter->y <= h - 30) mainCharacter->move(DOWN);
			break;
		}
		case FRKey::UP:
		{
			if (mainCharacter->y >= 30) mainCharacter->move(UP);
			break;
		}
		default:
			break;
		}
	}

	virtual void onKeyReleased(FRKey k) {}

	std::vector<std::string> split(std::string str, char sym)
	{
		std::vector<std::string> params;
		size_t cur, prev = 0;
		cur = str.find(sym);
		while (cur <= str.size())
		{
			params.push_back(str.substr(prev, cur - prev));
			prev = cur + 1;
			cur = str.find(sym, prev);
		}
		params.push_back(str.substr(prev, cur - prev));

		return params;
	}

	void setParams(std::string value, int& first, int& second)
	{
		std::vector<int> params;
		try
		{
			std::vector<std::string> s = split(value, 'x');
			for (auto number : s)
				params.push_back(std::stoi(number));

			if (params.size() == 1) {
				first = params.at(0);
				second = params.at(0);
			}
			if (params.size() == 2) {
				first = params.at(0);
				second = params.at(1);
			}
		}
		catch (std::invalid_argument)
		{
			std::cout << "invalid_argument";
		}
	}

	void parse()
	{
		std::cout << "init elements like \n" << "in format [command] [value] \n" << "like that:\n"
			<< "-window 600x600 -enemy 20 -map 1000x1000 -ammo 2 \n" << "Enter all required, value by default dint exist" << std::endl;

		std::vector<std::string> words;

		for(int i = 0; i<8; i++)
		{
			std::string str;
			std::cin >> str;
			words.push_back(str);
		}
		for (int i = 0; i < 8; i++)
		{
			if (words[i] == "-window")
			{
				if (words[i + 1] != "-")
				{
					setParams(words[i + 1], backGround.screen_width, backGround.screen_height);
				}
			}
			if (words[i] == "-enemy")
			{
				if (words[i + 1] != "-")
				{
					setParams(words[i + 1], backGround.enemy_max, backGround.enemy_max);
				}
			}
			if (words[i] == "-map")
			{
				if (words[i + 1] != "-")
				{
					setParams(words[i + 1], backGround.map_width, backGround.map_height);
				}
			}
			if (words[i] == "-ammo")
			{
				if (words[i + 1] != "-")
				{
					setParams(words[i + 1], backGround.max_bullets, backGround.max_bullets);
				}
			}
		}
	}
};

int main(int argc, char* argv[])
{
	return run(new MyFramework);
}