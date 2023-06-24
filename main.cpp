#include "mem_allocator.h"
#include "kc_lib.h"
#include "file_utils.h"

#include "drawing.h"
#include "draw.cpp"
#include "opengl.h"

char *WINDOW_NAME = "UI Thing2";

typedef struct application_state {
  memory_arena memArena;
  b32 applicationRunning; 
  device_input currentDeviceInput; 
  // opengl buffers sent to gpu before draw call
  kc_array<openglObjData, 10> opengPreBuf;
  kc_array<drawing, 1000> drawings;
  vi2 winDims; //width, height
} application_state;

global_var application_state APP_STATE = {};
global_var WINDOWPLACEMENT globalWindowPosition = {sizeof(globalWindowPosition)}; 

void inputCallback(KEY_TYPE keyType, b32 isUp, b32 isDown) {
    input_device_state *keyState = {};
    switch (keyType) {
        case MOUSE_LEFT: {
            keyState = &APP_STATE.currentDeviceInput.inputDevices[0].leftClick;
        }
        case MOUSE_RIGHT: {
            keyState = &APP_STATE.currentDeviceInput.inputDevices[0].rightClick;
        }
        case SPACEBAR: {
            keyState = &APP_STATE.currentDeviceInput.inputDevices[0].spacebar;
        }
          break;
    }
    // set the key state
    if (keyState->endedDown != isDown)
    {
        keyState->endedDown = isDown; 
        ++keyState->priorState; 
    }
}

void rawInputBufferCallback() {
}

#include "kc_win_window.h" //windows.h is defined in here

void exitApplicationCallback() {
    APP_STATE.applicationRunning = false;
}

i32 CALLBACK WinMain(HINSTANCE hInstance, 
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, 
    i32 nShowCmd) { 

  win32_window_handle_wrapper windowRef = win32_create_opengl_context_windows(hInstance, WINDOW_NAME);
  APP_STATE.memArena = init_memory_arena();
  APP_STATE.applicationRunning = true;
  Shaders shaders = {};
  shaders.vertexShaders[0] = "../shaders/vertex.vert";
  shaders.fragmentShaders[0] = "../shaders/fragment.frag";
  shaders.currentCount = CountActiveShaders(&shaders);
  for (i32 i = 0; i < shaders.currentCount; ++i) {
    shader s = InitShader(
            &APP_STATE.memArena,
            &windowRef.win32OpenglContext, 
            shaders.vertexShaders[i], 
            shaders.fragmentShaders[i]);
    // TODO when if we go to use this we need to update the enum positions
    windowRef.win32OpenglContext.shaders[MeshShader] = s;
  }

  initOpenGLShaderBuff(&windowRef.win32OpenglContext, 
          sizeof(APP_STATE.drawings.items)
          );

  APP_STATE.opengPreBuf.add(
          OpenGLRectangleSetupPreBuffered(
          &windowRef.win32OpenglContext,
          RectMinMax(v2{0.0f, 0.0f}, v2{1.0f, 1.0f}),
          Color(1.0f, 0.0f, 0.0f, 1.0f),
          windowRef.win32OpenglContext.shaders[MeshShader]
          )
    );

  while(APP_STATE.applicationRunning) {
      Win32_ProcessInputFromMessage(
          windowRef.windowHandle,
          inputCallback, 
          exitApplicationCallback
        );

      win32_window_dimensions winDims = Win32_GetWindowDimension(windowRef.windowHandle); 
      APP_STATE.winDims = winDims.value;

      Win32_opengl_Render(
              &APP_STATE, 
              GetDC(windowRef.windowHandle), 
              &windowRef.win32OpenglContext,
              winDims.value.x, 
              winDims.value.y);
  }
}
