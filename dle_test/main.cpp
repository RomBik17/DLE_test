
#include "Framework.h"
#include "ActorComponent.h"
#include <time.h>
#include <random>
#include <iostream>

/* Test Framework realization */
class MyFramework : public Framework {

private:

	float							ATime = 0;

	Sprite*							hero_sprite;
	Sprite*							enemy_sprite;
	Sprite*							sight_sprite;
	Sprite*							bullet_sprite;

	std::vector<Enemy>				enemy_pool;

	Player*							hero_pos;
	ActorPosition					sight_pos;

	BackgroundInfo					backGround;

	void SpawnEnemy()
	{
		int t = getTickCount();
		if (t % 1000 < 5 && enemy_pool.size() < backGround.enemy_max)
		{
			srand(time(0));
			int tx;
			int ty;

			tx = rand() % 600;
			ty = rand() % 300;


			Enemy en(tx, ty);
			enemy_pool.push_back(en);
		}
	}

public:
	
	virtual void PreInit(int& width, int& height, bool& fullscreen)
	{
		width						= 720;
		height						= 360;
		fullscreen					= false;

		backGround.screen_height	= width;
		backGround.screen_height	= height;
		backGround.enemy_max		= 3;

	}

	virtual bool Init() 
	{
		hero_sprite		= createSprite("data\\avatar.jpg");
		enemy_sprite	= createSprite("data\\enemy.png");
		sight_sprite	= createSprite("data\\aim_icon.png");
		bullet_sprite	= createSprite("data\\bullet.png");

		hero_pos = new Player(200, 200, 10);

		return true;
	}

	virtual void Close() {}

	virtual bool Tick() 
	{
		float DeltaTime = getTickCount() - ATime;
		ATime			= getTickCount();

		drawTestBackground();
		drawSprite(hero_sprite, hero_pos->x - 32, hero_pos->y - 32);
		drawSprite(sight_sprite, sight_pos.x - 15, sight_pos.y - 15);
		
		SpawnEnemy();

		for (auto& enemy : enemy_pool)
		{
			drawSprite(enemy_sprite, enemy.x - 6, enemy.y - 6);
			enemy.move(hero_pos->x, hero_pos->y);
		}

		for (auto& bullet : hero_pos->bulletPool)
		{
			drawSprite(bullet_sprite, bullet.x, bullet.y);
			bullet.move();
			bullet.collisionWithEnemy(enemy_pool);
		}

		hero_pos->reload();
		hero_pos->collisionWithEnemy(enemy_pool);

		if (hero_pos->dead) return true;;

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
				hero_pos->shoot(sight_pos.x, sight_pos.y);
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
			if (hero_pos->x <= w - 30) hero_pos->move(RIGHT);
			break;
		}
		case FRKey::LEFT:
		{
			if (hero_pos->x >= 30) hero_pos->move(LEFT);
			break; 
		}
		case FRKey::DOWN:
		{
			int w, h;
			getScreenSize(w, h);
			if (hero_pos->y <= h - 30) hero_pos->move(DOWN);
			break;
		}
		case FRKey::UP:
		{
			if (hero_pos->y >= 30) hero_pos->move(UP);
			break;
		}
		default:
			break;
		}
	}

	virtual void onKeyReleased(FRKey k) {}
};

int main(int argc, char* argv[])
{
	return run(new MyFramework);
}