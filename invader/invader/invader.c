/*
*	Programmer:	Terrence Johnson
*	Date:	October 17th, 2013
*   Due Date: October 18th, 2013 @ 23:55
*
Problem: Develop a C program called invader.c that implements a simplified invader game. There are three invaders. You will need to add background music
and sound effects for a missile hit. 

Controls
Mouse to move, left click to fire. 

Sound effects credit
Explosion: http://www.freesound.org/people/fridobeck/sounds/191694/
Laser: http://www.freesound.org/people/jobro/sounds/35686/
Music: Created myself

*/




#include <allegro.h>
#define BLUE makecol(102,191,250)
#define WHITE makecol(255,255,255)
#define BLACK makecol(0,0,0)
#define RED makecol (255,0,0)
#define GREEN makecol(0,255,0)
#define BULLETSPEED 25

// Struct to control the breakable INVADERS and their current condition

struct INVADERS
{
	int x, x2, y, y2, turretx, turretx2, turrety,turrety2;
	int clear;
    int color;
	int count;
	int dir;
	int contact;
	int bulletx, bullety, alive, bulletxspd,bulletyspd;

}invade[3];

int i, score = 0, endgame = 0;


// Struct to control the defense and it's current position
struct DEFENDER
{
	int x, x2, y, y2, turretx, turretx2, turrety,turrety2;
	int bulletx, bullety, alive, bulletxspd,bulletyspd;
	int posx;
	int clear; 
} defense;

// Function that implements the score area
void ScoreArea()
{
	textprintf_ex(screen, font, 0, 45, WHITE, 0, "Score: %d",score);
	if(score < 3)
	{
		textprintf_ex(screen, font, 0, 45, WHITE, 0, "Score: %d",score);
	}
	else
		textprintf_ex(screen, font, 0, 45, WHITE, 0, "Score: %d YOU WIN!!!", score);
	hline(screen, 0, 58, SCREEN_W,WHITE);

}
//Function that switches the INVADERS to the "ON" position
void INVADERSavailable()
{
	int n;
	for(n = 0 ; n < 3; n++)
		invade[n].clear = 1;
	
}
//Function to erase the defense 
void erasedefense()
{
	rectfill(screen, defense.x, defense.y, defense.x2, defense.y2, BLACK);
	rectfill(screen, defense.turretx, defense.turrety, defense.turretx2, defense.turrety2, BLACK);
}
//Function to move the defense 
void movedefense()
{
	erasedefense();
	defense.x = mouse_x;
	defense.x2 = mouse_x + 80;
	if(defense.x2 > SCREEN_W)
	{
		defense.x = SCREEN_W;
		defense.x2 = SCREEN_W - 80;
	}
	defense.turretx = ((defense.x+defense.x2)/2)-4;
	defense.turretx2 = ((defense.x+defense.x2)/2)+4;
	rectfill(screen, defense.x,defense.y, defense.x2,defense.y2, RED);
	rectfill(screen, defense.turretx, defense.turrety, defense.turretx2, defense.turrety2, RED); 
}
//Function to initialize invaders 
void invaderinit()
{
	//int x = 300, x2 = 360, y = 60, y2 = 90, inc = 0;
	int x = 0, x2 = 60, y = 60, y2 = 90, inc = 0;
	INVADERSavailable();
		while(inc < 3 && invade[inc].clear == 1 )
		{	
			invade[inc].x = x;
			invade[inc].x2 = x2;
			invade[inc].y = y;
			invade[inc].y2 = y2;
			invade[inc].turretx = ((invade[inc].x+invade[inc].x2)/2)-4;
			invade[inc].turretx2 = ((invade[inc].x+invade[inc].x2)/2)+4;
			invade[inc].turrety = invade[inc].y2 - 1;
			invade[inc].turrety2 = invade[inc].y2 + 11;
			invade[inc].dir = 3;
			invade[inc].contact = 0;
			x = x2 + 20;
			x2 = x2 + 80;
			inc +=1;
		}
}
//function to draw the invaders 
void drawINVADERS()
{
	int inc = 0;

	while(inc < 3 )
	{	
		if(invade[inc].clear == 1)
		{
			rectfill(screen, invade[inc].x , invade[inc].y, invade[inc].x2, invade[inc].y2, WHITE);
			rectfill(screen, invade[inc].turretx, invade[inc].turrety, invade[inc].turretx2, invade[inc].turrety2, WHITE);
		}
		inc +=1;
	}
	
}
//Function to erase the invaders 
void eraseblock()
{
	int count = 0;

	
	while(count < 3)
	{
		rectfill(screen, invade[count].x, invade[count].y, invade[count].x2, invade[count].y2, BLACK);
		rectfill(screen, invade[count].turretx, invade[count].turrety, invade[count].turretx2, invade[count].turrety2, BLACK);		
		count ++;
	}
}
//Function to fire update invader bullets 
void updateinvaderbullet(int num)
{
	SAMPLE *explode;
	
	int panning = 128;
    int pitch = 1000;
    int volume = 255;
	int x = invade[num].bulletx;
    int y = invade[num].bullety;


	explode = load_sample("explosion4.wav");

    if (invade[num].alive)
    {
        //erase bullet
        rect(screen, x-1, y-1, x+1, y+1, BLACK);        

        //move bullet       
        invade[num].bullety += invade[num].bulletyspd;
        x = invade[num].bulletx;
        y = invade[num].bullety;

        //stay within the screen
        if (x < 5 || x > SCREEN_W-5 || y < 20 || y > SCREEN_H-5)
        {
            invade[num].alive = 0;
            return;
        }
		
        //draw bullet
        x = invade[num].bulletx;
        y = invade[num].bullety;
        rect(screen, x-1, y-1, x+1, y+1, GREEN);    
		if(x > defense.x && x < defense.x2 && y > defense.y && y < defense.y2)
		{
			play_sample(explode, volume, panning, pitch, FALSE);
			endgame = 1;
		}

    }
}
//Function to fire invader weapon 
void fireinvaderweapon(int num)
{
	SAMPLE *laser;
	
	int panning = 128;
    int pitch = 1000;
    int volume = 20;
	int x = (invade[num].turretx + invade[num].turretx2)/2 ;
	int y = invade[num].turrety2;

	laser = load_sample("laser9.wav");
    //ready to fire again?
	if(invade[num].clear == 1)
	{
		if (!invade[num].alive)
		{
			invade[num].alive = 1;
      		invade[num].bulletx = x;
			invade[num].bullety = y + 20;
			invade[num].bulletxspd = 0;
			invade[num].bulletyspd = BULLETSPEED;   
			play_sample(laser, volume, panning, pitch, FALSE);
		}
	}
}
//function to move the invaders 
void MoveInvader()
{
	int inc = 0, test = 0;
	while(inc < 3)
	{
		if(invade[inc].dir == 1)
		{
			
			invade[inc].x -= 5;
			invade[inc].x2 -= 5;
			invade[inc].turretx = (((invade[inc].x+invade[inc].x2)/2)-4);
			invade[inc].turretx2 = (((invade[inc].x+invade[inc].x2)/2)+4);
			if(invade[inc].x <= 0)
			{
				invade[inc].contact +=1;
				if(invade[inc].contact == 3)
				{
					endgame = 1;
					break;
				}
				invade[inc].dir = 3;
				invade[inc].y += 100;
				invade[inc].y2 += 100;
				invade[inc].turrety +=100;
				invade[inc].turrety2 +=100;
				
			}
		}
		else if(invade[inc].dir == 3)
		{
			invade[inc].x += 5;
			invade[inc].x2 += 5;
			invade[inc].turretx = (((invade[inc].x+invade[inc].x2)/2)-4);
			invade[inc].turretx2 = (((invade[inc].x+invade[inc].x2)/2)+4);
			if(invade[inc].x > SCREEN_W - 55)
			{
				invade[inc].contact +=1;
				if(invade[inc].contact == 3)
				{
					endgame = 1;
					break;
				}
				invade[inc].dir = 1;
				invade[inc].y += 100;
				invade[inc].y2 += 100;
				invade[inc].turrety +=100;
				invade[inc].turrety2 +=100;
				
			}

		}
		
		inc ++;
		
	}
	
}
//function to update player bullets
void updatedefensebullet()
{
	SAMPLE *explode;
	
	int panning = 128;
    int pitch = 1000;
    int volume = 255;
	int x = defense.bulletx;
    int y = defense.bullety;
	int inc = 0;

	explode = load_sample("explosion4.wav");
    if (defense.alive)
    {
        //erase bullet
        rect(screen, x-1, y-1, x+1, y+1, BLACK);        

        //move bullet       
        defense.bullety -= defense.bulletyspd;
        x = defense.bulletx;
        y = defense.bullety;

        //stay within the screen
        if (x < 5 || x > SCREEN_W-5 || y < 58 || y > SCREEN_H - 5)
        {
            defense.alive = 0;
            return;
        }
		
        //draw bullet
        x = defense.bulletx;
        y = defense.bullety;
        rect(screen, x-1, y-1, x+1, y+1, GREEN);    
		for(inc = 0; inc < 3; inc++)
		{
			if(x >= invade[inc].x && x <= invade[inc].x2 && y > invade[inc].y && y < invade[inc].y2 && invade[inc].clear != 0)
			{
				play_sample(explode, volume, panning, pitch, FALSE);
				score += 1;
				invade[inc].clear = 0;	
			}
			
			
			
				
		}
    }

}
//function to fire player bullets
void defensecannon()
{
	SAMPLE *laser;
	
	int panning = 128;
    int pitch = 1000;
    int volume = 40;

	int x = (defense.turretx + defense.turretx2)/2 ;
	int y = defense.turrety2;

	laser = load_sample("laser9.wav");

    //ready to fire again?
    if (!defense.alive)
    {
        defense.alive = 1;
      	defense.bulletx = x;
        defense.bullety = y - 20;
        defense.bulletxspd = 0;
        defense.bulletyspd = BULLETSPEED;
		play_sample(laser, volume, panning, pitch, FALSE);
    }
}


int main()
{
	MIDI *music;

	int test = 0;
	

	allegro_init();
	install_keyboard();
	install_timer();
	install_mouse();
	srand(time(NULL));
	set_gfx_mode(GFX_SAFE, 640, 480, 0, 0);
	

	if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, "") != 0) 
    {
        allegro_message("Error initializing sound system");
        return 1;
    }
	
	music = load_midi("project1.mid");
	

	if(!music)
	{
		allegro_message("Error loading Midi file");
        return 1;
	}
	if(play_midi(music, 0) !=0)
	{
		allegro_message("Error playing Midi\n%s", allegro_error);
        return 1;
	}

	defense.x = 300;
	defense.x2 = 380;
	defense.y = 450;
	defense.y2 = 480;	
	defense.turretx = ((defense.x+defense.x2)/2)-4;
	defense.turretx2 = ((defense.x+defense.x2)/2)+4;
	defense.turrety = defense.y + 1 ;
	defense.turrety2 = defense.y - 11;
	invaderinit();

	while(!key[KEY_ESC] && endgame != 1 && score < 3)
	{
		acquire_screen();
		eraseblock();
		ScoreArea();
		updatedefensebullet();
		for(test = 0; test < 6; test++)
		{
			fireinvaderweapon(test);
			updateinvaderbullet(test);
			
		}
		if(mouse_b &1)
		{
			defensecannon();
		}
		MoveInvader();		
		drawINVADERS();
		movedefense();
		release_screen();
		rest(50);
	}
	if(score == 3)
	{
		ScoreArea();
		rest(1500);
	}
	else if(endgame = 1)
	{
		textprintf_ex(screen, font, SCREEN_W/2, SCREEN_H/2, RED, 0, "YOU LOSE");
		rest(1500);
		
	}

	stop_midi();
	destroy_midi(music);
	allegro_exit();
    return 0;
}

END_OF_MAIN()