#include "game.h"

void keymap_enable(entity_t *player, projectile_t *projectiles)
{
    if (IsKeyDown(KEY_D))
    {
        entity_spin(player, SDIR_LEFT);
    }
    if (IsKeyDown(KEY_A))
    {
        entity_spin(player, SDIR_RIGHT);
    }

    if (IsKeyDown(KEY_UP) && IsKeyDown(KEY_RIGHT))
    {
        entity_move(player, (entityMove_t){DIR_NORTHEAST, PLAYER_MOVE_SPEED});
        return;
    }
    if (IsKeyDown(KEY_DOWN) && IsKeyDown(KEY_RIGHT))
    {
        entity_move(player, (entityMove_t){DIR_SOUTHEAST, PLAYER_MOVE_SPEED});
        return;
    }
    if (IsKeyDown(KEY_DOWN) && IsKeyDown(KEY_LEFT))
    {
        entity_move(player, (entityMove_t){DIR_SOUTHWEST, PLAYER_MOVE_SPEED});
        return;
    }
    if (IsKeyDown(KEY_UP) && IsKeyDown(KEY_LEFT))
    {
        entity_move(player, (entityMove_t){DIR_NORTHWEST, PLAYER_MOVE_SPEED});
        return;
    }

    if (IsKeyDown(KEY_LEFT))
    {
        entity_move(player, (entityMove_t){DIR_WEST, PLAYER_MOVE_SPEED});
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        entity_move(player, (entityMove_t){DIR_EAST, PLAYER_MOVE_SPEED});
    }
    if (IsKeyDown(KEY_UP))
    {
        entity_move(player, (entityMove_t){DIR_NORTH, PLAYER_MOVE_SPEED});
    }
    if (IsKeyDown(KEY_DOWN))
    {
        entity_move(player, (entityMove_t){DIR_SOUTH, PLAYER_MOVE_SPEED});
    }
    if (IsKeyDown(KEY_SPACE))
    {
        entity_shoot(player, projectiles);
    }
}
