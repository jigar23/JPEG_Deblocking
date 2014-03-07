/*
 * deblock.cpp
 *
 *  Created on: Feb 26, 2014
 *      Author: jigar
 */

#include "deblock.h"

bool modeDecision(float *V);
float* smoothMode(float *V, int QP);
float minVal(float a, float b, float c);
float* defaultMode(float *V, int QP);
float clip(float x, float p, float q);
int sign(float number);
void printArray(float *V, int size);


/**
 * This function does the image deblocking in the horizontal directions.
 * It takes the input image (generally JPEG compressed) and applies the
 * deblocking filter as explained in the paper.
 */
void deblockJpegImageH(float ***Imgdata, int size){

	cout << "Horizontal Scanning-- " << endl;

	//temp array which stored the block to be processed.
	float *Harray = new float[BLOCKSIZE+2];
	//stores the modified data
	float *V;
	//The 2 while loops scan the whole 2-d image array
	//considering the blocks of size 10 at a time as
	//explained in the paper.
	int k = 0, l = 3,QP;
	while(k < size){
		while((l+BLOCKSIZE) < size){
			int no = 0;
			for(int i = l; i < (l+BLOCKSIZE+2); i++){
				//extract the block.
					Harray[no++] = Imgdata[k][i][0];
			}
//			printArray(Harray,10);
			//get the value of QP
			QP = dct::Q[k%8][0];

			//decide which mode to select
			bool dec = modeDecision(Harray);
			//smooth region mode
			if(dec) V = smoothMode(Harray,QP);
			//default region mode
			else V = defaultMode(Harray,QP);
//			cout << "Modified-->" << endl;
//			printArray(V,10);
			no = 0;
			//Copy the content of the modified data in the same 3-d array
			for (int i = l; i < (l + BLOCKSIZE + 2); i++) {
				//for horizontal scanning //Modify only the Y component for colored image.
				Imgdata[k][i][0] = V[no++];
//				cout << MImgdata[k][i] << " ";
				}
//			cout << endl;
			//---ELements copied--//

			l = l + BLOCKSIZE;
		}
		l = 3;
		k++;
	}
}

/**
 * This function does the image deblocking in the hvertical direction.
 * It takes the horizontally modified image and applies the
 * deblocking filter as explained in the paper and returs the
 * final modified raw image.
 */
void deblockJpegImageV(float ***Imgdata, int size){

	cout << "Vertical Scanning--" << endl;

	float *Harray = new float[BLOCKSIZE+2];
	float *V;
	int k = 0, l = 3,QP;
	while(k < size){
		while((l+BLOCKSIZE) < size){
			int no = 0;
			for(int i = l; i < (l+BLOCKSIZE+2); i++){
					Harray[no++] = Imgdata[i][k][0];
			}
//			printArray(Harray,10);
			QP = dct::Q[0][k%8];

			bool dec = modeDecision(Harray);
			//smooth region mode
			if(dec) V = smoothMode(Harray,QP);
			//default region mode
			else V = defaultMode(Harray,QP);
//			cout << "Modified-->" << endl;
//			printArray(V,10);

			no = 0;
			//Copy the content of the modified data back in the 3-d array
			for (int i = l; i < (l + BLOCKSIZE + 2); i++) {
				//for vertical scanning
				Imgdata[i][k][0] = V[no++];
//				cout << (int)MImgdata[i][k] << " ";
				}
			//---ELemenst copied--//
//			cout << endl;
			l = l + BLOCKSIZE;
		}
		l = 3;
		k++;
	}
}

/**
 * This function selects the mode:
 * return value 1: smooth mode
 * return value 2: default mode
 */
bool modeDecision(float *V){
	int F = 0;
	for(int i = 0; i <= BLOCKSIZE; i++){
		if(fabs(V[i]-V[i+1]) <= T1)
			F++;
	}

	if(F > T2)
		//smooth region mode
		return 1;
	//default mode
	return 0;
}

/**
 * All the operations related to the default mode
 * are done in this function.
 * It takes a block and the quantization value
 * and returns the modified block.
 * All the variable names are the same as used in the paper
 * so the code is easy to understand
 */
float* defaultMode(float *V, int QP){
//	cout << "entering Default mode" << endl;
	float c1 = 2, c2 = 5, c3 = 8;
	float *a3 = new float[3];
//	cout << "QP is : " << QP << endl;
	for(int k = 0; k <= 2 ; k++){
		a3[k] = (c1*V[2*k+1]-c2*V[2*k+2]+c2*V[2*k+3]-c1*V[2*k+4])/c3;
//		cout << a3[k] << " ";
	}
//	cout << endl;
	//if less than QP then return
	if(fabs(a3[1]) < QP) {
//		cout << "No change " << endl;
		return V;
	}

	float a31p = sign(a3[1])*minVal(fabs(a3[0]),fabs(a3[1]),fabs(a3[2]));
//	cout << "a31p = " << a31p << endl;
	float d = clip((c2*(a31p-a3[1])/c3),0,(V[4]-V[5]))/2;
//	cout << "D is: " << d << endl;
	V[4] = V[4] - d;
	V[5] = V[5] + d;
	return V;
}

/**
 * This function gives the sign of number;
 * e.g.
 * Sign of -10 is -1
 * Sign of 10 is 1
 * Sign of 0 is 0
 */
int sign(float number){
	if(number > 0) return 1;
	else if(number < 0) return -1;
	else return 0;
}

/**
 * This function gives the minimum of 3 values
 */
float minVal(float a, float b, float c){
	float temp;
//	cout <<"Minimum of " << a << "," << b <<"and" << c << "is: " << endl;
	return (temp = (a < b? a:b)) < c? temp:c;
}

//clip x between p and q
float clip(float x, float p, float q){
	float temp;
	if(x <= p) temp = p;
	else if(x >= q) temp = q;
	else temp = x;
	return temp;
}

/**
 * All the operations related to the smooth mode
 * are done in this function.
 * It takes a block and the quantization value
 * and returns the modified block.
 * All the variable names are the same as used in the paper
 * so the code is easy to understand
 */
float* smoothMode(float *V, int QP){
//	cout << "entering Smooth mode" << endl;
	int max = V[1];
	int min = V[1];
	//Find the max and min in the array from 1-8
	for(int i = 1; i <= 8 ;i++){
		if(V[i] > max) max = V[i];
		else if(V[i] < min) min = V[i];
	}
//	cout << "MAX: " << max << endl;
//	cout << "MIN: " << min << endl;
	//define values of P
	float P0 = V[1];
	float P9 = V[8];
	if(fabs(V[1]-V[0]) < QP) P0 = V[0];
	if(fabs(V[9]-V[8]) < QP) P9 = V[9];

	float Pnk, temp;
	//Compute new values of V -> Vp
	float *Vp = new float[BLOCKSIZE+2];

	if((max-min) >= 2*QP) return V;

	for(int n = 1; n <= 8 ; n++){
		temp = 0;
		for(int k = 0; k <= 8; k++){
			//get the value of Pnk
			if(n+k-4 < 1) Pnk = P0;
			else if(n+k-4 > 8) Pnk = P9;
			else Pnk = V[n+k-4];
//			cout << n << " " << k <<" "<< b[k] << " " <<Pnk << "|| ";
			temp += b[k]*Pnk;
		}
		Vp[n] = temp/16;
//		cout << endl << Vp[n] << endl;
	}

	Vp[0] = V[0];
	Vp[9] = V[9];
	return Vp;
}

void printArray(float *V, int size){
	for(int i = 0; i < size; i++)
		cout << V[i] <<" ";
	cout << endl;
}


unsigned char*** floatToChar(float ***data, int size, int bitsperpixel){

	//Create memory for data
	unsigned char ***Imagedata;
	Imagedata = new unsigned char**[size];
	for (int i = 0; i < size; ++i) {
		Imagedata[i] = new unsigned char*[size];

		for (int j = 0; j < size; ++j)
			Imagedata[i][j] = new unsigned char[bitsperpixel];
	}

	for(int i = 0; i < size; i++)
		for(int j = 0; j < size; j++)
			for(int k = 0; k < bitsperpixel; k++){
				unsigned char val = (unsigned char)data[i][j][k];
				if (val < 0 || val > 255)
				    {
				      cerr << "Error while converting from float to unsigned char..!!" << endl;
				      cout << "Value greater than 255 or less than 0." << endl;
				      cout << "Exiting the program..." << endl;
				      exit(1);
				    }
				Imagedata[i][j][k] = val;
			}
	// De-Allocate memory to prevent memory leak
	  for (int i = 0; i < size; ++i) {
	    for (int j = 0; j < size; ++j)
	      delete [] data[i][j];

	    delete [] data[i];
	  }
	  delete [] data;

	  return Imagedata;

}


