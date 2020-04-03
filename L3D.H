 /* FILE NAME: L3D.H
  * PROGRAMMER: TS5 
  * DATE: 14.06.2010
  * PURPOSE: .
  *          .
  */
#ifndef _L3D_H_
#define _L3D_H_

#include "TS5_VEC.H"


typedef struct tagUV
{
  DBL U, V;
} UV;

typedef struct tagCOLOR
{
  DBL R, G, B, A;
} COLOR;

typedef struct tagts5PRIM
{
  INT NumOfV, NumOfF, NumOfN, GridH, GridW; 
  VEC *V, *N;
  UV *Tex;
  INT (*F)[3];
  COLOR *Color;
  
  BOOL IsGrid;
  /* . . . */
} ts5PRIM;


extern INT Ws, Hs; /* ширина и высота экрана */

extern DBL Wp, Hp;    /* размеры плоскости проекции */
extern DBL ProjectDistance; /* рассто€ние до плоскости проекции */

/* матрица преобразовани€ф */
extern MATR MTrans;

VOID TS5_DrawPrim( ts5PRIM *P );
ts5PRIM CreatePrim( INT NumOfV, INT NumOfF );
VOID FreePrim( ts5PRIM *P );
ts5PRIM LoadPrim( CHAR *FileName );
ts5PRIM Box( VOID );
ts5PRIM Grid( INT N, INT M );
BOOL TS5_LoadTex( CHAR *FileName );

#endif /* _L3D_H_ */

/* END OF 'L3D.H' FILE */    