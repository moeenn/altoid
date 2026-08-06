#include "game.h"
#include <raylib.h>
#include <stdio.h>

explosion_t explosion_new(bool isDisplayed, Vector2 pos)
{
    return (explosion_t){
        .isDisplayed = isDisplayed,
        .pos = pos,
        .color = EXPLOSION_COLOR,
        .radius = EXPLOSION_MIN_RADIUS,
    };
}

void explosion_render(explosion_t *self)
{
    static const int MAX_OPACITY = 180;
    if (!self->isDisplayed || (self->radius == EXPLOSION_MAX_RADIUS && self->color.a == MAX_OPACITY))
    {
        self->isDisplayed = false;
        self->radius = EXPLOSION_MIN_RADIUS;
        self->color = EXPLOSION_COLOR;
        return;
    }

    self->radius = clampMax(self->radius + 1, EXPLOSION_MAX_RADIUS);
    self->color.a = clampMax(self->color.a + 5, MAX_OPACITY);
    DrawCircle((int)self->pos.x, (int)self->pos.y, (float)self->radius, self->color);
}

void explosion_add(explosion_t *explosions, Vector2 pos)
{
#pragma unroll
    for (size_t i = 0; i < MAX_EXPLOSIONS; i++)
    {
        if (!explosions[i].isDisplayed)
        {
            explosions[i] = explosion_new(true, pos);
        }
    }
}

void explosions_render(explosion_t *explosions)
{
#pragma unroll
    for (size_t i = 0; i < MAX_EXPLOSIONS; i++)
    {
        explosion_render(&explosions[i]);
    }
}
