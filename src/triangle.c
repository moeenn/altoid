#include "config.h"
#include "game.h"
#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>

Vector2 rotatePoint(Vector2 point, Vector2 center, float angleDeg)
{
    float angleRad = angleDeg * DEG2RAD;
    float cosA = cosf(angleRad);
    float sinA = sinf(angleRad);

    float dx = point.x - center.x;
    float dy = point.y - center.y;

    return (Vector2) {
        center.x + (dx * cosA - dy * sinA),
        center.y + (dx * sinA + dy * cosA)
    };
}

void triangle_render(const triangle_t* t)
{
    // without rotation.
    Vector2 v1 = { .x = t->center.x, .y = t->center.y - t->size };
    Vector2 v2 = { .x = t->center.x - (t->size), .y = t->center.y + (t->size) };
    Vector2 v3 = { .x = t->center.x + (t->size), .y = t->center.y + (t->size) };

    DrawTriangle(
        rotatePoint(v1, t->center, t->rotationDeg),
        rotatePoint(v2, t->center, t->rotationDeg),
        rotatePoint(v3, t->center, t->rotationDeg),
        t->color);
}

void triangle_spin(triangle_t* t, spinDirection_e d)
{
    static const int MAX_ANGLE = 360.0;

    switch (d) {
    case SDIR_LEFT:
        t->rotationDeg += SPIN_SPEED;
        break;

    case SDIR_RIGHT:
        t->rotationDeg -= SPIN_SPEED;
        break;
    }

    if (t->rotationDeg >= MAX_ANGLE) {
        t->rotationDeg -= MAX_ANGLE;
    }
}

void triangle_move(triangle_t* t, direction_e d, float speed)
{
    switch (d) {
    case DIR_DOWN:
        t->center.y += speed;
        if (t->center.y > WIN_HEIGHT) {
            t->center.y = WIN_HEIGHT;
        }
        break;

    case DIR_UP:
        t->center.y -= speed;
        if (t->center.y < 0) {
            t->center.y = 0;
        }
        break;

    case DIR_LEFT:
        t->center.x -= speed;
        if (t->center.x < 0) {
            t->center.x = 0;
        }
        break;

    case DIR_RIGHT:
        t->center.x += speed;
        if (t->center.x > WIN_WIDTH) {
            t->center.x = WIN_WIDTH;
        }
        break;
    }
}

void triangle_moveTowards(triangle_t* t, Vector2* other, float speed)
{
    Vector2 direction = Vector2Subtract(*other, t->center);
    float distance = Vector2Length(direction);

    // Already there (or close enough) — snap and stop
    if (distance <= speed || distance == 0.0f)
        return;

    direction = Vector2Scale(direction, 1.0f / distance); // normalize
    t->center = Vector2Add(t->center, Vector2Scale(direction, speed));
}

void triangle_shoot(triangle_t* t, projectile_t* projectiles, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        if (projectiles[i].isDisplayed == false) {
            projectiles[i] = projectile_create(t->center, t->rotationDeg);
            return;
        }
    }
}

void triangle_faceOther(triangle_t* t, triangle_t* other)
{
    t->rotationDeg = getAngleInDegrees(&t->center, &other->center);
}
