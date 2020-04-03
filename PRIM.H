 /* FILE NAME: PRIM.H
  * PROGRAMMER: TS5 
  * DATE: 16.06.2010
  * PURPOSE: .
  *          .
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "TS5_ANIM.H"
#include "ts5_vec.h"

/* Texture coordinates representation type */
typedef struct tagUV
{
  DBL U, V; /* 2D texture coordinates */
} UV;

/* Color representation type */
typedef struct tagCOLOR
{
  DBL R, G, B, A; /* Color components */
} COLOR;

/* 3D primitive representation type */
typedef struct tagts5PRIM
{
  INT
    NumOfV, NumOfF, /* Number of vertexes and facets */
    GridH, GridW;   /* Grid size */
  VEC *V, *N;       /* Vertex and normal array pointers */
  UV *Tex;          /* Vertex texture coordinates array pointers */
  COLOR *Color;     /* Vertex color array pointers */
  INT (*F)[3];      /* Facet array pointers */
  BOOL IsGrid;      /* Grid type of primitive flag */
  INT TexID;        /* Primitive texture number (-1 if no texture used) */
} ts5PRIM;

VOID TS5_DrawPrim( ts5PRIM *P );
ts5PRIM CreatePrim( INT NumOfV, INT NumOfF );
VOID FreePrim( ts5PRIM *P );
ts5PRIM LoadPrim( CHAR *FileName );
BOOL TS5_LoadTex( CHAR *FileName );
BOOL TS5_LoadTransTex( CHAR *FileName );


ts5PRIM Box( VOID );
ts5PRIM Grid( INT N, INT M );
ts5PRIM TS5_Sphere( INT N, INT M );
ts5PRIM TS5_Cilinder( INT N, INT M );