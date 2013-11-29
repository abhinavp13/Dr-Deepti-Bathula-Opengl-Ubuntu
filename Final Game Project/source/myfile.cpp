#include "SDL/SDL_mixer.h"
#include <iostream>
#include<sstream>
using namespace std;

//840x680

bool start=false; 
bool stop=false;

int arrow_x=350;
int arrow_y=295;

int choice=1;

int score=0;
int lives=10;

string sscore;
string slives;

ostringstream temp1,temp2;

char *temp;

//The music that will be played
Mix_Music *music = NULL;

//The sound effects that will be used
Mix_Chunk *fire = NULL;
Mix_Chunk *blast = NULL;
Mix_Chunk *shell = NULL;

//The event structure
SDL_Event event;


void music_start();
void music_stop();
void draw_startpage();
void check_arrow();
void output(int x, int y, char *string);
void output_h(int x, int y, char *string);
void check_arrow();
void music_fire();
void music_blast();
void sound_effect();
void draw_score();
void draw_gameover();



void init_startpage()
{

//Initialize SDL_mixer
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    	cout<<"ERROR initializing sound"<<endl; 
    
    music_start();
   
        
}

   
    
void deinit_startpage()
{
    
    //Free the music
    Mix_FreeMusic( music ); 
    
   //Free the sound effects
    Mix_FreeChunk( fire );
    Mix_FreeChunk( blast );  
    
    //Quit SDL_mixer
    Mix_CloseAudio();
    
}


void music_start()
{
	//Load the music
    music = Mix_LoadMUS( "music.wav" );
    
    //If there was a problem loading the music
    if( music == NULL )
    {
        cout<<"ERROR loading music"<<endl;    
    }

//Play the music
   if( Mix_PlayMusic( music, -1 ) == -1 )
   {
    	cout<<"ERROR playing music"<<endl;
   }  
   
    fire = Mix_LoadWAV( "fire.wav" );
    blast = Mix_LoadWAV( "blast.wav" );
    shell = Mix_LoadWAV ("shell.wav");
    
    //If there was a problem loading the sound effects
    if( ( fire == NULL ) || ( blast == NULL ) || shell == NULL )
    {
        cout<<"ERROR loading sound effects"<<endl;
    }  
}


void music_stop()
{
	Mix_HaltMusic();
}


void draw_startpage()
{
	
	output_h(250,100,"B A L L O O N      B L A S T E R");
	output(400,300, "PLAY");
	output(400,350, "EXIT");
	
	
	arrow_y=295+(choice-1)*50;
	
	glBegin(GL_TRIANGLES);
 
  	glVertex3f(arrow_x, arrow_y, 0);
  	glVertex3f(arrow_x-20, arrow_y-10, 0);
  	glVertex3f(arrow_x-20, arrow_y+10, 0);
  
  	glEnd();
  	glFlush();
  	
  	check_arrow();
}


void output_h(int x, int y, char *string)
{
  int len, i;
  //glColor3f(1.0, 0.0, 0.0);
  glRasterPos2f(x, y);
  len = (int) strlen(string);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
  }
}


void output(int x, int y, char *string)
{
  int len, i;
  //glColor3f(1.0, 0.0, 0.0);
  glRasterPos2f(x, y);
  len = (int) strlen(string);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
  }
}

void draw_score()
{

	output(600,600,"Score\t");
  	
	temp1<<score;
	sscore=temp1.str();
	temp = new char [sscore.length()+1];
  	strcpy (temp, sscore.c_str());
	output(700, 600, temp);
	delete temp;
	temp1.str("");

	output(600,650,"Lives\t");
 
	temp2<<lives;
	slives=temp2.str();  	
	temp = new char [slives.length()+1];
  	strcpy (temp, slives.c_str());
	output(700, 650, temp);
	delete temp;
	temp2.str("");
}



void check_arrow()
{

        while( SDL_PollEvent( &event ) )
        {
            //If a key was pressed
            if( event.type == SDL_KEYDOWN )
            {
		switch( event.key.keysym.sym )
                {
                    case SDLK_UP:
			if (choice >1 )
				choice--;
			break;
                    case SDLK_DOWN:
                    	if (choice<2)
                    		choice++;
                    	break;
                    	
		    case SDLK_RETURN:
		    	switch (choice)
		    	{
		    		case 1:
		    			music_stop();
		    			start=true;
		    			break;
		    		
		    		case 2:
		    			exit(0);
		    			break;
		    			
		    		default: ;
		    	}
		    			 
		    default : ;
                }
            }
       }
}


void music_fire()
{
	Mix_PlayChannel( -1, fire, 0 );
	Mix_PlayChannel (-1, shell, 0);
}


void music_blast()
{
	Mix_PlayChannel( -1, blast, 0 );
}


void sound_effect()
{
	while( SDL_PollEvent( &event ) )
        {
            //If a key was pressed
            if( event.type == SDL_KEYDOWN )
            {
		switch( event.key.keysym.sym )
                {
                    case SDLK_UP:
			music_fire();
			break;
			
                    case SDLK_DOWN:
                    	score++;
                    	//music_blast();
                    	break;
		    			 
		    default : ;
                }
            }
       }
}	

void draw_gameover()
{
    output_h(350,100,"G A M E    O V E R");
	output(400,200, "Your  Score ");
    temp1<<score;
	sscore=temp1.str();
	temp = new char [sscore.length()+1];
  	strcpy (temp, sscore.c_str());
	output(440, 250, temp);
	delete temp;
	temp1.str("");
}
