#include "obj.cpp"
#include <GL/glu.h>
#include <stdlib.h> 
#include <GL/glut.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h> //Needed for IMG_Load.  If you want to use bitmaps (SDL_LoadBMP), it appears to not be necessary
#include <SDL/SDL_opengl.h>
#include "ifs.c"
#include "myfile.cpp"
float angley = 20; 
 IFS_DATA*p;
//Changes :
 IFS_DATA *p1;
 bool flag=false;
 int dyb[8] = {100,100,100,100,100,100,100,100};
 bool disp[8] = {1,1,1,1,1,1,1,1};
 //int lives = 5;
 //int score = 0;
 int gameover = 0;
 float angle=-20;
 int arrx[8][2] = {{101,129},{456,476},{564,624},{811,826},{309,334},{698,721},{224,277},{485,538}};
 int arry[8][2] = {{324,365},{296,320},{317,393},{306,319},{259,296},{280,310},{400,472},{410,483}};
//////////////////////////////////////////////////////////

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"SDL.lib")
#pragma comment(lib,"SDLmain.lib")
#pragma comment(lib,"SDL_image.lib")
#define width 840
#define height 680 
const int screen_size[2] = {width,height};
 
GLuint texture[11];
int mousex=0,mousey=0;
float collisionx = -999999,collisiony = -999999;
void init_texture(void) {
    SDL_Surface* surface[11];
    surface[0] = IMG_Load("revolver_d.png");    // Revolver.
    surface[1] = IMG_Load("m.jpg");         //Background.

    surface[2] = IMG_Load("6.jpg");     // Balloon 
    surface[3] = IMG_Load("tryred.jpeg");     // Balloon 
    surface[4] = IMG_Load("trystar.jpg");     // Balloon 
    surface[5] = IMG_Load("2.jpg");     // Balloon 
    surface[6] = IMG_Load("tb.jpg");     // Balloon 
    surface[7] = IMG_Load("bp.jpg");     // Balloon 
    surface[8] = IMG_Load("3.jpg");     // Balloon 
    surface[9] = IMG_Load("tryred.jpeg");     // Balloon 
    surface[10] = IMG_Load("bred.jpeg");
    if (surface[0]==NULL||surface[1]==NULL||surface[2]==NULL||surface[3]==NULL||surface[4]==NULL||surface[5]==NULL||surface[6]==NULL||surface[7]==NULL||surface[8]==NULL||surface[9]==NULL||surface[10]==NULL) { 
        printf("Error: \"%s\"\n",SDL_GetError()); return;
    }
    glGenTextures(11,texture);
    for(int i=0;i<11;i++)
    {
    	glBindTexture(GL_TEXTURE_2D,texture[i]);
    	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface[i]->w,surface[i]->h, 0, GL_RGB,GL_UNSIGNED_BYTE,surface[i]->pixels);
    	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    	SDL_FreeSurface(surface[i]);
    }
}
void deinit_texture(void) {
    glDeleteTextures(11,texture);
}
 
bool get_input(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: return false;
            case SDL_MOUSEMOTION :  
                int x,y;
                SDL_GetMouseState(&x,&y);
                SDL_ShowCursor(SDL_DISABLE);
                
                if (x>mousex)
                	angle-=0.040*(x-mousex);
                else if(x<mousex)
                	angle+=0.040*(mousex-x);	
                if (y>mousey)
                	angley-=0.040*(y-mousey);
                else if(y<mousey)
                	angley+=0.040*(mousey-y);
                mousex=x,mousey=y;
                break;
            case SDL_MOUSEBUTTONDOWN :
                 if(event.button.button == SDL_BUTTON_LEFT)
                 {
                   music_fire();
                   SDL_GetMouseState(&x,&y);
                   collisionx = x; collisiony = y; 
                   
                 }
        }
    }
    return true;
}


void draw(void) {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,0);
    glViewport(0,0,screen_size[0],screen_size[1]);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1000.0, 1000.0);


glBindTexture(GL_TEXTURE_2D,texture[1]);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f,1.0f); 
    glVertex3f(900,800,-500);
    glTexCoord2f(1.0f,0.0f); 
    glVertex3f(900.0f,0.0f, -500.0f);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f( 0.0f,0.0f, -500.0f);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f( 0.0f,800.0f, -500.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);

	glLineWidth(2);
	glBindTexture(GL_TEXTURE_2D,texture[10]);
    glEnable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	glTexCoord2f(0.0f,0.0f);glVertex3f(mousex+5,mousey,-200);
	glTexCoord2f(1.0f,0.0f);glVertex3f(mousex,mousey,-200);
	glEnd();
	glBegin(GL_LINES);
	glTexCoord2f(0.0f,0.0f);glVertex3f(mousex-5,mousey,-200);
	glTexCoord2f(1.0f,0.0f);glVertex3f(mousex,mousey,-200);
	glEnd();
	glBegin(GL_LINES);
	glTexCoord2f(0.0f,0.0f);glVertex3f(mousex,mousey+5,-200);
	glTexCoord2f(1.0f,0.0f);glVertex3f(mousex,mousey,-200);
	glEnd();
	glBegin(GL_LINES);
	glTexCoord2f(0.0f,0.0f);glVertex3f(mousex,mousey-5,-200);
	glTexCoord2f(1.0f,0.0f);glVertex3f(mousex,mousey,-200);
	glEnd();
if(!start)
    draw_startpage();
else{


glBindTexture(GL_TEXTURE_2D,texture[0]);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
   glTranslatef(-90,-580,0);    
     glRotatef(-angle,0,1,0);
      glRotatef(angle,0,1,0);
       glRotatef(-angley,1,0,0);
        glRotatef(angle,0,1,0);
       glTranslatef(90,580,0);
	
     	
	for(int i=0;i<num_idx;i++)
    {
    	glBegin(GL_TRIANGLES);
    	float t1x=textures[faces[i].idx[0][1]-1].vt[0],t1y=1.0-textures[faces[i].idx[0][1]-1].vt[1];
    	float t2x=textures[faces[i].idx[1][1]-1].vt[0],t2y=1.0-textures[faces[i].idx[1][1]-1].vt[1];
    	float t3x=textures[faces[i].idx[2][1]-1].vt[0],t3y=1.0-textures[faces[i].idx[2][1]-1].vt[1];
    	glTexCoord2f(t1x,t1y);
    	glVertex3f(400*vertices[faces[i].idx[0][0]-1].v[0]+50,height-50-vertices[faces[i].idx[0][0]-1].v[1]*400,-400*vertices[faces[i].idx[0][0]-1].v[2]);

    	glTexCoord2f(t2x,t2y); 
    	glVertex3f (400*vertices[faces[i].idx[1][0]-1].v[0]+50,height-50-vertices[faces[i].idx[1][0]-1].v[1]*400,-400*vertices[faces[i].idx[1][0]-1].v[2]);
    	glTexCoord2f(t3x,t3y);
    	glVertex3f(400*vertices[faces[i].idx[2][0]-1].v[0]+50,height-50-vertices[faces[i].idx[2][0]-1].v[1]*400,-400*vertices[faces[i].idx[2][0]-1].v[2]);
    	glEnd();
    }
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();



if(!gameover)
{
    
	
	


int x[] = {65,415,544,768,271,659,201,460};
int y[] = {640-375,640-327,640-415,640-325,640-309,640-318,640-501,640-510};
///////////////////////////////////////////////////////// Balloons Starts /////////////////////////////////////////////////////////////////////////


for(int i = 0;i<8;i++)
{
  
  if(collisionx <=arrx[i][1] && collisionx >=arrx[i][0] && collisiony<=arry[i][1] && collisiony>=arry[i][0])
  {
     disp[i] = 0;
     score+=100;
     collisionx = -999999,collisiony = -999999;
  }
}



///////////////////////////////////////////////////////// Balloons1 Starts /////////////////////////////////////////////////////////////////////////
if(disp[0]){
    glBindTexture(GL_TEXTURE_2D,texture[2]);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
	glTranslatef(50,60,-500);
    for (int i=0; i<p1->numTriangles; ++i) 
    {
    	    glBegin(GL_TRIANGLES);
    	    glTexCoord2f(0.0f,0.0f);
        glVertex3f(x[0]+p1->triangles[i].a->x*50,-dyb[0]+height-y[0]-1*(p1->triangles[i].a->y*50),-p1->triangles[i].a->z*200);
    	    glTexCoord2f(1.0f,0.0f);
    	    glVertex3f(x[0]+p1->triangles[i].b->x*50,-dyb[0]+height-y[0]-1*(p1->triangles[i].b->y*50),-p1->triangles[i].b->z*200);
    	    glTexCoord2f(0.0f,1.0f);
    	    glVertex3f(x[0]+p1->triangles[i].c->x*50,-dyb[0]+height-y[0]-1*(p1->triangles[i].c->y*50),-p1->triangles[i].c->z*200);
    	    glEnd();
    }
     glPopMatrix();
}

///////////////////////////////////////////////////////// Balloons 1 Ends /////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////// Balloons 2 Starts /////////////////////////////////////////////////////////////////////////
if(disp[1]){
    glBindTexture(GL_TEXTURE_2D,texture[3]);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
	glTranslatef(50,60,-500);
    for (int i=0; i<p1->numTriangles; ++i) 
    {
    	    glBegin(GL_TRIANGLES);
    	    glTexCoord2f(0.0f,0.0f);
   	    	glVertex3f(x[1]+p1->triangles[i].a->x*30,-dyb[1]+height-y[1]-1*(p1->triangles[i].a->y*30),-p1->triangles[i].a->z*200);
    	    glTexCoord2f(1.0f,0.0f);
    	    glVertex3f(x[1]+p1->triangles[i].b->x*30,-dyb[1]+height-y[1]-1*(p1->triangles[i].b->y*30),-p1->triangles[i].b->z*200);
    	    glTexCoord2f(0.0f,1.0f);
    	    glVertex3f(x[1]+p1->triangles[i].c->x*30,-dyb[1]+height-y[1]-1*(p1->triangles[i].c->y*30),-p1->triangles[i].c->z*200);
    	    glEnd();
    }
     glPopMatrix();
}
    
///////////////////////////////////////////////////////// Balloons 2 Ends /////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////// Balloons 3 Starts /////////////////////////////////////////////////////////////////////////
if(disp[2]){
    glBindTexture(GL_TEXTURE_2D,texture[4]);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
	glTranslatef(50,60,-500);
    for (int i=0; i<p1->numTriangles; ++i) 
    {
    	    glBegin(GL_TRIANGLES);
    	    glTexCoord2f(0.0f,0.0f);
   	    	glVertex3f(x[2]+p1->triangles[i].a->x*100,-dyb[2]+height-y[2]-1*(p1->triangles[i].a->y*100),-p1->triangles[i].a->z*200);
    	    glTexCoord2f(1.0f,0.0f);
    	    glVertex3f(x[2]+p1->triangles[i].b->x*100,-dyb[2]+height-y[2]-1*(p1->triangles[i].b->y*100),-p1->triangles[i].b->z*200);
    	    glTexCoord2f(0.0f,1.0f);
    	    glVertex3f(x[2]+p1->triangles[i].c->x*100,-dyb[2]+height-y[2]-1*(p1->triangles[i].c->y*100),-p1->triangles[i].c->z*200);
    	    glEnd();
    }
     glPopMatrix();
}
    
///////////////////////////////////////////////////////// Balloons 3 Ends /////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////// Balloons 4 Starts /////////////////////////////////////////////////////////////////////////
if(disp[3]){
    glBindTexture(GL_TEXTURE_2D,texture[5]);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
	glTranslatef(50,60,-500);
    for (int i=0; i<p1->numTriangles; ++i) 
    {
    	    glBegin(GL_TRIANGLES);
    	    glTexCoord2f(0.0f,0.0f);
   	    	glVertex3f(x[3]+p1->triangles[i].a->x*20,-dyb[3]+height-y[3]-1*(p1->triangles[i].a->y*20),-p1->triangles[i].a->z*200);
    	    glTexCoord2f(1.0f,0.0f);
    	    glVertex3f(x[3]+p1->triangles[i].b->x*20,-dyb[3]+height-y[3]-1*(p1->triangles[i].b->y*20),-p1->triangles[i].b->z*200);
    	    glTexCoord2f(0.0f,1.0f);
    	    glVertex3f(x[3]+p1->triangles[i].c->x*20,-dyb[3]+height-y[3]-1*(p1->triangles[i].c->y*20),-p1->triangles[i].c->z*200);
    	    glEnd();
    }
     glPopMatrix();
}
    
///////////////////////////////////////////////////////// Balloons 4 Ends /////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////// Balloons 5 Starts /////////////////////////////////////////////////////////////////////////
if(disp[4]){
    glBindTexture(GL_TEXTURE_2D,texture[6]);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
	glTranslatef(50,60,-500);
    for (int i=0; i<p1->numTriangles; ++i) 
    {
    	    glBegin(GL_TRIANGLES);
    	    glTexCoord2f(0.0f,0.0f);
   	    	glVertex3f(x[4]+p1->triangles[i].a->x*50,-dyb[4]+height-y[4]-1*(p1->triangles[i].a->y*50),-p1->triangles[i].a->z*200);
    	    glTexCoord2f(1.0f,0.0f);
    	    glVertex3f(x[4]+p1->triangles[i].b->x*50,-dyb[4]+height-y[4]-1*(p1->triangles[i].b->y*50),-p1->triangles[i].b->z*200);
    	    glTexCoord2f(0.0f,1.0f);
    	    glVertex3f(x[4]+p1->triangles[i].c->x*50,-dyb[4]+height-y[4]-1*(p1->triangles[i].c->y*50),-p1->triangles[i].c->z*200);
    	    glEnd();
    }
     glPopMatrix();
}
    
///////////////////////////////////////////////////////// Balloons 5 Ends /////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////// Balloons 6 Starts /////////////////////////////////////////////////////////////////////////
if(disp[5]){
    glBindTexture(GL_TEXTURE_2D,texture[7]);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
	glTranslatef(50,60,-500);
    for (int i=0; i<p1->numTriangles; ++i) 
    {
    	    glBegin(GL_TRIANGLES);
    	    glTexCoord2f(0.0f,0.0f);
   	    	glVertex3f(x[5]+p1->triangles[i].a->x*40,-dyb[5]+height-y[5]-1*(p1->triangles[i].a->y*40),-p1->triangles[i].a->z*200);
    	    glTexCoord2f(1.0f,0.0f);
    	    glVertex3f(x[5]+p1->triangles[i].b->x*40,-dyb[5]+height-y[5]-1*(p1->triangles[i].b->y*40),-p1->triangles[i].b->z*200);
    	    glTexCoord2f(0.0f,1.0f);
    	    glVertex3f(x[5]+p1->triangles[i].c->x*40,-dyb[5]+height-y[5]-1*(p1->triangles[i].c->y*40),-p1->triangles[i].c->z*200);
    	    glEnd();
    }
     glPopMatrix();
}
    
///////////////////////////////////////////////////////// Balloons 6 Ends /////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////// Balloons 7 Starts /////////////////////////////////////////////////////////////////////////
if(disp[6]){
    glBindTexture(GL_TEXTURE_2D,texture[8]);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
	glTranslatef(50,60,-500);
    for (int i=0; i<p1->numTriangles; ++i) 
    {
    	    glBegin(GL_TRIANGLES);
    	    glTexCoord2f(0.0f,0.0f);
   	    	glVertex3f(x[6]+p1->triangles[i].a->x*100,-dyb[6]+height-y[6]-1*(p1->triangles[i].a->y*100),-p1->triangles[i].a->z*200);
    	    glTexCoord2f(1.0f,0.0f);
    	    glVertex3f(x[6]+p1->triangles[i].b->x*100,-dyb[6]+height-y[6]-1*(p1->triangles[i].b->y*100),-p1->triangles[i].b->z*200);
    	    glTexCoord2f(0.0f,1.0f);
    	    glVertex3f(x[6]+p1->triangles[i].c->x*100,-dyb[6]+height-y[6]-1*(p1->triangles[i].c->y*100),-p1->triangles[i].c->z*200);
    	    glEnd();
    }
     glPopMatrix();
}
    
///////////////////////////////////////////////////////// Balloons 7 Ends /////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////// Balloons 8 Starts /////////////////////////////////////////////////////////////////////////
if(disp[7]){
    glBindTexture(GL_TEXTURE_2D,texture[9]);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
	glTranslatef(50,60,-500);
    for (int i=0; i<p1->numTriangles; ++i) 
    {
    	    glBegin(GL_TRIANGLES);
    	    glTexCoord2f(0.0f,0.0f);
   	    	glVertex3f(x[7]+p1->triangles[i].a->x*100,-dyb[7]+height-y[7]-1*(p1->triangles[i].a->y*100),-p1->triangles[i].a->z*200);
    	    glTexCoord2f(1.0f,0.0f);
    	    glVertex3f(x[7]+p1->triangles[i].b->x*100,-dyb[7]+height-y[7]-1*(p1->triangles[i].b->y*100),-p1->triangles[i].b->z*200);
    	    glTexCoord2f(0.0f,1.0f);
    	    glVertex3f(x[7]+p1->triangles[i].c->x*100,-dyb[7]+height-y[7]-1*(p1->triangles[i].c->y*100),-p1->triangles[i].c->z*200);
    	    glEnd();
    }
     glPopMatrix();
}
    
///////////////////////////////////////////////////////// Balloons 8 Ends /////////////////////////////////////////////////////////////////////////

dyb[0]+=1;
arry[0][0]-=1;
arry[0][1]-=1;

dyb[1]+=1.5;
arry[1][0]-=1;
arry[1][1]-=1;

dyb[2]+=1.7;
arry[2][0]-=1;
arry[2][1]-=1;

dyb[3]+=1.3;
arry[3][0]-=1;
arry[3][1]-=1;


dyb[4]+=1.4;
arry[4][0]-=1;
arry[4][1]-=1;

dyb[5]+=1.1;
arry[5][0]-=1;
arry[5][1]-=1;

dyb[6]+=1.2;
arry[6][0]-=1;
arry[6][1]-=1;

dyb[7]+=1.6;
arry[7][0]-=1;
arry[7][1]-=1;


if(dyb[0]>500)
{
    collisionx = -999999,collisiony = -999999;
    arry[0][0] = 324;
    arry[0][1] = 365;
    if(disp[0]==1)
      lives--;
    dyb[0] = 100;
    disp[0] = 1;
    
}

if(dyb[1]>450)
{
    collisionx = -999999,collisiony = -999999;
    arry[1][0] = 296;
    arry[1][1] = 320;
    if(disp[1]==1)
      lives--;
    dyb[1] = 100;
    disp[1] = 1;
}

if(dyb[2]>500)
{
    collisionx = -999999,collisiony = -999999;
    arry[2][0] = 317;
    arry[2][1] = 393;
    if(disp[2]==1)
      lives--;
    dyb[2] = 100;
    disp[2] = 1;
}

if(dyb[3]>470)
{
    collisionx = -999999,collisiony = -999999;
    arry[3][0] = 306;
    arry[3][1] = 319;
    if(disp[3]==1)
      lives--;
    dyb[3] = 100;
    disp[3] = 1;
}

if(dyb[4]>450)
{
    collisionx = -999999,collisiony = -999999;
    arry[4][0] = 259;
    arry[4][1] = 296;
    if(disp[4]==1)
      lives--;
    dyb[4] = 100;
    disp[4] = 1;
}

if(dyb[5]>450)
{
    collisionx = -999999,collisiony = -999999;
    arry[5][0] = 280;
    arry[5][1] = 310;
    if(disp[5]==1)
      lives--;
    dyb[5] = 100;
    disp[5] = 1;
}

if(dyb[6]>600)
{
    collisionx = -999999,collisiony = -999999;
    arry[6][0] = 400;
    arry[6][1] = 472;
    if(disp[6]==1)
      lives--;
    dyb[6] = 100;
    disp[6] = 1;
}

if(dyb[7]>600)
{
    collisionx = -999999,collisiony = -999999;
    arry[7][0] = 410;
    arry[7][1] = 483;
    if(disp[7]==1)
      lives--;
    dyb[7] = 100;
    disp[7] = 1;
}

}// Game over if-else.
else
{


	if (!flag){
		music_blast();
		flag=true;
	}
		

       draw_gameover();




}
///////////////////////////////////////////////////////// Balloons Ends /////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   if(lives<=0){
     gameover = 1;
     lives = 0;
   }
    glDisable(GL_TEXTURE_2D);
    glEnd();
    if (!gameover)
        draw_score();
} // if else start 
    SDL_GL_SwapBuffers();
}
/////Changes : 



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



int main(int argc, char* argv[]) {
    p1 = load_ifs_file(argv[1]);
    g=fopen(argv[2],"r");
    parse();
 
    glutInit(&argc,argv);
   
///////////////////////////////////////////////////////////////////////////////
	
    SDL_Init(SDL_INIT_EVERYTHING|SDL_INIT_NOPARACHUTE);
 
    SDL_SetVideoMode(screen_size[0],screen_size[1], 32, SDL_OPENGL);//|SDL_FULLSCREEN);
 
    glEnable(GL_DEPTH_TEST);
    
    init_texture();
    init_startpage();
 
    while (true) {
        if (!get_input()) break;
        draw();
    }
 
    deinit_startpage();
    deinit_texture();
 
    SDL_Quit();
 
    return 0;
}
