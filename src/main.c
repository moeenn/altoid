#include "game.h"
#include <raylib.h>
#include <stdio.h>

int main()
{
    InitWindow(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE);
    SetTargetFPS(FPS);

    int score = 0;
    char scoreString[4];
    int sprintResult = 0;

    entity_t player = entity_newPlayer();
    entity_t enemies[MAX_ENEMIES];
    projectile_t projectiles[MAX_PROJECTILES];

    size_t pIdx = 0;
    size_t eIdx = 0;

#pragma unroll
    for (size_t i = 0; i < MAX_ENEMIES; i++)
    {
        enemies[i] = entity_newEnemy(&player.center);
    }

    while (!WindowShouldClose())
    {
        keymap_enable(&player, projectiles);
        sprintResult = sprintf(scoreString, "%d", score);
        if (sprintResult < 0)
        {
            score = 0;
        }

        BeginDrawing();
        ClearBackground(BLACK);
        {
            DrawText(scoreString, SCORE_POS_X, SCORE_POS_Y, SCORE_FONT_SIZE, SCORE_COLOR);
            entity_render(&player);
            enemies_render(enemies, &player);

#pragma unroll
            for (eIdx = 0; eIdx < MAX_ENEMIES; eIdx++)
            {

#pragma unroll
                for (pIdx = 0; pIdx < MAX_PROJECTILES; pIdx++)
                {
                    if (entity_isHit(&enemies[eIdx], &projectiles[pIdx]))
                    {
                        entity_reduceHealth(&enemies[eIdx]);
                        projectiles[pIdx] = projectile_hide();

                        if (enemies[eIdx].health <= 0)
                        {
                            // reset enemy.
                            score += 1;
                            enemies[eIdx] = entity_newEnemy(&player.center);
                        }
                    }
                }
            }

            projectiles_render(projectiles);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
