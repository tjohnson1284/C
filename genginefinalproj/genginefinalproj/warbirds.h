/////////////////////////////////////////////////////////
// Advanced Warbirds based on the Game Engine book 
//
/////////////////////////////////////////////////////////

#ifndef _WARBIRDS_H
#define _WARBIRDS_H

#include "allegro.h"
#include "mappyal.h"

//this must run at 640x480
#define MODE GFX_SAFE
#define WIDTH 640
#define HEIGHT 480

#define WHITE makecol(255,255,255)
#define GRAY makecol(60,60,60)
#define RED makecol(200,0,0)

#define MAX_ENEMIES 10
#define MAX_BULLETS 10
#define MAX_EXPLOSIONS 10
#define BOTTOM 4800 - HEIGHT

//define the sprite structure
typedef struct SPRITE
{
    int dir, alive;
    int x,y;
    int width,height;
    int xspeed,yspeed;
    int xdelay,ydelay;
    int xcount,ycount;
    int curframe,maxframe,animdir;
    int framecount,framedelay;
}SPRITE;

//y offset in the game world.
int yoffset = BOTTOM;

//player variables
int firecount = 0;
int firedelay = 120;  // To adjust time gap between bullets.
int health = 25;
int score = 0;

//timer variables
volatile int counter;
volatile int ticks;
volatile int framerate;

//bitmaps and sprites
BITMAP *buffer;	
BITMAP *temp;
BITMAP *explosion_images[6];
SPRITE *explosions[MAX_EXPLOSIONS];
BITMAP *bigexp_images[7];
SPRITE *bigexp;
BITMAP *player_images[3];                                  //change this to change plane animations
SPRITE *player;
BITMAP *bullet_images[3];
SPRITE *bullets[MAX_BULLETS];
BITMAP *enemy_plane_images[3];
SPRITE *enemy_planes[MAX_ENEMIES];
BITMAP *progress, *bar;
BITMAP *bonus_shot_image;
SPRITE *bonus_shot;

#endif
