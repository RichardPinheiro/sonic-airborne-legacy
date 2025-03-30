#ifndef UTILS_H
#define UTILS_H

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define CLAMP(x, low, high) (MAX(MIN(x, high), low))

#endif
