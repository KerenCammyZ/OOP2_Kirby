// GlobalSizes.h
#pragma once

constexpr float ENTITY_SIZE = 24.0f;

static int SCREEN_WIDTH = 1600;
static int SCREEN_HEIGHT = 1200;

static int VIEW_WIDTH = 600;
static int VIEW_HEIGHT = 300;

// --- NEW PHYSICS CONSTANTS ---
const float GRAVITY = 200.0f;
const float JUMP_STRENGTH = 120.0f;

// New constants for water physics
const float BUOYANCY = -200.0f; // An upward force that counters gravity
const float WATER_DRAG = 0.5f;   // How much water slows you down

//// Adjusted sizes for lower resolution displays
//constexpr float ENTITY_SIZE = 12.0f; // originally 24.0f
//static int SCREEN_WIDTH = 800; // originally 1600
//static int SCREEN_HEIGHT = 600; // originally 1200
//
//static int VIEW_WIDTH = 300; // originally 800
//static int VIEW_HEIGHT = 150; // originally 600

