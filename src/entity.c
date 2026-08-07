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
        .center = {(float)WIN_WIDTH / 2, (float)WIN_HEIGHT / 2},
        .size = (int)ENTITY_SIZE,
        .color = PLAYER_COLOR,
        .targetRotationDeg = 0.0F,
        .currentRotationDeg = 0.0F,
        .health = PLAYER_MAX_HEALTH,
    };
}

entity_t entity_newEnemy(Vector2 *playerPos)
{
    Vector2 pos = randomPositionOffScreen();
    float rotation = getAngleInDegrees(&pos, playerPos);
    return (entity_t){
        .size = (int)ENTITY_SIZE,
        .color = ENEMY_COLOR,
        .center = pos,
        .targetRotationDeg = rotation,
        .currentRotationDeg = rotation,
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
    static const float HEAD_ADJUSTMENT = 4.0F;
    Vector2 vec1 = {.x = self->center.x, .y = self->center.y - (float)self->size - HEAD_ADJUSTMENT};
    Vector2 vec2 = {.x = self->center.x - (float)self->size, .y = self->center.y + (float)self->size};
    Vector2 vec3 = {.x = self->center.x + (float)self->size, .y = self->center.y + (float)self->size};

    DrawTriangle(rotatePoint(vec1, self->center, self->currentRotationDeg),
                 rotatePoint(vec2, self->center, self->currentRotationDeg),
                 rotatePoint(vec3, self->center, self->currentRotationDeg), self->color);

    // draw healthbar.
    DrawRectangle((int)self->center.x + self->size, (int)self->center.y - self->size,
                  HEALTHBAR_CELL_WIDTH * self->health, HEALTHBAR_HEIGHT, HEALTHBAR_COLOR);
}

void entity_spin(entity_t *self, spinDirection_e dir)
{
    static const float MAX_ANGLE = 360.0F;

    switch (dir)
    {
    case SDIR_LEFT:
        self->currentRotationDeg += SPIN_SPEED;
        break;

    case SDIR_RIGHT:
        self->currentRotationDeg -= SPIN_SPEED;
        break;
    }

    if (self->currentRotationDeg >= MAX_ANGLE)
    {
        self->currentRotationDeg -= MAX_ANGLE;
    }
}

void entity_move(entity_t *self, const entityMove_t args)
{
    static const float DIAG_ADJUSTMENT = 0.7F;
    float adj = args.speed * DIAG_ADJUSTMENT;

    switch (args.dir)
    {
    case DIR_NORTH:
        self->center.y = clampMin(0, self->center.y - args.speed);
        break;

    case DIR_NORTHEAST:
        self->center.x = clampMax(self->center.x + adj, (float)WIN_WIDTH);
        self->center.y = clampMin(0.0F, self->center.y - adj);
        break;

    case DIR_EAST:
        self->center.x = clampMax(self->center.x + args.speed, (float)WIN_WIDTH);
        break;

    case DIR_SOUTHEAST:
        self->center.x = clampMax(self->center.x + adj, (float)WIN_WIDTH);
        self->center.y = clampMax(self->center.y + adj, (float)WIN_HEIGHT);
        break;

    case DIR_SOUTH:
        self->center.y = clampMax(self->center.y + args.speed, (float)WIN_HEIGHT);
        break;

    case DIR_SOUTHWEST:
        self->center.x = clampMin(0, self->center.x - adj);
        self->center.y = clampMax(self->center.y + adj, (float)WIN_HEIGHT);
        break;

    case DIR_WEST:
        self->center.x = clampMin(0.0F, self->center.x - args.speed);
        break;

    case DIR_NORTHWEST:
        self->center.x = clampMin(0, self->center.x - adj);
        self->center.y = clampMin(0, self->center.y - adj);
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
            projectiles[i] = projectile_create(self->center, self->currentRotationDeg);
            return;
        }
    }
}

void entity_faceOther(entity_t *self, entity_t *other)
{
    self->targetRotationDeg = getAngleInDegrees(&self->center, &other->center);
    float delta = self->currentRotationDeg - self->targetRotationDeg;

    float update =
        (delta > 0) ? (self->currentRotationDeg - ENTITY_SPIN_DRAG) : (self->currentRotationDeg + ENTITY_SPIN_DRAG);

    static const float MAX_ANGLE = 360.0F;
    if (update < 0.0F)
    {
        update = MAX_ANGLE + update;
    }
    if (update > MAX_ANGLE)
    {
        update = MAX_ANGLE - update;
    }

    self->currentRotationDeg = update;
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

void enemies_init(entity_t *enemies, entity_t *player)
{
#pragma unroll
    for (size_t i = 0; i < MAX_ENEMIES; i++)
    {
        enemies[i] = entity_newEnemy(&player->center);
    }
}

void enemies_render(entity_t *enemies, entity_t *player)
{
    size_t idx = 0;

#pragma unroll
    for (idx = 0; idx < MAX_ENEMIES; idx++)
    {
        entity_faceOther(&enemies[idx], player);
        entity_moveTowards(&enemies[idx], &player->center, ENEMY_MOVE_SPEED);
        entity_render(&enemies[idx]);
    }
}
