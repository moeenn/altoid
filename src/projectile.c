#include "config.h"
#include "game.h"
#include "raylib.h"
#include <math.h>

projectile_t projectile_create(Vector2 startingPos, float angleDeg)
{
    float radians = (angleDeg - 90.0) * DEG2RAD;
    return (projectile_t) {
        .pos = startingPos,
        .direction = { .x = cosf(radians), .y = sinf(radians) },
        .isDisplayed = true,
    };
}

void projectile_move(projectile_t* p)
{
    if (!p->isDisplayed)
        return;

    p->pos.x += p->direction.x * PROJECTILE_SPEED;
    p->pos.y += p->direction.y * PROJECTILE_SPEED;

    if (p->pos.x <= 0 || p->pos.x >= WIN_WIDTH || p->pos.y <= 0 || p->pos.y >= WIN_HEIGHT)
        p->isDisplayed = false;
}

void projectile_render(const projectile_t* p)
{
    if (p->isDisplayed == false)
        return;

    DrawCircle(p->pos.x, p->pos.y, PROJECTILE_SIZE, PROJECTILE_COLOR);
}
