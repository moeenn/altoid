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

entity_t entity_newPlayer()
{
    return (entity_t){
        .center = randomPosition(),
        .size = (int)ENTITY_SIZE,
        .color = PLAYER_COLOR,
        .rotationDeg = 0.0F,
        .health = PLAYER_MAX_HEALTH,
    };
}

entity_t entity_newEnemy(Vector2 *playerPos)
{
    Vector2 pos = randomPositionOffScreen();
    return (entity_t){
        .size = (int)ENTITY_SIZE,
        .color = ENEMY_COLOR,
        .center = pos,
        .rotationDeg = getAngleInDegrees(&pos, playerPos),
        .health = PLAYER_MAX_HEALTH,
    };
}

void entity_render(const entity_t *self)
{
    if (self->health <= 0)
    {
        return;
    }

    // without rotation.
    Vector2 vec1 = {.x = self->center.x, .y = self->center.y - (float)self->size};
    Vector2 vec2 = {.x = self->center.x - (float)self->size, .y = self->center.y + (float)self->size};
    Vector2 vec3 = {.x = self->center.x + (float)self->size, .y = self->center.y + (float)self->size};

    DrawTriangle(rotatePoint(vec1, self->center, self->rotationDeg), rotatePoint(vec2, self->center, self->rotationDeg),
                 rotatePoint(vec3, self->center, self->rotationDeg), self->color);

    // draw healthbar.
    static const int BAR_HEIGHT = 2;
    static const int BAR_CELL_WIDTH = 5;
    static const Color BAR_COLOR = BLUE;
    DrawRectangle((int)self->center.x + self->size, (int)self->center.y - self->size, BAR_CELL_WIDTH * self->health,
                  BAR_HEIGHT, BAR_COLOR);
}

void entity_spin(entity_t *self, spinDirection_e dir)
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

void entity_move(entity_t *self, const entityMove_t args)
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

void entity_moveTowards(entity_t *self, Vector2 *other, const float speed)
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

void entity_shoot(entity_t *self, projectile_t *projectiles)
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

void entity_faceOther(entity_t *self, entity_t *other)
{
    self->rotationDeg = getAngleInDegrees(&self->center, &other->center);
}

void entity_reduceHealth(entity_t *self)
{
    if (self->health > 0)
    {
        self->health -= 1;
    }
}

bool entity_isHit(const entity_t *self, projectile_t *projectile)
{
    if (!projectile->isDisplayed)
    {
        return false;
    }

    float dist = Vector2Distance(self->center, projectile->pos);
    return (bool)(dist <= (float)self->size);
}
