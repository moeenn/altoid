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

    entity_t player = entity__newPlayer();
    entity_t enemies[MAX_ENEMIES];
    projectile_t projectiles[MAX_PROJECTILES];
    explosion_t explosions[MAX_EXPLOSIONS];

    enemies__init(enemies, &player);
    explosions__init(explosions);

    size_t pIdx = 0;
    size_t eIdx = 0;

    while (!WindowShouldClose()) {
        keymap__enable(&player, projectiles);
        sprintResult = sprintf(scoreString, "%d", score);
        if (sprintResult < 0) {
            score = 0;
        }

        BeginDrawing();
        ClearBackground(WIN_BG);
        {
            DrawText(scoreString, SCORE_POS_X, SCORE_POS_Y, SCORE_FONT_SIZE, SCORE_COLOR);
            explosions__render(explosions);
            entity__render(&player);
            enemies__render(enemies, &player);

#pragma unroll
            for (eIdx = 0; eIdx < MAX_ENEMIES; eIdx++) {

#pragma unroll
                for (pIdx = 0; pIdx < MAX_PROJECTILES; pIdx++) {
                    if (entity__isHit(&enemies[eIdx], &projectiles[pIdx])) {
                        entity__reduceHealth(&enemies[eIdx]);
                        projectiles[pIdx] = projectile__hide();

                        if (enemies[eIdx].health <= 0) {
                            explosion__add(explosions, enemies[eIdx].center);
                            // reset enemy.
                            score += 1;
                            enemies[eIdx] = entity__newEnemy(&player.center);
                        }
                    }
                }
            }

            projectiles__render(projectiles);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
