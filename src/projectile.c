#include "game.h"
#include "raylib.h"
#include <math.h>

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

void projectile_render(const projectile_t *self)
{
    if (!self->isDisplayed)
    {
        return;
    }

    DrawCircle((int)self->pos.x, (int)self->pos.y, PROJECTILE_SIZE, PROJECTILE_COLOR);
}
