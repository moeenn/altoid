#pragma once
#include "raylib.h"
#include "stdint.h"
#include <stdio.h>

static const char *const WIN_TITLE = "Game";
static const int WIN_WIDTH = 800;
static const int WIN_HEIGHT = 600;
static const int FPS = 60;
static const float SPIN_SPEED = 5.0F;
static const float TRIANGLE_SIZE = 10.0F;
static const Color TRIANGLE_COLOR = GREEN;
static const float PLAYER_MOVE_SPEED = 5.0F;
static const float ENEMY_MOVE_SPEED = 2.0F;
static const float PROJECTILE_SIZE = 2.0F;
static const float PROJECTILE_SPEED = 7.0F;
static const Color PROJECTILE_COLOR = RED;
static const int MAX_PROJECTILES = 20;
static const int MAX_ENEMIES = 5;

#define ALIGN32 32

typedef struct
{
    Vector2 center;
    Color color;
    float rotationDeg;
    uint8_t size;
} __attribute__((aligned(ALIGN32))) triangle_t;

typedef enum
{
    SDIR_LEFT,
    SDIR_RIGHT,
} spinDirection_e;

typedef enum
{
    DIR_LEFT,
    DIR_RIGHT,
    DIR_UP,
    DIR_DOWN,
} direction_e;

typedef struct
{
    Vector2 pos;
    Vector2 direction;
    bool isDisplayed;
} __attribute__((aligned(ALIGN32))) projectile_t;

Vector2 randomPosition();
Vector2 randomPositionOffScreen();
float getAngleInDegrees(Vector2 *from, Vector2 *towards);
Vector2 rotatePoint(Vector2 point, Vector2 center, float angleDeg);
void triangle_render(const triangle_t *self);
void triangle_spin(triangle_t *self, spinDirection_e dir);
void triangle_move(triangle_t *self, direction_e dir, float speed);
void triangle_moveTowards(triangle_t *self, Vector2 *other, float speed);
void triangle_shoot(triangle_t *self, projectile_t *projectiles, size_t size);
void triangle_faceOther(triangle_t *self, triangle_t *other);
projectile_t projectile_create(Vector2 startingPos, float angleDeg);
void projectile_move(projectile_t *self);
void projectile_render(const projectile_t *self);
