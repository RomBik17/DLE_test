
#include "ActorComponent.h"

Enemy::Enemy(int _x, int _y, float speedKoef)
{
	x				= _x;
	y				= _y;
	speedKoeficient = speedKoef;
}

void Enemy::move(std::vector<Enemy> enemy_pool, int enemy_i, int playerX, int playerY, BackgroundInfo background)
{
	float deltaX = playerX - x;
	float deltaY = playerY - y;

	//calculate a single vector
	float oddX = (deltaX) / sqrt(pow(deltaX, 2) + pow(deltaY, 2));
	float oddY = (deltaY) / sqrt(pow(deltaX, 2) + pow(deltaY, 2));

	oddX = oddX * speedKoeficient;
	oddY = oddY * speedKoeficient;

	bool can_move = true;

	//check if on this coordinates is other enemy, if it is, enemy can't move
	for (int i = 0; i < enemy_i; i++)
	{
		if	(abs(enemy_pool[i].x + oddX - enemy_pool[enemy_i].x) < background.enemySpriteSize.x 
			&& abs(enemy_pool[i].y + oddY - enemy_pool[enemy_i].y) < background.enemySpriteSize.y)
		{
			can_move = false;
			break;
		}
	}

	if (can_move)
	{
		x += oddX;
		y += oddY;
	}
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

void Bullet::collisionWithEnemy(std::vector<Enemy>& enemy_pool, BackgroundInfo background)
{
	int i = 0;
	for (auto enemy : enemy_pool)
	{
		if (abs(x - enemy.x) < ((background.enemySpriteSize.x + background.bulletSpriteSize.x) / 2))
		{
			if (abs(y - enemy.y) < ((background.enemySpriteSize.y + background.bulletSpriteSize.y) / 2))
			{
				enemy_pool.erase(enemy_pool.begin() + i);
				penetration = true;
				return;
			}
		}
		else if (abs(y - enemy.y) < ((background.enemySpriteSize.y + background.bulletSpriteSize.y) / 2))
		{
			if (abs(x - enemy.x) < ((background.enemySpriteSize.x + background.bulletSpriteSize.x) / 2))
			{
				enemy_pool.erase(enemy_pool.begin() + i);
				penetration = true;
				return;
			}
		}
		i++;
	}
}

Player::Player(int _x, int _y, int relX, int relY, int bulletCount)
{
	dead		= false;

	x			= _x;
	y			= _y;
	relativeX	= relX;
	relativeY	= relY;

	maxBullets	= bulletCount;
}

void Player::move(Direction direction, std::vector<Enemy>& enemy_pool, BackgroundInfo background)
{
	ActorPosition	delta					= { 0, 0 };

	bool			mustChangeComponents	= true;

	switch (direction)
	{
	case Direction::LEFT:
	{
		delta = { -10, 0 };

		//if player is on the end of map
		if (x != background.screen_width / 2)
		{
			x -= 10;
			mustChangeComponents = false;
		}
		else if (relativeX - (background.screen_width / 2) - 10 < 0)
		{
			x -= 10;
			mustChangeComponents = false;
		}
		break;
	}
	case Direction::RIGHT:
	{
		delta = { 10, 0 };

		//if player is on the end of map
		if (x != background.screen_width / 2)
		{
			x += 10;
			mustChangeComponents = false;
		}
		else if (relativeX + (background.screen_width / 2) + 10 > background.map_width)
		{
			x += 10;
			mustChangeComponents = false;
		}
		break;
	}
	case Direction::UP:
	{
		delta = { 0, -10 };

		//if player is on the end of map
		if (y != background.screen_height / 2)
		{
			y -= 10;
			mustChangeComponents = false;
		}
		else if (relativeY - (background.screen_height / 2) - 10 < 0) 
		{ 
			y -= 10; 
			mustChangeComponents = false;
		}
		break;
	}
	case Direction::DOWN:
	{
		delta = { 0, 10 };

		//if player is on the end of map
		if (y != background.screen_height / 2)
		{
			y += 10;
			mustChangeComponents = false;
		}
		else if (relativeY + (background.screen_height / 2) + 10 > background.map_height)
		{
			y += 10;
			mustChangeComponents = false;
		}
		break;
	}
	default:
		break;
	}

	relativeX += delta.x;
	relativeY += delta.y;

	//if player is on the end of map, then player can walk to the end and map won't be recenterd,
	//so we won't need to change coordinates of other components
	if (mustChangeComponents)
	{
		for (auto& enemy : enemy_pool)
		{
			enemy.x -= delta.x;
			enemy.y -= delta.y;
		}

		for (auto& bullet : bulletPool)
		{
			bullet.x -= delta.x;
			bullet.y -= delta.y;
		}
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
		//if bullet is colliding with enemy, then destroy bullet
		if (bullet.penetration) bulletPool.erase(bulletPool.begin() + i);
		i++;
	}
}

void Player::collisionWithEnemy(std::vector<Enemy>& enemy_pool, BackgroundInfo background)
{
	int i = 0;
	for (auto enemy : enemy_pool)
	{
		if (abs(x - enemy.x) < ((background.enemySpriteSize.x + background.heroSpriteSize.x) / 2))
		{
			if (abs(y - enemy.y) < ((background.enemySpriteSize.y + background.heroSpriteSize.y) / 2))
			{
				enemy_pool.erase(enemy_pool.begin() + i);
				dead = true;
				return;
			}
		}
		else if (abs(y - enemy.y) < ((background.enemySpriteSize.y + background.heroSpriteSize.y) / 2))
		{
			if (abs(x - enemy.x) < ((background.enemySpriteSize.x + background.heroSpriteSize.x) / 2))
			{
				enemy_pool.erase(enemy_pool.begin() + i);
				dead = true;
				return;
			}
		}
		i++;
	}
}
