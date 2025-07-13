#pragma once
#include <SFML/System/Time.hpp>

class Kirby; // Forward-declaration to avoid circular dependencies

//enum class PowerUpType
//{
//	None,
//	Hyper,
//	Spark,
//	// Add other power-up types as needed
//};

// The abstract base class for all power-up commands
class PowerUpCommand
{
public:
	virtual ~PowerUpCommand() = default;

	// Applies the effect to Kirby
	virtual void execute(Kirby& kirby) = 0;

	// Reverts the effect from Kirby
	virtual void undo(Kirby& kirby) = 0;

	// Returns the duration of the command's effect
	virtual sf::Time getDuration() const = 0;

	// Returns the type of power-up this command represents
	//virtual PowerUpType getPowerUpType() const = 0;
};