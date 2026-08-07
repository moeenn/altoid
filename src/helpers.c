#include "game.h"
#include <math.h>
#include <raylib.h>
#include <stdio.h>

Vector2 randomPosition()
{
    int xPos = GetRandomValue(0, WIN_WIDTH);
    int yPos = GetRandomValue(0, WIN_HEIGHT);
    return (Vector2){
        .x = (float)xPos,
        .y = (float)yPos,
    };
}

Vector2 randomPositionOffScreen()
{
    const int PADDING = 20;
    const int MAX_W = WIN_WIDTH + PADDING;
    const int MAX_H = WIN_HEIGHT + PADDING;
    int randSide = GetRandomValue(0, 3); // 0 = top, clock-wise,

    int xPos = 0;
    int yPos = 0;

    switch (randSide)
    {
    case 0: // TOP.
        xPos = GetRandomValue(-PADDING, MAX_W);
        yPos = -PADDING;
        break;

    case 1: // RIGHT.
        yPos = GetRandomValue(-PADDING, MAX_H);
        xPos = MAX_W;
        break;

    case 2: // BOTTOM.
        xPos = GetRandomValue(-PADDING, MAX_W);
        yPos = MAX_H;
        break;

    case 3: // LEFT.
        yPos = GetRandomValue(-PADDING, MAX_H);
        xPos = -PADDING;
        break;

    default:
        xPos = 0;
        yPos = 0;
        break;
    }

    return (Vector2){
        .x = (float)xPos,
        .y = (float)yPos,
    };
}

float getAngleInDegrees(Vector2 *from, Vector2 *towards)
{
    float distX = towards->x - from->x;
    float distY = towards->y - from->y;
    float angleRad = atan2f(distY, distX);
    float angleDeg = angleRad * RAD2DEG;

    static const float ANGLE_ADJ = 90.0F;
    return angleDeg + ANGLE_ADJ;
}
