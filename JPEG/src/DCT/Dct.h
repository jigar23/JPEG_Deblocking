/*
 * Dct.h
 *
 *  Created on: Feb 24, 2014
 *      Author: jigar
 */

#ifndef DCT_H_
#define DCT_H_
//#include <tgmath.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "../global.h"

#define PI 3.1415926
#define BLOCKSIZE 8

char** computeDCTwithQuantization(char **imgData, int size, int level);
char** subtract128(unsigned char **imageData, int size);
double** computeDCT(char **imgData, int size);


#endif /* DCT_H_ */
