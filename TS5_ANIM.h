 /* FILE NAME: TS5_ANIM.H
  * PROGRAMMER: TS5 
  * DATE: 03.06.2010
  * PURPOSE: Animation pattern.
  *          Animation diclaration module.
  */

#ifndef _ANIM_H_
#define _ANIM_H_

#define WIN_CLASS "My window class"
#define _WIN32_WINNT_ 0x0500
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#pragma warning(disable: 4244 4996 4305)

typedef double DBL;


typedef struct tagts5ANIM ts5ANIM;
struct tagts5ANIM
{
  HWND hWnd;         /* Descriptor of window */
  HDC hDC;           /* Memory context */
  HGLRC hRC;         /* Context */
  INT W, H;          /* Value */
  DBL GlobalTime;    /* Seconds since start (no pause) */
  DBL GlobalDelta;   /* */
  DBL Time;          /* Seconds since start */
  DBL Delta;
  BOOL IsPause;
  BOOL IsFullScreen;

  DBL FPS;

  /* Keyboard */
  BYTE Kbd[256];

  /* Mouse */
  INT MX, MY, DMX, DMY;

  /* Joystic */
  DBL Jx, Jy, Jz, Jr;
  INT JCap[9];
  BYTE JBut[32];

  /* Set user functions */
  VOID (*Response)( ts5ANIM *Ani );
  VOID (*Render)( ts5ANIM *Ani );
};

VOID TS5_Init( HWND hWnd );

VOID TS5_Close( VOID );

VOID TS5_Resize( INT W, INT H );

VOID TS5_Response( VOID );

VOID TS5_Render( VOID );

VOID TS5_SetResponse( VOID (*Func)( ts5ANIM *Ani ) );

VOID TS5_SetRender( VOID (*Func)( ts5ANIM *Ani ) );

VOID TS5_CopyFrame( HDC hDC );

VOID TS5_SetFullScreen( BOOL IsToGoFullScreen );

#endif /* _ANIM_H_ */

/* END OF 'TS5_ANIM.H' FILE */