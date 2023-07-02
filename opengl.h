#undef func
#include "windows.h"
#include "gl/gl.h" 
#include <wingdi.h>
#define func static

#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_VALIDATE_STATUS                0x8B83
#define GL_ARRAY_BUFFER                   0x8892
#define GL_STATIC_DRAW                    0x88E4
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_BGRA_EXT                       0x80E1
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C
//TODO Active textures
#define GL_TEXTURE0                       0x84C0
#define GL_DEBUG_OUTPUT                   0x92E0
#define GL_MAJOR_VERSION                  0x821B
#define GL_MINOR_VERSION                  0x821C

#define GL_SHADER_STORAGE_BUFFER          0x90D2
#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_SHADER_STORAGE_BUFFER          0x90D2

// WINDOWS ONLY!
#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB       0x2093
#define WGL_CONTEXT_FLAGS_ARB             0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB      0x9126
#define WGL_CONTEXT_DEBUG_BIT_ARB         0x0001
#define WGL_DRAW_TO_PBUFFER_ARB           0x202D
#define WGL_DRAW_TO_WINDOW_ARB            0x2001
#define WGL_SUPPORT_OPENGL_ARB            0x2010
#define WGL_DOUBLE_BUFFER_ARB             0x2011
#define WGL_PIXEL_TYPE_EXT                0x2013
#define WGL_COLOR_BITS_ARB                0x2014
#define WGL_PIXEL_TYPE_ARB                0x2013
#define WGL_TYPE_RGBA_ARB                 0x202B
#define WGL_SHARE_DEPTH_ARB               0x200C
#define WGL_SHARE_STENCIL_ARB             0x200D
#define WGL_DEPTH_BITS_ARB                0x2022
#define WGL_STENCIL_BITS_ARB              0x202
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB    0x0002
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

#define MAX_ALLOWABLE_SHADERS 20
#define SIZE_OF_VERT 3

// TODO and NOTE some of these are returning 0x00000 or null which will 
// cause a crash
typedef char GLchar;
typedef void WINAPI gl_tex_image_2d_multisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void WINAPI gl_bind_framebuffer(GLenum target, GLuint framebuffer);
typedef void WINAPI gl_gen_framebuffers(GLsizei n, GLuint *framebuffers);
typedef void WINAPI gl_framebuffer_texture_2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef GLenum WINAPI gl_check_framebuffer_status(GLenum target);
typedef void WINAPI gl_blit_framebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef void WINAPI gl_attach_shader(GLuint program, GLuint shader);
typedef void WINAPI gl_compile_shader(GLuint shader);
typedef GLuint WINAPI gl_create_program(void);
typedef GLuint WINAPI gl_create_shader(GLenum type);
typedef void WINAPI gl_link_program(GLuint program);
typedef void WINAPI gl_shader_source(GLuint shader, GLsizei count, GLchar **string, GLint *length);
typedef void WINAPI gl_use_program(GLuint program);
typedef void WINAPI gl_get_program_info_log(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void WINAPI gl_get_shader_info_log(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void WINAPI gl_validate_program(GLuint program);
typedef void WINAPI gl_get_program_iv(GLuint program, GLenum pname, GLint *params);
typedef GLint WINAPI gl_get_uniform_location (GLuint program, const GLchar *name);
typedef void WINAPI gl_uniform_4fv(GLint location, GLsizei count, const GLfloat *value);
typedef void WINAPI gl_uniform_matrix_4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void WINAPI gl_uniform_1i(GLint location, GLint v0);
typedef GLboolean WINAPI gl_is_program(GLuint program);
typedef void WINAPI gl_get_shader_iv(GLuint shader, GLenum pname, GLint *params);
typedef void WINAPI gl_bind_buffer_base(GLenum program, GLuint index, GLuint buffer); 
// TODO guessing on the i32 and ui32 below based on the docs
typedef void WINAPI gl_buffer_sub_data(GLenum target, i32 offset, ui32 size, const void *data);

typedef void WINAPI gl_vertex_attrib_pointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef void WINAPI gl_enable_vertex_attrib_array(GLuint);
typedef void WINAPI gl_gen_buffers(GLuint n, GLuint* buffers);
typedef void WINAPI gl_gen_vertex_array(GLsizei n, GLuint *arrays);
typedef void WINAPI gl_bind_vertex_array(GLuint array);
typedef void WINAPI gl_bind_buffer(GLenum target, GLuint buffer);
//TODO i32 should be GLsizeipt but we do not have it defiend, need to figure this out
typedef void WINAPI gl_buffer_data(GLenum target, i32 size, const void *data, GLenum usuage);
typedef void WINAPI gl_active_texture(GLenum texture);
// TODO the debug opengl call is supposed ot take GLDEBUGPROC but could not find define.
// used void* instead? not sure if this will work ok
typedef void WINAPI gl_debug_message_callback(void* callback, const void *userParam);

// Windows specific setup for context
typedef HGLRC WINAPI wgl_create_context_attribsARB(HDC hDC, HGLRC hShareContext, const i32 *attribList);
typedef BOOL WINAPI wgl_choose_pixel_formatARB(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);

/*
 * Contains OpenGL shader program ID
 * Descriptor is for us to know which shader this could be 
 * Does not and should not carry and compile or raw string shader data
 */
typedef struct shader
{
   i32 programID; 
   i32 vertexID; 
   i32 fragmentID; 
   ui32 descriptor;
} shader;

/*
 * Contains opengl information about current 
 * platform
 */
typedef struct opengl_info
{
   char* vendor;
   char* renderer;
   char* version; 
   char* shadingLanguageVersion;
   char* extensions; 
   i32 majorVersion;
   i32 minorVersion; 
} opengl_info;

/*
 * Represents an opengl vertex
 */
#pragma pack(push, 1)  
typedef struct vertex 
{
   f32 x, y, z;
   f32 u, v;
   f32 c;
} vertex;
#pragma pack(pop)

#define VERTEX_COUNT 4
#pragma pack(push, 1)  
typedef struct soa_vertex 
{
   f32 positions[VERTEX_COUNT * 3];
   f32 normals[VERTEX_COUNT * 3];
   f32 colors[VERTEX_COUNT * 4];
} soa_vertex;
#pragma pack(pop)

//TODO we will want to change this so that our vertex has texture data
#define packed_vertex ui32
#define X_SHIFT 0
#define Y_SHIFT 10
#define Z_SHIFT 20
#define W_SHIFT 30

/*
 * Creates a ui32 with packed bits
 */
#define PackVertex(x, y, z, w) \
   ((x) << X_SHIFT) | \
   ((y) << Y_SHIFT) | \
   ((z) << Z_SHIFT) | \
   ((w) << W_SHIFT)

#define SIDES_PER_CUBE 6 /*6 sides to a cube*/
#define INDICIES_PER_CUBE_SIDE 2 * SIDES_PER_CUBE /*2 * 6 sidestraingles per side */
#define CHUNK_SIZE 16
/*
 * A list of packed vertex. Each vertex is part of a cube 
 * these cubes all form a chunk
 */
typedef struct _opengl_chunk 
{
   ui32 vao;
   ui32 vbo;
   ui32 count;
   packed_vertex verts[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
   ui32 sidesOfCube;
   ui32 numberOfVertsPerSide;
} _opengl_chunk;

/* 
 * Contains buffer index numbers 
 * to refer back to after a setup call to
 * opengl populates these
 */
typedef struct openglObjData 
{
   ui32 vao;
   ui32 vbo;
   ui32 ebo;
   ui32 indexBuffer;
} openglObjData;

typedef struct opengl_context
{
   opengl_info openglInfo;
   shader shaders[MAX_ALLOWABLE_SHADERS];
   i32 _shaderCount;
   b32 isValid;

   //opengl functions
   gl_create_shader *glCreateShader;
   gl_tex_image_2d_multisample *glTexImage2DMultisample;
   gl_bind_framebuffer *glBindFramebuffer;
   gl_gen_framebuffers *glGenFramebuffers;
   gl_framebuffer_texture_2D *glFramebufferTexture2D;
   gl_check_framebuffer_status *glCheckFramebufferStatus;
   gl_blit_framebuffer *glBlitFramebuffer;
   gl_attach_shader *glAttachShader;
   gl_compile_shader *glCompileShader;
   gl_create_program *glCreateProgram;
   gl_link_program *glLinkProgram;
   gl_shader_source *glShaderSource;
   gl_use_program *glUseProgram;
   gl_get_program_info_log *glGetProgramInfoLog;
   gl_get_shader_info_log *glGetShaderInfoLog;
   gl_validate_program *glValidateProgram;
   gl_get_program_iv *glGetProgramiv;
   gl_get_uniform_location *glGetUniformLocation;
   gl_uniform_4fv *glUniform4fv;
   gl_uniform_matrix_4fv *glUniformMatrix4fv;
   gl_uniform_1i *glUniform1i;
   gl_is_program *glIsProgram;
   gl_get_shader_iv *glGetShaderiv;
   gl_bind_buffer_base *glBindBufferBase;

   gl_vertex_attrib_pointer *glVertexAttribPointer;
   gl_enable_vertex_attrib_array *glEnableVertexAttribArray;
   gl_gen_vertex_array *glGenVertexArrays;
   gl_gen_buffers *glGenBuffers;
   gl_bind_vertex_array *glBindVertexArray;
   gl_bind_buffer *glBindBuffer;
   gl_buffer_data *glBufferData;
   gl_active_texture *glActiveTexture;
   gl_debug_message_callback *glDebugMessageCallback;
   wgl_create_context_attribsARB *wglCreateContextAttribsARB;
   gl_buffer_sub_data *glBufferSubData;
} opengl_context;

/*
 * uncompiled shader code
 * for example basic_texture.h
 */
//TODO deprecate this
typedef struct raw_shader_code
{
   char* vertexCode;
   char* fragmentCode;
} raw_shader_code;

/*
 * Gets and returns opengl info from current platform 
 * @return opengl_info
 */
opengl_info 
OpenGLGetInfo()
{
    opengl_info result = {};
    result.vendor = (char*)glGetString(GL_VENDOR);
    result.renderer = (char*)glGetString(GL_RENDERER);
    result.version = (char*)glGetString(GL_VERSION);
    result.shadingLanguageVersion = (char *)glGetString(GL_SHADING_LANGUAGE_VERSION);
    glGetIntegerv(GL_MAJOR_VERSION, &result.majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &result.minorVersion);
    return result;
}

/*
 *@desc Given a shader, create the given type and return the 
 related shader
 * @param programId, this would be the programId of the shader
 * that is created 
 * @return returns a programId or -1 for not sucesfull 
 */
i32 
OpenGLCompileShader(
        opengl_context *opengl,
        char* shaderCode,
        GLenum shaderType,
        i32 programID
        )
{
    i32 result = -1;
    ui32 shaderId = opengl->glCreateShader(shaderType);
    opengl->glShaderSource(shaderId, 1, &shaderCode, 0); 
    opengl->glCompileShader(shaderId); 
    GLsizei infoLength;
    char programErrors[4096];
    opengl->glGetShaderInfoLog(shaderId, sizeof(programErrors), &infoLength, programErrors);
    Assert(infoLength <= 0);
    result = shaderId;
    Assert(result != -1);
    return result;
}

b32 
OpenGLLinkProgram(opengl_context *opengl,
            i32 programID)
{
    b32 result = false; //opengl->glIsProgram(programID);
    // if (result == true) 
    {
        opengl->glLinkProgram(programID);

        i32 isLinked = false;
        opengl->glGetProgramiv(programID, GL_LINK_STATUS, &isLinked);
        result = isLinked;
        if (isLinked == false) 
        {
            //TODO we can log this
            GLsizei ignored;
            char programErrors[4096];
            opengl->glGetShaderInfoLog(programID, sizeof(programErrors), &ignored, programErrors);
            Assert(isLinked == true);
        }
    }
    return result;
}

i32
OpenGLCreateProgram(
        opengl_context *opengl)
{
    ui32 programID = opengl->glCreateProgram();
    opengl->glValidateProgram(programID); 
    return programID;
}

void 
OpenGLIsValidShader(
        opengl_context *opengl, i32 programID)
{
    opengl->glValidateProgram(programID); 
}

void 
OpenGLAttachShader(opengl_context *opengl, i32 programID, i32 shaderID) 
{
    opengl->glAttachShader(programID, shaderID); 
}

/*
 *
 *@desc Tells OpenGL to compile a shader and associate 
 it with a program
 */
shader 
OpenGLCreateProgram(
        opengl_context *opengl,
        char* header, 
        char* vertexCode, 
        char* fragmentCode)
{
    shader result = {};
    ui32 vertexID = opengl->glCreateShader(GL_VERTEX_SHADER);
    opengl->glShaderSource(vertexID , 1, &vertexCode, 0); 
    opengl->glCompileShader(vertexID); 

    ui32 fragShaderID = opengl->glCreateShader(GL_FRAGMENT_SHADER); 
    opengl->glShaderSource(fragShaderID, 1, &fragmentCode, 0);

    ui32 programID = opengl->glCreateProgram();
    opengl->glAttachShader(programID, vertexID); 
    opengl->glAttachShader(programID, fragShaderID); 
    opengl->glLinkProgram(programID);
    opengl->glValidateProgram(programID); 
    i32 linked = false;
    opengl->glGetProgramiv(programID, GL_LINK_STATUS, &linked); 
    if (linked == false)
    {
        //TODO we can log this
        GLsizei ignored;
        char fragmentErrors[4096];
        char vertexErrors[4096];
        char programErrors[4096];
        opengl->glGetShaderInfoLog(vertexID, sizeof(vertexErrors), &ignored, vertexErrors);
        opengl->glGetShaderInfoLog(fragShaderID, sizeof(fragmentErrors), &ignored, fragmentErrors);
        opengl->glGetShaderInfoLog(programID, sizeof(programErrors), &ignored, programErrors);
        Assert(linked == true);
    }
    result.programID = programID;
    vertexID = vertexID;
    result.fragmentID = fragShaderID;

    return result;
}

/*
 * TODO this does not work anymore, not sure why 
 * Bind a texture 
 * @shaderProgram the compiled shader program
 * @param data the bitmap or other data to apply to texture
 * @return returns the texture ID
 */
ui32
Bind2DTexture(
        opengl_context *opengl,
        shader shaderProgram,
        loaded_bitmap* buffer)
{
    ui8 *bitmap = (ui8*)buffer->pixels;
    i32 width = buffer->width;
    i32 height = buffer->height;
    opengl->glUseProgram(shaderProgram.programID);
    return 0;
}

void
OpenGLLogError(
     GLenum source,
     GLenum type,
     GLuint id,
     GLenum severity,
     GLsizei length,
     const GLchar* message,
     const void* userParam)
{
    GLenum err = 0;
    Assert(err == 0);
}

/*
 * Sets up the buffer for later use
 */
openglObjData 
OpenGLRectangleSetupPreBuffered(
        opengl_context *opengl,
        Rectangle2 rect, 
        color c, 
        shader shaderProgram) 
{
    v2 min = rect.min;
    v2 max = rect.max;
    i32 programID = shaderProgram.programID;
    color cNorm = c;
    
    f32 vertices[] = {
        // positions          // colors           // texture coords
        max.x, max.y, 0,   cNorm.r, cNorm.g, cNorm.b,  1.0f, 1.0f,   // top right
        max.x, min.y, 0,   cNorm.r, cNorm.g, cNorm.b,  1.0f, 0.0f,   // bottom right
        min.x, min.y, 0,   cNorm.r, cNorm.g, cNorm.b,  0.0f, 0.0f,   //  bottom left
        min.x, max.y, 0,   cNorm.r, cNorm.g, cNorm.b,  0.0f, 1.0f    // top left 
            // The above z used to be -max.x 2-13
    }; 

    ui32 indices[] = 
    { 
        0, 1, 3, 
        1, 2, 3
    };

    ui32 VAO; 
    ui32 VBO; 
    ui32 EBO;
    opengl->glUseProgram(programID);
    opengl->glGenVertexArrays(1, &VAO);
    opengl->glGenBuffers(1, &VBO);
    opengl->glGenBuffers(1, &EBO);
    //TODO we must import the following functions if we get a compiler error
    opengl->glBindVertexArray(VAO);

    opengl->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    opengl->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    opengl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    opengl->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    opengl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)0);
    opengl->glEnableVertexAttribArray(0);
    // color attribute
    opengl->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(3 * sizeof(f32)));
    opengl->glEnableVertexAttribArray(1);
    // texture coord attribute
    opengl->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(6 * sizeof(f32)));
    opengl->glEnableVertexAttribArray(2);
    opengl->glBindVertexArray(VAO);

    openglObjData result = {};
    result.vao = VAO;
    result.vbo = VBO;
    result.ebo = EBO;
    return result;
}

/*
 * Sets up the buffer for later use
 */
openglObjData 
OpenGLRectangleSetupPreBufferedBatched(
        opengl_context *opengl,
        Rectangle2 rect, 
        color c, 
        shader shaderProgram) 
{
    v2 min = rect.min;
    v2 max = rect.max;
    i32 programID = shaderProgram.programID;
    color cNorm = c;
    
    f32 pos[] = {
        // positions    
        max.x, max.y, 0,
        max.x, min.y, 0,
        min.x, min.y, 0,
        min.x, max.y, 0
            // The above z used to be -max.x 2-13
    }; 

    f32 norms[] = {
           // colors           
        cNorm.r, cNorm.g, cNorm.b,
        cNorm.r, cNorm.g, cNorm.b,
        cNorm.r, cNorm.g, cNorm.b,
        cNorm.r, cNorm.g, cNorm.b
            // The above z used to be -max.x 2-13
    }; 

    f32 tex[] = {
        // texture coords
        1.0f, 1.0f,   // top right
        1.0f, 0.0f,   // bottom right
        0.0f, 0.0f,   //  bottom left
        0.0f, 1.0f    // top left 
            // The above z used to be -max.x 2-13
    }; 

    ui32 indices[] = 
    { 
        0, 1, 3, 
        1, 2, 3
    };

    ui32 VAO; 
    ui32 VBO; 
    ui32 EBO;
    opengl->glUseProgram(programID);
    opengl->glGenVertexArrays(1, &VAO);
    opengl->glGenBuffers(1, &VBO);
    opengl->glGenBuffers(1, &EBO);
    //TODO we must import the following functions if we get a compiler error
    opengl->glBindVertexArray(VAO);

    opengl->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    opengl->glBufferData(GL_ARRAY_BUFFER, sizeof(norms) + sizeof(pos) + sizeof(tex), 0, GL_STATIC_DRAW);

    opengl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    opengl->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    opengl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), (void*)0);
    opengl->glEnableVertexAttribArray(0);
    // color attribute
    opengl->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), (void*)(sizeof(pos)));
    opengl->glEnableVertexAttribArray(1);
    // texture coord attribute
    opengl->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(f32), (void*)(sizeof(norms) + sizeof(pos)));
    opengl->glEnableVertexAttribArray(2);
    opengl->glBindVertexArray(VAO);

    opengl->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
    opengl->glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(norms), &norms);
    opengl->glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos) + sizeof(norms), sizeof(tex), &tex);

    openglObjData result = {};
    result.vao = VAO;
    result.vbo = VBO;
    result.ebo = EBO;
    return result;
}

void OpenGlUpdateSubBufferColor(
        opengl_context *opengl,
        openglObjData objData,
        color newColor,
        shader shaderProgram) {
    f32 norms[] = {
        newColor.r, newColor.g, newColor.b, // top right
        newColor.r, newColor.g, newColor.b, // bottom right
        newColor.r, newColor.g, newColor.b, // bottom left
        newColor.r, newColor.g, newColor.b // top left 
    }; 

    opengl->glUseProgram(shaderProgram.programID);
    opengl->glBindVertexArray(objData.vao);
    opengl->glBindBuffer(GL_ARRAY_BUFFER, objData.vbo);
    opengl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objData.ebo);

    // opengl->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(3 * sizeof(f32)));
    opengl->glBufferSubData(GL_ARRAY_BUFFER, (4*3)*sizeof(f32), sizeof(norms), &norms);
}

/*
 * Requires that vertices are pre-set to opengl
 */ 
void OpenGlDrawRectanglePreBuffered(
        opengl_context *opengl,
        openglObjData objData,
        m4 modelMat,
        m4 projMat, 
        m4 viewMat,
        shader shaderProgram) 
{
    i32 programID = shaderProgram.programID;
    opengl->glUseProgram(programID);
    opengl->glBindVertexArray(objData.vao);
    opengl->glBindBuffer(GL_ARRAY_BUFFER, objData.vbo);
    opengl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objData.ebo);

    i32 modelLoc = opengl->glGetUniformLocation(programID, "model");
    i32 viewLoc = opengl->glGetUniformLocation(programID, "view");
    i32 projLoc = opengl->glGetUniformLocation(programID, "proj");

    Assert(modelLoc >= 0);
    Assert(viewLoc >= 0);
    Assert(projLoc >= 0);

    opengl->glUniformMatrix4fv(projLoc, 1, GL_FALSE, projMat.e[0]);
    opengl->glUniformMatrix4fv(viewLoc, 1, GL_FALSE, viewMat.e[0]);
    opengl->glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMat.e[0]);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

/*
 * Stack allocates the verticies
 * CAN CAUSE MEMORY LEAK DUE TO WINDOWS API HEAP ALLOCATING
 * TODO clear free/delete buffer when done?
 */ 
void 
OpenGL2DRectangle_Raw(
        opengl_context *opengl,
        m4 modelMat,
        m4 projMat, 
        m4 viewMat,
        Rectangle2 rect, 
        color c, 
        shader shaderProgram) 
{
    v2 min = rect.min;
    v2 max = rect.max;
    i32 programID = shaderProgram.programID;
    color cNorm = c;
    
    f32 vertices[] = {
        // positions          // colors           // texture coords
        max.x, max.y, 0,   cNorm.r, cNorm.g, cNorm.b,  1.0f, 1.0f,   // top right
        max.x, min.y, 0,   cNorm.r, cNorm.g, cNorm.b,  1.0f, 0.0f,   // bottom right
        min.x, min.y, 0,   cNorm.r, cNorm.g, cNorm.b,  0.0f, 0.0f,   //  bottom left
        min.x, max.y, 0,   cNorm.r, cNorm.g, cNorm.b,  0.0f, 1.0f    // top left 
            // The above z used to be -max.x 2-13
    }; 

    ui32 indices[] = 
    { 
        0, 1, 3, 
        1, 2, 3
    };

    ui32 VAO; 
    ui32 VBO; 
    ui32 EBO;
    opengl->glUseProgram(programID);
    opengl->glGenVertexArrays(1, &VAO);
    opengl->glGenBuffers(1, &VBO);
    opengl->glGenBuffers(1, &EBO);
    //TODO we must import the following functions if we get a compiler error
    opengl->glBindVertexArray(VAO);

    opengl->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    opengl->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    opengl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    opengl->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    opengl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)0);
    opengl->glEnableVertexAttribArray(0);
    // color attribute
    opengl->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(3 * sizeof(f32)));
    opengl->glEnableVertexAttribArray(1);
    // texture coord attribute
    opengl->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(6 * sizeof(f32)));
    opengl->glEnableVertexAttribArray(2);
    opengl->glBindVertexArray(VAO);

    i32 modelLoc = opengl->glGetUniformLocation(programID, "model");
    i32 viewLoc = opengl->glGetUniformLocation(programID, "view");
    i32 projLoc = opengl->glGetUniformLocation(programID, "proj");

#if INTERNAL_BUILD
    vf4 testPoint = {max.x, min.y, -max.x, 1.0f};
    vf4 vert = projMat * viewMat * modelMat * testPoint;
#endif 

    Assert(modelLoc >= 0);
    Assert(viewLoc >= 0);
    Assert(projLoc >= 0);

    m4 ident = GetIdentityMatrix();
    //opengl->glUniformMatrix4fv(projLoc, 1, GL_FALSE, ident.e[0]);
    //opengl->glUniformMatrix4fv(viewLoc, 1, GL_FALSE, ident.e[0]);
    //opengl->glUniformMatrix3fv(modelLoc, 1, GL_FALSE, ident.e[0]);

    opengl->glUniformMatrix4fv(projLoc, 1, GL_FALSE, projMat.e[0]);
    opengl->glUniformMatrix4fv(viewLoc, 1, GL_FALSE, viewMat.e[0]);
    opengl->glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMat.e[0]);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void 
OpenGlClearAndSetViewPort(color clearColor, ui32 winWidth, ui32 winHeight) 
{
    f32 r = clearColor.r; 
    f32 g = clearColor.g;
    f32 b = clearColor.b;
    f32 a = clearColor.a;
    glViewport(0, 0, winWidth, winHeight); 
    glClearColor(r, g, b, a); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
}

void 
InitOpenGl(opengl_context *opengl, HWND window, HDC windowDC)
{
    HGLRC tempContext = wglCreateContext(windowDC); 
    HGLRC finalContext = {};
    opengl->isValid = (tempContext != 0); // this is not valid yet...
    Assert(tempContext != 0);
    if (wglMakeCurrent(windowDC, tempContext)) 
    {
        opengl->wglCreateContextAttribsARB = (wgl_create_context_attribsARB *)wglGetProcAddress("wglCreateContextAttribsARB");
        // wglDeleteContext(openGLRC); 
        // ReleaseDC(window, windowDC);
        // DestroyWindow(window);

        if (opengl->wglCreateContextAttribsARB)
        {
            //NOTE and TODO there are more items we can add here
            i32 win32_openGLAttribs[] =
            {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
                WGL_CONTEXT_MINOR_VERSION_ARB, 6,
                0,
            };

            // TODO https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL)
            // We don't want this to be compile time, but rather during run time depending on the users system and their opengl version
            if (opengl->wglCreateContextAttribsARB) {
                finalContext = opengl->wglCreateContextAttribsARB(windowDC, 0, win32_openGLAttribs);
                ui32 error = GetLastError();
                Assert(finalContext != 0);
                if (finalContext  == 0) 
                {
                    finalContext= wglCreateContext(windowDC); //NOTE this is a fix for if we loose our modern context again
                }
            }
            Assert(finalContext != 0); 
            wglMakeCurrent(0, 0);
            wglDeleteContext(tempContext); 
            // NOTE: make current for finalContext is called below
        }
        else 
        {
            finalContext = wglCreateContext(windowDC);
            InvalidCodePath("Opegnl createContext defaults used improperly");
        }
    }
    if (wglMakeCurrent(windowDC, finalContext)) 
    {
        Assert(wglGetProcAddress != 0); 
        opengl->glTexImage2DMultisample = (gl_tex_image_2d_multisample *)wglGetProcAddress("glTexImage2DMultisample");
        opengl->glBlitFramebuffer = (gl_blit_framebuffer *)wglGetProcAddress("glBlitFramebuffer");

        opengl->glAttachShader = (gl_attach_shader *)wglGetProcAddress("glAttachShader");
        opengl->glCompileShader = (gl_compile_shader *)wglGetProcAddress("glCompileShader");
        opengl->glCreateProgram = (gl_create_program *)wglGetProcAddress("glCreateProgram");
        opengl->glCreateShader = (gl_create_shader *)wglGetProcAddress("glCreateShader");
        opengl->glLinkProgram = (gl_link_program *)wglGetProcAddress("glLinkProgram");
        Assert(opengl->glLinkProgram != 0);
        opengl->glShaderSource = (gl_shader_source *)wglGetProcAddress("glShaderSource");
        opengl->glUseProgram = (gl_use_program *)wglGetProcAddress("glUseProgram");
        opengl->glGetProgramInfoLog = (gl_get_program_info_log *)wglGetProcAddress("glGetProgramInfoLog");
        opengl->glGetShaderInfoLog = (gl_get_shader_info_log *)wglGetProcAddress("glGetShaderInfoLog");
        opengl->glValidateProgram = (gl_validate_program *)wglGetProcAddress("glValidateProgram");
        opengl->glGetProgramiv = (gl_get_program_iv *)wglGetProcAddress("glGetProgramiv");
        opengl->glGetUniformLocation = (gl_get_uniform_location *)wglGetProcAddress("glGetUniformLocation");
        opengl->glUniform4fv = (gl_uniform_4fv *)wglGetProcAddress("glUniform4fv");
        opengl->glUniformMatrix4fv = (gl_uniform_matrix_4fv *)wglGetProcAddress("glUniformMatrix4fv");
        opengl->glUniform1i = (gl_uniform_1i *)wglGetProcAddress("glUniform1i");
        opengl->glVertexAttribPointer = (gl_vertex_attrib_pointer *)wglGetProcAddress("glVertexAttribPointer");
        opengl->glEnableVertexAttribArray = (gl_enable_vertex_attrib_array *)wglGetProcAddress("glEnableVertexAttribArray");
        opengl->glGenBuffers = (gl_gen_buffers *)wglGetProcAddress("glGenBuffers");
        opengl->glGenVertexArrays = (gl_gen_vertex_array *)wglGetProcAddress("glGenVertexArrays");
        opengl->glBindVertexArray = (gl_bind_vertex_array *)wglGetProcAddress("glBindVertexArray");
        opengl->glBindBuffer = (gl_bind_buffer *)wglGetProcAddress("glBindBuffer");
        opengl->glBufferData = (gl_buffer_data *)wglGetProcAddress("glBufferData");
        opengl->glActiveTexture = (gl_active_texture *)wglGetProcAddress("glActiveTexture");
        opengl->glBindBufferBase = (gl_bind_buffer_base*)wglGetProcAddress("glBindBufferBase");
        opengl->glBufferSubData = (gl_buffer_sub_data*)wglGetProcAddress("glBufferSubData");

        opengl->openglInfo = OpenGLGetInfo();
        // TODO debug only in debug builds
        // TODO we should some how validate each ptr, as their are some returning 0x000 
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEPTH_TEST);
        // TODO fix debug is now being 0x000 
        // opengl->glDebugMessageCallback(OpenGLLogError, 0);

        //  char openglInfoBuffer[256];
        /*snprintf(
                openglInfoBuffer, 
                sizeof(openglInfoBuffer), 
                "OPENGL INFO: vendor %s | version %s | shader lang version %s", 
                opengl->openglInfo.vendor,
                opengl->openglInfo.version, 
                opengl->openglInfo.shadingLanguageVersion);
        OutputDebugStringA(openglInfoBuffer); */
        //TODO do we need to do anything with switching versions 
        // like the createContextATtrib call? 
    }
    else 
    {
        InvalidCodePath; 
    }
    ReleaseDC(window, windowDC); 
}


typedef enum SHADER_TYPE_POSITION
{
    MeshShader = 0, 
    PureTexture = 1,
    PackedVertex = 2,
} SHADER_TYPE_POSITION;

#define MAX_SHADER_LOCATION_LENGTH 60
typedef struct Shaders
{
    char *vertexShaders[MAX_SHADER_LOCATION_LENGTH];
    char *fragmentShaders[MAX_SHADER_LOCATION_LENGTH];
    i32 currentCount;
} Shaders; 

i32 
CountActiveShaders(Shaders *shaders) 
{
    i32 vertexSize = 0;
    i32 fragmentSize = 0;
    for (i32 i = 0; i < MAX_SHADER_LOCATION_LENGTH; ++i) 
    {
        if (shaders->vertexShaders[i] != 0) 
        {
            ++vertexSize;
        }
        if (shaders->fragmentShaders[i] != 0)
        {
            ++fragmentSize;
        }
    }
    Assert(vertexSize == fragmentSize);
    return vertexSize; // they should match, so it does not matter what we return
}


b32 
isValidShader(opengl_context *opengl, shader *s) {
    if (s->programID && s->vertexID && s->fragmentID) {
        // opengl can also validate TODO log this out w/ opengl at somepoint
        OpenGLIsValidShader(opengl, s->programID);
        return true;
    }
    return false;
}

shader 
InitShader(
        memory_arena *memArena,
        opengl_context *opengl, 
        char* vertexLocation,
        char* fragmentLocation) 
{
    shader result = {};

    char* vertCode = readFileAsCharArr(memArena, vertexLocation);
    char* fragmentCode = readFileAsCharArr(memArena, fragmentLocation);
    result.vertexID = OpenGLCompileShader(
            opengl, 
            vertCode,
            GL_VERTEX_SHADER,
            -1 //TODO to add program ID for checking to make sure its valid
            );

    result.fragmentID = OpenGLCompileShader(
            opengl, 
            fragmentCode,
            GL_FRAGMENT_SHADER,
            -1
            );
    result.programID = OpenGLCreateProgram(opengl);

    OpenGLAttachShader(opengl, result.programID, result.vertexID);
    OpenGLAttachShader(opengl, result.programID, result.fragmentID);
    OpenGLLinkProgram(opengl, result.programID);

    b32 isValid = isValidShader(opengl, &result);
    Assert(isValid == true); 

    Assert(opengl->shaders != 0);
    
    return result;
}

void initOpenGLShaderBuff(opengl_context *context, ui32 bufferSize) {

    ui32 VAO = 0; 
    ui32 SSBO = 0;

    context->glGenVertexArrays(1, &VAO);
    context->glBindVertexArray(VAO);

    context->glGenBuffers(1, &SSBO);
    context->glBindBuffer(1, SSBO);
    context->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);

    context->glBufferData(GL_SHADER_STORAGE_BUFFER, bufferSize, 0, GL_DYNAMIC_DRAW); 
}
