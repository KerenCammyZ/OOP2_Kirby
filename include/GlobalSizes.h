// GlobalSizes.h
#pragma once

constexpr float ENTITY_SIZE = 24.0f;

static int SCREEN_WIDTH = 1600;
static int SCREEN_HEIGHT = 1200;

static int VIEW_WIDTH = 600;
static int VIEW_HEIGHT = 300;

// colors for world map
const sf::Color FLOOR(76, 255, 0);
const sf::Color WALL(255, 0, 0);
const sf::Color DOOR_A(0, 38, 255);
const sf::Color DOOR_B(0, 148, 255);

// --- NEW PHYSICS CONSTANTS ---
const float GRAVITY = 200.0f;
const float JUMP_STRENGTH = 120.0f;

// --- NEW RAMP-HANDLING CONSTANT ---
const float MAX_STEP_HEIGHT = 25.0f; // Kirby can automatically step up any ledge smaller than this


//// Adjusted sizes for lower resolution displays
//constexpr float ENTITY_SIZE = 12.0f; // originally 24.0f
//static int SCREEN_WIDTH = 800; // originally 1600
//static int SCREEN_HEIGHT = 600; // originally 1200
//
//static int VIEW_WIDTH = 300; // originally 800
//static int VIEW_HEIGHT = 150; // originally 600
//
