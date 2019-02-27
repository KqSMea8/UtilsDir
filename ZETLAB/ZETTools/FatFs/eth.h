#ifndef fatfs_eth_h
#define fatfs_eth_h 28062013


#include "integer.h"


typedef void * work_class_ptr;

typedef void (fatfs_eth_read_func)(BYTE, DWORD /*sector from*/, BYTE * /*buf*/, work_class_ptr);
typedef void (fatfs_eth_write_func)(BYTE, DWORD /*sector from*/, const BYTE * /*buf*/, work_class_ptr);


#endif	//fatfs_eth_h