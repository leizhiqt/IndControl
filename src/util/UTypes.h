#ifndef UTYPES_H
#define UTYPES_H

//#pragma pack(4)

typedef unsigned char uchar_8;

union _UStuff_t
{
  float  va_float;
  int  va_int;
  uchar_8  ascii[4];
};

#endif // UTYPES_H
