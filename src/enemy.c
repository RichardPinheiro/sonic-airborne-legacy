#include "game.h"

void enemy_motion(Sprite *enemy, Uint32 delta_time) {
    enemy->x += enemy->speed * get_time_scale_factor(delta_time);
    if (enemy->x + enemy->width < 0) {
        enemy->x = WINDOW_WIDTH + enemy->width;
        enemy->y = set_random_position(enemy);
    }
    update_sprite_boundaries(enemy);
}