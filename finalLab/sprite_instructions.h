//sprite_instructions.h

void g_draw_start_screen();
void g_draw_game_over();

void g_clear_player_status();
void g_clear_dice_sprite();
void g_clear_monster_sprite();
void g_clear_monster_info();
void g_clear_status_text();
void g_clear_panels();

void g_draw_borders();
void g_draw_player_status(int hp);
void g_draw_monster_status(char* mname, int hp, int atk);
void g_draw_status_text(char* status_l1, char* status_l2, char* status_l3);
void g_update_status(char* status_l1, char* status_l2, char* status_l3);

void g_dice_preroll();
void g_dice_midroll(char* wit);
void g_dice_postroll(int val1, int val2);

void g_draw_sphere();
void g_draw_cube();
void g_draw_pyramid();
void g_draw_twin();

void g_new_monster(int stagecode, int p_hp, int mon_hp, int mon_atk, char* mname);
void g_update_turn(int p_hp, int mon_hp, int mon_atk, char* mname);