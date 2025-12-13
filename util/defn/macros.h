#ifndef MACROS_H
#define MACROS_H

#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a < b ? b : a)

#define MIN3(a, b, c) (MIN(MIN(a, b), c))
#define MAX3(a, b, c) (MAX(MAX(a, b), c))

#define RAD_TO_DEG(a) (a * 180.f / PI)

#endif
