#include <allegro.h>


int i, score = 0, tries = 3;
// Struct to control the paddle and it's current position
#define WHITE makecol(255,255,255)
#define RED makecol (255,0,0)
struct LargePADDLE
{
	int x, x2, y, y2;
	int posx;
	int speed;
} paddle;
// Struct to control the ball and it's current position
struct largeBALL
{
	int posx, posy, dirx, diry;
	int speed;
} ball;

// Function that implements the score area
void ScoreArea()
{
	BITMAP *topborder, *bottomborder, *sideborder;
	int x, y;
	topborder = load_bitmap("borderstb.bmp", NULL);
    x = 5 ;
    y = 60;
	bottomborder = load_bitmap("borderstb.bmp", NULL);
	sideborder = load_bitmap("borderside.bmp", NULL);

	textprintf_ex(screen, font, 0, 45, WHITE, 0, "Score: %d",score);
	draw_sprite(screen, topborder, x, y);
	draw_sprite(screen, bottomborder,5, SCREEN_H - 10);
	draw_sprite(screen, sideborder, 0, 60);
        
	
	textprintf_ex(screen, font, 0, 45, WHITE, 0, "Score: %d",score);
	
	
	hline(screen, 0, 58, SCREEN_W,WHITE);

	

        
}

//Function to move the paddle
void movePADDLE()
{
	BITMAP *paddle2;
	paddle2 = load_bitmap("paddle.bmp", NULL);
	rectfill(screen, paddle.x, paddle.y, paddle.x + paddle2 -> w , paddle.y2 + paddle2 -> h, 0);
	paddle.x2 = paddle.x + paddle2 -> w;
	paddle.y2 = paddle.y + paddle2 -> h;
	paddle.speed = 10;

	acquire_screen();
	if(ball.posy > paddle.y && ball.posx > 100)
	{
		if(paddle.y2 > SCREEN_H)
		{
			paddle.y2 = SCREEN_H;
		}
		else
			paddle.y += paddle.speed;
		
	}
	else if(ball.posy < paddle.y && ball.posx > 100)
	{
		if(paddle.y < 60)
		{
			paddle.y = 60;
		}
		paddle.y -= paddle.speed;
		
	}
	draw_sprite(screen, paddle2, paddle.x,paddle.y);
	release_screen();
}
//function to reset the ball
void ResetBall()
{
	ball.posx = 320;
	ball.posy = 300;
	ball.dirx = 1;
	ball.diry = 2;
	ball.speed= 0;
}

//Function that controls overall ball movement
void ballcontrol()
{
	BITMAP *ball2;
	

	ball2 = load_bitmap("ball.bmp", NULL);
	rectfill(screen, ball.posx, ball.posy, ball.posx + ball2->w, ball.posy+ball2->h, 0);
	ball.posx += ball.dirx;
	
	// Makes sure ball doesn't go off the left side of the screen
	if (ball.posx + ball2->w < 5 +40 )
	{
		ball.posx = 6 + 39;
		ball.dirx = rand() % 2 + 4 + ball.speed;
	}
	// Makes sure the ball doesn't go off the right side of the screen
	if (ball.posx + ball2->w > SCREEN_W)
	{
		tries--;
		ResetBall();
    }
	
	ball.posy += ball.diry;
	
	// Makes sure the ball doesn't go over the border at the top
	if(ball.posy  < 60 +10)
	{
		ball.posy = 79;
		ball.diry = rand() % 2 + 4 + ball.speed;
	}
	
	// Monitors if the ball leaves the bottom off the screen and deducts tries
	if(ball.posy - ball2->h  > SCREEN_H-95)
	{	
		
		ball.posy = SCREEN_H - 95;
        ball.diry = rand() % 2 - 4 -ball.speed;
	}
	
	// Checks for collision with paddle
	if(ball.posx + ball2->w  > paddle.x && ball.posy + ball2->h -10 > paddle.y - 20
		&& ball.posy - ball2->h   < paddle.y2 -50)
	{
		ball.posx = paddle.x - 71;
		ball.dirx = rand() % 2 - 6 -ball.speed;
		score +=1;
		ball.speed += 1;
		
	}
	draw_sprite(screen, ball2, ball.posx, ball.posy);
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

	
	paddle.x = SCREEN_W - 50;
	
	ResetBall();
    while (!key[KEY_ESC] && tries > 0)
    {
		acquire_screen();
		ScoreArea();
		ballcontrol();
		movePADDLE();
		release_screen();
        rest(20);

    }
	if(tries == 0)
	{
		textprintf_ex(screen, font, SCREEN_W/2, SCREEN_H/2, RED, 0, "GAME OVER!!!", score);
		rest(1500);
	}

    allegro_exit();
    return 0;
}
END_OF_MAIN()