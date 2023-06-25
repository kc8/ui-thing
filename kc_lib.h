/////////// READ ME BEFORE USING ///////////
/// these functions may not be safe, nor are they 
/// tested. Use at your own risk!
////////////////////////////////////////////

#pragma once
#include "stdint.h" 
#include "xmmintrin.h"
#include "math.h"

#include "mem_allocator.h"
#include <stdlib.h>

// CUSTOM DEFINES / REDEFINES
#define UI32MAX 0XFFFFFFFF
#define F32MAX 3.402823466E+38F
#define F32MIN 1.175494351e-38F

#define local_persist static
#define global_var static
#define func static

/// TYPES 
typedef uint8_t ui8;
typedef uint16_t ui16; 
typedef uint32_t ui32; 
typedef uint64_t ui64; 

typedef int8_t i8;  
typedef int16_t i16; 
typedef int32_t i32; 
typedef int64_t i64; 

typedef float f32; 
typedef double f64; 

typedef int b32; 
#define true 1
#define false 0

typedef void* item; //note that we do not really use this 

// Debug Utilities 

#if INTERNAL_BUILD 
#define Assert(expression) if (!(expression)) {*(int*)0=0;}
#else 
#define Assert(expression)
#endif

#if INTERNAL_BUILD
#define NotImplYet Assert(!"Code Not Implemented Yet")
#else 
#define NotImplYet Assert(expression)
#endif

#if INTERNAL_BUILD 
#define InvalidCodePath Assert(!"Invalid Code Path")
#else 
#define InvalidCodePath(expression)
#endif

#define ArrayCount(array) (sizeof((array)) /sizeof((array)[0]))

#define Kilobytes(value) ((value) * 1024LL)
#define Megabytes(value) (Kilobytes((value)) * 1024LL)
#define Gigabytes(value) (Megabytes((value)) * 1024LL)
#define Terabytes(value) (Gigabytes((value)) * 1024LL)

#define Maximum(a, b) ((a < b) ? (a) : (b))
#define Minimum(a, b) ((a > b) ? (a) : (b))

// MATH

#define pi32 3.14159265359f


/////////// INTRINSICS ///////////////
inline f32 
TanF32(f32 angle)
{
    f32 result = tanf(angle); 
    return result;
}

inline f32 
SinF32(f32 angle)
{
    f32 result = sinf(angle); 
    return result;
}

inline f32 
CosF32(f32 angle)
{
    f32 result = cosf(angle); 
    return result;
}

inline i32
RoundFloatToInt(f32 floatVal)
{
    i32 result = (i32)roundf((floatVal)); 
    return result; 
}

inline ui32 
RoundFloatToUInt(f32 floatVal)
{
    ui32 result = (ui32)roundf(floatVal); 
    return result; 
}

inline f32
SquareRoot(f32 floatVal)
{
    f32 result = sqrtf(floatVal);
    return result;
}

inline ui32 
RoundF32ToUI32(f32 floatVal) 
{
    ui32 result = (ui32)_mm_cvtss_si32(_mm_set_ss(floatVal));
    return result;
}

typedef struct bit_scan_result 
{
    b32 found; 
    ui32 index; 
} bit_scan_result; 


inline bit_scan_result
FindLeastSignificantFirstBit(ui32 value)
{
    //TODO this function would be better with intrinsics
   bit_scan_result bitScanResult = {0};  
   bitScanResult.found = false;
#if COMPILER_MSVC 
   //NOTE C++ compiler gives error for long unsigned type mismatch, 
   //we just cast back and forth, they are really all unsigned 32 bit ints
    unsigned long result; 
    bitScanResult.found = _BitScanForward(&result, value);
    bitScanResult.index = (ui32)result; 
#else
    if (1)
    {
        // NOTE if this happens we do not have the intrinsic 
        i32 *index; 
        index[100]; 
    }
   for (ui32 test = 0; test < 32; ++test) 
   {
       // start at least sig bit, shift up, if that bit is one that 
       // is ourt starting index
       if (value & (1 << test)) 
       {
           bitScanResult.index = test; 
           bitScanResult.found = true; 
           break; 
       }
   }
#endif
   return bitScanResult; 
};

/*
 * Interpolate t between two values a and b
 * An Affine transform
 * For instance: a mid poiunt between two lines can be calculated 
 * by giving t = 1/2. This turns out to be the same formula
 * as the mid point equation
 * Book Explanations: 
 * Computer Graphics 3rd ed. page 135
 */
inline f32 
LERP(f32 a, f32 b, f32 t) 
{
    f32 result = a + (b - a) * t;
    return result;
}


////////// VECTOR ////////////////

//
// VECTOR Float 32 4 length
// 
typedef struct vf4
{
    union
    {
        struct
        {
            f32 x;
            f32 y; 
            f32 z; 
            f32 w; 
        };
        f32 e[4]; 
    };
    //Below is an example Comprimise if we were not using a union to 
    //access the elements with [] syntax
    //f32 &operator[](int index) {return((&x)[index]);}; 
} vf4;

//
// VECTOR UI 3
// 
typedef struct vui3 
{
    union
    {
        struct
        {
            ui32 x;
            ui32 y; 
            ui32 z; 
        };
        ui32 e[3]; 
    };
    //Below is an example Comprimise if we were not using a union to 
    //access the elements with [] syntax
    //f32 &operator[](int index) {return((&x)[index]);}; 
} vuif;

//
// VECTOR F 3
// 
typedef struct vf3
{
    union
    {
        struct
        {
            f32 x;
            f32 y; 
            f32 z; 
        };
        f32 e[3]; 
    };
    //Below is an example Comprimise if we were not using a union to 
    //access the elements with [] syntax
    //f32 &operator[](int index) {return((&x)[index]);}; 
} vf3;

inline vf3
operator-(vf3 a, vf3 b)
{
    vf3 result; 
    result.x = a.x - b.x;  
    result.y = a.y - b.y;  
    result.z = a.z - b.z;  
    return result; 
}

/// return the unary opposite of the vector
inline vf3 operator-(vf3 a)
{
    vf3 result; 
    result.x = -a.x; 
    result.y = -a.y; 
    result.y = -a.z; 
    return result;
}

inline vf3 
operator-=(vf3 a, vf3 b)
{
    a = a - b;
    return a; 
}

inline b32
operator==(vf3 a, vf3 b)
{
   b32 result = 0;
   if (a.x == b.x &&
       a.y == b.y && 
       a.z == b.z)
   {
        result = 1;
   }
   return result;
}

inline vf3 operator+(vf3 a, vf3 b)
{
    vf3 result; 
    result.x = a.x + b.x;  
    result.y = a.y + b.y;  
    result.z = a.z + b.z;  
    return result; 
}

// return a new vector scaled to a f32
inline vf3 operator*(f32 a, vf3 b)
{
    vf3 result = {};
    result.x = a * b.x;
    result.y = a * b.y; 
    result.z = a * b.z; 
    return result; 
}

inline vf3 operator*(vf3 b, f32 a)
{
    vf3 result = {}; 
    result.x = a * b.x;;  
    result.y = a * b.y;  
    result.z = a * b.z;  
    return result; 
}

inline vf3 &
operator*=(vf3 &a, f32 b)
{
    a = b * a; 
    return a;
}

// Dot or scalar product 
inline f32 
Inner(vf3 a, vf3 b)
{
    f32 result = a.x*b.x + a.y*b.y + a.z*b.z;
    return result; 
}

/* 
 * Often denoted ||v||
 * Often done by the sum of the squares of the entries
 * Length of 1 is a unit vector
 */
inline f32  
LengthSq(vf3 a)
{
    f32 result = Inner(a, a); 
    return result; 
}

/*
 * Non-communitive
 * Important properties: 
 *  - Going from v to Xv involves a rotation by 90 degrees in the same direction 
 *  as the rotation that takess the positive x-axis to the positive y-axis
 */
inline vf3
Cross(vf3 lhs, vf3 rhs)
{
    vf3 result = {};
    result.x = (lhs.y * rhs.z) - (lhs.z * rhs.y);
    result.y = (lhs.z * rhs.x) - (lhs.x * rhs.z);
    result.z = (lhs.x * rhs.y) - (lhs.y * rhs.x);
    return result;
}

/// 
/// Vectot 2 INT
///
typedef struct vi2 
{
    // Here would be a good example of a union use. 
    union
    {
        struct
        {
            i32 x, y; 
        };
        i32 e[2]; 
    };
    //Below is an example Comprimise if we were not using a union to 
    //access the elements with [] syntax
    //f32 &operator[](int index) {return((&x)[index]);}; 
} vi2;

inline vi2 operator*(vi2 b, i32 a)
{
    vi2 result = {}; 
    result.x = a * b.x;;  
    result.y = a * b.y;  
    return result; 
}

// TODO cast is most likely bad and wrong
inline vi2 operator*(vi2 b, f32 a)
{
    vi2 result = {}; 
    result.x = (i32)a * b.x;;  
    result.y = (i32)a * b.y;  
    return result; 
}

// VECTOR FLOAT 2
// 
typedef struct v2 
{
    // Here would be a good example of a union use. 
    union
    {
        struct
        {
            f32 x, y; 
        };
        f32 e[2]; 
    };
    //Below is an example Comprimise if we were not using a union to 
    //access the elements with [] syntax
    //f32 &operator[](int index) {return((&x)[index]);}; 
    v2 (f32 x, f32 y) {
        this->x = x;
        this->y = y;
    }
    v2 () {
    }
} v2;


// NOTE in C++ unions do not let us 
//  init like a struct will 
//  see NOTES regarding workaround at struct def. 
//  Leaving this here as an altenrative method
inline v2 Vec2(f32 x, f32 y)
{
    v2 result; 
    result.x = x; 
    result.y = y; 
    return result;
}

inline v2 Vec2(f32 e)
{
    v2 result; 
    result.x = e; 
    result.y = e; 
    return result;
}

// return a new vector scaled to a f32
inline v2 operator*(f32 a, v2 b)
{
    v2 result = {};
    result.x = a * b.x;
    result.y = a * b.y; 
    return result; 
}

#if 0
// This is just an example of SIMD operation
inline v2 SIMDVector(f32 a, f32 b) 
{
    v2 result = {0.0f, 0.0f}; 
    __m128 v0 = _mm_set_ps(1.0f, 2.0f, 34.0f, 20.0f); 
    __m128 v1 = _mm_set_ps(b, b, b, b);
    __m128 sum = _mm_add_ps(v0, v1); 
    return result;
}
#endif 

inline v2 operator*(v2 b, f32 a)
{
    v2 result = {}; 
    result.x = a * b.x;;  
    result.y = a * b.y;  
    return result; 
}

inline v2 operator+(v2 a, v2 b)
{
    v2 result; 
    result.x = a.x + b.x;  
    result.y = a.y + b.y;  
    return result; 
}

/// return the unary opposite of the vector
inline v2 operator-(v2 a)
{
    v2 result; 
    result.x = -a.x; 
    result.y = -a.y; 
    return result;
}

inline v2 
operator-(v2 a, v2 b)
{
    v2 result; 
    result.x = a.x - b.x;  
    result.y = a.y - b.y;  
    return result; 
}

inline v2 &
operator*=(v2 &a, f32 b)
{
    a = b * a; 
    return a;
}

inline v2 &  
operator+=(v2 &a, v2 b)
{
    a = a + b; 
    return a;
}

/* 
 * Dot or scalar product 
* Used for measuring angles 
* v dot w = cos(theta) where theta 
* is the angle between the vectors
*/
inline f32 
Inner(v2 a, v2 b)
{
    f32 result = a.x*b.x + a.y*b.y; 
    return result; 
}

// NOTE no cross product because its somewhat useless for 2d space

/* 
 * Often denoted ||v||
 * Often done by the sum of the squares of the entries
 * Length of 1 is a unit vector
 */
inline f32  
LengthSq(v2 a)
{
    f32 result = Inner(a, a); 
    return result; 
}
//END VEC 2

/* 
 * A List ofs TODOs and things to understand better
 * 
 * - Covectors pg 163 of Copmuter Graphics 3rd ed. These are helpful in representing 
 *   triangle normals (the normal to a triangle is a nonzero vector perpendicualr to the plane 
 *   of the triangle). But I believe this is really the dot product
 *
 */

inline b32 
Intereset(vf3 a, vf3 b) 
{
    b32 result = false;
    NotImplYet("Not implemented yet");
    return result;
}


/////////// MATRIX ////////////////

/*
* Describes a 4x4 matrix of type float 32
 * Row major
 */
typedef struct m4 
{
    f32 e[4][4];
} m4; 

/*
 * Describes an inverse and the forward 
 * matrix as to not have to calculate eachone multiple times
 */
typedef struct m4_inv
{
    m4 forward;
    m4 inverse;
} m4_inv; 

func m4
operator*(m4 a, m4 b)
{
    m4 result = {};
    for (i32 y = 0; y < 4; ++y)
    {
        for (i32 x = 0; x < 4; ++x)
        {
            result.e[y][x] = 
                (a.e[y][0] * b.e[0][x]) +
                (a.e[y][1] * b.e[1][x]) +
                (a.e[y][2] * b.e[2][x]) +
                (a.e[y][3] * b.e[3][x]);
        }
    }
    return result;
}

/* 
 * Transpose a matrix hows are n x k elements 
 * which becomes k x n
 */ 
func m4 
Transpose(m4 m)
{
    m4 result = {};
    for(int j = 0; j <= 3; ++j)
    {
        for(int i = 0; i <= 3; ++i)
        {
            result.e[j][i] = m.e[i][j];
        }
    }
    return result;
}

func vf3
operator*(m4 a, vf3 b)
{
    vf3 result = {};
    result.x = b.x*a.e[0][0] + b.y*a.e[0][1] + b.z*a.e[0][2]; //+ b.W*a.e[0][3];
    result.y = b.x*a.e[1][0] + b.y*a.e[1][1] + b.z*a.e[1][2]; //+ b.W*a.e[1][3];
    result.z = b.x*a.e[2][0] + b.y*a.e[2][1] + b.z*a.e[2][2]; //+ b.W*a.e[2][3];
    //result.W = b.X*a[3,0] + b.Y*a[3,1] + b.Z*a[3,2] + b.W*a[3,3];
    return result;
}

func vf4
operator*(m4 a, vf4 b)
{
    vf4 result = {};
    result.x = b.x*a.e[0][0] + b.y*a.e[0][1] + b.z*a.e[0][2] + b.w*a.e[0][3];
    result.y = b.x*a.e[1][0] + b.y*a.e[1][1] + b.z*a.e[1][2] + b.w*a.e[1][3];
    result.z = b.x*a.e[2][0] + b.y*a.e[2][1] + b.z*a.e[2][2] + b.w*a.e[2][3];
    result.w = b.x*a.e[3][0] + b.y*a.e[3][1] + b.z*a.e[3][2] + b.w*a.e[3][3];
    return result;
}

/*
 * Returns an identity matrix
 */
inline m4
GetIdentityMatrix()
{
    m4 result = 
    {
        {
        {1,0,0,0},
        {0,1,0,0},
        {0,0,1,0},
        {0,0,0,1}
        },
    };
    return result;
};

inline m4
Translation(vf3 t)
{
    m4 result = 
    {
        {
        {1,0,0, t.x},
        {0,1,0, t.y},
        {0,0,1, t.z},
        {0,0,0,1}
        }
    };
    return result;
};

/* Rotate a matrix around an axis by a degrees
 *@param a: is the degrees to rotate by 
 */          
inline m4
RotateAboutAxis(m4 mat, f32 a, vf3 axis) 
{
    f32 c = CosF32(a);
    f32 s = SinF32(a);
    f32 x = axis.x;
    f32 y = axis.y;
    f32 z = axis.z;

    m4 result = mat;
    result = 
    {
        {
        {x*x*(1-c)+c, x*y*(1-c)-z*s, x*z*(1-c)+y*s, 0},
        {y*x*(1-c)+z*s, y*y*(1-c)+c, y*z*(1-c)-x*s, 0},
        {z*x*(1-c)-y*s, z*y*(1-c)+x*s, z*z*(1-c)+c, 0}, 
        {0, 0, 0, 1}
        }
    };
    result = result *mat;
    return result;
}

//TODO should quaterions be used instead?
inline m4
ZRotation(f32 a)
{

    f32 c = CosF32(a);
    f32 s = SinF32(a);
    m4 result = 
    {
        {
        {c,-s,0,0},
        {s,c,0,0},
        {0,0,1,0},
        {0,0,0,1}
        }
    };
    return result;
}

inline m4
YRotation(f32 a)
{

    f32 c = CosF32(a);
    f32 s = SinF32(a);
    m4 result = 
    {
        {
        {c,0,s,0},
        {0,1,0,0},
        {-s,0,c,0},
        {0,0,0,1}
        }
    };
    return result;
}

inline m4
XRotation(f32 a)
{
    f32 c = CosF32(a);
    f32 s = SinF32(a);
    m4 result = 
    {
        {
        {1,0,0,0},
        {0,c,-s,0},
        {0,s,c,0},
        {0,0,0,1}
        }
    };
    return result;
}

//////////// other math //////////////

//value between 0 and 1
func vf3
NormalizeCoordiantes(vf3 a, vf3 b)
{
    vf3 result = {}; 
    if (b.x > 0 && b.y > 0 && b.z > 0) 
    {
        result.x = a.x/b.x; 
        result.y = a.y/b.y; 
        result.z = a.z/b.z; 
    }
    else 
    {
        result = a; 
    }
    return result; 
}

//value between 0 and 1
func v2
NormalizeCoordiantesV2(v2 a, v2 b)
{
    v2 result = {}; 
    if (b.x > 0 && b.y > 0)
    {
        result.x = a.x/b.x; 
        result.y = a.y/b.y;
    }
    else 
    {
        result = a; 
    }
    return result; 
}

func f32
Magnitude(v2 v)
{
    f32 result = 0.0f;
    f32 lengthSquared = SquareRoot(LengthSq(v));
    result = lengthSquared;
    return result; 
}

func v2
NormalizeVector(v2 v)
{ 
    f32 x = v.x; 
    f32 y = v.y; 
    v2 result = {x,y};
    if (x < 0 || 
        x > 1 || 
        y < 0 ||
        y > 1)
    {
        f32 mag = Magnitude(v);
        if (mag == 0)
        {
            result = v;
        }
        else
        {
            f32 mult = 1.0f/mag;
            result = v;
            result.x = v.x * mult;
            result.y = v.y * mult;
        }
    }
    return result;
}

func f32
Magnitude(vf3 v)
{
    f32 result = 0.0f;
    f32 lengthSquared = SquareRoot(LengthSq(v));
    result = lengthSquared;
    return result; 
}

func vf3
NormalizeVector(vf3 v)
{
    f32 x = v.x; 
    f32 y = v.y; 
    f32 z = v.z;
    vf3 result = {x,y,z};
    if (x < 0 || 
        x > 1 || 
        y < 0 ||
        y > 1 ||
        z < 0 ||
        z > 1)
    {
        f32 mag = Magnitude(v);
        if (mag == 0)
        {
            result = v;
        }
        else
        {
            f32 mult = 1.0f/mag;
            result = v;
            result.x = v.x * mult;
            result.y = v.y * mult;
            result.z = v.z * mult;
        }
    }
    return result;
}

f32 inline 
Square(f32 a)
{
    f32 result = a * a;
    return result; 
}

typedef struct Rectangle2
{
    v2 min; 
    v2 max; 
} Rectangle2;

typedef struct Cube
{
    vf3 min; 
    vf3 max; 
} Cube;

inline Rectangle2
RectMinMax(v2 min, v2 max)
{
    Rectangle2 result; 
    result.min = min; 
    result.max = max; 
    return result; 
}

inline Rectangle2
RectCenterHalfDim(v2 center, v2 halfDim)
{
    Rectangle2 result;
    result.min = center - halfDim; 
    result.max  = center + halfDim; 
    return result;
}

//Set corner nad compute out the rest of the rect
inline Rectangle2
RectMinDim(v2 min, v2 dim)
{
    Rectangle2 result;
    result.min = min; 
    result.max  = min + dim; 
    return result;
}

inline Rectangle2
RectCenterDim(v2 center, v2 dim)
{
    Rectangle2 result = RectCenterHalfDim(center, 0.5f*dim); 
    return result;
}

// INTERACTION Exculusive rectangle test
inline b32 
IsInRect(Rectangle2 rect, v2 test)
{
    b32 result = false; 
    result = (
            (test.x >= rect.min.x) && 
            (test.y >= rect.min.y) &&
            (test.x < rect.max.x) &&
            (test.y < rect.max.y)); 
    return result;  
}

inline f32 
ToRadians(f32 degree) 
{
    f32 result = 0.0f;
    result = degree * pi32/180;
    return result;
}

/*
 * Safe division (check if divisor > 0.0f) if not returns 
 * 1.0f (the default) or another value n if overwritten 
 */
inline f32 
Ratio(f32 numerator, f32 divisor, f32 n = 1.0f)
{
    f32 result = n;
    if (divisor > 0.0f) 
    {
        result = numerator/divisor; 
    }
    return result;
}

inline f32 
ClampF32(f32 val, f32 min, f32 max) {
    f32 result = val;
    if (val < min) 
    {
        result = min;
    }
    else if (val > max) 
    {
        result = max;
    }
    return result;
}

/*
 * Standard implicit form of a line
 * page 165 of Computer Graphics 3rd ed
 */
inline b32 IsPointOnLine(vf3 x, vf3 p, vf3 normal) 
{
    f32 result = Inner((x - p), normal);
    if (result == 0) return true;
    return false;
}


///////////// BITMAP /////////////////

// TODO replace with a bitmap library?
typedef struct loaded_bitmap 
{
    i32 height; 
    i32 width; 
    ui32 *pixels; 

} loaded_bitmap; 

// The assumption is that we do not have compression here
#pragma pack(push, 1)
struct bitmap_header
{
    ui16 fileType;
    ui32 fileSize; 
    ui16 reserved1; 
    ui16 reserved2; 
    ui32 bitmapOffset; 
    ui32 size; 
    i32 width; 
    i32 height;
    ui16 planes; 
    ui16 bitsPerPixel; 
    ui32 compression; 
    ui32 sizeOfBitmap; 
    i32 horzResolution; 
    i32 vertResolution; 
    ui32 colorsUsed; 
    ui32 colorsImportant; 

    ui32 redMask; 
    ui32 greenMask;
    ui32 blueMask; 
};
#pragma pack(pop)

////////////// OTHER UTILS //////////////

func void 
ReverseUI32ArrayInPlace(ui32 *array, ui32 start, ui32 end) 
{
    while(start < end) 
    {
        ui32 temp = *(array + start); 
        array[start] = array[end];
        array[end] = temp;
        start++;
        end--;
    }
}

/* 
 * Checks if the value of a char is between 0 and 9. 
 * Does not check sign and will return false if passed 
 * any kind of negative value
 */
inline b32 
IsNum(char c) 
{
    if (c >= '0' && c <= '9') 
    {
        return true;
    }
    return false;
}

inline i32
CharStarSeek(char* string, char seekTo) 
{
    char* lookingForString = string;
    i32 result = 0;
    while(*lookingForString++ != seekTo)
    {
        result++;
    }
    return result; 
}

inline ui32 
SafeTruncateU64(ui64 value)
{
    Assert(value <= UI32MAX);
    return (ui32)value; 
}; 


///////////// COLOR ////////////////

typedef struct color 
{
    union 
    {
        struct
        {
            f32 r, g, b, a;
        };
        f32 e[4];
    };
} color;

inline color 
Color(f32 r, f32 g, f32 b, f32 a)
{
   color result = {};
   result.r = r;
   result.g = g;
   result.b = b;
   result.a = a;
   return result;
};

inline color 
Color()
{
   color result = {};
   result.r = 0.0f;
   result.g = 0.0f;
   result.b = 0.0f;
   result.a = 0.0f;
   return result;
};


////// STRINGS ///////////

// NOTE passing to this means that you will need to 
// consider the null terminator of the source strings. 
// subtract 1 from their count when you pass in if needed
// thie method does add the null terminiator at the end
// TODO bounds checking
// TODO to get inside the header or to keep in a CPP file? 
func void 
ConcatStrings(size_t source_a_count, char *source_a, 
                size_t source_b_count, char *source_b,
                size_t dest_count, char *dest)
{
    for (int index=0; index < source_a_count; ++index)
    {
        *dest++ = *source_a++; 
    }
    for (int index=0; index < source_b_count; ++index)
    {
        *dest++ = *source_b++; 
    }

    *dest++ = 0; 
}

// This does not count the null terminator
func int 
ComputeStringLengthASCII(char* string)
{
    int count = 0; 
    while(*string++) 
    {
        ++count;
    }
    return count; 
}

/* TODO
 *  - Strip off any leading blank/spaces?
 *  (example more detailed implementation)http://www.beedub.com/Sprite093/src/lib/c/stdlib/atof.c
 *  - Accuracy, well we loose accuracy 
 */
func f32 
StringToFloat(char* string) 
{
    f32 result = 0.0f;
    char* charFloat = string;
    f32 sign = 1;
    f32 fractionPart = 1;
    b32 pointSeen = 0;

    if (*charFloat == '-')
    {
        sign = -1;
        charFloat++;
    }

    for (;*charFloat; charFloat++)
    {
        char curChar = *charFloat;
        if (IsNum(curChar) == false) 
        {
            if (curChar == '.') 
            {
                pointSeen = true;
                continue; 
            }
            else 
            {
                break;
            }
        }
        i32 curCharAsNum = (i32)curChar - '0';
        if (curCharAsNum >= 0 && curCharAsNum <= 9) 
        {
            if (pointSeen) 
            {
               fractionPart *= 0.10f; 
            }
            result = result * 10.0f + (f32)curCharAsNum;
        }
    }

    result = result * fractionPart * sign;
    return result; 
}

///////////////////// INPUTS ///////////////////

enum KEY_TYPE {
    MOUSE_LEFT = 0, 
    MOUSE_RIGHT = 1,
    SPACEBAR = 2,
};

typedef struct input_device_state {
    i32 priorState; 
    b32 endedDown; 
    b32 isUp;
} input_device_state;

#define ALLOWED_STATES 14
typedef struct input_device_attributes {
    vf3 startPos;
    vf3 minMaxPos;
    vf3 endPos;
    union {
        input_device_state buttons[ALLOWED_STATES];
        struct {
            // keyboard
            input_device_state spacebar;
            // mouse
            input_device_state leftClick;
            input_device_state rightClick;
        };
    };
} input_device_attributes;

typedef struct device_input {
    v2 normalizedMosePos;
    v2 prevMousePos;
    v2 rawMouseDelta;
    vf3 rawMousePos; 
    v2 prevwRawMousePos;
    f32 secondsElapsed;
    input_device_attributes inputDevices[1];
    f32 dtSeconds;
} input;


///////// DATA STRUCTS /////// 
///
// NOTE and TODO this is a work in progress and will get more features 
// as needed
/*
 * static array with some nice features
 */
template <typename T, i32 N>
struct kc_array {
    i32 capacity = N;
    i32 count = 0;
    T items[N];

    T& operator[](int index) {
        Assert(index <= capacity);
        Assert(index >= 0);
        return items[index];
    }
    
    constexpr i32 add(T element) {
        Assert(this->count < capacity);
        items[this->count] = element;
        this->count += 1;
        return count - 1; // return pos
    }
}; 

// hash table
//
//
func ui32 hash(char* str) {
    ui32 hash = 5381;
    while(true) {
        i32 c = *str++;
        if (!c) {
            break;
        }
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

/*
 * This is not a robust function. It hashes an item into an index 
 * and does not consider duplicates or if the index is taken. 
 * This could means key could collide
 */
template <typename T>
struct kc_hashTable {
    enum STATUS {
        DUPLICATE_KEY, 
        FAILED_TO_ADD,
        SUCCESS,
    };
    const static ui32 MAX_ELEMENTS = 1024;
    T items[MAX_ELEMENTS];
    ui32 currentKeys[MAX_ELEMENTS];

    ui32 getPos(char* key) {
        return hash(key) % MAX_ELEMENTS;
    }

    T& operator[](char* key) {
        ui32 pos = this->getPos(key);
        Assert(pos < MAX_ELEMENTS);
        return this->items[pos];
    }
    
    T& get(char* key) {
        ui32 pos = this->getPos(key);
        Assert(pos < MAX_ELEMENTS);
        if (currentKeys[pos] == 1) {
            return this->items[pos];
        }
        return 0;
    }

    T& pop(char* key) {
        ui32 pos = this->getPos(key);
        Assert(pos < MAX_ELEMENTS);
        if (currentKeys[pos] == 1) {
            currentKeys[pos] = 0;
            return this->items[pos];
        }
        return 0;
    }

    STATUS add(char key, T element) {
        ui32 pos = this->getPos(key);

        if (currentKeys[pos] == 1) {
            return  DUPLICATE_KEY;
        }
        currentKeys[pos] = 1;
        items[pos] = element;
        return SUCCESS;
    }
}; 


///////// UI /////

typedef struct kc_ui {
    // TODO i32 is not current but just to make sure things are working it should be okay
   kc_hashTable<i32> uiElements; 
} kc_ui;

// Rectangle2 has its own function for within rect bounds
