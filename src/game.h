#pragma once
#include "raylib.h"
#include "stdint.h"

#define ALIGN_8 8
#define ALIGN_16 16
#define ALIGN_32 32

#define clampMin(a, b) (((a) > (b)) ? (a) : (b))
#define clampMax(a, b) (((a) < (b)) ? (a) : (b))

static const char *const WIN_TITLE = "Game";
static const int WIN_WIDTH = 1000;
static const int WIN_HEIGHT = 800;
static const int FPS = 60;
static const float SPIN_SPEED = 5.0F;
static const float ENTITY_SIZE = 10.0F;
static const Color PLAYER_COLOR = GREEN;
static const Color ENEMY_COLOR = ORANGE;
static const float PLAYER_MOVE_SPEED = 5.0F;
static const float ENEMY_MOVE_SPEED = 2.0F;
static const float PROJECTILE_SIZE = 2.0F;
static const float PROJECTILE_SPEED = 7.0F;
static const Color PROJECTILE_COLOR = RED;
static const int MAX_PROJECTILES = 40;
static const int MAX_ENEMIES = 5;
static const int8_t PLAYER_MAX_HEALTH = 5;
static const Vector2 PLAYER_HEALTHBAR_POS = {10, 10};
static const int SCORE_POS_X = 10;
static const int SCORE_POS_Y = 10;
static const int SCORE_FONT_SIZE = 20;
static const Color SCORE_COLOR = WHITE;
static const int HEALTHBAR_HEIGHT = 2;
static const int HEALTHBAR_CELL_WIDTH = 5;
static const Color HEALTHBAR_COLOR = BLUE;

typedef struct
{
    int x;
    int y;
} __attribute__((aligned(ALIGN_8))) vec2_t;

typedef struct
{
    Vector2 center;
    Color color;
    float rotationDeg;
    int size;
    int8_t health;
} __attribute__((aligned(ALIGN_32))) entity_t;

typedef enum
{
    SDIR_LEFT,
    SDIR_RIGHT,
} spinDirection_e;

typedef enum
{
    DIR_NORTH,
    DIR_NORTHEAST,
    DIR_EAST,
    DIR_SOUTHEAST,
    DIR_SOUTH,
    DIR_SOUTHWEST,
    DIR_WEST,
    DIR_NORTHWEST,
} direction_e;

typedef struct
{
    direction_e dir;
    float speed;
} __attribute__((aligned(ALIGN_8))) entityMove_t;

typedef struct
{
    Vector2 pos;
    Vector2 direction;
    bool isDisplayed;
} __attribute__((aligned(ALIGN_32))) projectile_t;

void keymap_enable(entity_t *player, projectile_t *projectiles);
Vector2 randomPosition();
Vector2 randomPositionOffScreen();
float getAngleInDegrees(Vector2 *from, Vector2 *towards);
Vector2 rotatePoint(Vector2 point, Vector2 center, float angleDeg);
entity_t entity_newPlayer();
entity_t entity_newEnemy(Vector2 *playerPos);
void entity_render(const entity_t *self);
void entity_spin(entity_t *self, spinDirection_e dir);
void entity_move(entity_t *self, entityMove_t args);
void entity_moveTowards(entity_t *self, Vector2 *other, float speed);
void entity_shoot(entity_t *self, projectile_t *projectiles);
void entity_faceOther(entity_t *self, entity_t *other);
void entity_reduceHealth(entity_t *self);
bool entity_isHit(const entity_t *self, projectile_t *projectile);
void enemies_render(entity_t *enemies, entity_t *player);
projectile_t projectile_create(Vector2 startingPos, float angleDeg);
void projectile_move(projectile_t *self);
projectile_t projectile_hide();
void projectile_render(const projectile_t *self);
void projectiles_render(projectile_t *projectiles);
