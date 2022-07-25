#ifndef UTYPES_H
#define UTYPES_H

//#pragma pack(4)

typedef unsigned char  uchar_8;
typedef unsigned int   uint32_t;
typedef unsigned short uint16_t;

union UStuff32_t
{
  float  va_float;
  int  va_int;
  uchar_8  ascii[4];
};

#define hl_to_int16(l,h) ((uchar_8)h << 8 | (uchar_8)l)

#define hl_to_int32(l0,l1,h0,h1) ((uchar_8)h1 << 24 |(uchar_8)h0 << 16|(uchar_8)l1 << 8| (uchar_8)l0)

#endif // UTYPES_H
