//sprite_instructions.c
//by Ethan Geller
//
//instructions for drawing various sprites, including:
//1. Start screen
//2. dice, pre-roll
//3. dice, mid-roll
//4. dice, roll result
//5. enemy 1: "Chaos Sphere"
//6. enemy 2: "Shadow Cube"
//7. enemy 3: "Lightning Pyramid"
//8. enemy 4: "Twin Dragon"
//9. Game Over screen

#include "sprite_instructions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <f3d_lcd_sd.h>
#include <f3d_delay.h>
#include <glcdfont.h>

#define ANIMATE 1
#define ADELAY 10

//START SCREEN
void g_draw_start_screen(){
  f3d_lcd_fillScreen(WHITE);
  delay(100);
  f3d_lcd_drawString(0,0,"YATZDEE AND D", BLACK, WHITE);
  if(ANIMATE) delay(ADELAY);
  f3d_lcd_drawString(50, 19, "By Ethan Geller", RED, WHITE);
  if(ANIMATE) delay(ADELAY);
  f3d_lcd_drawString(30, 30, "and Johnny O'Donnell", BLUE, WHITE);
  if(ANIMATE) delay(ADELAY);
  f3d_lcd_drawString(0, 50, "In which Yatzdee and D,", GREEN, WHITE);
  if(ANIMATE) delay(ADELAY);
  f3d_lcd_drawString(10, 65, "two adventurous rogue dice,", RED, WHITE);
  if(ANIMATE) delay(ADELAY);
  f3d_lcd_drawString(0, 80, "fight", BLACK, WHITE);
  if(ANIMATE) delay(ADELAY);
  f3d_lcd_drawString(30, 90, "insidiously geometric", RED, WHITE);
  if(ANIMATE) delay(ADELAY);
  f3d_lcd_drawString(0, 110, "monsters...", BLACK, WHITE);
  if(ANIMATE) delay(ADELAY);
  f3d_lcd_drawString(0, 130, "Press C to start...", BLACK, WHITE);
}

//GAME OVER SCREEN
void g_draw_game_over(){
    f3d_lcd_fillScreen(BLACK);
    f3d_lcd_drawString(35, 70, "GAME OVER :(", RED, BLACK);
    delay(1000);
    f3d_lcd_drawString(35, 70, "GAME OVER :(", BLUE, BLACK);
    delay(1000);
    f3d_lcd_drawString(35, 70, "GAME OVER :(", GREEN, BLACK);
    delay(1000);
}

//GAME SCREEN
//Main Panels:
//Player Status: (0:44, 0:49)
//Blue Border (0:43, 50)
//Blue Border (45, 0:49)
//Dice Sprite: (0:43, 51:99)
//Red Border: (46:89, 50)
//Red Border: (45, 51:99)
//Monster Sprite: (46:89, 0:49)
//Monster Info: (46:89, 51:99)
//White Border(0:89, 100)
//Status Text: (0:89, 101:159)


//CLEAR FUNCTIONS
void g_clear_player_status(){
  int x,y;
  for(x=0; x<45; x++)
    for(y=0; y<50; y++)
      f3d_lcd_drawPixel(x,y,BLACK);
}

void g_clear_dice_sprite(){
  int x,y;
  for(x=0; x<45; x++)
    for(y=51; y<100; y++)
      f3d_lcd_drawPixel(x,y,BLACK);
}

void g_clear_monster_sprite(){
  int x,y;
  for(x=46; x<90;x++)
    for(y=0; y<50; y++)
      f3d_lcd_drawPixel(x,y,BLACK);
}

void g_clear_monster_info(){
  int x,y;
  for(x=46; x<90; x++)
    for(y=51; y<100; y++)
      f3d_lcd_drawPixel(x,y,BLACK);
}

void g_clear_status_text(){
  int x,y;
  for(x=0; x<90; x++)
    for(y=101; y<160; y++)
      f3d_lcd_drawPixel(x,y,BLACK);
}

void g_clear_panels(){
    g_clear_player_status();
    if(ANIMATE) delay(ADELAY);
    g_clear_dice_sprite();
    if(ANIMATE) delay(ADELAY);
    g_clear_monster_sprite();
    if(ANIMATE) delay(ADELAY);
    g_clear_monster_info;
    if(ANIMATE) delay(ADELAY);
    g_clear_status_text();
}

//DRAW INFO FUNCTIONS

void g_draw_borders(){
  f3d_lcd_fillScreen(BLACK);
  int x,y;
  for(x=0; x<45; x++)
    f3d_lcd_drawPixel(x, 50, BLUE);
  if(ANIMATE) delay(ADELAY);
  for(y=0;y<50;y++)
    f3d_lcd_drawPixel(45,y,BLUE);
  if(ANIMATE) delay(ADELAY);
  for(x=46; x<90; x++)
    f3d_lcd_drawPixel(x,50, RED);
  if(ANIMATE) delay(ADELAY);
  for(y=51;y<90; y++)
    f3d_lcd_drawPixel(45, y, RED);
  if(ANIMATE) delay(ADELAY);
  for(x=0; x<90; x++)
    f3d_lcd_drawPixel(x, 100, WHITE);
}

void g_draw_player_status(int hp){
    char str[7];
    sprintf(str, "HP: %d", hp);
    f3d_lcd_drawString(0,0,"YOU", WHITE,BLACK);
    f3d_lcd_drawString(0,15, str, GREEN, BLACK);
    if(hp<4)f3d_lcd_drawString(0,30, "LOW HP!", RED, BLACK);
}

void g_draw_monster_status(char* mname, int hp, int atk){
    char hp_str[7];
    char atk_str[7];
    sprintf(hp_str, "HP: %d", hp);
    sprintf(atk_str, "ATK: %d", atk);
    f3d_lcd_drawString(50, 52, mname, WHITE, BLACK);
    f3d_lcd_drawString(50, 62, hp_str, GREEN, BLACK);
    f3d_lcd_drawString(50, 78, atk_str, RED, BLACK);
}

void g_draw_status_text(char* status_l1, char* status_l2, char* status_l3){
    f3d_lcd_drawString(0, 102, status_l1, WHITE, BLACK);
    if(ANIMATE) delay(ADELAY);
    f3d_lcd_drawString(0, 122, status_l2, WHITE, BLACK);
    if(ANIMATE) delay(ADELAY);
    f3d_lcd_drawString(0, 142, status_l3, WHITE, BLACK);
}

void g_update_status(char* status_l1, char* status_l2, char* status_l3){
    g_clear_status_text();
    g_draw_status_text(status_l1,status_l2,status_l3);
}

//DICE SPRITES
//1: dice pre-roll
//2: dice mid-roll
//3: dice post-roll

void g_dice_preroll(){
    g_clear_dice_sprite();
    int x,y;
    for(x=5; x<21; x++){
        for(y=54; y<69; y++)
            f3d_lcd_drawPixel(x,y, CYAN);
    }
    if(ANIMATE) delay(ADELAY);
    for(x=25; x<40; x++){
        for(y=54; y<69; y++)
            f3d_lcd_drawPixel(x,y, WHITE);
    }
    f3d_lcd_drawString(3, 74, "c+shake", WHITE, BLACK);
}

void g_dice_midroll(char* wit){
    g_clear_dice_sprite();
    int x,y;
    for(x=5; x<30; x++){
        for(y=54; y<69; y++)
            f3d_lcd_drawPixel(x,y, CYAN);
    }
    if(ANIMATE) delay(ADELAY);
    for (x=25; x<40; x++){
        for(y=61; y<76; y++)
            f3d_lcd_drawPixel(x,y, WHITE);
    }
    f3d_lcd_drawString(3,78, wit, MAGENTA, BLACK);
}

void g_dice_postroll(int val1, int val2){
    g_clear_dice_sprite();
    char val1_str[3];
    char val2_str[3];
    sprintf(val1_str, "%d", val1);
    sprintf(val2_str, "%d", val2);
    int x,y;
    for(x=0; x<22; x++){
        for(y=51; y<99; y++)
            f3d_lcd_drawPixel(x,y, CYAN);
    }
    f3d_lcd_drawString(11,74,val1_str, WHITE, BLACK);
    if(ANIMATE) delay(ADELAY);
    for(x=22; x<45; x++){
        for(y=51; y<99; y++)
            f3d_lcd_drawPixel(x,y, WHITE);
    }
    f3d_lcd_drawString(33, 74, val2_str, WHITE, BLACK);
}

//MONSTER SPRITES
//monsters are called by the g_draw_monster method, which takes the same stagecode from the game_handler:
//0: Start screen, don't do anything
//1: Chaos Sphere g_draw_sphere
//2: Shadow Cube g_draw_cube
//3: Lighting Pyramid g_draw_pyramid
//4: Twin Dragon g_draw_twin
//5: Game Over, don't do anything

void g_draw_sphere(){
    //standard raster circle implementation, with coloristic choices
    g_clear_monster_sprite();
    int origin_x = 67;
    int origin_y = 24;
    int r = 20;
    int r_err = 1-r;
    int x = r;
    int y = 0;
    
    while(x>=y){
        f3d_lcd_drawPixel(x+origin_x, y+origin_y, WHITE);
        if(ANIMATE) delay(ADELAY);
        f3d_lcd_drawPixel(y+origin_x, x+origin_y, CYAN);
        if(ANIMATE) delay(ADELAY);
        f3d_lcd_drawPixel(-x+origin_x, y+origin_y, MAGENTA);
        if(ANIMATE) delay(ADELAY);
        f3d_lcd_drawPixel(-y+origin_x, x+origin_y, BLUE);
        if(ANIMATE) delay(ADELAY);
        f3d_lcd_drawPixel(-x+origin_x, -y+origin_y, RED);
        if(ANIMATE) delay(ADELAY);
        f3d_lcd_drawPixel(-y+origin_x, -x+origin_y, GREEN);
        if(ANIMATE) delay(ADELAY);
        f3d_lcd_drawPixel(x+origin_x, -y+origin_y, BLACK);
        if(ANIMATE) delay(ADELAY);
        f3d_lcd_drawPixel(y+origin_x, -x+origin_y, WHITE);
        if(ANIMATE) delay(ADELAY);
        y++;
        if(r_err<0) r_err += 2 * y + 1;
        else{
            x--;
            r_err += 2 * (y - x + 1);
        }
    }
}

void g_draw_cube(){
    //A cube so insidiously geometric it hides an entire dimension!
    g_clear_monster_sprite();
    int x,y;
    for(x=50; x<85; x++){
        for(y=5; y<45; y++){
            if(x%6==0 && y%7==0) f3d_lcd_drawPixel(x,y, CYAN);
            else f3d_lcd_drawPixel(x,y, BLUE);
        }
    }
}

void g_draw_pyramid(){
    g_clear_monster_sprite();
    int x,y;
    int a = 35;
    for(x=50; x<85; x++){
        for(y=5; y<45; y++){
            if(x<a) f3d_lcd_drawPixel(x,y, CYAN);
            else f3d_lcd_drawPixel(x,y, BLUE);
        }
	a--;
    }
}

void g_draw_twin(){
    //dragon curve implementation
    char max = 250; //might have to be changed
    char t=0;
    char k=0;
    char x=50;
    char y=40;
    while(t<max){
      if((((t&-t)<<1)&t)!=0) k++;
        
        switch (k%4) {
            case 0:
                y++;
                break;
                
            case 1:
                x--;
                break;
                
            case 2:
                y--;
                break;
                
            case 3:
                x++;
                break;
                
            default:
                break;
        }
        if(t%2) f3d_lcd_drawPixel(x,y, CYAN);
        else f3d_lcd_drawPixel(x,y, MAGENTA);
        if(ANIMATE) delay(ADELAY);
	t++;
    }
}

//HIGHER LEVEL DRAW FUNCTIONS
//1. New Monster Draw Event
//2. HP Update Event

void g_new_monster(int stagecode, int p_hp, int mon_hp, int mon_atk, char* mname){
    if(stagecode<1||stagecode>4) return;
    
    g_clear_panels();
    switch (stagecode) {
        case 1:
            g_draw_sphere();
            g_update_status("CHAOS SPHERE", "appeared!!", "!!!");
            break;
            
        case 2:
            g_draw_cube();
            g_update_status("SHADOW CUBE", "appeared!!", "!!!");
            break;
        
        case 3:
            g_draw_pyramid();
            g_update_status("LIGHTNING PYRAMID", "appeared!!", "!!!");
            break;
            
        case 4:
            g_draw_twin();
            g_update_status("TWINDRAGON", "appeared!!", "!!!");
            break;
            
        default:
            break;
    }
    
    g_draw_player_status(p_hp);
    if(ANIMATE) delay(ADELAY);
    g_draw_monster_status(mname, mon_hp, mon_atk);
    g_dice_preroll();
}

void g_update_turn(int p_hp, int mon_hp, int mon_atk, char* mname){
    g_clear_player_status();
    g_draw_player_status(p_hp);
    g_clear_monster_info();
    g_draw_monster_status(mname, mon_hp, mon_atk);
    g_dice_preroll();
    g_update_status("ROLL!", "AGAIN!!", "!!!");
}

//and thus concludes the macabre story of sprite_instructions.c
