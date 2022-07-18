#ifndef UTYPES_H
#define UTYPES_H

//#pragma pack(4)

typedef unsigned char  uchar_8;
typedef unsigned int   uint32_t;
typedef unsigned short uint16_t;

union _UStuff_t
{
  float  va_float;
  int  va_int;
  uchar_8  ascii[4];
};

#endif // UTYPES_H
