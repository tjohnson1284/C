/*
*	Programmer:	Terrence Johnson
*	Date:	November 1st, 2013
*   Due Date: November 3rd, 2013 @ 23:55
*
Write a C program called
touch_ball.c
that implements a tank sprite (or similar one) that tries to touch as many balls (= max: 5 balls) as 
possible in the time constrain (= 30 seconds). In the game, the tank sprite can move only four directions (up/d
own/right/left). Once the tank touches the ball sprite, the ball should disappear. A player can play the game only 30 seconds. 
Your program should display the remaining time and score. If the player touches all five balls, the program should stop at that time. 
Sound Effect: http://www.freesound.org/people/soundnimja/sounds/173328/
*/

#include <allegro.h>
#define WHITE makecol(255,255,255)
#define RED makecol (255,0,0)
#define BLACK makecol (0,0,0)
#define MAXSPEED 5
int i, score = 0, gameover = 0;
volatile int sec = 30;
BITMAP *tank_bmp[3];
// Struct to control the paddle and it's current position
void inc_sec(void)
{
   sec--;
   //CurrentTime = CurrentTime - 1;
}
struct Tank
{
    int x,y, x2, y2;
    int dir,speed;
} tank;
// Struct to control the ball and it's current position
struct largeBALL
{
	int posx, posy, dirx, diry;
	int clear;
} ball[5];

// Function that implements the score area
void ScoreArea()
{
	
	textprintf_ex(screen, font, 0, 35, WHITE, 0, "Score: %d",score);
	
	if(sec >= 10)
	{
		textprintf_ex(screen, font, 0, 45, WHITE, 0, "Time Remaining: %d", sec);
	}
	else
	{
		textprintf_ex(screen, font, 0, 45, BLACK, 0, "Time Remaining: %d", 10);
		textprintf_ex(screen, font, 0, 45, WHITE, 0, "Time Remaining: %d", sec);
	}
	hline(screen, 0, 58, SCREEN_W,WHITE);       
}
void drawtank()
{
    int dir = tank.dir;
    int x = tank.x-15;
    int y = tank.y-15;
    draw_sprite(screen, tank_bmp[dir], x, y);
}
void erasetank()
{
    int x = tank.x-17;
    int y = tank.y-17;
    rectfill(screen, x, y, x+33, y+33, BLACK);
}
void movetank(){
    int dir = tank.dir;
    int speed = tank.speed;

    //update tank position based on direction
    switch(dir)
    {
        case 0:
            tank.y -= speed;
            break;
        case 1:
            tank.x += speed;
            break;
        case 2:
            tank.y += speed;
            break;
        case 3:
            tank.x -= speed;
            break;
       /* case 4:
            tank.y += speed;
            break;
        case 5:
            tank.x -= speed;
            tank.y += speed;
            break;
        case 6:
            tank.x -= speed;
            break;
        case 7:
            tank.x -= speed;
            tank.y -= speed;
            break;*/
    }

    //keep tank inside the screen
    if (tank.x > SCREEN_W-22)
    {
        tank.x = SCREEN_W-22;
        tank.speed = 0;
    }
    if (tank.x < 22)
    {
        tank.x = 22;
        tank.speed = 0;
    }
    if (tank.y > SCREEN_H-22)
    {
        tank.y = SCREEN_H-22;
        tank.speed = 0;
    }
    if (tank.y < 77)
    {
        tank.y = 77;
        tank.speed = 0;
    }
}
void forward()
{
    tank.speed++;
    if (tank.speed > MAXSPEED)
        tank.speed = MAXSPEED;
}
void backward()
{
    tank.speed--;
    if (tank.speed < -MAXSPEED)
        tank.speed = -MAXSPEED;
}
void turnright()
{
    tank.dir++;
    if (tank.dir > 3)
        tank.dir = 0;
}
void turnleft()
{
    tank.dir--;
    if (tank.dir < 0)
        tank.dir = 3;
}
void getinput()
{
    //hit ESC to quit
    if (key[KEY_ESC])   gameover = 1;
    
    //WASD - SPACE keys control tank 1
    if (key[KEY_W])     forward(0);
    if (key[KEY_D])     turnright(0);
    if (key[KEY_A])     turnleft(0);
    if (key[KEY_S])     backward(0);

    //clear buffer after one keypress 
	clear_keybuf();
}
void setuptanks()
{
    int n;
    
    
    tank.x = SCREEN_W/2;
    tank.y = SCREEN_H/2;
    tank.speed = 0;
    tank.dir = 3;

    
    tank_bmp[0] = load_bitmap("tank1.bmp", NULL);
    

    for (n=1; n<4; n++)
    {
        tank_bmp[n] = create_bitmap(32, 32);
        clear_bitmap(tank_bmp[n]);
        rotate_sprite(tank_bmp[n], tank_bmp[0], 
            0, 0, itofix(n*64));
    }
}
//function to reset the ball
void ResetBall()
{
	int i; 

	for(i = 0 ; i < 5 ; i++)
	{
		ball[i].posx = rand() % 600;
		ball[i].posy = rand() % 400;
		ball[i].dirx = rand() % 2 + 4;
		ball[i].diry = rand() % 2 + 4;
		ball[i].clear= 0;
	}
}

//Function that controls overall ball movement
void ballcontrol(int n)
{
	BITMAP *ball2;
	SAMPLE *blip;
	int panning = 128;
    int pitch = 1000;
    int volume = 255;

	blip = load_sample("173328__soundnimja__blip-1.wav");

	ball2 = load_bitmap("ball.bmp", NULL);
	rectfill(screen, ball[n].posx, ball[n].posy, ball[n].posx + ball2->w, ball[n].posy+ball2->h, 0);
	ball[n].posx += ball[n].dirx;
	if(ball[n].clear == 0)
	{
		// Makes sure ball doesn't go off the left side of the screen
		if (ball[n].posx + ball2->w < 40 )
		{
			ball[n].posx = 6;
			ball[n].dirx = rand() % 2 + 4;
		}
		// Makes sure the ball doesn't go off the right side of the screen
		if (ball[n].posx + ball2->w > SCREEN_W)
		{
			ball[n].posx = SCREEN_W - 50;
			ball[n].dirx = rand() % 2 - 6;
		}

		ball[n].posy += ball[n].diry;

		// Makes sure the ball doesn't go over the border at the top
		if(ball[n].posy  < 60)
		{
			ball[n].posy = 79;
			ball[n].diry = rand() % 2 + 4;
		}

		// Monitors if the ball leaves the bottom off the screen and deducts tries
		if(ball[n].posy - ball2->h  > SCREEN_H-85)
		{	

			ball[n].posy = SCREEN_H - 40;
			ball[n].diry = rand() % 2 - 4;
		}

		// Checks for collision with Tank
		if(ball[n].posx + 10  > tank.x - 5 && ball[n].posx - 10 < tank.x + 5 && 
			ball[n].posy + 40 > tank.y - 20   && ball[n].posy - 40  < tank.y + 20  && ball[n].clear != 1 )
		{
			
			score +=1;
			ball[n].clear = 1;
			play_sample(blip, volume, panning, pitch, FALSE);
			ball[n].posx = SCREEN_W + 500;
			ball[n].posy = SCREEN_H + 500;
			if(score == 5)
			{
				gameover = 1;
			}
			

		}
		if(ball[n].clear == 0)
		{
			draw_sprite(screen, ball2, ball[n].posx, ball[n].posy);
		}
	}
}
int main() 
{
    
    int i = 0;

    allegro_init();
    install_keyboard();
	install_timer();
	install_mouse(); 
	srand(time(NULL));
    set_color_depth(24); 
    set_gfx_mode(GFX_SAFE, 640, 480, 0, 0);
	LOCK_VARIABLE(sec);
	LOCK_FUNCTION(inc_sec);
	
	install_int(inc_sec, 1000);
	if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, "") != 0) 
    {
        allegro_message("Error initializing sound system");
        return 1;
    }
	setuptanks();
	ResetBall();
    while (!gameover && sec >= 0)
    {
		acquire_screen();
		ScoreArea();
		erasetank();
		movetank();
		drawtank();
		for(i = 0; i < 5; i++)
		{
			ballcontrol(i);
		}
		if (keypressed())
            getinput();
		release_screen();
        rest(40);

    }
	if(sec < 0)
	{
		textprintf_ex(screen, font, SCREEN_W/2, SCREEN_H/2, RED, 0, "GAME OVER!!!");
		rest(5000);
	}
	else if(score == 5)
	{
		textprintf_ex(screen, font, SCREEN_W/2, SCREEN_H/2, WHITE, 0, "You Win!");
		rest(5000);
	}

    allegro_exit();
    return 0;
}
END_OF_MAIN()