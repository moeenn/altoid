#include "config.h"
#include "game.h"
#include <raylib.h>

int main()
{
    InitWindow(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE);
    SetTargetFPS(FPS);

    projectile_t projectiles[MAX_PROJECTILES];
    size_t pi = 0;

    triangle_t player = {
        .center = randomPosition(),
        .size = TRIANGLE_SIZE,
        .color = GREEN,
        .rotationDeg = 0.0f,
    };

    triangle_t enemies[MAX_ENEMIES];
    for (size_t i = 0; i < MAX_ENEMIES; i++) {
        Vector2 pos = randomPositionOffScreen();
        enemies[i] = (triangle_t) {
            .size = TRIANGLE_SIZE,
            .color = RED,
            .center = pos,
            .rotationDeg = getAngleInDegrees(&pos, &player.center),
        };
    }

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_E)) {
            triangle_spin(&player, SDIR_LEFT);
        }
        if (IsKeyDown(KEY_Q)) {
            triangle_spin(&player, SDIR_RIGHT);
        }
        if (IsKeyDown(KEY_LEFT)) {
            triangle_move(&player, DIR_LEFT, PLAYER_MOVE_SPEED);
        }
        if (IsKeyDown(KEY_RIGHT)) {
            triangle_move(&player, DIR_RIGHT, PLAYER_MOVE_SPEED);
        }
        if (IsKeyDown(KEY_UP)) {
            triangle_move(&player, DIR_UP, PLAYER_MOVE_SPEED);
        }
        if (IsKeyDown(KEY_DOWN)) {
            triangle_move(&player, DIR_DOWN, PLAYER_MOVE_SPEED);
        }
        if (IsKeyDown(KEY_SPACE)) {
            triangle_shoot(&player, projectiles, MAX_PROJECTILES);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        {
            triangle_render(&player);

            for (pi = 0; pi < MAX_ENEMIES; pi++) {
                triangle_faceOther(&enemies[pi], &player);
                triangle_moveTowards(&enemies[pi], &player.center, ENEMY_MOVE_SPEED);
                triangle_render(&enemies[pi]);
            }

            for (pi = 0; pi < MAX_PROJECTILES; pi++) {
                projectile_move(&projectiles[pi]);
                projectile_render(&projectiles[pi]);
            }
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
