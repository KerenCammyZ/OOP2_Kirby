#pragma once
#include <memory>

class Kirby; // Forward-declaration to avoid circular includes

class KirbyState
{
public:
	virtual ~KirbyState() = default;

	// Checks for input that would cause a state change (e.g., press Right to go from Standing to Moving)
	virtual std::unique_ptr<KirbyState> handleInput() = 0;

	// Updates Kirby's logic for the current state (e.g., apply movement)
	virtual void update(Kirby& kirby, float deltaTime) = 0;

	// A function to run code when entering a state (e.g., set an animation)
	virtual void enter(Kirby& kirby) {};
};