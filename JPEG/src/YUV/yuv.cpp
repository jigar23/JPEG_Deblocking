/*
 * yuv.cpp
 *
 *  Created on: Feb 27, 2014
 *      Author: jigar
 */

#include "yuv.h"

namespace color{
	float rgb2yuvMat[3][3] = {
			{0.299,0.587,0.114},
			{-0.14713,-0.28886,0.436},
			{0.615,-0.51499,-0.10001},
	};
	float yuv2rgbMat[3][3] = {
			{1,0,1.13983},
			{1,-0.39465,-0.58060},
			{1,2.03211,0},
	};
}

void rgb2yuv(float*** ImageData,int size){
	for(int i = 0; i < size; i++)
		for(int j = 0; j < size; j++){
				float r = ImageData[i][j][0];
				float g = ImageData[i][j][1];
				float b = ImageData[i][j][2];
				for(int k = 0; k < 3; k++)
					ImageData[i][j][k] = color::rgb2yuvMat[k][0]*r + color::rgb2yuvMat[k][1]*g +
								color::rgb2yuvMat[k][2]*b;
				}
}


void yuv2rgb(float*** ImageData,int size){
	for(int i = 0; i < size; i++)
		for(int j = 0; j < size; j++){
				float y = ImageData[i][j][0];
				float u = ImageData[i][j][1];
				float v = ImageData[i][j][2];
				for(int k = 0; k < 3; k++)
					ImageData[i][j][k] = color::yuv2rgbMat[k][0]*y + color::yuv2rgbMat[k][1]*u +
							color::yuv2rgbMat[k][2]*v;
				}
}
