/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: 
 * Maintainer: 
 * Created: Thu Jan 10 11:23:43 2013
 * Last-Updated: 
 *           By: 
 *     Update #: 0
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change log:
 * 
 * 
 */
/* Code: */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
//#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuk.h>
#include <f3d_rtc.h>
#include <f3d_systick.h>
#include <ff.h>
#include <diskio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <f3d_delay.h>
#include "queue.h"
#include "sprite_instructions.h"


#define TIMER 20000
#define AUDIOBUFSIZE 128
#define MAXFILES 20

//tweaks!
#define SHAKETOROLL 0
#define AUDIO 1
#define DBG_HUD 1
#define CONSERVEMEM 1

#define PLAYERHP 16

#define ROLLFORCE 1

#define MONSTERROLLMAX 10

#define SPHEREHP 1
#define SPHEREATK 1

#define CUBEHP 3
#define CUBEATK 1

#define PYRHP 1
#define PYRATK 3

#define TWINHP 5
#define TWINATK 3


extern uint8_t Audiobuf[AUDIOBUFSIZE];
extern int audioplayerHalf;
extern int audioplayerWhole;

nunchuk_t nunchuk_data;

#if CONSERVEMEM
FATFS Fatfs;		/* File system object */
FIL fid;		/* File object */
BYTE Buff[512];		/* File read buffer */
int ret, filecount;
FRESULT rc;

char* filenames;
#endif
char* dbg;

#if CONSERVEMEM
struct ckhd {
  uint32_t ckID;
  uint32_t cksize;
};

struct fmtck {
  uint16_t wFormatTag;      
  uint16_t nChannels;
  uint32_t nSamplesPerSec;
  uint32_t nAvgBytesPerSec;
  uint16_t nBlockAlign;
  uint16_t wBitsPerSample;
};

void readckhd(FIL *fid, struct ckhd *hd, uint32_t ckID) {
  f_read(fid, hd, sizeof(struct ckhd), &ret);
  if (ret != sizeof(struct ckhd))
    exit(-1);
  if (ckID && (ckID != hd->ckID))
    exit(-1);
}

void die (FRESULT rc) {
    //printf("Failed with rc=%u.\n", rc);
  while (1);
}

void playWavFile(char* filename){
  rc = f_open(&fid,filename, FA_READ);
  
  if (!rc) {
    struct ckhd hd;
    uint32_t  waveid;
    struct fmtck fck;
    
    readckhd(&fid, &hd, 'FFIR');
    
    f_read(&fid, &waveid, sizeof(waveid), &ret);
    if ((ret != sizeof(waveid)) || (waveid != 'EVAW'))
      return;
    
    readckhd(&fid, &hd, ' tmf');
    
    f_read(&fid, &fck, sizeof(fck), &ret);
    
    // skip over extra info
    
    if (hd.cksize != 16) {
        //printf("extra header info %d\n", hd.cksize - 16);
      f_lseek(&fid, hd.cksize - 16);
    }
    /*
    printf("audio format 0x%x\n", fck.wFormatTag);
    printf("channels %d\n", fck.nChannels);
    printf("sample rate %d\n", fck.nSamplesPerSec);
    printf("data rate %d\n", fck.nAvgBytesPerSec);
    printf("block alignment %d\n", fck.nBlockAlign);
    printf("bits per sample %d\n", fck.wBitsPerSample);
    */
    // now skip all non-data chunks !
    
    while(1){
      readckhd(&fid, &hd, 0);
      if (hd.ckID == 'atad')
	break;
      f_lseek(&fid, hd.cksize);
    }
    
      //printf("Samples %d\n", hd.cksize);
    
    // Play it !
    
    //      audioplayerInit(fck.nSamplesPerSec);
    
    f_read(&fid, Audiobuf, AUDIOBUFSIZE, &ret);
    hd.cksize -= ret;
    audioplayerStart();
    while (hd.cksize > 0) {
      int next = hd.cksize > AUDIOBUFSIZE/2 ? AUDIOBUFSIZE/2 : hd.cksize;
      if (audioplayerHalf) {
	if (next < AUDIOBUFSIZE/2)
	  bzero(Audiobuf, AUDIOBUFSIZE/2);
	f_read(&fid, Audiobuf, next, &ret);
	hd.cksize -= ret;
	audioplayerHalf = 0;
      }
      if (audioplayerWhole) {
	if (next < AUDIOBUFSIZE/2)
	  bzero(&Audiobuf[AUDIOBUFSIZE/2], AUDIOBUFSIZE/2);
	f_read(&fid, &Audiobuf[AUDIOBUFSIZE/2], next, &ret);
	hd.cksize -= ret;
	audioplayerWhole = 0;
      }
    }
    audioplayerStop();
  }
  
    //printf("\nClose the file.\n");
  rc = f_close(&fid);
}

#endif
char h_roll(){
    //r code:
    //0: player attacks
    //1: monster attacks
    //2: crit: 2 damage to monster
    
    char i = rand()%7;
    
    char* wit;
    
    char r;
    
    char proll[14];
    char mroll[13];
    char* result;
    
    char pr1 = rand()%6+1;
    char pr2 = rand()%6+1;
    char mr= (rand()%MONSTERROLLMAX) + 1;
    
    if((pr1+pr2)>mr){
        result="you attack!";
        r = 0;
    }
    else if((pr1+pr2)<mr){
        result="you're hit!";
        r = 1;
    }
    else if((pr1+pr2)==mr){
      result="CRITICAL ATTACK!";
        r = 2;
    }
    
    sprintf(proll, "you rolled %d!", pr1+pr2);
    #if DBG_HUD
    dbg = "RL RD1";
    f3d_lcd_drawString(90,0,dbg, GREEN, BLACK);
    #endif

    sprintf(mroll, "it rolled %d!", mr);

    #if DBG_HUD
    dbg = "RL RD2";
    f3d_lcd_drawString(90,0,dbg, GREEN, BLACK);
    #endif

    switch (i) {
        case 0:
	  wit="let 'em go!";
            break;
        
        case 1:
	  wit="let 'er rip!";
            break;
        
        case 2:
	  wit="good luck!";
            break;
            
        case 3:
	  wit="roll!";
            break;
            
        case 4:
	  wit="oh god!!";
            break;
            
        case 5:
	  wit="watch out!";
            break;
            
        case 6:
	  wit="not afraid!";
            break;
            
        default:
            break;
    }
    #if DBG_HUD
    f3d_lcd_drawString(90,0,"RL IDL", GREEN, BLACK);
    #endif
    f3d_nunchuk_read(&nunchuk_data);
    while(!nunchuk_data.c){
      f3d_nunchuk_read(&nunchuk_data);
      delay(50);
    }
    #if DBG_HUD
    f3d_lcd_drawString(90,0,"RL BGN", GREEN, BLACK);
    #endif
    g_dice_midroll(wit);
    while(nunchuk_data.c){
      #if SHAKETOROLL
      while(nunchuk_data.ax<ROLLFORCE || nunchuk_data.ay<ROLLFORCE || nunchuk_data.az<ROLLFORCE){
	f3d_nunchuk_read(&nunchuk_data);
	delay(50);
      }	
      #endif
      #if AUDIO
      playWavFile("roll.wav");
      #endif
      f3d_nunchuk_read(&nunchuk_data);
	delay(50);
    }
    #if DBG_HUD
    f3d_lcd_drawString(90,0,"RL END", GREEN, BLACK);
    #endif
    g_dice_postroll(pr1, pr2);
    g_update_status(proll, mroll, result);
    #if AUDIO
    playWavFile("hit.wav");
    #endif
    delay(500);
    return r;
}

//SOUNDS REQUIRED:
//roll.wav
//hit.wav
//scream.wav
//sphere.wav
//cube.wav
//pyramid.wav
//twin.wav
void h_game(){
  char roll, matk;
    char  mhp;
    char p_hp = PLAYERHP;
    char* mon_name;
    
    //start!
    g_draw_start_screen();
    f3d_nunchuk_read(&nunchuk_data);
    //hover on start screen
    while(!nunchuk_data.c){
      f3d_nunchuk_read(&nunchuk_data);
      delay(50);
    }    
    g_draw_borders();
    
    //CHAOS SPHERE
    g_update_status("CHAOS", "SPHERE", "appeared!");
    mon_name="CHS SPHR";
    mhp = SPHEREHP;
    matk = SPHEREATK;
    g_new_monster(1, p_hp, mhp, matk, mon_name);
    #if AUDIO
    playWavFile("sphere.wav");
    #endif
    while(p_hp<240||mhp<240){
        #if DBG_HUD
        f3d_lcd_drawString(90,0,"RL SRT", GREEN, BLACK);
        #endif
        roll = h_roll();
        if(!roll) mhp--;
        if(roll==1)p_hp = p_hp-matk;
        if(roll==2)mhp-2;
	if(p_hp>240 || mhp>240) break;
        g_update_turn(p_hp, mhp, matk, mon_name);
    }
    if(p_hp>240){
        #if AUDIO
        playWavFile("scream.wav");
	#endif
        g_draw_game_over();
        return;
    }
    g_update_status("you win!", "it's dead!", "ORDER restored!");
    delay(2000);
    
    //SHADOW CUBE
    g_update_status("SHADOW", "CUBE", "appeared!");
    mon_name="SHDW QB";
    mhp = CUBEHP;
    matk = CUBEATK;
    g_new_monster(2, p_hp, mhp, matk, mon_name);
    #if AUDIO
    playWavFile("cube.wav");
    #endif
    while(p_hp<240||mhp<240){
        roll = h_roll();
        if(!roll) mhp--;
        if(roll==1)p_hp = p_hp-matk;
        if(roll==2)mhp-2;
	if(p_hp>240 || mhp>240) break;
        g_update_turn(p_hp, mhp, matk, mon_name);
    }
    if(p_hp>240){
        #if AUDIO
        playWavFile("scream.wav");
	#endif
        g_draw_game_over();
        return;
    }
    g_update_status("you win!", "it's dead!", "LIGHT restored!");
    delay(2000);

    //LIGHTNING PYRAMID
    g_update_status("LIGHTNING", "PYRAMID", "appeared!");
    mon_name="LNG PMD";
    mhp = PYRHP;
    matk = PYRATK;
    g_new_monster(3, p_hp, mhp, matk, mon_name);
    #if AUDIO
    playWavFile("pyramid.wav");
    #endif
    while(p_hp<240||mhp<240){
        roll = h_roll();
        if(!roll) mhp--;
        if(roll==1)p_hp= p_hp-matk;
        if(roll==2)mhp-2;
	if(p_hp>240 || mhp>240) break;
        g_update_turn(p_hp, mhp, matk, mon_name);
    }
    if(p_hp>240){
        #if AUDIO
        playWavFile("scream.wav");
	#endif
        g_draw_game_over();
        return;
    }
    g_update_status("you killed!", "the monster!", "GROUND restored!");
    delay(2000);
    
    //TWIN DRAGON
    g_update_status("TWIN", "DRAGON", "appeared!");
    mon_name="TWN DRGN";
    mhp = TWINHP;
    matk = TWINHP;
    g_new_monster(4, p_hp, mhp, matk, mon_name);
    #if AUDIO
    playWavFile("twin.wav");
    #endif
    while(p_hp<240||mhp<240){
        roll = h_roll();
        if(!roll) mhp--;
        if(roll==1)p_hp= p_hp-matk;
        if(roll==2)mhp-2;
	if(p_hp>240 || mhp>240) break;
        g_update_turn(p_hp, mhp, matk, mon_name);
    }
    if(p_hp>240){
        #if AUDIO
        playWavFile("scream.wav");
	#endif
        g_draw_game_over();
        return;
    }
    g_update_status("you killed!", "everything!", "victory!");
    delay(2000);
    
}

int main(void) { 
  #if CONSERVEMEM
  //FRESULT rc;			/* Result code */
  DIR dir;			/* Directory object */
  FILINFO fno;			/* File information object */
  UINT bw, br;
  unsigned int retval, cursor;
  int bytesread;
  #endif

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  f3d_delay_init();
  delay(10);
  f3d_rtc_init();
  delay(10);
  f3d_systick_init();
  delay(10);
  f3d_uart_init();
  delay(10);
  f3d_i2c1_init();
  //printf("I2C init... \n");       
  delay(10);
  f3d_gyro_init();
  delay(10);
  f3d_nunchuk_init();
  delay(100);
  f3d_accel_init();
  delay(10);
  f3d_lcd_init();
  delay(10);
  f3d_timer2_init();
  delay(10);
  f3d_dac_init();
  delay(10);
  f3d_button_init();
  
  #if CONSERVEMEM
  f_mount(0, &Fatfs);/* Register volume work area */
  #endif
  f3d_lcd_fillScreen(RED);
  delay(1000);
  //while(1);
  while (1){
    h_game();

  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1){
    
  }
}
#endif


/* main.c ends here */
