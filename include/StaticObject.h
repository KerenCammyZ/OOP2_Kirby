#include "GameObject.h"

class StaticObject : public GameObject
{
public:
	StaticObject(b2World& world, b2Vec2 position);
	~StaticObject();
	virtual void initPhysics(b2World& world, const b2Vec2& position, const b2Vec2& size);
	virtual void update(float deltaTime) {};

private:
};