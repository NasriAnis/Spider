typedef __UINT8_TYPE__   uint8_t;   // 1 byte
typedef __UINT16_TYPE__  uint16_t;  // 2 bytes
typedef __UINT32_TYPE__  uint32_t;  // 4 bytes
typedef __UINT64_TYPE__  uint64_t;  // 8 bytes

typedef __SIZE_TYPE__    size_t;
typedef __UINTPTR_TYPE__ uintptr_t;
#ifndef NULL
    #ifdef __cplusplus
        #define NULL 0
    #else
        #define NULL ((void *)0)
    #endif
#endif