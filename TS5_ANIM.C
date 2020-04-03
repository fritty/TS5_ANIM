 /* FILE NAME: TS5_ANIM.C
  * PROGRAMMER: TS5 
  * DATE: 03.06.2010
  * PURPOSE: Animation pattern.
  *          Animation implementation module.
  */

#include "TS5_anim.h"
#include <mmsystem.h>
#include <assert.h>

#pragma comment(lib, "winmm")
#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")


static ts5ANIM TS5_Ani;

__int64 TS5_StartTime, TS5_OldTime, TS5_LastTime, TS5_TimesPerSecond, TS5_PauseTime;

__int64 TS5_FrameCount;


/* Start of Initiate System function 
 */
VOID TS5_Init( HWND hWnd )
{
  INT mode;
  LARGE_INTEGER Time;
  PIXELFORMATDESCRIPTOR pd = {0};

  assert(hWnd != NULL);

  /* Clear memory */
  memset(&TS5_Ani, 0, sizeof(TS5_Ani));

  /* Inicialization */
  TS5_Ani.hWnd = hWnd;
  TS5_Ani.hDC = GetDC(hWnd);
  assert(TS5_Ani.hDC != NULL);

  mode = GetPixelFormat(TS5_Ani.hDC);
  DescribePixelFormat(TS5_Ani.hDC, mode, sizeof(PIXELFORMATDESCRIPTOR), &pd);

  /* Get dot format */
  pd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pd.nVersion = 1;
  pd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pd.iPixelType = PFD_TYPE_RGBA;
  pd.cColorBits = 32;
  pd.cDepthBits = 24;
  pd.cStencilBits = 8;

  /* Get format */
  mode = ChoosePixelFormat(TS5_Ani.hDC, &pd);
  DescribePixelFormat(TS5_Ani.hDC, mode, sizeof(PIXELFORMATDESCRIPTOR), &pd);
  if (!SetPixelFormat(TS5_Ani.hDC, mode, &pd))
  {
    MessageBox(hWnd, "!SetPixelFormat", "ERROR", MB_OK | MB_ICONSTOP);
    exit(0);
  }

  /* Context */
  TS5_Ani.hRC = wglCreateContext(TS5_Ani.hDC);
  wglMakeCurrent(TS5_Ani.hDC, TS5_Ani.hRC);

  glEnable(GL_DEPTH_TEST);

  /* Start Image */
  TS5_Ani.W = TS5_Ani.H = 10;

  /* TIME */
  QueryPerformanceCounter(&Time);
  TS5_StartTime = TS5_OldTime = TS5_LastTime = Time.QuadPart;
  QueryPerformanceFrequency(&Time);
  TS5_TimesPerSecond = Time.QuadPart;
} /* End of 'TS5_Init' function */


/* Start of Close System function 
 * Arguments: NO                  
 * Value: NO
 */
VOID TS5_Close( VOID )                         
{
  /* Delete contexts */
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(TS5_Ani.hRC);

  /* Delete resources */
  ReleaseDC(TS5_Ani.hWnd, TS5_Ani.hDC);
  memset(&TS5_Ani, 0, sizeof(TS5_Ani));
}/* End of 'TS5_Close' function */

/* Start of Resize Window function
 */
VOID TS5_Resize( INT W, INT H )
{
  /* Change structure of frame */
  TS5_Ani.W = W;
  TS5_Ani.H = H;
}/* End of 'TS5_Resize' function */

/* Start of Response function */
VOID TS5_Response( VOID )
{
  INT i;
  POINT pt;
  JOYCAPS jc;
  JOYINFOEX ji;
  LARGE_INTEGER Time;

  QueryPerformanceCounter(&Time);
  TS5_Ani.GlobalTime = (DBL)(Time.QuadPart - TS5_StartTime) / TS5_TimesPerSecond;
  TS5_Ani.GlobalDelta = (DBL)(Time.QuadPart - TS5_OldTime) / TS5_TimesPerSecond;

  /* FPS */
  if (Time.QuadPart - TS5_LastTime > TS5_TimesPerSecond)
  {
    TS5_Ani.FPS = (DBL)TS5_FrameCount * TS5_TimesPerSecond / (Time.QuadPart - TS5_LastTime);
    TS5_LastTime = Time.QuadPart;
    TS5_FrameCount = 0;
  }

  TS5_Ani.Time = (DBL)(Time.QuadPart - TS5_StartTime - TS5_PauseTime) / TS5_TimesPerSecond;
  if (TS5_Ani.IsPause)
  {
    TS5_Ani.Delta = 0;
    TS5_PauseTime += Time.QuadPart - TS5_OldTime;
  }
  else
    TS5_Ani.Delta = TS5_Ani.GlobalDelta;

  TS5_OldTime = Time.QuadPart;


  /* Keyboard */
  GetKeyboardState(TS5_Ani.Kbd);
  for (i = 0; i < 256; i++)
    TS5_Ani.Kbd[i] >>= 7;


  /* Joystick */
  if (joyGetNumDevs() > 0)
  {  
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      ji.dwSize = sizeof(ji);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        TS5_Ani.Jx = ((DBL)(ji.dwXpos - jc.wXmin) / (jc.wXmax - jc.wXmin)) * 2 - 1;
        TS5_Ani.Jy = ((DBL)(ji.dwYpos - jc.wYmin) / (jc.wYmax - jc.wYmin)) * 2 - 1;
        TS5_Ani.Jz = ((DBL)(ji.dwZpos - jc.wZmin) / (jc.wZmax - jc.wZmin)) * 2 - 1;
        TS5_Ani.Jr = ((DBL)(ji.dwRpos - jc.wRmin) / (jc.wRmax - jc.wRmin)) * 2 - 1;

        for (i = 0; i < 8; i++)
          TS5_Ani.JCap[i] = 0;
        if (ji.dwPOV == 0xFFFF)
          TS5_Ani.JCap[0] = 1;
        else
          TS5_Ani.JCap[ji.dwPOV / 4500] = 1;

        for (i = 0; i < 32; i++)
          TS5_Ani.JBut[i] = (ji.dwButtons >> i) & 1;
      }
    }                                              
  }

  /* Mouse */
  GetCursorPos(&pt);
  ScreenToClient(TS5_Ani.hWnd, &pt);
  TS5_Ani.DMX = pt.x - TS5_Ani.MX;
  TS5_Ani.DMY = pt.y - TS5_Ani.MY;
  TS5_Ani.MX = pt.x;
  TS5_Ani.MY = pt.y;
  /* Call user function */
  if (TS5_Ani.Response != NULL)
  {
    TS5_Ani.Response(&TS5_Ani);
  }

}/* End of 'TS5_Response' function */

VOID TS5_Render( VOID )
{
  /* Clear frame */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* Call user function */
  if (TS5_Ani.Render != NULL)
    TS5_Ani.Render(&TS5_Ani);

  glFinish();
}

VOID TS5_SetResponse( VOID (*Func)( ts5ANIM *Ani) )
{
  TS5_Ani.Response = Func;
}

VOID TS5_SetRender( VOID (*Func)( ts5ANIM *Ani) )
{
  TS5_Ani.Render = Func;
}

VOID TS5_CopyFrame( HDC hDC )
{
  /* Copy back buffer */
  SwapBuffers(TS5_Ani.hDC);
  /* Increase frame count */
  TS5_FrameCount++;
}

VOID TS5_SetFullScreen( BOOL IsToGoFullScreen )
{
  RECT rc;
  DWORD Style;
  static RECT SaveRC;

  if (IsToGoFullScreen == TS5_Ani.IsFullScreen)
    return;
  if ((TS5_Ani.IsFullScreen = IsToGoFullScreen) != FALSE)
  {
    GetWindowRect(TS5_Ani.hWnd, &SaveRC);

    rc.left = 0;
    rc.top = 0;
    rc.right = GetSystemMetrics(SM_CXSCREEN);
    rc.bottom = GetSystemMetrics(SM_CYSCREEN);

    Style = GetWindowLong(TS5_Ani.hWnd, GWL_STYLE);

    AdjustWindowRect(&rc, Style, FALSE);

    SetWindowPos(TS5_Ani.hWnd, HWND_TOP,
      rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOREDRAW | SWP_NOOWNERZORDER | SWP_NOSENDCHANGING);
  }
  else    
    SetWindowPos(TS5_Ani.hWnd, HWND_TOP,
      SaveRC.left, SaveRC.top, SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER | SWP_NOSENDCHANGING);
}