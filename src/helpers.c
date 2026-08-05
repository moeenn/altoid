#include "config.h"
#include "game.h"
#include <math.h>
#include <raylib.h>

Vector2 randomPosition()
{
    int x = GetRandomValue(0, WIN_WIDTH);
    int y = GetRandomValue(0, WIN_HEIGHT);
    return (Vector2) { x, y };
}

Vector2 randomPositionOffScreen()
{
    const int PADDING = 20;
    const int MAX_W = WIN_WIDTH + PADDING;
    const int MAX_H = WIN_HEIGHT + PADDING;
    int randSide = GetRandomValue(0, 3); // 0 = top, clock-wise,

    int x = 0;
    int y = 0;

    switch (randSide) {
    case 0: // TOP.
        x = GetRandomValue(-PADDING, MAX_W);
        y = -PADDING;
        break;

    case 1: // RIGHT.
        y = GetRandomValue(-PADDING, MAX_H);
        y = MAX_W;
        break;

    case 2: // BOTTOM.
        x = GetRandomValue(-PADDING, MAX_W);
        y = MAX_H;
        break;

    case 3: // LEFT.
        y = GetRandomValue(-PADDING, MAX_H);
        x = -PADDING;
        break;
    }

    return (Vector2) { x, y };
}

float getAngleInDegrees(Vector2* from, Vector2* to)
{
    float dx = to->x - from->x;
    float dy = to->y - from->y;
    float angleRad = atan2f(dy, dx);
    float angleDeg = angleRad * RAD2DEG;
    return angleDeg + 90.0;
}
