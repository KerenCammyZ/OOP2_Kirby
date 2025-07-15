// GlobalSizes.h
#pragma once

// --- Global Sizes and Constants --

constexpr float ENTITY_SIZE = 24.0f;

constexpr int SCREEN_WIDTH = 1600;
constexpr int SCREEN_HEIGHT = 1200;

constexpr int VIEW_WIDTH = 600;
constexpr int VIEW_HEIGHT = 300;

// --- Jumping and falling physics ---
constexpr float GRAVITY = 200.0f;
constexpr float JUMP_STRENGTH = 120.0f;

// -- Water physics ---
constexpr float BUOYANCY = -100.0f; // An upward force that counters gravity
constexpr float WATER_DRAG = 1.0f;   // How much water slows you down

// --- HUD Configuration ---
constexpr int HUD_HEIGHT = SCREEN_HEIGHT / 5;
constexpr int GAME_HEIGHT = SCREEN_HEIGHT - HUD_HEIGHT;
