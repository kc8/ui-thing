#include "mem_allocator.h"
#include "kc_lib.h"
#include "file_utils.h"

#include "drawing.h"
#include "draw.cpp"
#include "opengl.h"

char *WINDOW_NAME = "UI Thing";

typedef struct application_state {
  memory_arena memArena;
  b32 applicationRunning; 
  device_input currentDeviceInput; 
  
  // opengl buffers sent to gpu before draw call
  // kc_array<openglObjData, 10> opengPreBuf;
  // hash table(s) keep elements of one another
  kc_hashTable<openglObjData> opengPreBuf;
  kc_hashTable<kc_ui_id> uiElements; 

  kc_array<drawing, 1000> drawings;
  vi2 winDims; //width, height: full window resolution
} application_state;

global_var application_state APP_STATE = {};
global_var WINDOWPLACEMENT globalWindowPosition = {sizeof(globalWindowPosition)}; 

void inputCallback(KEY_TYPE keyType, b32 isUp, b32 isDown) {
    input_device_state *keyState = {};
    switch (keyType) {
        case MOUSE_LEFT: {
            keyState = &APP_STATE.currentDeviceInput.inputDevices[0].leftClick;
            keyState->endedDown = isDown;
            break;
        }break;
        case MOUSE_RIGHT: {
            keyState = &APP_STATE.currentDeviceInput.inputDevices[0].rightClick;
            keyState->endedDown = isDown;
            break;
        }break;
        case SPACEBAR: {
            keyState = &APP_STATE.currentDeviceInput.inputDevices[0].spacebar;
            // set the key state
            if (keyState->endedDown != isDown)
            {
                keyState->endedDown = isDown; 
                ++keyState->priorState; 
            }
        }break;
          break;
    }
}

/*
 * We want to translate our coordinate space from 
 * what windows gives us to our opengl coordinate space
 * TODO improve this
 */
void rawMousePos(i32 x, i32 y) {
    // TODO  we need to handle negatives correctly
  v2 midPoint =
      v2{(f32)((APP_STATE.winDims.x / 2.0f)), (f32)((-APP_STATE.winDims.y / 2.0f))};
  v2 translatedCoord = v2{
        (f32)(x - (APP_STATE.winDims.x / 2.0f)),
        (f32)(y + (-APP_STATE.winDims.y / 2.0f))
      };
  v2 clicked = NormalizeCoordiantesV2(translatedCoord, midPoint);

  APP_STATE.currentDeviceInput.prevMousePos = APP_STATE.currentDeviceInput.normalizedMosePos;
  APP_STATE.currentDeviceInput.normalizedMosePos = clicked;
  APP_STATE.currentDeviceInput.rawMouseDelta = clicked - APP_STATE.currentDeviceInput.prevMousePos;
  vi2 nothing = APP_STATE.winDims;
}

#include "kc_win_window.h" //windows.h is defined in here

void exitApplicationCallback() {
    vf3 center = vf3{((f32)APP_STATE.winDims.x/2.0f), ((f32)APP_STATE.winDims.y/2.0f)};
    APP_STATE.applicationRunning = false;
}

void updateUI(application_state *appState, win32_window_handle_wrapper *windowRef) {
    if (APP_STATE.currentDeviceInput.inputDevices[0].leftClick.endedDown) {
            b32 isInRect = IsInRect(appState->uiElements["red-square"].position, appState->currentDeviceInput.normalizedMosePos);
            Rectangle2 rect = appState->uiElements["red-square"].position;
            v2 mousePos = appState->currentDeviceInput.normalizedMosePos; 
            v2 nothing0 = appState->currentDeviceInput.rawMouseDelta;
            if (appState->currentDeviceInput.rawMouseDelta > 0.0f || appState->currentDeviceInput.rawMouseDelta < 0.0f) {
                b32 smoething = false;
                if (IsInRect(appState->uiElements["red-square"].position, appState->currentDeviceInput.normalizedMosePos)) {
                    color c = appState->uiElements["red-square"].c;
                    v2 mouseForColor = appState->currentDeviceInput.normalizedMosePos;
                    color newColor = Color(mouseForColor.x, mouseForColor.y, mouseForColor.x, c.a);
                    appState->uiElements["red-square"].c = newColor;
                    OpenGlUpdateSubBufferColor(
                        &windowRef->win32OpenglContext,
                        appState->opengPreBuf["red-square"],
                        newColor,
                        windowRef->win32OpenglContext.shaders[MeshShader]);
            } else {
                smoething = true;
            }
        }
    }
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

    openglObjData redSquare = OpenGLRectangleSetupPreBufferedBatched(
          &windowRef.win32OpenglContext,
          RectMinMax(v2{0.0f, 0.0f}, v2{0.5f, 0.5f}),
          Color(1.0f, 0.0f, 0.0f, 1.0f),
          windowRef.win32OpenglContext.shaders[MeshShader]);

    APP_STATE.opengPreBuf["red-square"] = redSquare;
    APP_STATE.uiElements["red-square"] = kc_ui_id{
      .uiId = 0,
      .position = RectMinMax(v2{0.0f, 0.0f}, v2{0.5f, 0.5f}),
      .c = Color(1.0f, 0.0f, 0.0f, 1.0f),
      .renderData = renderableData{
           .vao = redSquare.vao,
           .vbo = redSquare.vbo,
           .ebo = redSquare.ebo,
           .indexBuffer = redSquare.indexBuffer
      }
  };

  device_input input[2] = {}; 
  device_input *newInput = &input[0];
  device_input *oldInput = &input[1];

  while(APP_STATE.applicationRunning) {
      input_device_attributes *oldKeyboardInput = 
          &oldInput->inputDevices[0]; 
      input_device_attributes *newKeyboardInput =
          &newInput->inputDevices[0]; 
      *newKeyboardInput = {}; 
      i8 btnArrayCount = ArrayCount(newKeyboardInput->buttons); 
      for(i8 btnIndex = 0; btnIndex < btnArrayCount; ++btnIndex)
      {
          newKeyboardInput->buttons[btnIndex].endedDown =
              oldKeyboardInput->buttons[btnIndex].endedDown;
      }

      Win32_ProcessInputFromMessage(
          windowRef.windowHandle,
          inputCallback, 
          rawMousePos,
          exitApplicationCallback
        );

      win32_window_dimensions winDims = Win32_GetWindowDimension(windowRef.windowHandle); 
      APP_STATE.winDims = winDims.value;

      updateUI(&APP_STATE, &windowRef);

      Win32_opengl_Render(
              &APP_STATE, 
              GetDC(windowRef.windowHandle), 
              &windowRef.win32OpenglContext,
              winDims.value.x, 
              winDims.value.y);
  }
        device_input *tempInput = newInput; 
        newInput = oldInput; 
        oldInput = tempInput;

        // TODO not handling the mouse button UP events correctly, this will most likely cause 
        // bugs later
        inputCallback(MOUSE_RIGHT, 1, 0);
        inputCallback(MOUSE_LEFT, 1, 0);
}
