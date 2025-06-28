// GlobalSizes.h
#pragma once

constexpr float ENTITY_SIZE = 24.0f;

static int SCREEN_WIDTH = 1600;
static int SCREEN_HEIGHT = 1200;

static int VIEW_WIDTH = 600;
static int VIEW_HEIGHT = 300;

// --- NEW PHYSICS CONSTANTS ---git 
const float GRAVITY = 200.0f;
const float JUMP_STRENGTH = 120.0f;

//// Adjusted sizes for lower resolution displays
//constexpr float ENTITY_SIZE = 12.0f; // originally 24.0f
//static int SCREEN_WIDTH = 800; // originally 1600
//static int SCREEN_HEIGHT = 600; // originally 1200
//
//static int VIEW_WIDTH = 300; // originally 800
//static int VIEW_HEIGHT = 150; // originally 600

// --- HUD Configuration ---
static int HUD_HEIGHT = SCREEN_HEIGHT / 5;  // Height reserved for HUD at bottom (can be adjusted)
static int GAME_HEIGHT = SCREEN_HEIGHT - HUD_HEIGHT;  // Remaining height for gameplay
