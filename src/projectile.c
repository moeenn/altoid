#include "game.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>

projectile_t projectile_create(Vector2 startingPos, float angleDeg)
{
    static const float ROTATION_ADJ = 90.0F;
    float radians = (angleDeg - ROTATION_ADJ) * DEG2RAD;
    return (projectile_t){
        .pos = startingPos,
        .direction = {.x = cosf(radians), .y = sinf(radians)},
        .isDisplayed = true,
    };
}

void projectile_move(projectile_t *self)
{
    if (!self->isDisplayed)
    {
        return;
    }

    self->pos.x += self->direction.x * PROJECTILE_SPEED;
    self->pos.y += self->direction.y * PROJECTILE_SPEED;

    if (self->pos.x <= 0 || self->pos.x >= (float)WIN_WIDTH || self->pos.y <= 0 || self->pos.y >= (float)WIN_HEIGHT)
    {
        self->isDisplayed = false;
    }
}

projectile_t projectile_hide()
{
    return (projectile_t){
        .isDisplayed = false,
        .pos = {0, 0},
        .direction = {0, 0},
    };
}

void projectile_render(const projectile_t *self)
{
    if (!self->isDisplayed)
    {
        return;
    }

    DrawCircle((int)self->pos.x, (int)self->pos.y, PROJECTILE_SIZE, PROJECTILE_COLOR);
}

void projectiles_render(projectile_t *projectiles)
{
    size_t idx = 0;

#pragma unroll
    for (idx = 0; idx < MAX_PROJECTILES; idx++)
    {
        projectile_move(&projectiles[idx]);
        projectile_render(&projectiles[idx]);
    }
}
