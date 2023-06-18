#pragma once
#include "windows.h"
#include "kc_lib.h"

/// TODO memory arena allocator etc.
#define memory_index size_t 
#define PushStruct(memArena, type) (type *)PushSize_(memArena, sizeof(type))
#define PushArray(memArena, count, type) (type *)PushSize_(memArena, count*sizeof(type))

typedef struct memory_arena {
  b32 isInitialized; 

  ui64 permanentStorageSize; 
  void* permanentStorage; 

  ui64 totalMemorySize;
  memory_index used;
  memory_index size;

  ui8* base;
} memory_arena;

inline void* PushSize_(memory_arena *memArena, memory_index size)
{
    //NOTE: Windows will clear this all to zero for us, but we need to handle 
    //any other cases when this might not be true
    Assert((memArena->used + size) <= memArena->size); 
    void *result = memArena->base + memArena->used; 
    memArena->used += size; 
    return result; 
}

memory_arena
init_memory_arena() {
    memory_arena result = {};

    result.permanentStorageSize = Gigabytes((ui64)2); 
    result.totalMemorySize = result.permanentStorageSize;
#if INTERNAL_BUILD
    // set it really high for easier debugging
            LPVOID baseAddress = (LPVOID)Terabytes((ui64)2);
#else                           
            LPVOID baseAddress = 0;
#endif 

    result.permanentStorage = VirtualAlloc(baseAddress,
                result.totalMemorySize, 
                MEM_COMMIT | MEM_RESERVE,
                PAGE_READWRITE); 
    result.size = result.totalMemorySize;
    result.base = (ui8 *)result.permanentStorage;
    result.used = 0;
    return result;
}
