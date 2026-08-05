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

    float distX = point.x - center.x;
    float distY = point.y - center.y;

    return (Vector2){center.x + (distX * cosA - distY * sinA), center.y + (distX * sinA + distY * cosA)};
}

void triangle_render(const triangle_t *self)
{
    // without rotation.
    Vector2 vec1 = {.x = self->center.x, .y = self->center.y - (float)self->size};
    Vector2 vec2 = {.x = self->center.x - (float)self->size, .y = self->center.y + (float)self->size};
    Vector2 vec3 = {.x = self->center.x + (float)self->size, .y = self->center.y + (float)self->size};

    DrawTriangle(rotatePoint(vec1, self->center, self->rotationDeg), rotatePoint(vec2, self->center, self->rotationDeg),
                 rotatePoint(vec3, self->center, self->rotationDeg), self->color);
}

void triangle_spin(triangle_t *self, spinDirection_e dir)
{
    static const float MAX_ANGLE = 360.0F;

    switch (dir)
    {
    case SDIR_LEFT:
        self->rotationDeg += SPIN_SPEED;
        break;

    case SDIR_RIGHT:
        self->rotationDeg -= SPIN_SPEED;
        break;
    }

    if (self->rotationDeg >= MAX_ANGLE)
    {
        self->rotationDeg -= MAX_ANGLE;
    }
}

void triangle_move(triangle_t *self, const triangleMove_t args)
{
    switch (args.dir)
    {
    case DIR_DOWN:
        self->center.y += args.speed;
        if (self->center.y > (float)WIN_HEIGHT)
        {
            self->center.y = (float)WIN_HEIGHT;
        }
        break;

    case DIR_UP:
        self->center.y -= args.speed;
        if (self->center.y < 0.0F)
        {
            self->center.y = 0.0F;
        }
        break;

    case DIR_LEFT:
        self->center.x -= args.speed;
        if (self->center.x < 0.0F)
        {
            self->center.x = 0.0F;
        }
        break;

    case DIR_RIGHT:
        self->center.x += args.speed;
        if (self->center.x > (float)WIN_WIDTH)
        {
            self->center.x = (float)WIN_WIDTH;
        }
        break;
    }
}

void triangle_moveTowards(triangle_t *self, Vector2 *other, const float speed)
{
    Vector2 direction = Vector2Subtract(*other, self->center);
    float distance = Vector2Length(direction);

    // Already there (or close enough) — snap and stop
    if (distance <= speed || distance == 0.0F)
    {
        return;
    }

    direction = Vector2Scale(direction, 1.0F / distance); // normalize
    self->center = Vector2Add(self->center, Vector2Scale(direction, speed));
}

void triangle_shoot(triangle_t *self, projectile_t *projectiles)
{
#pragma unroll
    for (size_t i = 0; i < MAX_PROJECTILES; i++)
    {
        if (!projectiles[i].isDisplayed)
        {
            projectiles[i] = projectile_create(self->center, self->rotationDeg);
            return;
        }
    }
}

void triangle_faceOther(triangle_t *self, triangle_t *other)
{
    self->rotationDeg = getAngleInDegrees(&self->center, &other->center);
}
