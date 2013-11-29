#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<cstring>
#include <GL/glut.h>
#include <GL/glu.h>
using namespace std;
float ex=0,ey=0,ez=0;
float px=0,py=0,pz=0.1;
float ux=0,uy=1,uz=0;
float lx,ly,lz;
FILE*g;
struct vertex{float v[3];}vertices[100000];
struct normal{float vn[3];}normals[100000];
struct texture{float vt[2];}textures[100000];
struct face
{
	//bool flag;
	int idx[3][3];
}faces[400000];
int num_v=0;
int num_vt=0;
int num_vn=0;
int num_idx=0;
void parse()
{
	char str1[10000];
	while(fgets(str1,1000,g)!=NULL)
	{
		char*str=strtok(str1,"\n");
		char*p=strtok(str," ");
		if(p==NULL) continue;
		if(!strcmp(p,"v"))
		{
			int k=0;
			while(p=strtok(NULL," "))
			{
				vertices[num_v].v[k++]=atof(p);
			}
			num_v++;
		}
		else if(!strcmp(p,"vn"))
		{
			int k=0;
			while(p=strtok(NULL," "))
			{
				normals[num_vn].vn[k++]=atof(p);
			}
			num_vn++;
		}
		else if(!strcmp(p,"vt"))
		{
			int k=0;
			while(p=strtok(NULL," "))
			{
				textures[num_vt].vt[k++]=atof(p);
			}
			num_vt++;
		}
		else if(!strcmp(p,"f"))
		{
			int k=0;char s[3][50];
			while(p=strtok(NULL," "))
			{
				strcpy(s[k],p);
				k++;
			}
			//int t;
			//if(k==3) {faces[num_idx].flag=0;t=3;}
			//else if(k==4) {faces[num_idx].flag=1;t=4;}
			for(int i=0;i<3;i++)
			{
				int m=0;
				char*q=strtok(s[i],"/");
				faces[num_idx].idx[i][m++]=atof(q);
				while(q=strtok(NULL,"/"))
				{
					faces[num_idx].idx[i][m++]=atof(q);
				}
			}
			num_idx++;
		}
		else continue;
	}
}
void print()
{
	//for(int i=0;i<num_v;i++) cout<<"v "<<vertices[i].v[0]<<" "<<vertices[i].v[1]<<" "<<vertices[i].v[2]<<endl;
	//for(int i=0;i<num_vn;i++) cout<<"vn "<<normals[i].vn[0]<<" "<<normals[i].vn[1]<<" "<<normals[i].vn[2]<<endl;
	for(int i=0;i<num_vt;i++) cout<<"vt "<<textures[i].vt[0]<<" "<<textures[i].vt[1]<<" "<<endl;
	/*for(int i=0;i<num_idx;i++) 
	{
		cout<<"f ";
		for(int j=0;j<3;j++)
		{
			for(int k=0;k<3;k++)
			{
				cout<<faces[i].idx[j][k];
				if(k<2) cout<<"/";
			}
			cout<<" ";
		}
		cout<<endl;
	}*/
}
void LookAt(float ex,float ey,float ez,float cx,float cy,float cz,float ux,float uy,float uz)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(ex,ey,ez,cx,cy,cz,ux,uy,uz);
}
void translate(float x,float y,float z)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x,y,z);
}
/*void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0,0.0,0.0,0.0);
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	for(int i=0;i<num_idx;i++)
	{
		glBegin(GL_TRIANGLES);
		glVertex3f(vertices[faces[i].idx[0][0]-1].v[0],vertices[faces[i].idx[0][0]-1].v[1],vertices[faces[i].idx[0][0]-1].v[2]);
		glVertex3f(vertices[faces[i].idx[1][0]-1].v[0],vertices[faces[i].idx[1][0]-1].v[1],vertices[faces[i].idx[1][0]-1].v[2]);
		glVertex3f(vertices[faces[i].idx[2][0]-1].v[0],vertices[faces[i].idx[2][0]-1].v[1],vertices[faces[i].idx[2][0]-1].v[2]);
		glEnd();
	}
	glFlush();
}
void key(unsigned char key, int x, int y)
{	
	switch(key)
	{
		case 27: exit(0);break;
	}
}
int main(int argc,char*argv[])
{
	fp=fopen(argv[1],"r");
	parse();
	print();
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(800,600);
	glutCreateWindow("Prateek");
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
  	glLoadIdentity();
  	glOrtho(-2,2,-2,2,-10.0,500.0);
  	//gluPerspective(60,800.0/600.0,1.0,500.0);
	glutKeyboardFunc(key);
	//glutPassiveMotionFunc(mouse);
	glutDisplayFunc(draw);
	glutMainLoop();
	return 0;
}*/
