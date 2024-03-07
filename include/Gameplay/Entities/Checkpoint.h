#ifndef __CHECKPOINT_H__
#define __CHECKPOINT_H__

#include "Core/Animation.h"
#include "Gameplay/Entities/Entity.h"
#include "Utils/Point.h"

#include <SDL.h>


struct SDL_Texture;

class Checkpoint : public Entity
{
public:

	Checkpoint();
	virtual ~Checkpoint();

	bool Awake() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

	void OnCollision(PhysBody* physA, PhysBody* physB) override;

public:

	bool isPicked = false;
	SDL_Texture* texture;
	const char* texturePath;
	Animation statesAnimation;
	iPoint size;

private:

	PhysBody* pbody;
};

#endif // __CHECKPOINT_H__