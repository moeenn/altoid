#include "game.h"
#include <raylib.h>
#include <stdio.h>

void enemies_render(entity_t *enemies, entity_t *player)
{
    size_t idx = 0;

#pragma unroll
    for (idx = 0; idx < MAX_ENEMIES; idx++)
    {
        entity_faceOther(&enemies[idx], player);
        entity_moveTowards(&enemies[idx], &player->center, ENEMY_MOVE_SPEED);
        entity_render(&enemies[idx]);
    }
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

int main()
{
    InitWindow(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE);
    SetTargetFPS(FPS);

    size_t pidx = 0;
    size_t eidx = 0;

    int score = 0;
    char scoreString[4];
    int sprintResult = 0;

    entity_t player = entity_newPlayer();
    entity_t enemies[MAX_ENEMIES];
    projectile_t projectiles[MAX_PROJECTILES];

#pragma unroll
    for (size_t i = 0; i < MAX_ENEMIES; i++)
    {
        enemies[i] = entity_newEnemy(&player.center);
    }

    while (!WindowShouldClose())
    {
        if (IsKeyDown(KEY_D))
        {
            entity_spin(&player, SDIR_LEFT);
        }
        if (IsKeyDown(KEY_A))
        {
            entity_spin(&player, SDIR_RIGHT);
        }
        if (IsKeyDown(KEY_LEFT))
        {
            entity_move(&player, (entityMove_t){DIR_LEFT, PLAYER_MOVE_SPEED});
        }
        if (IsKeyDown(KEY_RIGHT))
        {
            entity_move(&player, (entityMove_t){DIR_RIGHT, PLAYER_MOVE_SPEED});
        }
        if (IsKeyDown(KEY_UP))
        {
            entity_move(&player, (entityMove_t){DIR_UP, PLAYER_MOVE_SPEED});
        }
        if (IsKeyDown(KEY_DOWN))
        {
            entity_move(&player, (entityMove_t){DIR_DOWN, PLAYER_MOVE_SPEED});
        }
        if (IsKeyDown(KEY_SPACE))
        {
            entity_shoot(&player, projectiles);
        }

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
            for (pidx = 0; pidx < MAX_ENEMIES; pidx++)
            {

#pragma unroll
                for (eidx = 0; eidx < MAX_PROJECTILES; eidx++)
                {
                    if (entity_isHit(&enemies[pidx], &projectiles[eidx]))
                    {
                        entity_reduceHealth(&enemies[pidx]);
                        projectiles[eidx] = projectile_hide();

                        if (enemies[pidx].health <= 0)
                        {
                            // reset enemy.
                            score += 1;
                            enemies[pidx] = entity_newEnemy(&player.center);
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
