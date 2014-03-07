/*
 * fileIO.h
 *
 *  Created on: Feb 24, 2014
 *      Author: jigar
 */

#ifndef FILEIO_H_
#define FILEIO_H_
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
using namespace std;

int getFileSize(char filename[]);
float*** readRawImage(char filename[], int size, int);
void writeRawImage(unsigned char ***Imagedata, int Size, int bitsperpixel);
unsigned char** readBWrawImage(char filename[], int size);

#endif /* FILEIO_H_ */
