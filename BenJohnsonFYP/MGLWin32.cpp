/*************************************************************************
MGLWin32.cpp

Win32 setup code for an OpenGL application.

Author   :   Dave Astle

Written for More OpenGL Game Programming
*************************************************************************/

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include "MGLApp.h"
//#include "../../_common/MGLTimer.h"
#include "MGLTimer.h"

MGLApp* g_pApp    = NULL;
bool g_exiting    = false;
HDC g_hDC         = NULL;
bool g_fullscreen = false;

//////////////////////////////////////////////////////////////////////////
// Chooses a default pixel format
BOOL SetupPixelFormat(HDC hDC)
{
  PIXELFORMATDESCRIPTOR pfd;
  memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

  pfd.nSize		= sizeof(PIXELFORMATDESCRIPTOR);	// size
  pfd.dwFlags		= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.nVersion	= 1;                    // version
  pfd.iPixelType	= PFD_TYPE_RGBA;      // color type
  pfd.cColorBits	= MGL_COLOR_BITS;     // prefered color depth
  pfd.cDepthBits	= MGL_DEPTH_BITS;     // depth buffer
  pfd.cStencilBits = MGL_STENCIL_BITS;  // stencil buffer
  pfd.iLayerType	= PFD_MAIN_PLANE;     // main layer

  int pixelFormat = ChoosePixelFormat(hDC, &pfd);
  if( pixelFormat == 0 )
    return FALSE;

  if( SetPixelFormat(hDC, pixelFormat, &pfd) == FALSE )
    return FALSE;

  return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// Windows message handler
LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HDC hDC;
  static HGLRC hRC;

  // dispatch messages
  switch (uMsg)
  {  
  case WM_CREATE: // window creation

    // choose a pixel format for this window's device context
    hDC = GetDC(hWnd);
    SetupPixelFormat(hDC);

    // create and activate a GL context
    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);

    break;

  case WM_DESTROY:
  case WM_QUIT:
  case WM_CLOSE:  // window is closing

    // detach and delete the GL context
    wglMakeCurrent(hDC, NULL);
    wglDeleteContext(hRC);

    // exit the application
    PostQuitMessage(0);

    break;

  case WM_SIZE:	// window is resizing
    {
      // notify the demo
      if (g_pApp)
        g_pApp->SetupProjection(LOWORD(lParam), HIWORD(lParam));

    } break;

  case WM_ACTIVATEAPP:  // activate app
    break;

  case WM_PAINT: // paint
    {
      PAINTSTRUCT ps;
      BeginPaint(hWnd, &ps);
      EndPaint(hWnd, &ps);
      break;
    }

  case WM_KEYDOWN:  // keyboard key
    {
      switch( wParam )
      {
      case VK_ESCAPE:
        DestroyWindow(hWnd);
        break;
      default:
        break;
      }

      break;
    }

  case WM_KEYUP:        // keyboard key release
    break;

  case WM_LBUTTONDOWN:  // left mouse button
    break;

  case WM_LBUTTONUP:    // left button release
    break;

  case WM_RBUTTONDOWN:  // right mouse button
    break;

  case WM_RBUTTONUP:    // right button release
    break;

  case WM_MOUSEMOVE:    // mouse movement
    break;

  default:
    break;
  }

  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
  WNDCLASSEX  windowClass;  // window class
  RECT        windowRect;   // window dimensions
  MGL::Timer      timer;    // a high-perf windows timer

  // create the demo object
  g_pApp = new MGLApp();

  // set the window dimensions
  windowRect.left		= (long)0;
  windowRect.right	= (long)MGL_WINDOW_WIDTH;
  windowRect.top		= (long)0;
  windowRect.bottom	= (long)MGL_WINDOW_HEIGHT;

  // fill out the window class structure
  windowClass.cbSize			= sizeof(WNDCLASSEX);
  windowClass.style			= CS_HREDRAW | CS_VREDRAW;
  windowClass.lpfnWndProc		= MainWindowProc;
  windowClass.cbClsExtra		= 0;
  windowClass.cbWndExtra		= 0;
  windowClass.hInstance		= hInstance;
  windowClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
  windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);
  windowClass.hbrBackground	= NULL;
  windowClass.lpszMenuName	= NULL;
  windowClass.lpszClassName	= "GLClass";
  windowClass.hIconSm			= LoadIcon(NULL, IDI_WINLOGO);

  // register the windows class
  if (!RegisterClassEx(&windowClass))
    return 0;

  // query the user for fullscreen
  if (MGL_FORCE_FULLSCREEN)
    g_fullscreen = true;
 // else
 //   g_fullscreen = (MessageBox( NULL, "Run in fullscreen mode?", MGL_APP_NAME, MB_YESNO|MB_ICONQUESTION) == IDYES);

  // attempt to set up a full-screen context
  if (g_fullscreen)
  {
    DEVMODE dmScreenSettings;
    memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));

    dmScreenSettings.dmSize			= sizeof(dmScreenSettings);  
    dmScreenSettings.dmPelsWidth	= MGL_WINDOW_WIDTH;   // screen width
    dmScreenSettings.dmPelsHeight	= MGL_WINDOW_HEIGHT;  // screen height
    dmScreenSettings.dmBitsPerPel	= MGL_COLOR_BITS;     // bits per pixel
    dmScreenSettings.dmFields		= DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

    if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
    {
      // setting display mode failed, switch to windowed
      MessageBox(NULL, "Failed to create a fullscreen window; switching to windowed...", NULL, MB_OK|MB_ICONERROR);
      g_fullscreen = FALSE;  
    }
  }

  DWORD dwExStyle; // window extended style
  DWORD dwStyle;   // window style

  // set the window style
  if (g_fullscreen)
  {
    dwExStyle = WS_EX_APPWINDOW;
    dwStyle = WS_POPUP;
  }
  else
  {
    dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    dwStyle = WS_OVERLAPPEDWINDOW;
  }

  // adjust window rect to accommodate style flags
  AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

  // class registered, so now create our window
  HWND hwnd = CreateWindowEx(NULL,				// extended style
    "GLClass",							// class name
    MGL_APP_NAME,							// app name
    dwStyle | WS_CLIPCHILDREN |
    WS_CLIPSIBLINGS,					// style
    0, 0,								// x,y coordinate
    windowRect.right - windowRect.left,
    windowRect.bottom - windowRect.top,	// width, height
    NULL,								// handle to parent
    NULL,								// handle to menu
    hInstance,							// application instance
    NULL);								// no extra params

  // check if window creation failed (hwnd would equal NULL)
  if (!hwnd)
    return 0;

  // get a handle to the device context
  g_hDC = GetDC(hwnd);

  // present the window
  ShowWindow(hwnd, SW_SHOW);
  UpdateWindow(hwnd);

  // initialize the demo
  if (!g_pApp->Init(g_hDC))
  {
    delete g_pApp;
    return 0;
  }

  // main application loop
  timer.Init();

  MSG msg;

  while (!g_exiting)
  {
    // render and swap
    g_pApp->Update( timer.GetElapsedSeconds() );
    g_pApp->Render();
    SwapBuffers(g_hDC);

    // process windows messages
    while (PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE))
    {
      if (!GetMessage (&msg, NULL, 0, 0))
      {
        g_exiting = true;
        break;
      }

      TranslateMessage (&msg);
      DispatchMessage (&msg);
    }
  }

  // cleanup
  g_pApp->Shutdown();
  delete g_pApp;

  if (g_fullscreen)
  {
    ChangeDisplaySettings(NULL,0);
  }

  return (int)msg.wParam;
}