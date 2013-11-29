#include<GL/gl.h>
#include<GL/glut.h>                    
#define width 640
#define height 480                             // Fixing the height and width of the screen.
#include<stdio.h>
#include<math.h>
const char* ch;                               // Input arguments are saved in this character pointer.
float xi,xf,yi,yf,xt,yt;                      // Six global variables : xi,xf,yi,yf -> used in lines drawing algorithm && xi,xf,yi,yf,xt,yt -> used in triangle drawing algorithm.
float r[3] = {-1,-1,-1},g[3] = {-1,-1,-1}, b[3] = {-1,-1,-1},a[3] = {-1,-1,-1};             // Color for each vertex of triangle and also for each end point of line segment.(in line drawing case, third element has -1 as its value(as we need only two indices only).)
float red,green,blue,alpha;                // Finally acheived colors by calculating using ratios formula.


//########### Color Calculation ###########//

void color(float t)                          // t is the ratio in which two vertices are divided by.
{
   red = r[0] + t*(r[1]-r[0]);               // Using:  line -> (x1,y1)------(x2,y2) , a point p in between divides this line with t:(1-t).
   green = g[0] + t*(g[1]-g[0]);
   blue = b[0] + t*(b[1]-b[0]);
   alpha = a[0] + t*(a[1]-a[0]);
} 
//---------------------------------------------------------------------------//

//########### Bresenham Algorithm for drawing pixel per pixel line ##########//

void bresenham(void)
{
  float slope;                              // Variable to store slope of the line with sign.
  float dx,dy,x,y;                          // Variables to store change in end points x,y values.
  dx = xf-xi;                     
  dy = yf-yi; 
  if(dx!=0)
    slope = dy/dx;
  else
    slope = 99999999;
  x = xi;
  y = yi;
  glPointSize(1.0f);                        // Setting point size to be one to assure a pixel draw.
  glBegin(GL_POINTS);                       
  float t = 0;
  if (dx ==0)                               // If change in x is zero slope -> infinity, only y needs to be incremented.
  {
        while(y!=yf)
        {
           color(fabs((y-yi)/(yf-yi)));     //Calling Color function for color calculation.
           glColor4f(red/255.0,green/255.0,blue/255.0,alpha/255.0);         //Choosing color as calculated.
           if(yf>yi)                       // If yf > yi => y needs increment positively.
              glVertex3f(x,y++,1.0f);
           else 
              glVertex3f(x,y--,1.0f);      // else y needs negative increment.
        }
  }   
  else if(fabs(slope)<=1)                  // If slope is between -1 and 1.
  {
          while(xf!=x){                    // Since, slope is not steep, therefore, while xf is not reached keeps on incrementing x(positively or negatively).
          if(xf>xi)
             x = x +1;
          else
             x = x - 1;
          if(yf>yi)                       // If yf > yi =>  y needs to be positively incremented.
          {
              if(y+0.5 <= yi + slope*(x-xi))             // if midpoint of the next and its above pixel is less than corresponding value of line for this x, then y needs positive increment  else no increment required.
                 y = y+1; 
          }
          else 
          {
             if(y-0.5 >=  yi + slope*(x-xi))                // if midpoint of the next and its below pixel is greator than corresponding value of line for this x, then y needs negative increment  else no increment required.
                y = y-1;
          }
           color(fabs((x-xi)/(xf-xi)));             // Calling Color function for color calculation.
           glColor4f(red/255.0,green/255.0,blue/255.0,alpha/255.0);               // Choosing color as calculated.
           glVertex3f(x,y,1.0f);          
         }
  }
  else                       // If slope is out of the limits of -1 and 1, then this else statement gets executed.
  {
     while(yf!=y)            // Since, slope is steep, therefore, while yf is not reached keeps on incrementing y(positively or negatively).
     {
         if(yf>yi)           // if yf > yi => increment y positively
            y++;
         else                // Else increment y negatively.
            y--;
         if(xf>xi)           // if xf > xi => x needs positive increment
         {
            if(x+0.5<= xi+((y-yi)/slope))        // if midpoint of the left and its right pixel is less than corresponding value of line for this y, then x needs positive increment  else no increment required.
               x++;  
         }    
         else 
         {
            if(x-0.5>= xi+((y-yi)/slope))           // if midpoint of the left and its right pixel is greator than corresponding value of line for this y, then x needs negative increment  else no increment required.
               x--;  
         }
        color(fabs((y-yi)/(yf-yi)));           // Calling the color function for calculating the corresponding colors.
        glColor4f(red/255.0,green/255.0,blue/255.0,alpha/255.0);
        glVertex3f(x,y,1.0f);       
     }

  }
  glEnd();
  glFlush();                    // Flushing the buffer for synchronizing drawing.
}



//-------------------------------------------------------------//





//############### Area of Triangle ##################//

float area()
{
   float s = (sqrt((xf-xi)*(xf-xi) + (yf-yi)*(yf-yi)) + sqrt((xt-xi)*(xt-xi) + (yt-yi)*(yt-yi)) + sqrt((xf-xt)*(xf-xt) + (yf-yt)*(yf-yt))) /2;
   return sqrt(s*(s-(sqrt((xf-xi)*(xf-xi) + (yf-yi)*(yf-yi))))*(s-(sqrt((xt-xi)*(xt-xi) + (yt-yi)*(yt-yi))))*(s-(sqrt((xf-xt)*(xf-xt) + (yf-yt)*(yf-yt)))));
}

//-------------------------------------------------------------------------//

//############## Area w.r.t point (x,y),(xf,yf),(xt,yt) ###################//

float areaone(float x, float y)
{
   float s = (sqrt((xf-x)*(xf-x) + (yf-y)*(yf-y)) + sqrt((xt-x)*(xt-x) + (yt-y)*(yt-y)) + sqrt((xf-xt)*(xf-xt) + (yf-yt)*(yf-yt))) /2;
   return sqrt(s*(s-(sqrt((xf-x)*(xf-x) + (yf-y)*(yf-y))))*(s-(sqrt((xt-x)*(xt-x) + (yt-y)*(yt-y))))*(s-(sqrt((xf-xt)*(xf-xt) + (yf-yt)*(yf-yt)))));
}

//-------------------------------------------------------------------------//

//############## Area w.r.t point (x,y),(xi,yi),(xt,yt) ###################//

float areatwo(float x,float y)
{
   float s = (sqrt((x-xi)*(x-xi) + (y-yi)*(y-yi)) + sqrt((xt-xi)*(xt-xi) + (yt-yi)*(yt-yi)) + sqrt((x-xt)*(x-xt) + (y-yt)*(y-yt))) /2;
   return sqrt(s*(s-(sqrt((x-xi)*(x-xi) + (y-yi)*(y-yi))))*(s-(sqrt((xt-xi)*(xt-xi) + (yt-yi)*(yt-yi))))*(s-(sqrt((x-xt)*(x-xt) + (y-yt)*(y-yt)))));
}

//-------------------------------------------------------------------------//

//############## Area w.r.t point (x,y),(xf,yf),(xi,yi) ###################//

float areathree(float x,float y)
{
   float s = (sqrt((xf-xi)*(xf-xi) + (yf-yi)*(yf-yi)) + sqrt((x-xi)*(x-xi) + (y-yi)*(y-yi)) + sqrt((xf-x)*(xf-x) + (yf-y)*(yf-y))) /2;
   return sqrt(s*(s-(sqrt((xf-xi)*(xf-xi) + (yf-yi)*(yf-yi))))*(s-(sqrt((x-xi)*(x-xi) + (y-yi)*(y-yi))))*(s-(sqrt((xf-x)*(xf-x) + (yf-y)*(yf-y)))));
}


//-------------------------------------------------------------------------//

//############## Check for Point to lie within Triangle ###################//

bool PointInTriangle(float x,float y)
{
     float slope = (yf-yi)/(xf-xi);
     if(((y-yi)-slope*(x-xi))*((yt-yi)-slope*(xt-xi))>0)
     {
        slope = (yt-yi)/(xt-xi);
        if(((y-yi)-slope*(x-xi))*((yf-yi)-slope*(xf-xi))>0)
        {
           slope = (yt-yf)/(xt-xf);
           if(((y-yf)-slope*(x-xf))*((yi-yf)-slope*(xi-xf))>0)
              return true;
        }
     }
   return false;
}


//--------------------------------------------------------------------//


//################# Triangle Drawing Algorithm #######################//

void triangles(void)
{ 
    float maxx;
    maxx = xi > xf ? xi : xf;
    maxx = xt >maxx ? xt : maxx;                 // Getting the max of x values.
    float maxy =  yi > yf ? yi : yf;
    maxy = yt >maxy ? yt : maxy;                 // Getting the max of y values.
    float miny = yi > yf ? yf : yi;
    miny  = yt > miny ? miny : yt;               // Getting the min of y values.
    float minx = xi > xf ? xf : xi;
    minx  = xt > minx ? minx : xt;               // Getting the min of x values.
    float i,j;
    glBegin(GL_POINTS);
    for(i = minx; i<=maxx;i++)                   // Running the two nested 'for' loops and if the any point lie in the triangle, then drawing it, leaving out the rest. 
    {
       for(j = miny; j <= maxy; j++)
       {
          if(PointInTriangle(i,j))              // Calling the function PointInTriangle for check regarding point lying inside.
            {
               // Calculating the color for pixels by taking the ratios of the areas of the area formed by point with each of the vertices.                       
               red = ((areaone(i,j) * r[0] + areatwo(i,j) * r[1] + areathree(i,j) * r[2]) / area() ) / 255.0;
               green = ((areaone(i,j) * g[0] + areatwo(i,j) * g[1] + areathree(i,j) * g[2]) / area() ) / 255.0;
               blue = ((areaone(i,j) * b[0] + areatwo(i,j) * b[1] + areathree(i,j) * b[2]) / area() ) / 255.0;
               alpha = ((areaone(i,j) * a[0] + areatwo(i,j) * a[1] + areathree(i,j) * a[2]) / area() ) / 255.0;
               glColor4f(red,green,blue,alpha);
               glVertex3f(i,j,1.0f);
            }
       }
    }
    glEnd();
    glFlush();       // Flushing the buffer for synchronizing drawing.
}

//-----------------------------------------------------------------------------------------//


//################### Calling the display function ################//

void display()
{
   
   glClear(GL_COLOR_BUFFER_BIT);
   int numoflines;
   FILE* in = fopen(ch,"r+");              // File opening and inputting the values ....
   fscanf(in,"%d",&numoflines);            
   while(numoflines!=0){
   fscanf(in,"%f",&xi); 
   fscanf(in,"%f",&yi);
   fscanf(in,"%f",&r[0]); 
   fscanf(in,"%f",&g[0]);
   fscanf(in,"%f",&b[0]);
   fscanf(in,"%f",&a[0]);
   fscanf(in,"%f",&xf); 
   fscanf(in,"%f",&yf);
   fscanf(in,"%f",&r[1]); 
   fscanf(in,"%f",&g[1]);
   fscanf(in,"%f",&b[1]);
   fscanf(in,"%f",&a[1]);
   bresenham();                              // Calling the bresenham algorithm from here.
   numoflines--;
   }
   
    int numoftriangles  = 0;
    fscanf(in,"%d",&numoftriangles);
    while(numoftriangles!=0){
    fscanf(in,"%f",&xi); 
    fscanf(in,"%f",&yi);
    fscanf(in,"%f",&r[0]); 
    fscanf(in,"%f",&g[0]);
    fscanf(in,"%f",&b[0]);
    fscanf(in,"%f",&a[0]);
    fscanf(in,"%f",&xf); 
    fscanf(in,"%f",&yf);
    fscanf(in,"%f",&r[1]); 
    fscanf(in,"%f",&g[1]);
    fscanf(in,"%f",&b[1]);
    fscanf(in,"%f",&a[1]);
    fscanf(in,"%f",&xt); 
    fscanf(in,"%f",&yt);
    fscanf(in,"%f",&r[2]); 
    fscanf(in,"%f",&g[2]);
    fscanf(in,"%f",&b[2]);
    fscanf(in,"%f",&a[2]);
    triangles();                           // Calling the triangle algorithm from here.
    numoftriangles--;
    }       
} 

//------------------------------------------------------------------------------------//



//#################### Initializing Function #######################//

void init()
{
  
  glClearColor(0.0,0.0,0.0,1.0);                 // Setting the background as : black.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-320,320,-240,240,-1.0f,1.0f);         // Setting the coordinates as : x-axis-> -320 to 320, y-axis-> -240 to 240, z-axis-> -1 to 1.  

}

//-----------------------------------------------------------------------------------//


//################################ The Main Function ######################################//


int main(int argc,char** argv)                  // Ready to take arguments.
{
  glutInit(&argc,argv);
  if(argc>=2)                                  // If argument does not have input.txt file, then ask for it ....
  ch = argv[1];
  else{
  printf("\nPlease  execute as : './rasterize input.txt'\n");
  return 0;}
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);    // Using only Single Buffer for drawing.
  glutInitWindowSize(width,height);
  glutInitWindowPosition(100,100);
  glutCreateWindow("Test");
  init();                                        // Initializing with line endpoints.
  glutDisplayFunc(display);
  glutMainLoop();
}
