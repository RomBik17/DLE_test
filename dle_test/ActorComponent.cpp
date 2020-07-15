
#include "ActorComponent.h"

Enemy::Enemy(int _x, int _y)
{
	x = _x;
	y = _y;
}

void Enemy::move(int playerX, int playerY)
{
	float deltaX = playerX - x;
	float deltaY = playerY - y;

	float oddX = (deltaX) / sqrt(pow(deltaX, 2) + pow(deltaY, 2));
	float oddY = (deltaY) / sqrt(pow(deltaX, 2) + pow(deltaY, 2));

	x += oddX;
	y += oddY;
}

Bullet::Bullet(int _x, int _y, int directionX, int directionY)
{
	penetration = false;

	x = _x;
	y = _y;

	float deltaX = directionX - _x;
	float deltaY = directionY - _y;

	dx = (deltaX) / sqrt(pow(deltaX, 2) + pow(deltaY, 2));
	dy = (deltaY) / sqrt(pow(deltaX, 2) + pow(deltaY, 2));
}

void Bullet::move()
{
	x += dx;
	y += dy;
}

void Bullet::collisionWithEnemy(std::vector<Enemy>& enemy_pool)
{
	int i = 0;
	for (auto enemy : enemy_pool)
	{
		if (abs(x - enemy.x) < 23)
		{
			if (abs(y - enemy.y) < 23)
			{
				enemy_pool.erase(enemy_pool.begin() + i);
				penetration = true;
				return;
			}
		}
		else if (abs(y - enemy.y) < 23)
		{
			if (abs(x - enemy.x) < 23) 
			{
				enemy_pool.erase(enemy_pool.begin() + i);
				penetration = true;
				return;
			}
		}
		i++;
	}
}

Player::Player(int _x, int _y, int bulletCount)
{
	dead = false;

	x = _x;
	y = _y;

	maxBullets = bulletCount;
}

void Player::move(Direction direction)
{
	switch (direction)
	{
	case Direction::LEFT:
	{
		x -= 10;
		break;
	}
	case Direction::RIGHT:
	{
		x += 10;
		break;
	}
	case Direction::UP:
	{
		y -= 10;
		break;
	}
	case Direction::DOWN:
	{
		y += 10;
		break;
	}
	default:
		break;
	}
}

void Player::shoot(int directionX, int directionY)
{
	Bullet bl(x, y, directionX, directionY);
	bulletPool.push_back(bl);
}

void Player::reload()
{
	int i = 0;
	for (auto bullet : bulletPool)
	{
		if (bullet.penetration) bulletPool.erase(bulletPool.begin() + i);
	}
}

void Player::collisionWithEnemy(std::vector<Enemy>& enemy_pool)
{
	int i = 0;
	for (auto enemy : enemy_pool)
	{
		if (abs(x - enemy.x) < 38)
		{
			if (abs(y - enemy.y) < 38)
			{
				enemy_pool.erase(enemy_pool.begin() + i);
				dead = true;
				return;
			}
		}
		else if (abs(y - enemy.y) < 38)
		{
			if (abs(x - enemy.x) < 38)
			{
				enemy_pool.erase(enemy_pool.begin() + i);
				dead = true;
				return;
			}
		}
		i++;
	}
	return;
}
