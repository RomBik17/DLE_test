
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
			
			if ( hero_pos->x > backGround.screen_width / 2) 
				tx = rand() % backGround.screen_width / 2 - backGround.heroSpriteSize.x *2;
			else 
				tx = backGround.screen_width / 2 + rand() % backGround.screen_width / 2 + backGround.heroSpriteSize.x * 2;


			if (hero_pos->x > backGround.screen_height / 2) 
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
		width						= 720;
		height						= 360;
		fullscreen					= false;

		backGround.screen_width		= width;
		backGround.screen_height	= height;
		backGround.enemy_max		= 3;

	}

	virtual bool Init() 
	{
		hero_sprite		= createSprite("data\\avatar.jpg");
		enemy_sprite	= createSprite("data\\enemy.png");
		sight_sprite	= createSprite("data\\aim_icon.png");
		bullet_sprite	= createSprite("data\\bullet.png");

		hero_pos		= new Player(backGround.screen_width / 2, 
									backGround.screen_height / 2, 
									2);

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

		delete hero_pos;
	}

	virtual bool Tick() 
	{
		float DeltaTime = getTickCount() - ATime;
		ATime			= getTickCount();

		drawTestBackground();

		drawSprite	(hero_sprite, 
					hero_pos->x - (backGround.heroSpriteSize.x / 2),
					hero_pos->y - (backGround.heroSpriteSize.y / 2));

		drawSprite	(sight_sprite, 
					sight_pos.x - (backGround.sightSpriteSize.x / 2),
					sight_pos.y - (backGround.sightSpriteSize.y / 2));
		
		SpawnEnemy();

		for (auto& enemy : enemy_pool)
		{
			drawSprite	(enemy_sprite, 
						enemy.x - (backGround.enemySpriteSize.x / 2),
						enemy.y - (backGround.enemySpriteSize.y / 2));

			enemy.move(hero_pos->x, hero_pos->y);
		}

		for (auto& bullet : hero_pos->bulletPool)
		{
			drawSprite	(bullet_sprite, 
						bullet.x - (backGround.bulletSpriteSize.x / 2),
						bullet.y - (backGround.bulletSpriteSize.y / 2));

			bullet.move();
			bullet.collisionWithEnemy	(enemy_pool, 
										backGround.enemySpriteSize, 
										backGround.bulletSpriteSize);
		}

		hero_pos->reload();
		hero_pos->collisionWithEnemy	(enemy_pool, 
										backGround.enemySpriteSize, 
										backGround.heroSpriteSize);

		if (hero_pos->dead) return true;

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
				if (hero_pos->bulletPool.size() == hero_pos->maxBullets) 
					hero_pos->bulletPool.erase(hero_pos->bulletPool.begin());
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