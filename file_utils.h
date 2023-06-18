#include "kc_lib.h"
#include "windows.h"

typedef struct file_result_internal {
    b32 status;
    ui32 fileSize;
    char* data;        
    HANDLE fileHandle;
    DWORD bytesRead;
} file_result;

func file_result_internal readFileInternal(char* path, memory_arena *arena) {
    file_result_internal result = {};
    result.fileHandle = CreateFile(
            path, 
            GENERIC_READ, 
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            0, 
            OPEN_EXISTING, 
            0, 
            0
            );
    if(result.fileHandle != INVALID_HANDLE_VALUE)
    {
        result.fileSize = GetFileSize(result.fileHandle, NULL);
        if (result.fileSize) {

            // TODO implement allocate
            result.data = (char*)PushArray(arena, result.fileSize+1, char);

            if (result.data) {
                // NOTE we do not support larger than 4 gb files
                if (ReadFile(result.fileHandle, result.data, result.fileSize, &result.bytesRead, 0)) {
                    result.status = true;
                    result.data[result.bytesRead] = 0; // null term
                }
                else 
                {
                    result.data = 0; 
                }
            }
        }
    }
    return result; 
}

func char* readFileAsCharArr(memory_arena *arena, char* path) {
    file_result_internal result = readFileInternal(path, arena);
    if (result.status == true) {
        CloseHandle(result.fileHandle);
    }
    return result.data;
}
