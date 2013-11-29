#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include<GL/gl.h>
#include<GL/glut.h>      
#include<GL/glu.h>              
#define width 840
#define height 680  
#define UINTSIZE sizeof(unsigned int)
#define FLOATSIZE sizeof(float)
using namespace std;


//////////////////////////////////////////  Some of Functions signatures /////////////////////////////////////////////

void global_transpose(GLfloat x,GLfloat y,GLfloat z);
void LookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble centerx,GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy, GLdouble upz);
float RandomFloat(float a, float b);
void global_Rotation(GLfloat theta,GLfloat x,GLfloat y,GLfloat z);
void global_Scale(GLfloat x, GLfloat y,GLfloat z);
void arbitarary_global_Rotation(float a,float b,float c,float u,float v,float w);

/*----------------------------------------------------------------//
                         Vector4
//---------------------------------------------------------------*/
class Vector4
{
 public:
   float x[4]; 
   Vector4();                         //Vector constructor.
   Vector4(float a,float b,float c,float d);   // Vector Constructor with four floats.
   Vector4(float obj[4]);        // Vector Constructor with float array.
   Vector4& operator+(Vector4& obj);   // Vectors addition. 
   Vector4& operator-(Vector4& obj);   // Vectors subtraction.
   void Homogenize();                // Homogenize a Vector.
   // Relational :: 
   bool operator==(Vector4& obj);    
   bool operator!=(Vector4& obj);    
   bool operator<(Vector4& obj); 
   bool operator<=(Vector4& obj);
   bool operator>(Vector4& obj);
   bool operator>=(Vector4& obj);
   
   // Cross Product :
   Vector4& Cross_Product(Vector4& g,Vector4& vc);
};

Vector4::Vector4()
{
  x[0] = 0,x[1] = 0,x[2] = 0,x[3] = 0,x[4] = 0;
}

Vector4::Vector4(float a, float b,float c,float d)
{
  x[0] = a,x[1] = b,x[2] = c,x[3] = d;
}

void Vector4::Homogenize()
{  
   x[0] = x[0]/x[3], x[1] = x[1]/x[3], x[2] = x[2]/x[3], x[3] = 1.0f; 
}

Vector4::Vector4(float obj[4])
{
  for(int i = 0;i<=3;i++)
      x[i] = obj[i];
}
Vector4& Vector4::operator+(Vector4& obj)
{
  for(int i = 0;i<=3;i++)
     x[i]+=obj.x[i];
  return *this;
}

Vector4& Vector4::operator-(Vector4& obj)
{
  for(int i = 0;i<=3;i++)
     x[i]-=obj.x[i];
  return *this;
}

bool Vector4::operator==(Vector4& obj)
{
  for(int i=0;i<=3;i++)
     if(x[i]!=obj.x[i])
        return false;
  return true;
}
bool Vector4::operator!=(Vector4& obj)
{
  for(int i=0;i<=3;i++)
     if(x[i] == obj.x[i])
       return false;
  return true;
}

bool Vector4::operator<(Vector4& obj)
{
  for(int i = 0;i<=3;i++)
     if(x[i]>=obj.x[i])
       return false;
  return true;
}
bool Vector4::operator<=(Vector4& obj)
{
  for(int i = 0;i<=3;i++)
     if(x[i]>obj.x[i])
       return false;
  return true;
}
bool Vector4::operator>=(Vector4& obj)
{
  for(int i = 0;i<=3;i++)
     if(x[i]<obj.x[i])
       return false;
  return true;
}
bool Vector4::operator>(Vector4& obj)
{
  for(int i = 0;i<=3;i++)
     if(x[i]<=obj.x[i])
       return false;
  return true;
}

Vector4& Vector4::Cross_Product(Vector4& g,Vector4& vc)
{
 float a1 = g.x[0],a2 = g.x[1],a3 = g.x[2];
 float b1 = vc.x[0],b2 = vc.x[1],b3 = vc.x[2];
 Vector4 *p = new Vector4(a2*b3-a3*b2,a3*b1-a1*b3,a1*b2-a2*b1,1);
 return *p;  
}


/*----------------------------------------------------------------//
                         Matrix4
//---------------------------------------------------------------*/
class Matrix4
{
  public:
    float m[16];

    Matrix4();        // Matrix class constructor.
    Matrix4(float a,float b,float c,float d,float aa,float bb,float cc,float dd,float aaa,float bbb,float ccc,float ddd,float aaaa,float bbbb,float cccc,float dddd);          // Matrix class constructor with 16 inputs.
    Matrix4(float obj[16]);       // Matrix class constructor with array of 16 length.
    // Relational Operators in Matrix.
    Matrix4& operator+(Matrix4& obj);    
    Matrix4& operator-(Matrix4& obj);
    Matrix4& operator*(float scale);
    Vector4& operator*(Vector4& v);
    Matrix4& operator*(Matrix4& obj);
    Matrix4& Transpose();
    Matrix4& Inverse();
    float cof(float a, float b, float c,float d, float e, float f,float g, float h, float i);
    

    // Affine Transformations ::


    Matrix4& Translate(float x,float y,float z);
    Matrix4& Scale(float x,float y,float z);
    Vector4& RotateX(float theta,Vector4& v);
    Vector4& RotateY(float theta,Vector4& v);
    Vector4& RotateZ(float theta,Vector4& v);

    // Arbitary Transformation :: 


    Matrix4& Rotate(float theta,float x,float y,float z);
    
};

 
Matrix4::Matrix4()
{
  for(int i=0;i<=15;i++)
      m[i] = 0.0;
  m[0] = 1.0,m[5] = 1.0,m[10] = 1.0,m[15] = 1.0;
}

Matrix4::Matrix4(float a,float b,float c,float d,float aa,float bb,float cc,float dd,float aaa,float bbb,float ccc,float ddd,float aaaa,float bbbb,float cccc,float dddd)
{
 m[0] = a,m[1] = b,m[2] = c,m[3] = d;
 m[4] = aa,m[5] = bb,m[6] = cc,m[7] = dd;
 m[8] = aaa,m[9] = bbb,m[10] = ccc,m[11] = ddd;
 m[12] = aaaa,m[13] = bbbb,m[14] = cccc,m[15] = dddd;
}

Matrix4::Matrix4(float obj[16])
{
  for(int i = 0;i<=15;i++)
     m[i] = obj[i];
}

Matrix4& Matrix4::operator+(Matrix4& obj)
{
  for(int i =0;i<=15;i++)
     m[i] = m[i]+obj.m[i];
  return *this;
}

Matrix4& Matrix4::operator-(Matrix4& obj)
{
  for(int i = 0;i<=15;i++)
     m[i] = m[i] - obj.m[i];
  return *this;
}
 
Vector4& Matrix4::operator*(Vector4& obj)
{
   Vector4 *ans = new Vector4(obj.x[0]*m[0]+obj.x[1]*m[1]+obj.x[2]*m[2]+obj.x[3]*m[3],obj.x[0]*m[4]+obj.x[1]*m[4+1]+obj.x[2]*m[4+2]+obj.x[3]*m[4+3],obj.x[0]*m[8]+obj.x[1]*m[8+1]+obj.x[2]*m[8+2]+obj.x[3]*m[8+3],obj.x[0]*m[12]+obj.x[1]*m[12+1]+obj.x[2]*m[12+2]+obj.x[3]*m[12+3]);
   return *ans;  
}

Matrix4& Matrix4::operator*(float scale)
{
  for(int i = 0;i<=15;i++)
    m[i] = m[i]*scale;
  return *this;
}

Matrix4& Matrix4::operator*(Matrix4& obj)
{
  Matrix4 *p = new Matrix4(m[0]*obj.m[0]  + m[1]*obj.m[4]  + m[2]*obj.m[8]  + m[3]*obj.m[12],   m[0]*obj.m[1]  + m[1]*obj.m[5]  + m[2]*obj.m[9]  + m[3]*obj.m[13],   m[0]*obj.m[2]  + m[1]*obj.m[6]  + m[2]*obj.m[10]  + m[3]*obj.m[14],   m[0]*obj.m[3]  + m[1]*obj.m[7]  + m[2]*obj.m[11]  + m[3]*obj.m[15],m[4]*obj.m[0]  + m[5]*obj.m[4]  + m[6]*obj.m[8]  + m[7]*obj.m[12],   m[4]*obj.m[1]  + m[5]*obj.m[5]  + m[6]*obj.m[9]  + m[7]*obj.m[13],   m[4]*obj.m[2]  + m[5]*obj.m[6]  + m[6]*obj.m[10]  + m[7]*obj.m[14],   m[4]*obj.m[3]  + m[5]*obj.m[7]  + m[6]*obj.m[11]  + m[7]*obj.m[15], m[8]*obj.m[0]  + m[9]*obj.m[4]  + m[10]*obj.m[8] + m[11]*obj.m[12],  m[8]*obj.m[1]  + m[9]*obj.m[5]  + m[10]*obj.m[9] + m[11]*obj.m[13],  m[8]*obj.m[2]  + m[9]*obj.m[6]  + m[10]*obj.m[10] + m[11]*obj.m[14],  m[8]*obj.m[3]  + m[9]*obj.m[7]  + m[10]*obj.m[11] + m[11]*obj.m[15], m[12]*obj.m[0] + m[13]*obj.m[4] + m[14]*obj.m[8] + m[15]*obj.m[12],  m[12]*obj.m[1] + m[13]*obj.m[5] + m[14]*obj.m[9] + m[15]*obj.m[13],  m[12]*obj.m[2] + m[13]*obj.m[6] + m[14]*obj.m[10] + m[15]*obj.m[14],  m[12]*obj.m[3] + m[13]*obj.m[7] + m[14]*obj.m[11] + m[15]*obj.m[15]);
  return *p;
  
}

Matrix4& Matrix4::Translate(float x,float y,float z)
{
  Matrix4 obj(1,0,0,x,0,1,0,y,0,0,1,z,0,0,0,1);  
  Matrix4 obj1(m[0],m[1],m[2],m[3],m[4],m[5],m[6],m[7],m[8],m[9],m[10],m[11],m[12],m[13],m[14],m[15]);
  Matrix4 *obj2 = new Matrix4(obj * obj1);
  return *obj2;
}

Matrix4& Matrix4::Scale(float x,float y,float z)
{
  Matrix4 obj(x,0,0,0,0,y,0,0,0,0,z,0,0,0,0,1);
  Matrix4 obj1(m[0],m[1],m[2],m[3],m[4],m[5],m[6],m[7],m[8],m[9],m[10],m[11],m[12],m[13],m[14],m[15]);
  Matrix4 *obj2 = new Matrix4(obj * obj1);
  return *obj2;
}

Vector4& Matrix4::RotateX(float theta,Vector4& v)
{
  Matrix4 obj(1,0,0,0,0,cosf(theta*(3.141/180.0)),-1*sinf(theta*(3.141/180.0)),0,0,sinf(theta*(3.141/180.0)),cosf(theta*(3.141/180.0)),0,0,0,0,1);
   Vector4 *p = new Vector4(obj * v);
  return *p;
}

Vector4&  Matrix4::RotateY(float theta,Vector4& v)
{
  Matrix4 obj(cosf(theta*(3.141/180.0)),0,sinf(theta*(3.141/180.0)),0,0,1,0,0,-1*sinf(theta*(3.141/180.0)),0,cosf(theta*(3.141/180.0)),0,0,0,0,1);
  Vector4 *p = new Vector4(obj*v); 
  return *p;
}

Vector4&  Matrix4::RotateZ(float theta,Vector4& v)
{
  Matrix4 obj(cosf(theta*(3.141/180.0)),-1*sinf(theta*(3.141/180.0)),0,0,sinf(theta*(3.141/180.0)),cosf(theta*(3.141/180.0)),0,0,0,0,1,0,0,0,0,1);
  Vector4 *p = new Vector4(obj*v); 
  return *p;

}

Matrix4& Matrix4::Rotate(float theta,float x,float y,float z)
{
   float c = cos(theta*(3.14159265358979323846/180.0));
   float s = sin(theta*(3.14159265358979323846/180.0)); 
   if(fabs(s) <10e-4 )
      s = 0;
   if(fabs(c) <10e-4 )
      c = 0;
   Matrix4 obj(x*x*(1-c)+c, x*y*(1-c)-z*s, x*z*(1-c)+y*s,0,
               x*y*(1-c)+z*s, y*y*(1-c)+c, y*z*(1-c)-x*s,0, 
               x*z*(1-c)-y*s, y*z*(1-c)+x*s, z*z*(1-c)+c,0,
                  0         ,      0       ,     0      ,1);
   Matrix4 obj1(m[0],m[1],m[2],m[3],m[4],m[5],m[6],m[7],m[8],m[9],m[10],m[11],m[12],m[13],m[14],m[15]);
  Matrix4 *obj2 = new Matrix4(obj* obj1);
  return *obj2;
              
}


Matrix4& Matrix4::Transpose()
{

   Matrix4 *obj = new Matrix4(m[0],m[4],m[8],m[12],m[1],m[5],m[9],m[13],m[2],m[6],m[10],m[14],m[3],m[7],m[11],m[15]);
   return *obj;
}

float Matrix4::cof(float a, float b, float c,float d, float e, float f,float g, float h, float i)
{
    return a * (e * i - f * h)-b * (d * i - f * g)+c * (d * h - e * g);
}

Matrix4& Matrix4::Inverse()
{
   float cof0 = cof(m[5],m[6],m[7], m[9],m[10],m[11], m[13],m[14],m[15]);
   float cof1 = cof(m[4],m[6],m[7], m[8],m[10],m[11], m[12],m[14],m[15]);
   float cof2 = cof(m[4],m[5],m[7], m[8],m[9], m[11], m[12],m[13],m[15]);
   float cof3 = cof(m[4],m[5],m[6], m[8],m[9], m[10], m[12],m[13],m[14]);
   float det = m[0] * cof0 - m[1] * cof1 + m[2] * cof2 - m[3] * cof3;
   // Four cofactors enough to calculate Determinant.
   if(det == 0 || fabs(det) <=0.0001f)   // Non - invertible.
   { 
      for(int i = 0;i<=15;i++)
         m[i] = 0;
      return *this;
   }
   float cof4 = cof(m[1],m[2],m[3], m[9],m[10],m[11], m[13],m[14],m[15]);
   float cof5 = cof(m[0],m[2],m[3], m[8],m[10],m[11], m[12],m[14],m[15]);
   float cof6 = cof(m[0],m[1],m[3], m[8],m[9], m[11], m[12],m[13],m[15]);
   float cof7 = cof(m[0],m[1],m[2], m[8],m[9], m[10], m[12],m[13],m[14]);
   float cof8 = cof(m[1],m[2],m[3], m[5],m[6], m[7], m[13],m[14],m[15]);
   float cof9 = cof(m[0],m[2],m[3], m[4],m[6], m[7], m[12],m[14],m[15]);
   float cof10= cof(m[0],m[1],m[3], m[4],m[5], m[7], m[12],m[13],m[15]);
   float cof11= cof(m[0],m[1],m[2], m[4],m[5], m[6], m[12],m[13],m[14]);
   float cof12= cof(m[1],m[2],m[3], m[5],m[6], m[7], m[9], m[10],m[11]);
   float cof13= cof(m[0],m[2],m[3], m[4],m[6], m[7], m[8], m[10],m[11]);
   float cof14= cof(m[0],m[1],m[3], m[4],m[5], m[7], m[8], m[9],m[11]);
   float cof15= cof(m[0],m[1],m[2], m[4],m[5], m[6], m[8], m[9],m[10]);
   det = 1.0f/det;  
   m[0] = cof0*det,m[1] = -1*cof4*det,m[2] = cof8*det,m[3] = -1*cof12*det,m[4] = -1*cof1*det,m[5] = cof5*det,m[6] = -1*cof9*det,m[7] = cof13*det,m[8] = cof2*det,m[9] = -1*cof6*det,m[10] = cof10*det,m[11] = -1*cof14*det,m[12] = -1*cof3*det,m[13] = cof7*det,m[14] = -1*cof11*det,m[15] = cof15*det; 
   return *this;
     
} 


/*----------------------------------------------------------------//
                         GluLookAt Equivalent
//---------------------------------------------------------------*/


void LookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble centerx,GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy, GLdouble upz)
{
    float forward[3], up[3];
    GLfloat m[16];

    forward[0] = centerx - eyex;
    forward[1] = centery - eyey;
    forward[2] = centerz - eyez;

    up[0] = upx;
    up[1] = upy;
    up[2] = upz;

    float mod  = sqrt(pow(forward[0],2) + pow(forward[1],2) + pow(forward[2],2));
    forward[0]  = forward[0]/mod;
    forward[1] = forward[1]/mod;
    forward[2] = forward[2]/mod;

    Vector4 side(1,1,1,1); 
    Vector4 forw(1,1,1,1);
    forw.x[0] = forward[0],forw.x[1] = forward[1],forw.x[2] = forward[2];
    Vector4 u(1,1,1,1); 
    u.x[0] = up[0],u.x[1] = up[1],u.x[2] = up[2];
    side = forw.Cross_Product(forw,u);

    mod  = sqrt(pow(side.x[0],2) + pow(side.x[1],2) + pow(side.x[2],2));
    side.x[0] = side.x[0]/mod,side.x[1] = side.x[1]/mod,side.x[2] = side.x[2]/mod;
   
    u = side.Cross_Product(side,forw);
   
    m[0] = side.x[0];
    m[4] = side.x[1];
    m[8] = side.x[2];
    m[12] = 0;

    m[1] = up[0];
    m[4+1] = up[1];
    m[8+1] = up[2];
    m[12+1] = 0;

    m[2] = -forward[0];
    m[4+2] = -forward[1];
    m[8+2] = -forward[2];
    m[12+2] = 0;

    m[3] = 0;
    m[4+3] = 0;
    m[8+3] =0;
    m[12+3] = 1;

   //////////////////////////////////////////////////////////////////////////
    // GLMultMatrix implementation ::
    
    GLfloat mm[16]; 
    glGetFloatv (GL_MODELVIEW_MATRIX, mm);
    Matrix4 obj(m);
    Matrix4 obj1(mm); 
    obj1 = obj*obj1;
    glLoadMatrixf(obj1.m); 
    
 ///// GL translation Implementation :: 
    GLfloat mmm[16];
    glGetFloatv (GL_MODELVIEW_MATRIX, mmm);
    Matrix4 obj2(1,0,0,-eyex,0,1,0,-eyey,0,0,1,-eyez, 0,0,0,1);
    Matrix4 obj3(mmm); 
    obj2 = obj2.Transpose();
    obj3 = obj2*obj3;
    glLoadMatrixf(obj3.m); 
}







/*----------------------------------------------------------------//
                         Mesh Rendering
//---------------------------------------------------------------*/


typedef struct {
    unsigned int id; /* index of this vertex, starting from 0 */
    float x;         /* x component */
    float y;         /* y component */
    float z;         /* z component */
} VERTEX;

typedef struct {
    VERTEX* a;       /* first vertex */
    VERTEX* b;       /* second vertex */
    VERTEX* c;       /* third vertex */
} TRIANGLE;          /* triangle face in the right-hand orientation */

typedef struct {
    char* modelName;           /* Short Description of this model */
    unsigned int numVertices;  /* Number of vertices */
    VERTEX* vertices;          /* malloc'ed array of vertices */
    unsigned int numTriangles; /* Number of triangles */
    TRIANGLE* triangles;       /* malloc'ed array of triangles */
} IFS_DATA;

IFS_DATA* load_ifs_file (const char* filename);
void print_ifs_summary(FILE* target, IFS_DATA* ifs_data);
void free_ifs_data(IFS_DATA** ifs_data);






ssize_t read_uint32(int infd, unsigned int* uint_star) {
    if (read(infd, uint_star, UINTSIZE) == UINTSIZE) {
	return UINTSIZE;
    } else {
	fprintf(stderr, "Error reading size of a uint32 field\n");
	exit(-1);
    }
}

ssize_t read_float32(int infd, float* float_star) {
    if (read(infd, float_star, FLOATSIZE) == FLOATSIZE) {
        return FLOATSIZE;
    } else {
        fprintf(stderr, "Error reading size of a float32 field\n");
        exit(-1);
    }
}

ssize_t read_string32 (int infd, char** buf) {
    unsigned int mem_len = 0;
    read_uint32(infd, &mem_len);
    *buf = (char*)realloc(*buf, mem_len);
    if (mem_len == read(infd, *buf, mem_len)) {
	return mem_len;
    }
    else {
	fprintf(stderr, "Error reading a string32 field\n");
	exit(-1);
    }
}

IFS_DATA* load_ifs_file (const char* filename) {
    int infd;
    IFS_DATA* ifs_data = NULL;
    float version;
    char* ifstag = NULL;
    unsigned int i;
    unsigned int nVertices = 0;
    unsigned int nTriangles = 0;
    unsigned int tmp_Index = 0;
    
    if ((infd = open(filename, O_RDONLY)) < 2) {
	fprintf(stderr, "Error opening an input IFS file\n");
	exit(-1);
    }

    ifs_data = (IFS_DATA*) malloc(sizeof(IFS_DATA));
    ifs_data->modelName = NULL;
    ifs_data->numVertices = 0;
    ifs_data->vertices = NULL;
    ifs_data->numTriangles = 0;
    ifs_data->triangles = NULL;
    
    read_string32(infd, &ifstag);
    if (strcmp(ifstag, "IFS") != 0) {
	fprintf(stderr, "Not IFS filetype\n");
	exit(-1);
    }
    free(ifstag); ifstag = NULL;

    read_float32(infd, &version);
    if (version != 1.0) {
	fprintf(stderr, "Invalid version number: %f\n", version);
	exit(-1);
    }

    read_string32(infd, &(ifs_data->modelName));
    
    read_string32(infd, &ifstag);
    if (strcmp(ifstag, "VERTICES") != 0) {
	fprintf(stderr, "Not IFS filetype\n");
	exit(-1);
    }
    free(ifstag); ifstag = NULL;

    read_uint32(infd, &nVertices);
    ifs_data->numVertices = nVertices;

    ifs_data->vertices = (VERTEX*) malloc(nVertices * sizeof(VERTEX));
    for (i =0; i < ifs_data->numVertices; ++i) {
	ifs_data->vertices[i].id = i;
	read_float32(infd, &((ifs_data->vertices)[i].x));
	read_float32(infd, &((ifs_data->vertices)[i].y));
	read_float32(infd, &((ifs_data->vertices)[i].z));
    }
    
    read_string32(infd, &ifstag);
    if (strcmp(ifstag, "TRIANGLES") != 0) {
	fprintf(stderr, "Not IFS filetype\n");
	exit(-1);
    }
    free(ifstag); ifstag = NULL;
	
    read_uint32(infd, &nTriangles);
    ifs_data->numTriangles = nTriangles;
    
    ifs_data->triangles = (TRIANGLE*) malloc(nTriangles * sizeof(TRIANGLE));
    for (i =0; i < ifs_data->numTriangles; ++i) {
	read_uint32(infd, &tmp_Index);
	if (tmp_Index >= nVertices) {
	    fprintf(stderr, "Invalid Vertex index\n");
	    exit(-1);
	}
	ifs_data->triangles[i].a = &((ifs_data->vertices)[tmp_Index]);
	read_uint32(infd, &tmp_Index);
	if (tmp_Index >= nVertices) {
	    fprintf(stderr, "Invalid Vertex index\n");
	    exit(-1);
	}
	ifs_data->triangles[i].b = &((ifs_data->vertices)[tmp_Index]);
	read_uint32(infd, &tmp_Index);
	if (tmp_Index >= nVertices) {
	    fprintf(stderr, "Invalid Vertex index\n");
	    exit(-1);
	}
	ifs_data->triangles[i].c = &((ifs_data->vertices)[tmp_Index]);
    }

    if (close(infd) == -1) {
	fprintf(stderr, "Error closing an input IFS file\n");
	exit(-1);
    }
	
    return ifs_data;
}

void free_ifs_data(IFS_DATA** ifs_data) {
    if (ifs_data) {
	free((*ifs_data)->modelName);
	free((*ifs_data)->vertices);
	free((*ifs_data)->triangles);
    }
    free(*ifs_data);
    *ifs_data = NULL;
}

//////////////////////////////////////////////////////////////////////////////


float **vt;                               // Storing Vertices.
int **tri;                                  // Storing Triangles.
IFS_DATA* ifs_data;
int flag = 0;
void global_translate(GLfloat x, GLfloat y,GLfloat z )  // GLTranslate Equivalent. 
{
    GLfloat m[16]; 
    glGetFloatv (GL_MODELVIEW_MATRIX, m);   // Extract Current Matrix.
    Matrix4 obj2(1,0,0,x,0,1,0,y,0,0,1,z, 0,0,0,1);
    Matrix4 obj3(m); 
    obj2 = obj2.Transpose();
    obj3 = obj2*obj3;
    glLoadMatrixf(obj3.m);          // Replace current matrix.
}

void global_Scale(GLfloat x, GLfloat y,GLfloat z) // Only scales all Object Vertices.
{
    for (int i=0; i<ifs_data->numVertices; ++i) 
    {
     vt[i][0] = vt[i][0]*x ,vt[i][1] = vt[i][1]*y ,vt[i][2] = vt[i][2]*z;
    }
    //glLoadMatrixf(obj3.m);
}
void global_ScaleA(GLfloat x, GLfloat y,GLfloat z) // GLScale equivalent only for A/a key presses.
{
    GLfloat m[16]; 
    glGetFloatv (GL_MODELVIEW_MATRIX, m);     // Extract current matrix.
    Matrix4 obj2(x,0,0,0,0,y,0,0,0,0,z,0, 0,0,0,1);
    Matrix4 obj3(m); 
    obj2 = obj2.Transpose();
    obj3 = obj2*obj3;
    glLoadMatrixf(obj3.m);           // Replace current matrix.
}

void global_Rotation(GLfloat theta,GLfloat x,GLfloat y,GLfloat z)  // Rotate all objects vertices.
{
   float c = cos(theta*(3.14159265358979323846/180.0));
   float s = sin(theta*(3.14159265358979323846/180.0)); 
   if(fabs(s) <10e-4 )
      s = 0;
   if(fabs(c) <10e-4 )
      c = 0;
     Matrix4 obj2(x*x*(1-c)+c, x*y*(1-c)-z*s, x*z*(1-c)+y*s,0,
               x*y*(1-c)+z*s, y*y*(1-c)+c, y*z*(1-c)-x*s,0, 
               x*z*(1-c)-y*s, y*z*(1-c)+x*s, z*z*(1-c)+c,0,
                  0         ,      0       ,     0      ,1);

   Vector4 req(1,1,1,1);
    for (int i=0; i<ifs_data->numVertices; ++i) 
    {
     req.x[0] = vt[i][0],req.x[1] =  vt[i][1],req.x[2] = vt[i][2];
     req = obj2 * req;
     vt[i][0] = req.x[0] ,vt[i][1] = req.x[1] ,vt[i][2] = req.x[2];
    }

}

void arbitarary_global_Rotation(float theta,float a,float b,float c,float u,float v,float w) //Rotate about u,v,w vector which passes through a,b,c point.
{
   float ct = cos(theta*(3.14159265358979323846/180.0));
   float st = sin(theta*(3.14159265358979323846/180.0));
       
   if(fabs(st) <10e-4 )
      st = 0;
   if(fabs(ct) <10e-4 )
      ct = 0;
     Matrix4 obj2(u*u+(v*v+w*w)*ct ,   u*v*(1-ct)-w*st,          u*w*(1-ct)+v*st,                          (a*(v*v+w*w)-u*(b*v+c*w))* (1-ct) +(b*w-c*v)*st,
    
     u*v*(1-ct)+w*st,             v*v+(u*u+w*w)*ct ,    v*w*(1-ct)-u*st,                       (b*(u*u+w*w)-v*(a*u+c*w))*(1-ct)+(c*u-a*w)*st,

     u*w*(1-ct)-v*st,            v*w*(1-ct)+u*st,         w*w+(u*u+v*v)*ct,                 (c*(u*u+v*v)-w*(a*u+b*v))*(1-ct)+(a*v-b*u)*st,

        0,                                      0,                                   0,                                                              1);
    
  
    GLfloat m[16];
    glGetFloatv (GL_MODELVIEW_MATRIX, m);
    Matrix4 obj3(m); 
    obj2 = obj2.Transpose();
    obj3 = obj2*obj3;
    glLoadMatrixf(obj3.m); 
  
}


void init()
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 100.0); // Perspective view.

   GLfloat mat_specular[] = { 1.0, 1.0, 1.0 };  //Specular color.
   GLfloat mat_shininess[] = { 18.0 };  // Shininess  factor.

   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);  
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
 
 // Two lights implementation ::
    glEnable(GL_NORMALIZE);
 
  GLfloat light0_diffuse[] = {1.0, 0.0, 0.0, 1.0};
  GLfloat light0_position[] = {-1.0, 0.0, 0.1, 0.0};
  GLfloat lightcolor[] = { 1.0, 0.0, 0.0 };
  glLightfv(GL_LIGHT0,GL_SPECULAR,lightcolor);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
  glEnable(GL_LIGHT0);

  GLfloat light1_diffuse[] = {0.0, 0.0, 1.0, 1.0};
  GLfloat light1_position[] = {1.0, 0.0, 0.1, 0.0};
  GLfloat lightcolor1[] = { -1.0, 0.0, 0.0 };
  glLightfv(GL_LIGHT1,GL_SPECULAR,lightcolor1);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
  glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
  glEnable(GL_LIGHT1);
  

  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);


}
 
void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glShadeModel(GL_SMOOTH);
  // Drawing Vertices :: 
  glBegin(GL_TRIANGLES);
  //glNormal3f(0.0f, 0.0f, 1.0f);
  Vector4 vec(1,1,1,1);
  Vector4 vec1(1,1,1,1);
  for (int i=0; i<ifs_data->numTriangles; ++i) 
  {
        
     int v1 = tri[i][0],v2 = tri[i][1],v3 = tri[i][2];    
     vec.x[0] = vt[v1][0] - vt[v2][0],vec.x[1] = vt[v1][1] - vt[v1][1], vec.x[2] = vt[v1][2] - vt[v2][2];
     vec1.x[0] = vt[v1][0] - vt[v3][0],vec1.x[1] = vt[v1][1] - vt[v3][1], vec1.x[2] = vt[v1][2] - vt[v3][2];
    
     vec1 = vec.Cross_Product(vec1,vec);
     float mod = sqrt( pow(vec1.x[0],2)+pow(vec1.x[1],2)+pow(vec1.x[2],2));
     vec1.x[0]/mod,vec1.x[1]/mod,vec1.x[2]/mod;
     glNormal3f(vec1.x[0], vec1.x[1], vec1.x[2]);
     glVertex3f(vt[v1][0],vt[v1][1],vt[v1][2]);
     glVertex3f(vt[v2][0],vt[v2][1],vt[v2][2]);
     glVertex3f(vt[v3][0],vt[v3][1],vt[v3][2]);
     
  }
   
   glEnd();
  glEnable(GL_NORMALIZE);

//Drawing x,y,z axis ::
   glBegin(GL_LINES);

  glColor3f(1.0,1.0,1.0);
    glVertex3f(-2.5,0, 0);
    glVertex3f(2.5,0,0);
    glVertex3f(0,-2.5, 0);
    glVertex3f(0,2.5, 0);
    glVertex3f(0,0,-2.5);
    glVertex3f(0,0, 2.5);
  glEnd();
   
   glFlush();
}

GLvoid window_key(unsigned char key, int x, int y);

float ex = 0.0f,ey =0.0f,ez = 0.0f;

 


//////// Some Transformations ///////////////////////////////////////////////


void Transformations()                //These tranformations make view more realistic.
{
      global_Scale(1.6,1.6,1.6);
      global_Rotation(10,0.0f,1.0f,0.0f); 
      glPolygonMode(GL_FRONT, GL_LINE);
      glPolygonMode(GL_BACK, GL_LINE);
      glutPostRedisplay();
      window_key('v',0,0);
      window_key('v',0,0);
      //window_key('v',0,0);
      window_key('u',0,0);
      window_key('u',0,0);
      window_key('u',0,0);
      //global_translate(0,0,0.3);
      global_Scale(1/0.9,1/0.9,1/0.9);
      glPolygonMode(GL_FRONT, GL_FILL);
      glPolygonMode(GL_BACK, GL_FILL);
      window_key('u',0,0);
     // glutPostRedisplay();
}  
   
void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   if (w <= h)
      glOrtho (-1.5, 1.5, -1.5*(GLfloat)h/(GLfloat)w,
         1.5*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
   else
      glOrtho (-1.5*(GLfloat)w/(GLfloat)h,
         1.5*(GLfloat)w/(GLfloat)h, -1.5, 1.5, -10.0, 10.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

float one[3] = {1.0f,0.0f,0.0f},two[3] = {0.0f,1.0f,0.0f},three[3] = {0.0f,0.0f,-1.0f};



GLvoid window_key(unsigned char key, int x, int y) 
{  
  Vector4 obj(0,0,-1,1);
  Matrix4 ans(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1); 
  Vector4 w(0,0,0,1);
  Vector4 u(0,0,0,1);
  Vector4 v(0,0,0,1);
  switch (key) {    

case 'l':
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
        glutPostRedisplay();
       break;

case 'L':
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glutPostRedisplay(); 
        break;

case 'B':
        glPolygonMode(GL_FRONT, GL_FILL);
        glPolygonMode(GL_BACK, GL_FILL);
        glutPostRedisplay();
        break;

case 'b':
        glPolygonMode(GL_FRONT, GL_LINE); 
        glPolygonMode(GL_BACK, GL_LINE);
        glutPostRedisplay();
        break;

case 'x':
   global_translate(-0.3,0.0,0.0); 
   ex+=0.3;
   glutPostRedisplay();
   break;

case 'X':
   global_translate(0.3,0,0);

   ex-=0.3;
   glutPostRedisplay();
   break;

case 'y':

   global_translate(0,-0.3,0);
   ey+=0.3;
   glutPostRedisplay();
   break;

case 'Y':
   global_translate(0,0.3,0);
   ey-=0.3; 
   glutPostRedisplay();
   break;

case 'Z':
   ez+=0.3;
   global_translate(0,0,0.3);
   global_Scale(1/0.7,1/0.7,1/0.7);
   glutPostRedisplay();
   break;

case 'z':
    ez-=0.3;
    global_translate(0,0,-0.3);
    global_Scale(0.7,0.7,0.7);
    glutPostRedisplay();
    break;

 case 'j':
     
    arbitarary_global_Rotation(10,ex,ey,ez-1,two[0],two[1],two[2]);
    glutPostRedisplay();  
   break;

 case 'J':
 
    arbitarary_global_Rotation(-10,ex,ey,ez-1,two[0],two[1],two[2]);
    glutPostRedisplay();  
    break;

case 'i':

    arbitarary_global_Rotation(10,ex,ey,ez-1,one[0],one[1],one[2]);
    glutPostRedisplay();
    break;

case 'I':
    arbitarary_global_Rotation(-10,ex,ey,ez-1,one[0],one[1],one[2]);
    glutPostRedisplay();   
    break;

case 'k':

    arbitarary_global_Rotation(10,ex,ey,ez-1,three[0],three[1],three[2]);
    glutPostRedisplay();   
    break;

case 'K':

    arbitarary_global_Rotation(-10,ex,ey,ez-1,three[0],three[1],three[2]);
    glutPostRedisplay();   
    break;

  case 'u':
    global_Rotation(10,1.0f,0.0f,0.0f);  
    glutPostRedisplay();
    break;

  case 'U':
    global_Rotation(-10,1.0f,0.0f,0.0f); 
    glutPostRedisplay();
    break;

  case 'v':
    global_Rotation(10,0.0f,1.0f,0.0f); 
    glutPostRedisplay();
    break;
    
  case 'V':
    global_Rotation(-10,0.0f,1.0f,0.0f); 
    glutPostRedisplay();
    break;

  case 'w':
    global_Rotation(10,0.0f,0.0f,1.0f);
    glutPostRedisplay();
    break;
  case 'W':
    global_Rotation(-10,0.0f,0.0f,1.0f);
    glutPostRedisplay();
    break;
  case 'a':
    global_ScaleA(1/0.7,1/0.7,1/0.7);
    glutPostRedisplay();
    break;
  case 'A':
    global_ScaleA(0.7,0.7,0.7);
    glutPostRedisplay();
    break;
  case 27:  
    exit(1);                      
    break;    
  case '+':
    global_Scale(1.3,1.3,1.3);
    glutPostRedisplay();
    break; 
  case '-':
    global_Scale(0.7,0.7,0.7);
    glutPostRedisplay();
    break;
  default:
    printf ("Pressing ASCII %d doesn't do anything.\n", key);
    break;
  }    
}

int main(int argc,char** argv)
{ 
  const char* ch;  
  if(argc>=2)                                  // If argument does not have input.txt file, then ask for it ....
  ch = argv[1];
  else{
  printf("\nPlease  execute as : './viewer <IFS_File_Name>.ifs'\n");
  return 0;}
  ifs_data = load_ifs_file(ch);
  unsigned int i;
 tri = new int*[ifs_data->numTriangles];
 vt = new float*[ifs_data->numVertices];
 for (i=0; i<ifs_data->numVertices; ++i) 
 { 
           vt[i] = new float[3];
           vt[i][0] = ifs_data->vertices[i].x;
           vt[i][1] = ifs_data->vertices[i].y;
           vt[i][2] = ifs_data->vertices[i].z;
}         
    for (i=0; i<ifs_data->numTriangles; ++i) 
   {
         tri[i] = new int[3];
         tri[i][0] = ifs_data->triangles[i].a->id;
         tri[i][1] = ifs_data->triangles[i].b->id;
         tri[i][2] = ifs_data->triangles[i].c->id;
  }
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (width, height);
   glutInitWindowPosition (150, 25);
   glutCreateWindow (ifs_data->modelName);
   init ();
   glutDisplayFunc(display);
   Transformations();
   glutReshapeFunc(reshape);
   glutKeyboardFunc (window_key);
   glutMainLoop();
}
