 /* FILE NAME: PRIM.C
  * PROGRAMMER: TS5 
  * DATE: 16.06.2010
  * PURPOSE: .
  *          .
  */

#include <math.h>

#include "prim.h"



VOID TS5_DrawPrim( ts5PRIM *P )
{
  INT i, j, n, n1;

  glPushAttrib(GL_ALL_ATTRIB_BITS);

  if (P->TexID != -1)
  {
    glBindTexture(GL_TEXTURE_2D, P->TexID);
    glEnable(GL_TEXTURE_2D);
  }
  else
    glDisable(GL_TEXTURE_2D);

  glVertexPointer(3, GL_DOUBLE, sizeof(VEC), P->V);
  glNormalPointer(GL_DOUBLE, sizeof(VEC), P->N);
  glTexCoordPointer(2, GL_DOUBLE, sizeof(UV), P->Tex);
  glColorPointer(4, GL_DOUBLE, sizeof(COLOR), P->Color);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);          
  
  /* glDrawArrays(GL_TRIANGLES, 0, P->NumOfV); */

  if (P->IsGrid)
  {
    for (i = 0; i < P->GridH; i++)
    {
      n = i * (P->GridW + 1);
      n1 = n + (P->GridW + 1);
      glBegin(GL_QUAD_STRIP);
      for (j = 0; j <= P->GridW; j++)
      {
        glArrayElement(n1++);
        glArrayElement(n++);
      }
      glEnd();
    }
  }
  else
  {
    glBegin(GL_TRIANGLES);
    for (i = 0; i < P->NumOfF; i++)
      for (j = 0; j <  3; j++)
        glArrayElement(P->F[i][j]);
    /*
    for (i = 0; i < P->NumOfF; i++)
      for (j = 0; j <  3; j++)
      {
        n = P->F[i][j];
        glNormal3dv((DBL *)&P->N[n]);
        glColor4dv((DBL *)&P->Color[n]);
        glTexCoord2dv((DBL *)&P->Tex[n]);
        glVertex3dv((DBL *)&P->V[n]);
      }
    */
    glEnd();
  }
 /* glBegin(GL_LINES);
  for (i = 0; i < P->NumOfV; i++)
  {
    glVertex3dv((DBL *)&P->V[i]);
    glVertex3d(P->V[i].X + 0.1 * P->N[i].X,
               P->V[i].Y + 0.1 * P->N[i].Y,
               P->V[i].Z + 0.1 * P->N[i].Z);
  }
  glEnd();  */
  glPopAttrib();
}

/* Primitive creation function.
 * ARGUMENTS:
 *   - number of vertexes and factes:
 *       INT NumOfV, NumOfF;
 * RETURNS:
 *   (ts5PRIM) new primitive structure.
 */
ts5PRIM TS5_PrimCreate( INT NumOfV, INT NumOfF )
{
  UINT size;
  ts5PRIM p = {0};
 
  /* Allocate memory for primitive - mem bulk */
  size = (sizeof(VEC) * 2 + sizeof(UV) + sizeof(COLOR)) * NumOfV +
          sizeof(INT [3]) * NumOfF;
  if ((p.V = malloc(size)) == NULL)
    return p;
  memset(p.V, 0, size);

  /* Primitive inside pointers setup */
  p.N = (VEC *)(p.V + NumOfV);
  p.Tex = (UV *)(p.N + NumOfV);
  p.F = (INT (*)[3])(p.Tex + NumOfV);
  p.Color = (COLOR *)(p.F + NumOfF);
  
  p.NumOfV = NumOfV;
  p.NumOfF = NumOfF;
  p.TexID = -1;
  return p;
} /* End of 'TS5_PrimCreate' function */

/* Primitive free up function.
 * ARGUMENTS:
 *   - pointer to primitive:
 *       ts5PRIM *P;
 * RETURNS: None.
 */
VOID TS5_PrimFree( ts5PRIM *P )
{
  if (P->V != NULL)
    free(P->V);
  memset(P, 0, sizeof(ts5PRIM));
} /* End of 'TS5_PrimFree' function */

/* Load primitive from file function.
 * ARGUMENTS:
 *   - pointer to file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (ts5PRIM) new primitive structure.
 */
ts5PRIM TS5_LoadPrim( CHAR *FileName )
{
  INT i, j;
  ts5PRIM p = {0};
  FILE *F;
  CHAR Buf[1000];

  if ((F = fopen(FileName, "rt")) == NULL)
    return p;

  /* Count quantity of vertex and facets */
  i = j = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      i++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      j++;
  }
  p = TS5_PrimCreate(i, j);
  if (p.V == NULL)
  {
    fclose(F);
    return p;
  }
  
  /* Load vertex and facets */
  rewind(F);
  i = j = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%lf %lf %lf", &p.V[i].X, &p.V[i].Y, &p.V[i].Z);
      i++;
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      if (sscanf(Buf + 2, "%i/%*i/%*i %i/%*i/%*i %i/%*i/%*i",
            &p.F[j][0], &p.F[j][1], &p.F[j][2]) == 3 ||
          sscanf(Buf + 2, "%i//%*i %i//%*i %i//%*i",
            &p.F[j][0], &p.F[j][1], &p.F[j][2]) == 3 ||
          sscanf(Buf + 2, "%i/%*i %i/%*i %i/%*i",
            &p.F[j][0], &p.F[j][1], &p.F[j][2]) == 3 ||
          sscanf(Buf + 2, "%i %i %i",
            &p.F[j][0], &p.F[j][1], &p.F[j][2]) == 3)
        ;
      p.F[j][0]--;
      p.F[j][1]--;
      p.F[j][2]--;
      j++;
    }
  }
  fclose(F);
  return p;
} /* End of 'TS5_LoadPrim' function */

/* Load texture from G24 image file to OpenGL memory function.
 * ARGUMENTS:
 *   - pointer to file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL TS5_LoadTex( CHAR *FileName )
{
  UCHAR *img;
  INT w = 0, h = 0;
  FILE *F;

  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  fread(&w, 2, 1, F);
  fread(&h, 2, 1, F);

  if ((img = malloc(3 * w * h)) == NULL)
  {
    fclose(F);
    return FALSE;
  }

  fread(img, 3, w * h, F);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  //glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, img);
  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, w, h, GL_BGR_EXT, GL_UNSIGNED_BYTE, img);
  free(img);
  fclose(F);

  return TRUE;
} /* End of 'TS5_LoadTex' function */

/* Load transparent texture from G24 image file to OpenGL memory function.
 * ARGUMENTS:
 *   - pointer to file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL TS5_LoadTransTex( CHAR *FileName )
{
  DWORD *img, *ptr, key_color;
  INT w = 0, h = 0, i, j;
  FILE *F;

  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  fread(&w, 2, 1, F);
  fread(&h, 2, 1, F);

  if ((img = malloc(4 * w * h)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  memset(img, 0, 4 * w * h);

  for (ptr = img, i = 0; i < h; i++)
    for (j = 0; j < w; j++)
      fread(ptr++, 3, 1, F);
  key_color = *img;
  for (ptr = img, i = 0; i < h; i++)
    for (j = 0; j < w; j++, ptr++)
      if (*ptr == key_color)
        *ptr = 0x00FFFFFF;
      else
        *ptr |= 0xFF000000;

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  //glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, img);
  gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_BGRA_EXT, GL_UNSIGNED_BYTE, img);
  free(img);
  fclose(F);

  return TRUE;
} /* End of 'TS5_LoadTransTex' function */


ts5PRIM Box( VOID )
{
  INT i;
  ts5PRIM p;

  p = TS5_PrimCreate(24, 12);

  /* верхн€€ грань */
  p.V[0] = VecSet(0, 1, 0); p.N[0] = VecSet(0, 1, 0);
  p.V[1] = VecSet(0, 1, 1); p.N[1] = VecSet(0, 1, 0);
  p.V[2] = VecSet(1, 1, 0); p.N[2] = VecSet(0, 1, 0);
  p.F[0][0] = 0; p.F[0][1] = 1; p.F[0][2] = 2;

  p.V[3] = VecSet(1, 1, 1); p.N[3] = VecSet(0, 1, 0);
  p.F[1][0] = 2; p.F[1][1] = 1; p.F[1][2] = 3;

  /* нижн€€ грань */
  p.V[4] = VecSet(0, 0, 0); p.N[4] = VecSet(0, -1, 0);
  p.V[5] = VecSet(0, 0, 1); p.N[5] = VecSet(0, -1, 0);
  p.V[6] = VecSet(1, 0, 0); p.N[6] = VecSet(0, -1, 0);
  p.F[2][0] = 5; p.F[2][1] = 4; p.F[2][2] = 6;

  p.V[7] = VecSet(1, 0, 1); p.N[7] = VecSet(0, -1, 0);
  p.F[3][0] = 5; p.F[3][1] = 6; p.F[3][2] = 7;

  /* лева€ грань */
  p.V[8] = VecSet(0, 0, 0); p.N[8] = VecSet(-1, 0, 0);
  p.V[9] = VecSet(0, 0, 1); p.N[9] = VecSet(-1, 0, 0);
  p.V[10] = VecSet(0, 1, 0); p.N[10] = VecSet(-1, 0, 0);
  p.F[4][0] = 8; p.F[4][1] = 9; p.F[4][2] = 10;

  p.V[11] = VecSet(0, 1, 1); p.N[11] = VecSet(-1, 0, 0);
  p.F[5][0] = 10; p.F[5][1] = 9; p.F[5][2] = 11;

  /* права€ грань */
  p.V[12] = VecSet(1, 0, 0); p.N[12] = VecSet(1, 0, 0);
  p.V[13] = VecSet(1, 0, 1); p.N[13] = VecSet(1, 0, 0);
  p.V[14] = VecSet(1, 1, 0); p.N[14] = VecSet(1, 0, 0);
  p.F[6][0] = 13; p.F[6][1] = 12; p.F[6][2] = 14;

  p.V[15] = VecSet(1, 1, 1); p.N[15] = VecSet(1, 0, 0);
  p.F[7][0] = 13; p.F[7][1] = 14; p.F[7][2] = 15;

  /* задн€€ грань */
  p.V[16] = VecSet(0, 0, 0); p.N[16] = VecSet(0, 0, -1);
  p.V[17] = VecSet(0, 1, 0); p.N[17] = VecSet(0, 0, -1);
  p.V[18] = VecSet(1, 0, 0); p.N[18] = VecSet(0, 0, -1);
  p.F[8][0] = 16; p.F[8][1] = 17; p.F[8][2] = 18;

  p.V[19] = VecSet(1, 1, 0); p.N[19] = VecSet(0, 0, -1);
  p.F[9][0] = 18; p.F[9][1] = 17; p.F[9][2] = 19;

  /* передн€€ грань */
  p.V[20] = VecSet(0, 0, 1); p.N[20] = VecSet(0, 0, 1);
  p.V[21] = VecSet(0, 1, 1); p.N[21] = VecSet(0, 0, 1);
  p.V[22] = VecSet(1, 0, 1); p.N[22] = VecSet(0, 0, 1);
  p.F[10][0] = 21; p.F[10][1] = 20; p.F[10][2] = 22;

  p.V[23] = VecSet(1, 1, 1); p.N[23] = VecSet(0, 0, 1);
  p.F[11][0] = 21; p.F[11][1] = 22; p.F[11][2] = 23;


  for (i = 0; i < p.NumOfV; i++)
  {
    p.Color[i].R = 1;
    p.Color[i].G = 1;
    p.Color[i].B = 1;
  }
  return p;
} /* End of '' function */

ts5PRIM Grid( INT N, INT M )
{
  INT i, j, n;
  ts5PRIM p;

  p = TS5_PrimCreate((N + 1) * (M + 1), 0);
  p.IsGrid = TRUE;
  p.GridW = M;
  p.GridH = N;
  for (i = 0; i <= N; i++)
    for (j = 0; j <= M; j++)
    {
      n = i * (M + 1) + j;
      p.V[n] = VecSet(j / (DBL)M, i / (DBL)N, 0);
      p.N[n] = VecSet(0, 0, 1);
      p.Tex[n].U = j / (DBL)M;
      p.Tex[n].V = i / (DBL)N;
    }
  for (i = 0; i < p.NumOfV; i++)
  {
    p.Color[i].R = 1;
    p.Color[i].G = 1;
    p.Color[i].B = 1;
    p.Color[i].A = 1;
  }
  return p;
} /* End of '' function */


ts5PRIM TS5_Sphere( INT N, INT M )
{
  INT i, j, n;
  DBL phi, theta;
  ts5PRIM p;

  p = Grid(N, M);

  for (i = 0; i <= N; i++)
    for (j = 0; j <= M; j++)
    {  
      phi = 2 * PI / M * i;
      theta = PI / N * j;
      n = i * (M + 1) + j;

      p.V[n].X = cos(phi) * sin(theta);
      p.V[n].Y = sin(phi) * sin(theta);
      p.V[n].Z = cos(theta);

      p.N[n].X = p.V[n].X;
      p.N[n].Y = p.V[n].Y;
      p.N[n].Z = p.V[n].Z;
    }
  return p;
} /* End of '' function */


ts5PRIM TS5_Cilinder( INT N, INT M )
{
  INT i, j, n;
  DBL a;
  ts5PRIM p;

  p = Grid(N, M);
  for (j = 0, n = 0; j < N; j++)
    for (i = 0, a = 0; i < M; i++, a += 2 * PI / M, n++)
    {
      //glTexCoord2d(1, 1);
      p.V[n].X = cos(a);
      p.V[n].Z = sin(a);

      p.N[n].X = cos(a);
      p.N[n].Z = sin(a);
      p.N[n].Z = 0;
    } 
  return p;
} /* End of 'TS5_Cilinder' function */

/* END OF 'PRIM.C' FILE */
