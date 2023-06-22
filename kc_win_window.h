/*** 
 * Creates windows only functionality for the following: 
 * - Window w/ opengl context
 * - 'raw_input' for mouse and keyboard
 ***/
#include "kc_lib.h"
#undef func
#include "windows.h"
#define func static

#define WIN32_STATE_FILENAME_COUNT MAX_PATH
#define updateHz(expression) ((expression)/2)

typedef struct win32_window_dimensions 
{
    vi2 value; // x width, y height
} win32_window_dimensions;

typedef struct win32_offscreen_buffer
{
    BITMAPINFO bitmapInfo;  
    void* bitmapMemory; 
    i32 memorySize; 
    i32 height;
    i32 width;
    i32 pitch; 
    i32 bytesPerPixel; 
} win32_offscreen_buffer; 

typedef struct win32_window_handle_wrapper {
    WNDCLASSEXA win32Window; 
    HWND windowHandle;
    HDC windowContext;
    win32_offscreen_buffer offscreenBuffer;
    b32 success;
    opengl_context win32OpenglContext;

} win32_window_handle_wrapper;

void 
Win32_ResizeDIBSection(win32_offscreen_buffer *buffer, i32 width, i32 height)
{
   if(buffer->bitmapMemory)
   {
       VirtualFree(buffer->bitmapMemory, 0, MEM_RELEASE);
   }
    buffer->bytesPerPixel = 4; 
    buffer->height = height; 
    buffer->width = width;
    buffer->pitch = buffer->width * buffer->bytesPerPixel; 
    buffer->bitmapInfo.bmiHeader.biSize = sizeof(buffer->bitmapInfo.bmiHeader); 
    //buffer->bitmapInfo.bmiHeader.biSizeImage = 0; 
    // Windows has a top down view of bitmap, so we need to reposition 
    // so that the top left is at 0,0 
    buffer->bitmapInfo.bmiHeader.biWidth= buffer->width; 
    buffer->bitmapInfo.bmiHeader.biHeight = -buffer->height; 
    buffer->bitmapInfo.bmiHeader.biPlanes = 1; 
    buffer->bitmapInfo.bmiHeader.biBitCount = 32; 
    buffer->bitmapInfo.bmiHeader.biCompression = BI_RGB;
    i32 bitmapMemorySize = 
        (buffer->width*buffer->height)*buffer->bytesPerPixel; 
    buffer->memorySize = bitmapMemorySize; 
    buffer->bitmapMemory = (void*)VirtualAlloc(
            0, 
            bitmapMemorySize, 
            MEM_RESERVE | MEM_COMMIT, 
            PAGE_READWRITE);
    if(!buffer->bitmapMemory)
    {
        //TODO log this
        Assert(!buffer->bitmapMemory); 
    }
}

LRESULT CALLBACK
Win32_WindowCallback(
        HWND hWnd, 
        ui32 msg, 
        WPARAM wParam, 
        LPARAM lParam)
{
    //DEBUG_ConsolePrint("message: %x\n", msg);
    LRESULT result = 0; 
    switch(msg)
    {
        case(WM_PAINT): 
        {
            // TODO we need to handle this at some point
            PAINTSTRUCT paint; 
            HDC context = BeginPaint(hWnd, &paint);
#if 0
            win32_window_dimensions windowDimensions
                 = Win32_GetWindowDimension(hWnd);
            shader* listOfShaders = global_listOfShdaders;
            Win32_DisplayBufferInWindow
                (
                 listOfShaders,
                 &globalBackBuffer, 
                 context, 
                 windowDimensions.width, 
                 windowDimensions.height); 
#endif
            EndPaint(hWnd, &paint);
            return 1;
            break; 
        }
#if 0
        case(WM_SIZE): 
        {
            //NOTE this is if the window is resized and we decide to do something 
            // break;
        }
        case (WM_SETCURSOR):
        {
            //SetCursor(globalDebugCursor); 
        } break;
        case(WM_DESTROY): 
        {
            //TODO recreate the window
            //PostQuitMessage(0); //Closes the window through windows
            globalRunning = false;
            break;
        }
        case(WM_CLOSE):
        {
            globalRunning = false; 
            break;
        }
#endif 
        case(WM_ACTIVATEAPP): 
        {
            break;
        }
        default: 
        {
            result = DefWindowProc(hWnd, msg, wParam, lParam); 
        } break;
    }
    return result;
}

void 
Win32_InitOpenGL(opengl_context *opengl, HWND window)
{
    //NOTE this is a little confusing
    HDC windowDC = GetDC(window);

    PIXELFORMATDESCRIPTOR desiredPixelFormat = {}; 
    desiredPixelFormat.nSize = sizeof(desiredPixelFormat); 
    desiredPixelFormat.nVersion = 1; 
    // Tell windows we are going to use Opengl, 
    // the back buffer and draw to the window
    desiredPixelFormat.dwFlags = 
            PFD_SUPPORT_OPENGL | 
            PFD_DRAW_TO_WINDOW | 
            PFD_DOUBLEBUFFER;
    desiredPixelFormat.cColorBits = 32; 
    desiredPixelFormat.cAlphaBits = 8; 
    desiredPixelFormat.iLayerType = PFD_MAIN_PLANE; 
    desiredPixelFormat.iPixelType = PFD_TYPE_RGBA; 
    i32 suggestedPixelFormatIndex = ChoosePixelFormat(windowDC, &desiredPixelFormat); 
    Assert(suggestedPixelFormatIndex != 0);
#if 0 // I don't think we need this.
     wgl_choose_pixel_formatARB *choosePixelFormat = (wgl_choose_pixel_formatARB*)wglGetProcAddress("wglChoosePixelFormatARB");
    const int attribList[] =
    {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_STENCIL_BITS_ARB, 8,
        0, // End
    };
     i32 pixelFormat = 0;
    ui32 numFormats = 0;
    i32 suggestedPixelFormatIndex = choosePixelFormat(windowDC, attribList, 0, 1, &pixelFormat,  &numFormats); 
#endif 
    PIXELFORMATDESCRIPTOR suggestedPixelFormat = {}; 
    DescribePixelFormat(
            windowDC, 
            suggestedPixelFormatIndex, 
            sizeof(suggestedPixelFormat), 
            &suggestedPixelFormat); 
    SetPixelFormat(windowDC, suggestedPixelFormatIndex, &suggestedPixelFormat); 
    
    // We need to allow OpenGL to have the current context
    InitOpenGl(opengl, window, windowDC);
    Assert(opengl->isValid == 1);
}

win32_window_handle_wrapper
win32_create_opengl_context_windows(HINSTANCE hInstance, char* windowName) {
  // create/setup a basic window
  HRESULT hr = {};

  win32_window_handle_wrapper result = {};
  WNDCLASSEXA win32Window = {0};
  Win32_ResizeDIBSection(&result.offscreenBuffer, 1080, 920);
  win32Window.cbSize = sizeof(WNDCLASSEX);
  win32Window.lpfnWndProc = Win32_WindowCallback;
  win32Window.hInstance = hInstance;
  win32Window.lpszClassName = windowName;
  win32Window.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

  if (RegisterClassEx(&win32Window)) {
    HWND windowHandle = CreateWindowEx(
        0, win32Window.lpszClassName, windowName,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
    Assert(windowHandle);

    result.win32OpenglContext = {};

    Win32_InitOpenGL(&result.win32OpenglContext, windowHandle);
    DWORD lastError = GetLastError();

    if (windowHandle) {
      HDC DCHandle = GetDC(windowHandle);
      Assert(DCHandle != 0);
      i32 monitorRefreshHZ = 60; // default
      i32 win32MonitorRefereshRate = GetDeviceCaps(DCHandle, VREFRESH);
      ReleaseDC(windowHandle, DCHandle);
      Assert(win32MonitorRefereshRate >
             0); // NOTE just to see if this is working
      if (win32MonitorRefereshRate > 0) {
        monitorRefreshHZ = win32MonitorRefereshRate;
      }

      i32 gameUpdateHz = updateHz(monitorRefreshHZ);
      f32 expectedFramesPerUpdate = 1.0f;
      f32 targetSecondsElapsedPerFrame =
          expectedFramesPerUpdate / (f32)gameUpdateHz;

      result.success = true;
      result.windowHandle = windowHandle;
    }
  }
  return result;
}

// TODO finish this... 
// 1. its need an allocator passed in
device_input
win32_init_input_devices() {
    // create two here to keep state from previous to next (we do keep 
    // track of some prior deltas and changes in each device_input as well)
    device_input  input[2] = {}; 
    device_input *newInput = &input[0];
    device_input  *oldInput = &input[1];

    // windows init for mouse
    RAWINPUTDEVICE rid[1];
    rid[0].usUsagePage = 0x01;          // HID_USAGE_PAGE_GENERIC
    rid[0].usUsage = 0x02;              // HID_USAGE_GENERIC_MOUSE
    rid[0].dwFlags = RIDEV_NOLEGACY;    // adds mouse and also ignores legacy mouse messages
    rid[0].hwndTarget = 0;

    i32 mouse = RegisterRawInputDevices(rid, 1, sizeof(rid[0]));
    if (mouse == 0)
    {
        DWORD lastError = 0;
        lastError = GetLastError(); 
        Assert(mouse != 0);
    }

    Assert(!"todo this is not done yet");
    return input[0];
}

//Return the window dimensions of the screen
win32_window_dimensions
Win32_GetWindowDimension(HWND hWnd)
{
    RECT rect;
    win32_window_dimensions dim;
    GetClientRect(hWnd, &rect);
    dim.value.y = rect.bottom - rect.top;
    dim.value.x = rect.right - rect.left;
    return dim;
}

//NOTE code from Raymond Chen
//https://devblogs.microsoft.com/oldnewthing/20100412-00/?p=14353
void 
Win32_ToggleFullScreen(HWND hwnd)
{
    DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);
    if (dwStyle & WS_OVERLAPPEDWINDOW) 
    {
        MONITORINFO monitorInfo = { sizeof(monitorInfo) };
        if (GetWindowPlacement(hwnd, &globalWindowPosition) &&
            GetMonitorInfo(MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY), 
                &monitorInfo)) 
        {
          SetWindowLong(hwnd, GWL_STYLE,
                        dwStyle & ~WS_OVERLAPPEDWINDOW);
          SetWindowPos(hwnd, HWND_TOP,
                       monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.top,
                       monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
                       monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
                       SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }
    } 
    else 
    {
        SetWindowLong(hwnd, GWL_STYLE,
                      dwStyle | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(hwnd, &globalWindowPosition);
        SetWindowPos(hwnd, NULL, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                     SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    }
}

// TODO  finish all this
void 
Win32_ProcessInputFromMessage(
        HWND hWnd, 
        void (*inputCallback)(KEY_TYPE, b32, b32),
        void (*exitApplication)()) {
    MSG message; 
    while(PeekMessage(&message, hWnd, 0, 0, PM_REMOVE)) {
        if (message.message == WM_QUIT) {
           exitApplication();
        }
        switch(message.message) {
            case(WM_SYSKEYDOWN): 
            case(WM_SYSKEYUP):
            case(WM_KEYDOWN): 
            case(WM_KEYUP): {
           //case(WM_INPUT):
                // NOTE we are attempting to correct for the windows repeat key feature here
                // by "skipping" the next frame" if its the next frame and we update the key input 
                // with a repeat press and then an key up on the next check 
                ui32 dwSize; 
                ui32 pcbSize;
                //We can also get this data buffered
                char buffer[sizeof(RAWINPUT)] = {}; 
                RAWINPUT *rawInput = {0}; 
                RAWINPUT *bufferInput = {0}; 
                GetRawInputBuffer(bufferInput, &pcbSize, sizeof(RAWINPUTHEADER));
                if (GetRawInputData((HRAWINPUT)message.lParam, RID_INPUT, &buffer, 
                        &dwSize, sizeof(RAWINPUTHEADER)) > 0) {
                    rawInput = (RAWINPUT *)buffer;  
                } else {
                    Assert(!"Error"); 
                }
                ui32 deviceAmt = 2; 
                RAWINPUTDEVICE devices[2] = {0}; 
                GetRegisteredRawInputDevices(devices, &deviceAmt, sizeof(RAWINPUTDEVICE));
                ui32 vkCode = (ui32)message.wParam;
                // bit shift to get some of the intersting info out of lparam
                b32 wasDown = ((message.lParam & (1 << 30)) != 0);
                b32 isDown = ((message.lParam & (1UL << 31)) == 0);
                b32 keyPressTransitionState = 30; 
                if (isDown) {
                    //NOTE 1 is key down before, zero if key is up
                    keyPressTransitionState = ((message.lParam << 30) == 1);
                }

                b32 isUp = wasDown;
                if (isDown != wasDown) {
                    if (vkCode == VK_SPACE)
                    {
                        inputCallback(SPACEBAR, isUp, isDown); 
                    }
                    if (isDown) 
                    {
                        b32 isAltKeyDown = ((message.lParam & (1 << 29)) != 0); 
                        if (isAltKeyDown && (vkCode == VK_RETURN))
                        {
                            Win32_ToggleFullScreen(message.hwnd);
                        }
                        if ((vkCode == VK_F4) && isAltKeyDown)
                        {
                            exitApplication();
                        }
                    }
                }
            } break;
            // TODO maybe we are using this? not sure
#if 0
            case (WM_INPUT): 
            {
                if (state->rawInputBufferSize <= 0) 
                {
                    UINT dwSize;
                    GetRawInputData((HRAWINPUT)message.lParam, RID_INPUT, 0, &dwSize, sizeof(RAWINPUTHEADER));
                    state->rawInputBufferSize = dwSize;
                    // TODO we are going to crash the if this is not the case Need to implement a method for to dynmically allocate 
                    Assert(dwSize == 48); 
                }

                unsigned char* lpb = state->rawInputBuffer;
                ui32 dwSize = state->rawInputBufferSize;
                Assert(GetRawInputData((HRAWINPUT)message.lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) == dwSize);

                RAWINPUT* raw = (RAWINPUT*)lpb;

                if (raw->header.dwType == RIM_TYPEKEYBOARD) 
                {
                    //NOTE not implemented: see the MS docs. We would 
                    //need to init this first but we are using VK_KEYS etc. 
                }
                else if (raw->header.dwType == RIM_TYPEMOUSE) 
                {
                    Assert(raw != 0);
                    // NOTE that we are only using relative, see MS docs for RawInput and the RAWMOUSE Structure
                    Assert(raw->data.mouse.usFlags == 0);
                    LONG dx = raw->data.mouse.lLastX; 
                    LONG dy = raw->data.mouse.lLastY;
                    
                    statefulInput->rawMouseDelta.x = (f32)dx; 
                    statefulInput->rawMouseDelta.y = (f32)dy; 
#if 0
                    DEBUG_ConsolePrint("long x lastx %ld, lasty %ld\n",
                            statefulInput->rawMouseDelta.x,
                            statefulInput->rawMouseDelta.y
                            );
#endif
                } 
                
            } break;
#endif
            case(WM_LBUTTONDOWN):
            {
                b32 wasDown = ((message.lParam & (1 << 30)) != 0);
                b32 isDown = ((message.lParam & (1UL << 31)) == 0);
                b32 keyPressTransitionState = 30; 
                if (isDown) 
                {
                    //NOTE 1 is key down before, zero if key is up
                    keyPressTransitionState = ((message.lParam << 30) == 1);
                }

                b32 isUp = wasDown;
                inputCallback(MOUSE_LEFT, isUp, isDown);
            } break;
            case(WM_RBUTTONDOWN):
            {
                b32 wasDown = ((message.lParam & (1 << 30)) != 0);
                b32 isDown = ((message.lParam & (1UL << 31)) == 0);
                b32 keyPressTransitionState = 30; 
                if (isDown) 
                {
                    //NOTE 1 is key down before, zero if key is up
                    keyPressTransitionState = ((message.lParam << 30) == 1);
                }

                b32 isUp = wasDown;
                inputCallback(MOUSE_RIGHT, isUp, isDown);
            } break;
            case (WM_MOUSEMOVE):
            {
                if(message.wParam & MK_LBUTTON)
                {
                }

            } break; 
            default:
            {
                TranslateMessage(&message); 
                DispatchMessage(&message); 
            }
        }
    }
}

void Win32_openglInitBuffers() {
}

void 
Win32_opengl_Render(
    memory_arena *memArena,
    HDC deviceContext,
    opengl_context *openglContext,
    i32 winWidth,
    i32 winHeight) {

    color clearColor = Color(0.1f, 0.1f, 0.1f, 0.0f); 
    OpenGlClearAndSetViewPort(clearColor, winWidth,  winHeight);

    m4 view = GetIdentityMatrix(); 
    SwapBuffers(deviceContext); 
#if 0 
    OpenGlDrawRectnagle_Performant(
            opengl, 
            gameState->world->worldRenderData[chunkIndex],
            modelTransposed,
            renderData.projMat,
            view,
            opengl->shaders[MeshShader]
            );
#endif
}

vi2
func compuateWindowCenter(win32_window_dimensions dims) {
    return dims.value * 0.5f;
}
