#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <string.h>
#include <math.h>
#include "pic.h"
#include <iostream>
#include <stdio.h>
using namespace std;

#define MAX_TRIANGLES 10000
#define MAX_SPHERES 10000
#define MAX_LIGHTS 10000

char *filename;
#define MODE_DISPLAY 1
#define MODE_JPEG 2
int mode=MODE_DISPLAY;

//you may want to make these smaller for debugging purposes
#define WIDTH 640
#define HEIGHT 480

//the field of view of the camera
#define fov 75.0

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Vector3 {

public:
	double x;
	double y;
	double z;
    //constructor :
	Vector3();
    Vector3(double x,double y,double z);
    //Functions :
	double vector_dot_product(Vector3 b);
    double mod();
    Vector3 normalize();
    Vector3 scalar_product(double t);
	Vector3 vector_cross_product(Vector3 b);
	Vector3 vector_addition(Vector3 a);
	Vector3 vector_subtraction(Vector3 b);
};

Vector3::Vector3()
{
	x=0.0;
	y=0.0;
	z=0.0;
}

Vector3::Vector3(double a,double b,double c)
{
	x=a;
	y=b;
	z=c;
}

double Vector3::vector_dot_product(Vector3 a)
{
	double ans = a.x*x + a.y*y + a.z*z;
	return ans;
}
Vector3 Vector3::vector_cross_product(Vector3 b)
{
	Vector3 v(y*b.z-z*b.y,z*b.x-x*b.z,x*b.y-y*b.x);
	return v;
}
Vector3 Vector3::normalize()
{
	double fac=sqrt(x*x+y*y+z*z);
	Vector3 v(x/fac,y/fac,z/fac);
	return v;
}

Vector3 Vector3::vector_addition(Vector3 a)
{
	Vector3 v(a.x+x,a.y+y,a.z+z);
	return v;
}

Vector3 Vector3::vector_subtraction(Vector3 a)
{
	Vector3 v(x-a.x,y-a.y,z-a.z);
	return v;
}

double Vector3::mod()
{
	double d=sqrt(x*x+y*y+z*z);
	return d;
}

Vector3 Vector3::scalar_product(double t)
{
	Vector3 v(t*x,t*y,t*z);
	return v;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Ray {
public:
	Vector3 P1;
	Vector3 P2;
	Ray();
	Ray(Vector3 x,Vector3 y);
};
Ray::Ray()
{
	P1.x=P1.y=P1.z=P2.x=P2.y=	P2.z=0.0;
}
Ray::Ray(Vector3 a,Vector3 b)
{
	P1.x=a.x,	P1.y=a.y,	P1.z=a.z,	P2.x=b.x,	P2.y=b.y,	P2.z=b.z;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Points {
public:
    int type; 
	int id; 
	Ray r;
	Vector3 v;
	Points();
};

Points::Points() {
	r.P2=Vector3();
	r.P1=Vector3();
	v.x=0.0;
	v.y=0.0;
	v.z=0.0;
	type=0;
	id=-1;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Vertex
{
  double position[3];
  double color_diffuse[3];
  double color_specular[3];
  double normalize[3];
  double shininess;
};



struct Triangle_plane
{
	double a,b,c,d;             
};           

typedef struct _Triangle
{
  struct Vertex v[3];
  struct Triangle_plane tp;
} Triangle;

typedef struct _Sphere
{
  double position[3];
  double color_diffuse[3];
  double color_specular[3];
  double shininess;
  double radius;
} Sphere;

typedef struct _Light
{
  double position[3];
  double color[3];
} Light;

Triangle triangles[MAX_TRIANGLES];
Sphere spheres[MAX_SPHERES];
Light lights[MAX_LIGHTS];
double ambient_light[3];

int num_triangles=0;
int num_spheres=0;
int num_lights=0;
int Global_D;
Vector3 ray_tracing(Ray r,int depth);
double void_const = -1000007; //Just a constant, eg: Big negative prime number.

//  Triangle Interpolation : (specular) :: /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Vector3 TriangleInterpolation_specular(int index,Vector3 v) 
{
	Vector3 one(triangles[index].v[0].color_specular[0],triangles[index].v[0].color_specular[1],triangles[index].v[0].color_specular[2]);
	Vector3 two(triangles[index].v[1].color_specular[0],triangles[index].v[1].color_specular[1],triangles[index].v[1].color_specular[2]);
	Vector3 three(triangles[index].v[2].color_specular[0],triangles[index].v[2].color_specular[1],triangles[index].v[2].color_specular[2]);
	
    Vector3 req1(triangles[index].v[0].position[0],triangles[index].v[0].position[1],triangles[index].v[0].position[2]);
    Vector3 direc2=v.vector_subtraction(req1);
	Vector3 req2(triangles[index].v[1].position[0],triangles[index].v[1].position[1],triangles[index].v[1].position[2]);
	Vector3 req3(triangles[index].v[2].position[0],triangles[index].v[2].position[1],triangles[index].v[2].position[2]);
	
    Vector3 direc1=req3.vector_subtraction(req2);
	
	float comp1=(((req1.vector_cross_product(direc2)).vector_subtraction(req2.vector_cross_product(direc2))).mod())/((direc1.vector_cross_product(direc2)).mod());
	float comp2=(((req2.vector_cross_product(direc1)).vector_subtraction(req1.vector_cross_product(direc1))).mod())/((direc2.vector_cross_product(direc1)).mod());
	Vector3 p=three.scalar_product(comp1).vector_addition(two.scalar_product(1.0-comp1));
	Vector3 ans=one.scalar_product((comp2-1.0)/comp2).vector_addition(p.scalar_product(1.0/comp2));
	return ans;

}

// Triangle Interpolation : (diffuse) :: /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Vector3 TriangleInterpolation_diffuse(int index,Vector3 v) 
{   
	Vector3 one(triangles[index].v[0].color_diffuse[0],triangles[index].v[0].color_diffuse[1],triangles[index].v[0].color_diffuse[2]);
	Vector3 two(triangles[index].v[1].color_diffuse[0],triangles[index].v[1].color_diffuse[1],triangles[index].v[1].color_diffuse[2]);
	Vector3 three(triangles[index].v[2].color_diffuse[0],triangles[index].v[2].color_diffuse[1],triangles[index].v[2].color_diffuse[2]);
	
    Vector3 req1(triangles[index].v[0].position[0],triangles[index].v[0].position[1],triangles[index].v[0].position[2]);
    Vector3 direc2=v.vector_subtraction(req1);
	Vector3 req2(triangles[index].v[1].position[0],triangles[index].v[1].position[1],triangles[index].v[1].position[2]);
	Vector3 req3(triangles[index].v[2].position[0],triangles[index].v[2].position[1],triangles[index].v[2].position[2]);
	
    Vector3 direc1=req3.vector_subtraction(req2);
	
	float comp1=(((req1.vector_cross_product(direc2)).vector_subtraction(req2.vector_cross_product(direc2))).mod())/((direc1.vector_cross_product(direc2)).mod());
	float comp2=(((req2.vector_cross_product(direc1)).vector_subtraction(req1.vector_cross_product(direc1))).mod())/((direc2.vector_cross_product(direc1)).mod());
	Vector3 p=three.scalar_product(comp1).vector_addition(two.scalar_product(1.0-comp1));
	Vector3 ans=one.scalar_product((comp2-1.0)/comp2).vector_addition(p.scalar_product(1.0/comp2));
	return ans;

}

// Triangle Interpolation : (normailze) //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Vector3 TriangleInterpolation_normalize(int index,Vector3 v) 
{
    Vector3 one(triangles[index].v[0].normalize[0],triangles[index].v[0].normalize[1],triangles[index].v[0].normalize[2]);
	Vector3 two(triangles[index].v[1].normalize[0],triangles[index].v[1].normalize[1],triangles[index].v[1].normalize[2]);
	Vector3 three(triangles[index].v[2].normalize[0],triangles[index].v[2].normalize[1],triangles[index].v[2].normalize[2]);
	
    Vector3 req1(triangles[index].v[0].position[0],triangles[index].v[0].position[1],triangles[index].v[0].position[2]);
    Vector3 direc2=v.vector_subtraction(req1);
	Vector3 req2(triangles[index].v[1].position[0],triangles[index].v[1].position[1],triangles[index].v[1].position[2]);
	Vector3 req3(triangles[index].v[2].position[0],triangles[index].v[2].position[1],triangles[index].v[2].position[2]);
	
    Vector3 direc1=req3.vector_subtraction(req2);
	
	float comp1=(((req1.vector_cross_product(direc2)).vector_subtraction(req2.vector_cross_product(direc2))).mod())/((direc1.vector_cross_product(direc2)).mod());
	float comp2=(((req2.vector_cross_product(direc1)).vector_subtraction(req1.vector_cross_product(direc1))).mod())/((direc2.vector_cross_product(direc1)).mod());
	Vector3 p=three.scalar_product(comp1).vector_addition(two.scalar_product(1.0-comp1));
	Vector3 ans=one.scalar_product((comp2-1.0)/comp2).vector_addition(p.scalar_product(1.0/comp2));
	return ans;

}

// Triangle Intersection Detection :: ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

double TriangleIntersectionDetection(Ray r,int index)
{
	
	Vector3 normal(triangles[index].tp.a,triangles[index].tp.b,triangles[index].tp.c);
    Vector3 direction=(r.P2).vector_subtraction(r.P1);  //Intersection with Triangle Plane.
	double x=normal.vector_dot_product(r.P1);
	double y=normal.vector_dot_product(direction);
	if(y==0.0)
		return void_const;
	double Value=-(triangles[index].tp.d+x)/y;  
	if(Value>0.0)  
	{
        Vector3 temp1(triangles[index].v[0].position[0],triangles[index].v[0].position[1],triangles[index].v[0].position[2]);
	    Vector3 temp2(triangles[index].v[1].position[0],triangles[index].v[1].position[1],triangles[index].v[1].position[2]);
	    Vector3 temp3(triangles[index].v[2].position[0],triangles[index].v[2].position[1],triangles[index].v[2].position[2]);
		
        Vector3 u=temp2.vector_subtraction(temp1);
		Vector3 v=temp3.vector_subtraction(temp1);
        Vector3 Points(r.P1.x+direction.x*Value,r.P1.y+direction.y*Value,r.P1.z+direction.z*Value);
		Vector3 diff=Points.vector_subtraction(temp1);
		double ans=(u.vector_dot_product(u)*diff.vector_dot_product(v)-u.vector_dot_product(v)*diff.vector_dot_product(u))/(v.vector_dot_product(v)*u.vector_dot_product(u)-pow(v.vector_dot_product(u),2));
		double anst1=(v.vector_dot_product(v)*diff.vector_dot_product(u)-v.vector_dot_product(u)*diff.vector_dot_product(v))/(v.vector_dot_product(v)*u.vector_dot_product(u)-pow(v.vector_dot_product(u),2));
   	    if(!(ans<0 || ans>1.0 || anst1<0 || anst1>1.0 || ((ans+anst1)>1.0)))   //Intersection.
		  return Value;
	}
	return void_const;
}

// Sphere Intersection Dectection :: ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

double SphereIntersectionDetection(Ray r,int i)
{
	Vector3 direction = r.P2.vector_subtraction(r.P1);
	double a = direction.x*direction.x+direction.y*direction.y+direction.z*direction.z;
	double b =2*(direction.x*(r.P1.x-spheres[i].position[0]) + direction.y*(r.P1.y-spheres[i].position[1]) + direction.z*(r.P1.z-spheres[i].position[2]));
	double c =(r.P1.x-spheres[i].position[0])*(r.P1.x-spheres[i].position[0]) + (r.P1.y-spheres[i].position[1])*(r.P1.y-spheres[i].position[1]) + (r.P1.z-spheres[i].position[2])*(r.P1.z-spheres[i].position[2])-(spheres[i].radius*spheres[i].radius);
    double Det=b*b-4*a*c;
	if(Det<0)
		return void_const;
	else
	{
		if(Det==0)
		{
			if( (-b/(2*a))>1.0 )
				return (-b/(2*a));
		}
		else
		{
			if( ((-b-sqrt(Det))/(2.0*a)) > 0.0 )
 			  return ((-b-sqrt(Det))/(2.0*a));
		}
	}
	return void_const ;
}

// Diffuse Color Vector :: ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Vector3 Diffuse(Points p,Vector3 light,Vector3 lightdirec)
{
	if(p.type==1)    // Triangle
	{
		Vector3 normal=TriangleInterpolation_normalize(p.id,p.v).normalize();
		Vector3 Kd=TriangleInterpolation_diffuse(p.id,p.v);
		double cos=light.vector_dot_product(normal) > 0.0 ? light.vector_dot_product(normal) : 0.0 ;
        Vector3 Diffuse(lightdirec.x*Kd.x*cos,lightdirec.y*Kd.y*cos,lightdirec.z*Kd.z*cos);
		return Diffuse;
	}
	else if(p.type==2)     // Sphere
	{
        Vector3 center(spheres[p.id].position[0],spheres[p.id].position[1],spheres[p.id].position[2]);
	    Vector3 normal=(p.v.vector_subtraction(center)).scalar_product(1.0/spheres[p.id].radius).normalize();
		double cos=light.vector_dot_product(normal)>0.0 ? light.vector_dot_product(normal) : 0.0 ;
		Vector3 Kd(spheres[p.id].color_diffuse[0],spheres[p.id].color_diffuse[1],spheres[p.id].color_diffuse[2]);
		Vector3 Diffuse(lightdirec.x*Kd.x*cos,lightdirec.y*Kd.y*cos,lightdirec.z*Kd.z*cos);
		return Diffuse;
	}
}

// Specular Color Vector :: ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Vector3 Specular(Points p,Vector3 light,Vector3 Iightdirec,Ray r)
{
	if(p.type==1)   // Triangle
	{
       // Shine interpolate :
        Vector3 temp1(triangles[p.id].v[0].position[0],triangles[p.id].v[0].position[1],triangles[p.id].v[0].position[2]);
	    Vector3 temp2(triangles[p.id].v[1].position[0],triangles[p.id].v[1].position[1],triangles[p.id].v[1].position[2]);
	    Vector3 temp3(triangles[p.id].v[2].position[0],triangles[p.id].v[2].position[1],triangles[p.id].v[2].position[2]);
   
        Vector3 direc1=temp3.vector_subtraction(temp2);
	    Vector3 direc2=p.v.vector_subtraction(temp1);
	    
        double anst1=(((temp1.vector_cross_product(direc2)).vector_subtraction(temp2.vector_cross_product(direc2))).mod())/((direc1.vector_cross_product(direc2)).mod());
	    double anst2=(((temp2.vector_cross_product(direc1)).vector_subtraction(temp1.vector_cross_product(direc1))).mod())/((direc2.vector_cross_product(direc1)).mod());
	    double val =triangles[p.id].v[2].shininess*anst1 + triangles[p.id].v[1].shininess*(1.0-anst1);
	    double shine =triangles[p.id].v[0].shininess*(anst2-1.0)/anst2+val*(1.0/anst2);
	
        Vector3 normal=TriangleInterpolation_normalize(p.id,p.v);
		Vector3 reflection=((normal.scalar_product(normal.vector_dot_product(light))).scalar_product(2.0)).vector_subtraction(light).normalize();
		Vector3 vec=r.P1.vector_subtraction(r.P2).normalize();
		Vector3 Ks=TriangleInterpolation_diffuse(p.id,p.v);             // Specular Coefficient
   	    double cos= vec.vector_dot_product(reflection) > 0.0 ? vec.vector_dot_product(reflection):0.0 ;
        Vector3 Specular(Iightdirec.x*Ks.x*pow(cos,shine),Iightdirec.y*Ks.y*pow(cos,shine),Iightdirec.z*Ks.z*pow(cos,shine));
		return Specular;
	}
	else if(p.type==2)
	{
		Vector3 center(spheres[p.id].position[0],spheres[p.id].position[1],spheres[p.id].position[2]);
	    Vector3 normal=(p.v.vector_subtraction(center)).scalar_product(1.0/spheres[p.id].radius).normalize();
		Vector3 reflection=(normal.scalar_product(normal.vector_dot_product(light)).scalar_product(2.0)).vector_subtraction(light).normalize();
        Vector3 dir=r.P1.vector_subtraction(r.P2).normalize();
		Vector3 Ks(spheres[p.id].color_specular[0],spheres[p.id].color_specular[1],spheres[p.id].color_specular[2]);
        double cos=dir.vector_dot_product(reflection);
        cos = cos > 0.0 ? pow(cos,spheres[p.id].shininess) : 0.0;
        Vector3 Specular(Iightdirec.x*Ks.x*cos,Iightdirec.y*Ks.y*cos,Iightdirec.z*Ks.z*cos);
		return Specular;
	}
}

// Ambient Color Vector :: ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Vector3 Ambient(Points p)
{	
    if(p.type==1)  // Triangle
    {
        Vector3 light_amb(ambient_light[0],ambient_light[1],ambient_light[2]);
		Vector3 Ka=TriangleInterpolation_diffuse(p.id,p.v);
		Vector3 Ambient(light_amb.x*Ka.x,light_amb.y*Ka.y,light_amb.z*Ka.z);
		return Ambient;
    }
    else if(p.type==2)  // Sphere
    {
        Vector3 light_amb(ambient_light[0],ambient_light[1],ambient_light[2]);
  	    Vector3 Ka(spheres[p.id].color_diffuse[0],spheres[p.id].color_diffuse[1],spheres[p.id].color_diffuse[2]);
		Vector3 Ambient(light_amb.x*Ka.x,light_amb.y*Ka.y,light_amb.z*Ka.z);
		return Ambient;
	}

}

// Ray Tracing Recursively :: ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Vector3 ray_tracing(Ray r,int depth)
{
    Vector3 color;                      // Final color to be returned and flushed on screen.
	double smallest = 9999999,temp;
    Vector3 P1xyz(r.P1.x,r.P1.y,r.P1.z);
	Vector3 P2xyz(r.P2.x,r.P2.y,r.P2.z);

    Points p;
	p.id=0;
    p.r=r;
//Triangle Intersection :

	for(int i=0;i<num_triangles;i++)
	{
		temp=TriangleIntersectionDetection(r,i);
		if(temp<smallest && temp!=void_const)
		{
			smallest=temp;
			Vector3 ans(P1xyz.x+temp*(P2xyz.vector_subtraction(P1xyz)).x,P1xyz.y+temp*(P2xyz.vector_subtraction(P1xyz)).y,P1xyz.z+temp*(P2xyz.vector_subtraction(P1xyz)).z);
			p.v=ans;
			p.type=1;
			p.id=i;
		}
	}

// Sphere Intersection : 
	for(int i=0;i<num_spheres;i++)
	{
		temp=SphereIntersectionDetection(r,i);
        if(temp<smallest && temp!=void_const)
		{
			smallest=temp;
			Vector3 ans(P1xyz.x+temp*(P2xyz.vector_subtraction(P1xyz)).x,P1xyz.y+temp*(P2xyz.vector_subtraction(P1xyz)).y, P1xyz.z+temp*(P2xyz.vector_subtraction(P1xyz)).z);
			p.v=ans;
			p.type=2;
			p.id=i;
		}
	}


// Color :
     Vector3 color_addition(0.0,0.0,0.0);
	if(p.type>0)          // Is Triangle or Sphere. 
	{
	   color=Ambient(p);
       for(int i=0;i<num_lights;i++)
	   {
		  Vector3 light1(lights[i].position[0],lights[i].position[1],lights[i].position[2]);
		  Ray temp_ray(p.v,light1);
          int s  = 1;
          double temp1;
	      for(int k=0;k<num_triangles;k++)
      	  {
		     if(p.type != 1 || p.id!=k)
		     {
			    temp1=TriangleIntersectionDetection(temp_ray,k);
			    if(temp1!=void_const && temp1<1.0 && temp1>0.0) 
			      s = 0;
		     }
	      }

	      for(int k=0;k<num_spheres;k++)
	      {
		    temp1=SphereIntersectionDetection(temp_ray,k);
		    if(temp1!=void_const && temp1<1.0 && temp1>0.0)
		      s = 0;
	      } 

		  Vector3 Iightc(lights[i].color[0],lights[i].color[1],lights[i].color[2]);
		  Vector3 light2=light1.vector_subtraction(p.v).normalize();
		  color=color.vector_addition((Diffuse(p,light2,Iightc).vector_addition(Specular(p,light2,Iightc,r))).scalar_product(s));
       }

	  Vector3 R=p.r.P1.vector_subtraction(p.r.P2).normalize();
	  Vector3 normal;

//Reflection :

	  if(p.type==1)  // Triangle
		normal=TriangleInterpolation_normalize(p.id,p.v).normalize();
	  else if(p.type==2)  // Sphere
      {
        Vector3 center(spheres[p.id].position[0],spheres[p.id].position[1],spheres[p.id].position[2]);
	    normal = (p.v.vector_subtraction(center)).scalar_product(1.0/spheres[p.id].radius).normalize();
	  }
  
    // Specular Coefficient :
	   Vector3 reflect=((normal.scalar_product(normal.vector_dot_product(R))).scalar_product(2.0)).vector_subtraction(R);
	   Ray reflected_Ray(p.v,p.v.vector_addition(reflect));
       Vector3 ks;
       if(p.type==1)  // Triangle
		 ks = TriangleInterpolation_specular(p.id,p.v);
	   else if(p.type==2)  // Sphere
	   {
		 Vector3 temporary(spheres[p.id].color_specular[0],spheres[p.id].color_specular[1],spheres[p.id].color_specular[2]);
         ks = temporary;
	   }
        
 	   if(depth<Global_D && ks.x>0.0 && ks.y>0.0 && ks.z>0.0)
	   {
		  color_addition=ray_tracing(reflected_Ray,depth+1);
		  color_addition.x=color_addition.x*pow(ks.x,depth);
		  color_addition.y=color_addition.x*pow(ks.y,depth);
		  color_addition.z=color_addition.x*pow(ks.z,depth);
		  color.x=color.x*(1-ks.x);
		  color.y=color.y*(1-ks.y);
		  color.z=color.z*(1-ks.z);
 	   }
       color=color.vector_addition(color_addition);
	}
	else
			color.x=color.y=color.z=0.0;  
    return color;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void plot_pixel_display(int x,int y,unsigned char r,unsigned char g,unsigned char b);
void plot_pixel_jpeg(int x,int y,unsigned char r,unsigned char g,unsigned char b);
void plot_pixel(int x,int y,unsigned char r,unsigned char g,unsigned char b);



//MODIFY THIS FUNCTION
void draw_scene()
{
      double pi = 3.141592653589793;
	  float dh=2*tan((fov/2)*pi/180)/HEIGHT;
   	  float dw=2*tan((fov/2)*pi/180)/WIDTH;
	  Vector3 x1,x2;
	  x1.x = x1.y = x1.z = 0.0, x2.z = -1;
      for(int i=0; i < WIDTH; i++)
	  {
        glBegin(GL_POINTS);
	    x2.x = i*dw - tan((fov/2)*pi/180);
	    for(int j=0; j < HEIGHT; j++)
	    {
              x2.y = j*dh - tan((fov/2)*pi/180);
		      Ray r(x1,x2);
              Vector3 color; 
		      color = ray_tracing(r,1);  //Ray Tracing.
              if(color.x == 0.0 && color.y ==0.0 && color.z == 0.0)
                color.x = color.y = color.z = 0.77;                     // BACKGROUND COLOR.
              glPointSize(1.0);
              glColor3f(((double)((int)(color.x*255)%256))/256.f,((double)((int)(color.y*255)%256))/256.f,((double)((int)(color.z*255)%256))/256.f);
              glVertex2i(i,j);
	    }
	    glEnd();
	    glFlush();
	  }
}

void plot_pixel_display(int x,int y,unsigned char r,unsigned char g,unsigned char b)
{
  glColor3f(((double)r)/256.f,((double)g)/256.f,((double)b)/256.f);
  glVertex2i(x,y);
}

void plot_pixel_jpeg(int x,int y,unsigned char r,unsigned char g,unsigned char b)
{
  //buffer[HEIGHT-y-1][x][0]=r;
  //buffer[HEIGHT-y-1][x][1]=g;
  //buffer[HEIGHT-y-1][x][2]=b;
}

void plot_pixel(int x,int y,unsigned char r,unsigned char g, unsigned char b)
{
  plot_pixel_display(x,y,r,g,b);
  if(mode == MODE_JPEG)
      plot_pixel_jpeg(x,y,r,g,b);
}

void save_jpg()
{
  /*Pic *in = pic_alloc(640, 480, 3, NULL);
  printf("Saving JPEG file: %s\n", filename);

  memcpy(in->pix,buffer,3*WIDTH*HEIGHT);

  if (jpeg_write(filename, in))
    printf("File saved Successfully\n");
  else
    printf("Error in Saving\n");

  pic_free(in);
*/
}


void parse_check(char *expected,char *found)
{
  if(strcasecmp(expected,found))
    {
      char error[100];
      printf("Expected '%s ' found '%s '\n",expected,found);
      printf("Parse error, abnormalize abortion\n");
      exit(0);
    }

}

void parse_doubles(FILE*file, char *check, double p[3])
{
  char str[100];
  fscanf(file,"%s",str);
  parse_check(check,str);
  fscanf(file,"%lf %lf %lf",&p[0],&p[1],&p[2]);
  printf("%s %lf %lf %lf\n",check,p[0],p[1],p[2]);
}

void parse_rad(FILE*file,double *r)
{
  char str[100];
  fscanf(file,"%s",str);
  parse_check("rad:",str);
  fscanf(file,"%lf",r);
  printf("rad: %f\n",*r);
}

void parse_shi(FILE*file,double *shi)
{
  char s[100];
  fscanf(file,"%s",s);
  parse_check("shi:",s);
  fscanf(file,"%lf",shi);
  printf("shi: %f\n",*shi);
}

int loadScene(char *argv)
{
  FILE *file = fopen(argv,"r");
  int number_of_types;
  char type[50];
  int i;
  Triangle t;
  Sphere s;
  Light l;
  fscanf(file,"%i",&number_of_types);

  printf("number of types: %i\n",number_of_types);
  char str[200];

  parse_doubles(file,"amb:",ambient_light);

  for(i=0;i < number_of_types;i++)
    {
      fscanf(file,"%s\n",type);
      printf("%s\n",type);
      if(strcasecmp(type,"triangle")==0)
	{

	  printf("found triangle\n");
	  int j;

	  for(j=0;j < 3;j++)
	    {
	      parse_doubles(file,"pos:",t.v[j].position);
	      parse_doubles(file,"nor:",t.v[j].normalize);
	      parse_doubles(file,"dif:",t.v[j].color_diffuse);
	      parse_doubles(file,"spe:",t.v[j].color_specular);
	      parse_shi(file,&t.v[j].shininess);
	    }

	  if(num_triangles == MAX_TRIANGLES)
	    {
	      printf("too many triangles, you should increase MAX_TRIANGLES!\n");
	      exit(0);
	    }
	  triangles[num_triangles++] = t;
      int i = num_triangles - 1;
      Vector3 temp1(triangles[i].v[0].position[0],triangles[i].v[0].position[1],triangles[i].v[0].position[2]);
	  Vector3 temp2(triangles[i].v[1].position[0],triangles[i].v[1].position[1],triangles[i].v[1].position[2]);
	  Vector3 temp3(triangles[i].v[2].position[0],triangles[i].v[2].position[1],triangles[i].v[2].position[2]);
	  Vector3 temp4=temp2.vector_subtraction(temp1);
	  Vector3 temp5=temp3.vector_subtraction(temp1);
	  Vector3 n=temp5.vector_cross_product(temp4).normalize();
      triangles[i].tp.a=n.x,triangles[i].tp.b=n.y,triangles[i].tp.c=n.z,triangles[i].tp.d=-temp1.vector_dot_product(n);
	}
    else if(strcasecmp(type,"sphere")==0)
	{
	  printf("found sphere\n");

	  parse_doubles(file,"pos:",s.position);
	  parse_rad(file,&s.radius);
	  parse_doubles(file,"dif:",s.color_diffuse);
	  parse_doubles(file,"spe:",s.color_specular);
	  parse_shi(file,&s.shininess);

	  if(num_spheres == MAX_SPHERES)
	    {
	      printf("too many spheres, you should increase MAX_SPHERES!\n");
	      exit(0);
	    }
	  spheres[num_spheres++] = s;
	}
      else if(strcasecmp(type,"light")==0)
	{
	  printf("found light\n");
	  parse_doubles(file,"pos:",l.position);
	  parse_doubles(file,"col:",l.color);

	  if(num_lights == MAX_LIGHTS)
	    {
	      printf("too many lights, you should increase MAX_LIGHTS!\n");
	      exit(0);
	    }
	  lights[num_lights++] = l;
	}
      else
	{
	  printf("unknown type in scene description:\n%s\n",type);
	  exit(0);
	}
    }
  return 0;
}

void display()
{

}

void init()
{
  glMatrixMode(GL_PROJECTION);
  glOrtho(0, WIDTH, 0, HEIGHT,1,-1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT);
}

void idle()
{
  //hack to make it only draw once
  static int once=0;
  if(!once)
      draw_scene();
  once=1;
}


int main (int argc, char ** argv)
{
  if (argc < 2 || argc > 2)
  {  
    printf ("usage: ./Raytracer <scenefile> \n");
    exit(0);
  }
  mode = MODE_DISPLAY; 
  Global_D = 3;            // Setting Depth of Recursion Ray Tracing.
  glutInit(&argc,argv);
  loadScene(argv[1]);
  glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
  glutInitWindowPosition(0,0);
  glutInitWindowSize(WIDTH,HEIGHT);
  int window = glutCreateWindow("Ray Tracer");
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  init();
  glutMainLoop();
}
