#ifndef __ROPEENTITY_H__
#define __ROPEENTITY_H__

#include "Gameplay/Entities/Entity.h"
#include "Utils/Point.h"

#include <SDL.h>
#include <box2d/b2_math.h>

struct SDL_Texture;

class RopeEntity : public Entity
{
public:

	RopeEntity();
	virtual ~RopeEntity();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

public:

	bool isPicked = false;

private:

	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* pbody;

	b2Vec2 spawnPosition;
	b2Vec2 startPos;
	b2Vec2 endPos;

	int ropeLength;
	int segmentWidth;
	int segmentHeight;

	int tmp;
};

#endif // __ROPEENTITY_H__