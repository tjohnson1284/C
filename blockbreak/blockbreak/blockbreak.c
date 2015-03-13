/*
*	Programmer:	Terrence Johnson
*	Date:	October 3rd, 2013
*   Due Date: October 4th, 2013 @ 23:55
*
Problem: Write a C program called blockbreak.c in Allegro that implements a conventional block breaking game. 
Player has three balls/tries to break all five blocks. If he loses his three tries, game ends. If he breaks all five blocks, game is won
Moving the paddle is done with the mouse instead of keys.

*/




#include <allegro.h>
#define BLUE makecol(102,191,250)
#define WHITE makecol(255,255,255)
#define BLACK makecol(0,0,0)
#define RED makecol (255,0,0)

// Struct to control the breakable blocks and their current condition
struct BLOCKS
{
	int x, x2, y;
	int clear;
    int color;
};

int i, score = 0, tries = 3;
struct BLOCKS test[5];

// Struct to control the paddle and it's current position
struct LargePADDLE
{
	int x, x2, y, y2;
	int posx;
} paddle;
// Struct to control the ball and it's current position
struct largeBALL
{
	int posx, posy, dirx, diry;
} ball;

// Function that implements the score area
void ScoreArea()
{
	textprintf_ex(screen, font, 0, 45, WHITE, 0, "Score: %d",score);
	if(score < 5)
	{
		textprintf_ex(screen, font, 0, 45, WHITE, 0, "Score: %d",score);
	}
	else
		textprintf_ex(screen, font, 0, 45, WHITE, 0, "Score: %d YOU WIN!!!", score);
	hline(screen, 0, 58, SCREEN_W,WHITE);

}
//Function that switches the blocks to the "ON" position
void blocksavailable()
{
	int n;
	for(n = 0 ; n < 5; n++)
		test[n].clear = 1;
	
}
//Function to erase the paddle
void erasePADDLE()
{
	rectfill(screen, paddle.x, paddle.y, paddle.x2, paddle.y2, BLACK);
}
//Function to move the paddle
void movePADDLE()
{
	erasePADDLE();
	paddle.x = mouse_x;
	paddle.x2 = mouse_x + 130;
	rectfill(screen, paddle.x,paddle.y, paddle.x2,paddle.y2, RED);
}
//Function to draw the blocks
void drawblocks()
{
	int save = 100, count = 0;

	
	while(count < 5)
	{
		test[count].y = 60;
		if(test[count].clear == 1 && count == 0)	
		{	
			test[count].x = 0;
			test[count].x2 = save;
			rectfill(screen, test[count].x, 60, test[count].x2, 80, BLUE);			
		}
	
		else if(test[count].clear == 1 && count > 0)	
		{
			test[count].x = test[count-1].x2 + 30;
			test[count].x2 = test[count].x + 100;
			rectfill(screen, test[count].x, 60, test[count].x2, 80, BLUE);					
		}
		count ++;
	}
	
}
//Function that erases blocks based on the struct's clear flag
void eraseblock()
{
	int count = 0;

	
	while(count < 5)
	{
		if(test[count].clear == 0)
		{
			rectfill(screen, test[count].x, 60, test[count].x2, 80, BLACK);
		}
		count ++;
	}
}
//Function that resets the ball after a try is lost
void ResetBall()
{
	ball.posx = 320;
	ball.posy = 300;
	ball.dirx = 1;
	ball.diry = 2;
}

//Function that controls overall ball movement
void ballcontrol()
{
	
	int i;
	circlefill(screen, ball.posx, ball.posy, 20, BLACK);
	ball.posx += ball.dirx;
	
	// Makes sure ball doesn't go off the left side of the screen
	if (ball.posx - 20 < 0 )
	{
		ball.posx = 21;
		ball.dirx = rand() % 2 + 4;
	}
	// Makes sure the ball doesn't go off the right side of the screen
	if (ball.posx + 20 > SCREEN_W)
    {
		ball.posx = SCREEN_W - 25;
        ball.dirx = rand() % 2 - 6;
    }
	
	ball.posy += ball.diry;
	
	// Makes sure the ball doesn't go over the border at the top
	if(ball.posy - 20 < 59)
	{
		ball.posy = 79;
		ball.diry = rand() % 2 + 4;
	}
	
	// Monitors if the ball leaves the bottom off the screen and deducts tries
	if(ball.posy - 20  > SCREEN_H)
	{
		tries--;
		ResetBall();
	}
	
	// Checks for collision with blocks, if collision occurs, toggles block's clear flag
	for( i = 0 ; i < 5 ; i ++)
	{
		if(ball.posx - 20 > test[i].x && ball.posx < test[i].x2  && ball.posy + 20 > test[i].y 
			&& ball.posy - 20 < test[i].y +20 && test[i].clear == 1)
		{
			ball.posy = test[i].y + 80;
			ball.diry = rand() % 2 + 4;
			test[i].clear = 0;
			score += 1;
			
		}
	}
	// Checks for collision with paddle
	if(ball.posx - 20 > paddle.x && ball.posx < paddle.x2 && ball.posy + 20 > paddle.y
		&& ball.posy -20 < paddle.y2)
	{
		ball.posy = paddle.y - 21;
		ball.diry = rand() % 2 - 6;
	}
	circlefill(screen, ball.posx, ball.posy, 20, WHITE);
}
int main()
{
	
	allegro_init();
	install_keyboard();
	install_timer();
	install_mouse();
	srand(time(NULL));
	set_gfx_mode(GFX_SAFE, 640, 480, 0, 0);
	
	blocksavailable();
	paddle.x = 300;
	paddle.x2 = 350;
	paddle.y = 450;
	paddle.y2 = 480;
	

	ResetBall();
	while(!key[KEY_ESC] && tries > 0 && score < 5)
	{
		ScoreArea();
		drawblocks();
		movePADDLE();
		ballcontrol();		
		eraseblock();		
		rest(15);
	}
	if(score == 5)
	{
		ScoreArea();
		rest(1500);
	}
	allegro_exit();
    return 0;
}

END_OF_MAIN()