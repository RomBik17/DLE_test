#pragma once
#include <math.h>
#include <vector>

enum Direction {RIGHT, LEFT, UP, DOWN};

struct ActorPosition
{
	int x;
	int y;
};

struct BackgroundInfo
{
	int				screen_width;
	int				screen_height;
	int				map_width;
	int				map_height;
	int				enemy_max;
	int				max_bullets;

	ActorPosition	heroSpriteSize;
	ActorPosition	bulletSpriteSize;
	ActorPosition	enemySpriteSize;
	ActorPosition	sightSpriteSize;
};

class ActorComponent
{
public:

	float x;
	float y;
	float relativeX;
	float relativeY;

	virtual ~ActorComponent() {}

};

class Enemy : public ActorComponent
{
private:

	float speedKoeficient;

public:

	Enemy(int _x, int _y, float speedKoef = 0.1);

	void move(int playerX, int playerY);

};

class Bullet : public ActorComponent
{
private:

	float dx;
	float dy;

public:

	bool penetration;

	Bullet(int _x, int _y, int directionX, int directionY);

	void move();
	void collisionWithEnemy(std::vector<Enemy>& enemy_pool, ActorPosition enemySpriteSize, ActorPosition bulletSpriteSize);

};

class Player : public ActorComponent
{

public:

	int					maxBullets;

	bool				dead;

	std::vector<Bullet> bulletPool;

	Player(int _x, int _y, int relX, int relY, int bulletCount);

	void move(Direction direction, std::vector<Enemy>& enemy_pool);
	void shoot(int directionX, int directionY);
	void reload();
	void collisionWithEnemy(std::vector<Enemy>& enemy_pool, ActorPosition enemySpriteSize, ActorPosition heroSpriteSize);

};