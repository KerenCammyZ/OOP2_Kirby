#include "MovingObject.h"

MovingObject::MovingObject() {}

MovingObject::~MovingObject() 
{
	if(m_body && m_body->GetWorld())
	{
		m_body->GetWorld()->DestroyBody(m_body);
	}
	else
		throw std::runtime_error("MovingObject: Body is null or not in a valid world.");
}

void MovingObject::initPhysics(b2World& world, const b2Vec2& position)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = position;
	bodyDef.fixedRotation = fixedRotation;
	m_body = world.CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	// Convert global size to meters (assuming your physics scale)
	const float SCALE = 30.0f; // Define your scale factor
	boxShape.SetAsBox(TILE_SIZE / (2.0f * SCALE),
		TILE_SIZE / (2.0f * SCALE));

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	m_body->CreateFixture(&fixtureDef);

	// set size and position in pixels
	//m_size = sf::Vector2f(size.x * 32.0f, size.y * 32.0f);
	//m_position = sf::Vector2f(position.x * 100.0f, position.y * 100.0f);
}

void MovingObject::update(float dt)
{
	move(dt);

	if (m_body)
	{
		b2Vec2 position = m_body->GetPosition();
		m_position = sf::Vector2f(position.x * 100.0f, position.y * 100.0f);
	}
	else
		throw std::runtime_error("MovingObject: Body is null during update.");
}

void MovingObject::setVelocity(const b2Vec2& velocity)
{
	if (m_body)
	{
		m_body->SetLinearVelocity(velocity);
	}
	else
		throw std::runtime_error("MovingObject: Body is null when setting velocity.");
}

b2Vec2 MovingObject::getVelocity() const
{
	if(m_body)
	{
		return m_body->GetLinearVelocity();
	}
	else
		throw std::runtime_error("MovingObject: Body is null when getting velocity.");
}

