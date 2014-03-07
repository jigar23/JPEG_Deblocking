/*
 * deblock.h
 *
 *  Created on: Feb 26, 2014
 *      Author: jigar
 */

#ifndef DEBLOCK_H_
#define DEBLOCK_H_
#include <math.h>
#include <iostream>
#include "../global.h"
#include <stdlib.h>

using namespace std;
#define BLOCKSIZE 8

const int T1 = 2;
const int T2 = 6;
const int b[] = {1,1,2,2,4,2,2,1,1};

void deblockJpegImageV(float ***Imgdata, int size);
void deblockJpegImageH(float ***Imgdata, int size);
unsigned char*** floatToChar(float ***data, int size, int bitsperpixel);

#endif /* DEBLOCK_H_ */
