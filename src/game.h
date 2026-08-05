#ifndef GAME_H
#define GAME_H

#include "config.h"
#include "raylib.h"
#include "stdint.h"
#include <stdio.h>

typedef struct {
    Vector2 center;
    Color color;
    uint8_t size;
    float rotationDeg;
} triangle_t;

typedef enum {
    SDIR_LEFT,
    SDIR_RIGHT,
} spinDirection_e;

typedef enum {
    DIR_LEFT,
    DIR_RIGHT,
    DIR_UP,
    DIR_DOWN,
} direction_e;

typedef struct {
    Vector2 pos;
    Vector2 direction;
    bool isDisplayed;
} projectile_t;

Vector2 randomPosition();
Vector2 randomPositionOffScreen();
float getAngleInDegrees(Vector2* from, Vector2* to);
Vector2 rotatePoint(Vector2 point, Vector2 center, float angleDeg);
void triangle_render(const triangle_t* t);
void triangle_spin(triangle_t* t, spinDirection_e d);
void triangle_move(triangle_t* t, direction_e d, float speed);
void triangle_moveTowards(triangle_t* t, Vector2* other, float speed);
void triangle_shoot(triangle_t* t, projectile_t* projectiles, size_t size);
void triangle_faceOther(triangle_t* t, triangle_t* other);
projectile_t projectile_create(Vector2 startingPos, float angleDeg);
void projectile_move(projectile_t* p);
void projectile_render(const projectile_t* p);

#endif
