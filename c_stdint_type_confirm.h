#ifndef _TYPE_BASIC_H
#define _TYPE_BASIC_H

typedef unsigned char      uint8_t;
typedef signed char        int8_t;

typedef unsigned short     uint16_t;
typedef signed short       int16_t;

typedef unsigned int       uint32_t;
typedef signed int         int32_t;

typedef unsigned long long uint64_t;
typedef signed long long   int64_t;

/* register access qualifiers */
#define __I   volatile const
#define __O   volatile
#define __IO  volatile
#endif /* TYPE_BASIC_H */