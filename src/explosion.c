#include "game.h"
#include <raylib.h>
#include <stdio.h>

explosion_t explosion__new(bool isDisplayed, Vector2 pos)
{
    return (explosion_t){
        .isDisplayed = isDisplayed,
        .pos = pos,
        .color = EXPLOSION_COLOR,
        .radius = EXPLOSION_MIN_RADIUS,
    };
}

void explosion__render(explosion_t *self)
{
    if (!self->isDisplayed)
    {
        return;
    }

    if (self->radius == EXPLOSION_MAX_RADIUS && self->color.a == 0)
    {
        self->isDisplayed = false;
        return;
    }

    self->radius = clampMax(self->radius + 1, EXPLOSION_MAX_RADIUS);
    self->color.a = clampMin(0, self->color.a - 5);
    DrawCircle((int)self->pos.x, (int)self->pos.y, (float)self->radius, self->color);
}

void explosion__add(explosion_t *explosions, Vector2 pos)
{
#pragma unroll
    for (size_t i = 0; i < MAX_EXPLOSIONS; i++)
    {
        if (!explosions[i].isDisplayed)
        {
            explosions[i] = explosion__new(true, pos);
            return;
        }
    }
}

void explosions__init(explosion_t *explosions)
{
#pragma unroll
    for (size_t i = 0; i < MAX_EXPLOSIONS; i++)
    {
        explosions[i].isDisplayed = false;
    }
}

void explosions__render(explosion_t *explosions)
{
#pragma unroll
    for (size_t i = 0; i < MAX_EXPLOSIONS; i++)
    {
        explosion__render(&explosions[i]);
    }
}
