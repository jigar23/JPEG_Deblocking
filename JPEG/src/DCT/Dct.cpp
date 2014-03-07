/*
 * Dct.cpp
 *
 *  Created on: Feb 24, 2014
 *      Author: jigar
 */

#include "Dct.h"

double** getQuantizationMatrix(int N);

/**
 * Subtract 128 from each pixel value to change the range of pixel values to [-128,127]
 * instead of the original [0, 255]. This process is needed because the DCT is designed to
 * work on symmetric pixel values ranging from -128 to 127.
 *
 */
char** subtract128(unsigned char **imageData, int size){

	char **outputData = new char*[size];
	for(int i = 0; i < size; i++)
		outputData[i] = new char[size];

	for(int i = 0; i < size; i++)
		for(int j = 0; j < size; j++)
			outputData[i][j] = imageData[i][j] - 128;

	return outputData;
}

namespace dct1{
	char **p;

}

/**
 * This function computes a single DCT coefficient
 * using the formula  specified in the report.
 */
double getDctCoeff(int i, int j, int k, int l){
	double val = 0, C_i = 1, C_j = 1;
	if(i == 0) C_i = 1/sqrt(2);
	if(j == 0) C_j = 1/sqrt(2);

	for(int x = 0; x < BLOCKSIZE; x++)
		for(int y = 0; y < BLOCKSIZE; y++)
			val += dct1::p[x+k][y+l] * cos((2*x+1)*i*PI/(2*BLOCKSIZE)) * cos((2*y+1)*j*PI/(2*BLOCKSIZE));

	val *= C_i*C_j/(sqrt(2*BLOCKSIZE));

	return val;
}

/**
 * This function computes the DCT of full image by dividing the image
 * into block sizes of 8x8.
 * It also does the quantization of the images.
 */
char** computeDCTwithQuantization(char **imgData, int size, int level){
	int k = 0, l = 0;
	//assigning the pointer of original image data to global variable
	//so that it can be easily accessible in the function getDCTCoeff();
	dct1::p = imgData;

	//Assigning memory to the DCT Coefficient data
	char **D = new char*[size];
	for(int i = 0; i < size; i++)
		D[i] = new char[size];

	//Get the quantization matrix
	double **Q = getQuantizationMatrix(level);

	while(k < size){
		while(l < size){
			for(int i = 0; i < 8; i++)
				for(int j = 0; j < 8; j++)
//					D[i+k][j+l] = static_cast<char>(round(getDctCoeff(i,j,k,l)/Q[i][j]));
					D[i+k][j+l] = getDctCoeff(i,j,k,l);
			l = l + BLOCKSIZE;
		}
		k = k + BLOCKSIZE;
		l = 0;
	}

	return D;
}

/**
 * This function gives the desired quantization matrix
 * given the level.
 * The quantization matrix of size 8x8 only.
 */
double** getQuantizationMatrix(int N){
	int size = 8;
	double temp;
	double **Q = new double*[size];
	for(int i = 0; i < size; i++)
		Q[i] = new double[size];

	if(N >= 50 && N <= 100){
		temp = (float)(100-N)/50;
	}
	else if(N < 50 && N > 0){
		temp = (float)50/N;
	}
	else{
		std::cerr << "Invalid level.!! Level should be between 1 and 100 only." << std::endl;
		exit(1);
	}

	for(int i = 0; i < size; i++)
		for(int j = 0; j < size; j++)
			Q[i][j] = temp * dct::Q[i][j];

	for(int i = 0; i < 8; i++){
			for(int j = 0; j < 8; j++)
				std::cout << (int)Q[i][j] << " ";
			std::cout << std::endl;
		}

	return Q;
}

/**
 * This function just computes the DCT of full image by dividing the image
 * into block sizes of 8x8.
 */
double** computeDCT(char **imgData, int size){
	int k = 0, l = 0;
	//assigning the pointer of original image data to global variable
	//so that it can be easily accessible in the function getDCTCoeff();
	dct1::p = imgData;

	//Assigning memory to the DCT Coefficient data
	double **D = new double*[size];
	for(int i = 0; i < size; i++)
		D[i] = new double[size];

	while(k < size){
		while(l < size){
			for(int i = 0; i < 8; i++)
				for(int j = 0; j < 8; j++)
					D[i+k][j+l] = getDctCoeff(i,j,k,l);
			l = l + BLOCKSIZE;
		}
		k = k + BLOCKSIZE;
		l = 0;
	}

	return D;
}
