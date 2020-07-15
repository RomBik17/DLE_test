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
	int screen_width;
	int screen_height;
	int map_width;
	int map_height;
	int enemy_max;
};

class ActorComponent
{
public:
	float x;
	float y;

	virtual ~ActorComponent() {}
};

class Enemy : public ActorComponent
{
public:
	Enemy(int _x, int _y);

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
	void collisionWithEnemy(std::vector<Enemy>& enemy_pool);
};

class Player : public ActorComponent
{
private:
	int maxBullets;
public:
	bool dead;

	std::vector<Bullet> bulletPool;

	Player(int _x, int _y, int bulletCount);
	void move(Direction direction);
	void shoot(int directionX, int directionY);
	void reload();
	bool ifCollisionwithEnemy(std::vector<Enemy>& enemy_pool);
};