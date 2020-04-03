 /* FILE NAME: T08MAIN.H
  * PROGRAMMER: TS5 
  * DATE: 15.06.2010
  * PURPOSE: 
  *          
  */

#include "TS5_anim.h"


LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg,
                          WPARAM wParam, LPARAM lParam );
VOID MyResponse( ts5ANIM *Ani );
VOID MyRender( ts5ANIM *Ani );


INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  /* Class register */

  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;                                              
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, (CHAR *)IDC_ARROW);
  wc.hIcon = LoadIcon(NULL, (CHAR *)IDI_APPLICATION);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = WinFunc;
  wc.lpszClassName = WIN_CLASS;
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR",
      MB_OK | MB_ICONSTOP);
    return 0;
  }

  /* Create window */
  hWnd = CreateWindow(WIN_CLASS, 
    "Title",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, 
    CW_USEDEFAULT, CW_USEDEFAULT,
    NULL, 
    NULL, 
    hInstance, 
    NULL);

  if (hWnd == NULL)
  {
    MessageBox(NULL, "Error create window", "ERROR",
      MB_OK | MB_ICONSTOP);
    return 0;
  }

  TS5_SetRender(MyRender);
  TS5_SetResponse(MyResponse);

  ShowWindow(hWnd, ShowCmd);
  UpdateWindow(hWnd);

  
    
  /* Message loop */
  while (TRUE)
  {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
        break;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
      SendMessage(hWnd, WM_TIMER, 0, 0);
  }

  return msg.wParam;
} /* End of 'WinMain' function */

LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;

  switch (Msg)
  {
  case WM_CREATE:
    TS5_Init(hWnd);
    return 0;
  case WM_SIZE:
    TS5_Resize(LOWORD(lParam), HIWORD(lParam));
    TS5_Render();
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_TIMER:
    TS5_Response();
    TS5_Render();
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_ERASEBKGND:
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    TS5_CopyFrame(hDC);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    TS5_Close();
    PostQuitMessage(0);
    return 0;
  }

  return DefWindowProc(hWnd, Msg, wParam, lParam);
}