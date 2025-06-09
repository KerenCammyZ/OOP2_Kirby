#include "StaticObject.h"

StaticObject::StaticObject(b2World& world, b2Vec2 position)
{
	initPhysics(world, position, b2Vec2(1.0f, 1.0f));
}

StaticObject::~StaticObject()
{
	if (m_body && m_body->GetWorld())
	{
		m_body->GetWorld()->DestroyBody(m_body);
	}
	else
		throw std::runtime_error("StaticObject: Body is null or not in a valid world.");
}

void StaticObject::initPhysics(b2World& world, const b2Vec2& position, const b2Vec2& size)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position = position;
	m_body = world.CreateBody(&bodyDef);
	b2PolygonShape boxShape;
	boxShape.SetAsBox(size.x / 2.0f, size.y / 2.0f);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 0.0f; // Static objects typically have no density
	m_body->CreateFixture(&fixtureDef);
}