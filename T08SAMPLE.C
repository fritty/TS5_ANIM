/* FILE NAME: T08SAMPLE.C
  * PROGRAMMER: TS5 
  * DATE: 16.06.2010
  * PURPOSE: Animation pattern.
  *          Animation sample.
  */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "TS5_anim.h"
#include "ts5_vec.h"
#include "prim.h"

#define PI 3.14159265358979323846
#define M 200
#define SCALE 1
#define SPEED 2
#define SUN 0
#define TREE 1
#define SKY 2
#define GROUND 3

                                        

ts5PRIM SkyBox, Ground, Sphere;
BOOL GRID = FALSE, FOG = FALSE;

GLdouble ex = M / 2, ey = 0, ey1 = 4, ez = M / 2, theta = 0, phi = PI / 2;


VOID MyResponse( ts5ANIM *Ani )
{ 
  BOOL is_press = FALSE;
  static BOOL is_was = FALSE;
  static GLdouble ax = 0, ay = 0, az = 3, ux = 0, uy = 1, uz = 0, cot, sit, cop, sip;
  INT n;
  

  if (Ani->Kbd[VK_F4] || Ani->Kbd[VK_SPACE] || Ani->Kbd[VK_F1] || Ani->Kbd[VK_F2])
  {
    if (!is_was)
      is_press = TRUE, is_was = TRUE;
  }
  else
    is_was = FALSE;

  if (Ani->Kbd[VK_ESCAPE])
  {      
    PostQuitMessage(0);
  } 
    

  if (is_press)
  {
    if (Ani->Kbd[VK_SPACE])
    {
      if (Ani->IsPause)
        Ani->IsPause = FALSE;
      else
        Ani->IsPause = TRUE;
    }
    if (Ani->Kbd[VK_F1])
    {
      if (GRID)
      {
        GRID = FALSE;
        glEnable(GL_LIGHTING);
      }
      else
      {
        GRID = TRUE;
        glDisable(GL_LIGHTING);
      }
    }
    if (Ani->Kbd[VK_F2])
    {
      if (FOG)
        FOG = FALSE;
      else
        FOG = TRUE;
    }
    if (Ani->Kbd[VK_F4])
    {
      if (Ani->IsFullScreen)
        TS5_SetFullScreen(FALSE);
      else
        TS5_SetFullScreen(TRUE);
    } 
  } 
  glViewport(0, 0, Ani->W, Ani->H);

  if (Ani->Jr > 0.05 || Ani->Jr < -0.05 || Ani->Jx > 0.05 || Ani->Jx < -0.05 || Ani->Jy > 0.05 || Ani->Jy < -0.05 || Ani->Jz > 0.05 || Ani->Jz < -0.05)
  {
    glMatrixMode(GL_PROJECTION);

    if ((phi > 0 && Ani->Jy < 0) || (phi < PI && Ani->Jy > 0))
      phi += (Ani->GlobalDelta) * (Ani->Jy);
    theta += (Ani->GlobalDelta) * (Ani->Jx);

    sit = sin(theta);
    cot = cos(theta);
    sip = sin(phi);
    cop = cos(phi);
    if (ex > 0 && ez > 0)
    {
      if (ex < M  && ez < M)
      {
        ex += (Ani->Jr / 5 * sit + Ani->Jz / 5 * cot) * SPEED;
        ez += (Ani->Jz / 5 * sit - Ani->Jr / 5 * cot) * SPEED;
      }
      else
      {
        ex -= 2;
        ez -= 2;
      }
    }
    else
    {
      ex += 2;
      ez += 2;
    }
    
    ey1 += (Ani->JCap[7] - Ani->JCap[4]) / 5.0 * SPEED;

    n = (INT)ez * M + (INT)ex;
    ey = Ground.V[n].Y + 
         (ex - (INT)ex) * (Ground.V[n + 1].Y - Ground.V[n].Y) + 
         (ez - (INT)ez) * (Ground.V[n + M].Y - Ground.V[n].Y) + ey1;

    ax = ex - cot * sip;
    ay = ey + cop;                    
    az = ez - sit * sip;
    glLoadIdentity();
    glFrustum(-1, 1, -1, 1, 1, 2000);
    gluLookAt(ex, ey, ez, ax, ay, az, ux, uy, uz);
    glMatrixMode(GL_MODELVIEW);     
  }
}

DBL r1()
{
  return rand() / (DBL)RAND_MAX;
}

/* Initialization */
VOID MyInit( VOID )
{
  INT i, j, n;
  FLOAT pos[4] = {10, 10, 10, 0}, dir[3] = {M, 0, M}, a, px, ty; 

  /* init light */
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0); 
  glLightfv(GL_LIGHT0, GL_POSITION, pos);
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);
  glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 50);
  glDisable(GL_LIGHTING);

   /* init texture */
  glBindTexture(GL_TEXTURE_2D, TREE);
  TS5_LoadTex("Q:\\TEX\\TREES\\TREE.g24");
  glBindTexture(GL_TEXTURE_2D, SUN);
  //TS5_LoadTransTex("E:\\SPR11\\T08ALL\\mb.g24");
  TS5_LoadTransTex("X:\\PICS\\PYATACK.G24");
  glBindTexture(GL_TEXTURE_2D, SKY);
  TS5_LoadTex("E:\\SPR11\\T08ALL\\sky2.g24");
  glBindTexture(GL_TEXTURE_2D, GROUND);
  TS5_LoadTex("E:\\SPR11\\T08ALL\\moon2.g24");
  //TS5_LoadTex("X:\\PICS\\m.g24");

  /* init ground */
  Ground = Grid(M - 1, M - 1);

  Ground.TexID = GROUND;
  for (i = 0; i < M; i++)
    for (j = 0, a = 0; j < M; j++, a += 0.1)
    {
      n = i * M + j;          

      Ground.V[n].X = j;
      Ground.V[n].Z = i;
      Ground.V[n].Y = 0;
    }  


  for (i = 0; i < M; i++)
    for (j = 0, a = 0; j < M; j++, a += 0.1)
    {       
      Ground.V[i * M + j].Y += sin(a) * 3 ;
      Ground.V[j * M + i].Y += sin(a) * 3 ;
    }


  for (i = 1; i < M; i++)    
  {
    for (j = 1; j < M; j++)
    {
      n = i * M + j;
      
                                        
      px = (atan(Ground.V[n].Y - Ground.V[n + 1].Y) + 
            atan(Ground.V[n - 1].Y - Ground.V[n].Y)) / 2;
      ty = (atan(Ground.V[n].Y - Ground.V[n + M].Y) + 
            atan(Ground.V[n - M].Y - Ground.V[n].Y)) / 2;

      Ground.N[n].Y = cos(px) * cos(ty);
      Ground.N[n].X = sin(px);
      Ground.N[n].Z = sin(ty);
      //Ground.V[j * M + i].Y += sin(a) * 3;   
    }  
  }

  /* init SkyBox */
  SkyBox = TS5_Sphere(M, M);
  SkyBox.TexID = SKY;
  /*for (i = 0, a = 0; i < M * M; i++, a += 2 * PI / M)
  {
    //glTexCoord2d(1, 1);
    SkyBox.V[i].X = cos(a);
    SkyBox.V[i].Z = sin(a);
  } */

  /* init Sphere */
  
}



VOID GetTree( GLdouble H )
{
  static BOOL is_first = TRUE;

  if (GRID)
  {
    glBegin(GL_LINES);
    glColor3d(0, 0.5, 0.2);
    glVertex3d(0, 0, 0);
    glVertex3d(0, H / 2, 0);
    
    glColor3d(0, 0.5, 0.2);

    glVertex3d(0 + H / 5, H / 2, 0);
    glVertex3d(0, H, 0);
    glVertex3d(0, H / 2, 0);

    glVertex3d(0, H / 2, 0);
    glVertex3d(0, H, 0);
    glVertex3d(0 - H / 2, H / 2, 0);

    glVertex3d(0, H / 2, 0 + H / 5);
    glVertex3d(0, H, 0);
    glVertex3d(0, H / 2, 0);

    glVertex3d(0, H / 2, 0);
    glVertex3d(0, H, 0);
    glVertex3d(0, H / 2, 0 - H / 5);

    glEnd();
  }
  else
  {  
    glPushMatrix();
   // glTranslated(50, 0, 0);

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, TREE);

    glBegin(GL_QUADS);

    glTexCoord2d(1, 1);
    glVertex3d(0, 0, - H / 2);
    glTexCoord2d(1, 0);
    glVertex3d(0, H, - H / 2);
    glTexCoord2d(0, 0);
    glVertex3d(0, H, H / 2);
    glTexCoord2d(0, 1);
    glVertex3d(0, 0, H / 2);


    glEnd();
    glDisable(GL_TEXTURE_2D);
  }
  glPopMatrix();
}


/* Draw sky */
VOID GetSky( DBL Time )
{  
  static BOOL is_first = TRUE;
  INT i, j, n;

  glDepthMask(0);
  glPushMatrix();
  glTranslated(ex, ey, ez);
  glScaled(100, 100, 100);
  glRotated(Time, 0, 0, 1);

  
  if (GRID)
  {
    glBegin(GL_LINES);
    glColor3d(0, 0.5, 0);
    for (i = 0, n = 0; i < M; i++)
    {
      for (j = 0; j <= M; j++, n++)
      {
        glVertex3d(SkyBox.V[n].X, SkyBox.V[n].Y, SkyBox.V[n].Z);
        glVertex3d(SkyBox.V[n + 1].X, SkyBox.V[n + 1].Y, SkyBox.V[n + 1].Z);

        glVertex3d(SkyBox.V[n].X, SkyBox.V[n].Y, SkyBox.V[n].Z);
        glVertex3d(SkyBox.V[n + M  + 1].X, SkyBox.V[n + M + 1].Y, SkyBox.V[n + M + 1].Z);

        //glVertex3d(SkyBox.V[j * M + i].X, SkyBox.V[j * M + i].Y, SkyBox.V[j * M + i].Z);
        //glVertex3d(SkyBox.V[(j + 1) * M + i].X, SkyBox.V[(j + 1) * M + i].Y, SkyBox.V[(j + 1) * M + i].Z);
      }
    }
    glEnd();
  }
  else
  {
    glDisable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, SKY);
    TS5_DrawPrim(&SkyBox);
   /* glBegin(GL_QUADS);
      glTexCoord2d(0, 0);
      glVertex2d(0, 0);

      glTexCoord2d(1, 0);
      glVertex2d(1, 0);

      glTexCoord2d(1, 1);
      glVertex2d(1, 1);

      glTexCoord2d(0, 1);
      glVertex2d(0, 1);
    glEnd();              */
    glEnable(GL_LIGHTING);
  }
  
  /*
  if (GRID)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
  {
    glDisable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
  TS5_DrawPrim(&SkyBox);
  
  if (!GRID)
    glEnable(GL_LIGHTING);
    */
  glPopMatrix();
  glDepthMask(1);
}


/* Draw ground */
VOID GetGround( ts5ANIM *Ani )                        
{                               
  static INT i, j, n;
  static DBL a;
                           
 
  if (GRID)
  {
    glPushMatrix();
    glBegin(GL_LINES);
    glColor3d(0.1, 0.6, 0.1);
    for (i = 0; i < M - 1; i++)
    {
      for (j = 0; j < M - 1; j++)
      {
        glVertex3d(Ground.V[i * M + j].X, Ground.V[i * M + j].Y, Ground.V[i * M + j].Z);
        glVertex3d(Ground.V[i * M + j + 1].X, Ground.V[i * M + j + 1].Y, Ground.V[i * M + j + 1].Z);
    
        glVertex3d(Ground.V[j * M + i].X, Ground.V[j * M + i].Y, Ground.V[j * M + i].Z);
        glVertex3d(Ground.V[(j + 1) * M + i].X, Ground.V[(j + 1) * M + i].Y, Ground.V[(j + 1) * M + i].Z);

        glVertex3d(Ground.V[j * M + i].X, Ground.V[j * M + i].Y, Ground.V[j * M + i].Z);
        glVertex3d(Ground.N[j * M + i].X + Ground.V[j * M + i].X, 
                   Ground.N[j * M + i].Y + Ground.V[j * M + i].Y,
                   Ground.N[j * M + i].Z + Ground.V[j * M + i].Z);
      }
    }
    
    glEnd();
    glPopMatrix();
  }
  else                                     
  {     
    TS5_DrawPrim(&Ground);
   /* glPushMatrix();
    glBegin(GL_QUADS);
    for (i = 0; i < M - 1; i++)
    {
      for (j = 0; j < M - 1; j++)
      {
        glColor3d(0, (Ground.V[(i + 1) * M + j + 1].Y - Ground.V[i * M + j].Y) / 3 + 0.5, 0);

        glVertex3d(Ground.V[i * M + j].X, Ground.V[i * M + j].Y, Ground.V[i * M + j].Z);
        glVertex3d(Ground.V[i * M + j + 1].X, Ground.V[i * M + j + 1].Y, Ground.V[(i) * M + j + 1].Z);
        glVertex3d(Ground.V[(i + 1) * M + j + 1].X, Ground.V[(i + 1) * M + j + 1].Y, Ground.V[(i + 1) * M + j + 1].Z);
        glVertex3d(Ground.V[(i + 1) * M + j].X, Ground.V[(i + 1) * M + j].Y, Ground.V[(i + 1) * M + j].Z);
      }
    } */
   /* for (i = 0; i < M; i++)
    {
      for (j = 0; j < M - 1; j++)
      {
        glVertex3d(Ground.V[j * M + i].X, Ground.V[j * M + i].Y, Ground.V[j * M + i].Z);
        glVertex3d(Ground[j+1][i].X, Ground[j+1][i].Y, Ground[j+1][i].Z);
      }
    }  */
    glEnd();
    glPopMatrix();
  }

}


VOID GetFog( VOID )
{
  float fog[4] = { 0.1, 0.5, 0.7, 1 };

  glMatrixMode(GL_PROJECTION);
  glEnable(GL_FOG);
  glFogf(GL_FOG_MODE, GL_LINEAR);
  glFogf(GL_FOG_START, ez);
  glFogf(GL_FOG_END, ez + 100);
  glFogfv(GL_FOG_COLOR, fog);
  glMatrixMode(GL_MODELVIEW);

}


VOID GetSun( VOID )
{
  static BOOL is_first = TRUE;
  FLOAT pos[4] = {0, 0, 0, 1}, dir[3],
        amb[4] = {0.01, 0, 0.1, 0.1}, dif[4] = {1, 1, 1, 1}, spec[4] = {0.1, 0.1, 0.1, 0.1},
        samb[4] = {0.01, 0, 0.01, 1},
        oamb[4] = {1, 1, 1, 1}, odif[4] = {1, 1, 1, 1}, ospec[4] = {1, 1, 1, 1};

  glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
  glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, samb);

  glPushMatrix();
  //glDepthMask(0);
  glTranslated(M / 2, 40, M /2);
  glRotated(-theta * 180 / PI, 0, 1, 0);

  pos[0] = 1;
  pos[1] = 0;
  pos[2] = 1;
  dir[0] = pos[0];
  dir[1] = pos[1];
  dir[2] = pos[2];
  glLightfv(GL_LIGHT0, GL_POSITION, pos);
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);

  if (!GRID)
  {
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    ///glDisable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 1 - 1 / 256.);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);
   
    glBindTexture(GL_TEXTURE_2D, SUN);

    glBegin(GL_QUADS);

    glColor3d(1, 1, 1);

    glTexCoord2d(1, 1);
    glVertex3d(0, -5, -5);
    glTexCoord2d(1, 0);
    glVertex3d(0, 5, - 5);
    glTexCoord2d(0, 0);
    glVertex3d(0, 5, 5);
    glTexCoord2d(0, 1);
    glVertex3d(0, -5, 5);

    glEnd();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_ALPHA_TEST);
  }
  else
  {
    glBegin(GL_LINE_STRIP);
    glColor3d(0, 1, 0);
    glVertex3d(0, -5, - 5);             
    glVertex3d(0, 5, - 5);         
    glVertex3d(0, 5, 5);   
    glVertex3d(0, -5, 5);
    glVertex3d(0, -5, - 5);
    glEnd();
  }
 // glDepthMask(1);
  glPopMatrix();
}


/* Draw house */
VOID GetHouse( VOID )
{
  ts5PRIM p;
  INT i, j;
  p = TS5_Sphere(100, 100);

  glPushMatrix();

  glTranslated(M / 2, 0, M / 2);
  glScaled(10, 10, 10);

  if (GRID)
  {
    glBegin(GL_LINES);
    glColor3d(0.1, 0.6, 0.1);
    for (i = 0; i < 100; i++)
    {
      for (j = 0; j < 100; j++)
      {
        glVertex3d(p.V[i * 100 + j].X, p.V[i * 100 + j].Y, p.V[i * 100 + j].Z);
        glVertex3d(p.V[i * 100 + j + 1].X, p.V[i * 100 + j + 1].Y, p.V[i * 100 + j + 1].Z);
    
        glVertex3d(p.V[j * 100 + i].X, p.V[j * 100 + i].Y, p.V[j * 100 + i].Z);
        glVertex3d(p.V[(j + 1) * 100 + i].X, p.V[(j + 1) * 100 + i].Y, p.V[(j + 1) * 100 + i].Z);

        glVertex3d(p.V[j * 100 + i].X, p.V[j * 100 + i].Y, p.V[j * 100 + i].Z);
        glVertex3d(p.N[j * 100 + i].X + p.V[j * 100 + i].X, 
                   p.N[j * 100 + i].Y + p.V[j * 100 + i].Y,
                   p.N[j * 100 + i].Z + p.V[j * 100 + i].Z);
      }
    }
    
    glEnd();
  }
  else
    TS5_DrawPrim(&p);


  glPopMatrix();
}


VOID GetBird( DBL Time )
{
  VEC dot[3];
  DBL s = sin(sin(Time) / 2) / 2, c = cos(sin(Time) / 2),
      s1 = s + 0.4 + 0.8 * sin(sin(Time + 5) / 4), c1 = c + cos(sin(Time + 5) / 2);

  dot[0].X = 0.3;
  dot[0].Y = -s / 2;
  dot[0].Z = 0;

  dot[1].X = 0.2;
  dot[1].Y = s + 0.7;
  dot[1].Z = c;

  dot[2].X = 0;
  dot[2].Y = s1;
  dot[2].Z = c1;

  if (GRID)
  {
    glBegin(GL_LINE_STRIP);

    glVertex3d(dot[0].X, dot[0].Y, dot[0].Z);    
    glVertex3d(dot[1].X, dot[1].Y, dot[1].Z);     
    glVertex3d(dot[2].X, dot[2].Y, dot[2].Z);
    glVertex3d(-dot[1].X, dot[1].Y, dot[1].Z);
    glVertex3d(-dot[0].X, dot[0].Y, dot[0].Z); 
    glVertex3d(-dot[1].X, dot[1].Y, -dot[1].Z);
    glVertex3d(-dot[2].X, dot[2].Y, -dot[2].Z);
    glVertex3d(dot[1].X, dot[1].Y, -dot[1].Z);
    glVertex3d(dot[0].X, dot[0].Y, dot[0].Z);

    glEnd();
  }
  else
  {
    glBegin(GL_POLYGON);
    glVertex3d(dot[0].X, dot[0].Y, dot[0].Z);
    glVertex3d(-dot[0].X, dot[0].Y, dot[0].Z);
    glVertex3d(-dot[1].X, dot[1].Y, dot[1].Z);     
    glVertex3d(dot[2].X, dot[2].Y, dot[2].Z);
    glVertex3d(dot[1].X, dot[1].Y, dot[1].Z);
    glVertex3d(dot[0].X, dot[0].Y, dot[0].Z);

    glEnd();
    glBegin(GL_POLYGON);

    glVertex3d(-dot[0].X, dot[0].Y, dot[0].Z);
    glVertex3d(dot[0].X, dot[0].Y, dot[0].Z);
    glVertex3d(dot[1].X, dot[1].Y, -dot[1].Z);
    glVertex3d(dot[2].X, dot[2].Y, -dot[2].Z);
    glVertex3d(-dot[1].X, dot[1].Y, -dot[1].Z);
    glVertex3d(-dot[0].X, dot[0].Y, dot[0].Z);
    glEnd();
  }
}


VOID Get( ts5ANIM *Ani )
{
  INT i, j, x, z;
  DBL a;

  glPushMatrix();

  
  glScaled(SCALE, SCALE, SCALE);

 
  GetSky(-Ani->Time * 100);
  GetSun();

  for (i = 0; i < 100; i++)
  {
    glPushMatrix();
    glTranslated(M / 2, 5, M / 2);
    glRotated(Ani->Time * (r1() * 2 - 1) * 60, 0, 1, 0);
    glTranslated(0, r1() * 25, r1() * 50 + 10 + sin((Ani->Time + r1() * 10)) * 5);
    GetBird(Ani->Time * 6);
    glPopMatrix();
  }

  //GetHouse();

  
 /* glPushMatrix();
  for (a = 0; a < 2 * PI; a += 0.1)
  {  
    glLoadIdentity();
    glScaled(SCALE, SCALE, SCALE);
    glTranslated(Ra * sin(a), 0, Ra * cos(a));
    glRotated(a * 180 / PI, 0, 1, 0);
    GetBoard();
  }
  glPopMatrix();*/
  /*
  glPushMatrix();
  glTranslated(0, 2, 0);
  GetRoof(-1, 0, -2.1, -1.1, 1, 1, 1, 0);
  glPopMatrix();
   */

  GetGround(Ani);

  if (FOG)
    GetFog();
  else
    glDisable(GL_FOG);

  srand(0);
 /* for (i = 0; i < 100; i++)
  {
    glPushMatrix();
    x = (INT)(r1() * M);
    z = (INT)(r1() * M);
    
    glTranslated(x, Ground.V[z * M + x].Y, z);
    glRotated(-theta * 180 / PI, 0, 1, 0);
    GetTree(10);
    glPopMatrix();
  }*/ 

  glPopMatrix();   
}

VOID MyRender( ts5ANIM *Ani )
{ 
  static BOOL is_first = TRUE;
  //sndPlaySound(
  if (is_first)
  {
    MyInit();
    is_first = FALSE;
  }
  glLoadIdentity();

  Get(Ani);
}                        