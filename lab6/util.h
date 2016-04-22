#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <cmath>
#include <stdio.h>
#include <cstdlib>
#include <random>
#include <tuple>
#include <string>
#include <bitset>
#define PI (3.14159265)
#define SQRT2 (0.707106)
#define BUFSIZE (15)
using namespace std;

double hamming_distance(bitset<2> c,bitset<2> d);
double calculate_ber(bitset<BUFSIZE*8> rx,bitset<BUFSIZE*8> tx);
#endif
