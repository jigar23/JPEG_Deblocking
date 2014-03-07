/*
 * fileIO.cpp
 *
 *  Created on: Feb 24, 2014
 *      Author: jigar
 */

#include "fileIO.h"

int getFileSize(char filename[]){
	streampos size;
	ifstream file (filename, ios::in|ios::binary|ios::ate);
	if(file.is_open()) {
	    size = file.tellg();
	    file.close();
	}else{
		cout << "Unable to open output file.!!!" << endl;
	}
	return size;
}

/**
 * This function reads images and returns the data in 2-D float array
 * so that processing is simple
 */
float*** readRawImage(char filename[], int size, int bitsperpixel){

	FILE *file;
	// image data array. Assigning memory
	unsigned char* imgArray = new unsigned char[size*size*bitsperpixel];

	// read image "ride.raw" into image data matrix
	if (!(file = fopen(filename, "rb"))) {
		cout << "Cannot open file: " << filename << endl;
		exit(1);
	}
	fread(imgArray, sizeof(unsigned char), size * size * bitsperpixel, file);
	fclose (file);

//	unsigned char **Imagedata = new unsigned char*[size];
//	for(int i = 0; i < size; i++){
//		Imagedata[i] = new unsigned char[size];
//	}

	// Create 3-d image array
	float ***Imagedata;
	Imagedata = new float**[size];
	  for (int i = 0; i < size; ++i) {
		  Imagedata[i] = new float*[size];

	    for (int j = 0; j < size; ++j)
	    	Imagedata[i][j] = new float[bitsperpixel];
	  }

	//Convert 1-d array element to 3-D
	int no = 0;
	for(int i = 0; i < size; i++)
		for(int j = 0; j < size; j++)
			for(int k = 0; k < bitsperpixel; k++)
				Imagedata[i][j][k] = imgArray[no++];

	delete [] imgArray;

	return Imagedata;
}

std::ofstream myFile;

/**
 * Open the file once.
 * @param-
 * 1. fileName : name of the file in which you
 * want to write
 */
void writePrepare(char *fileName){
	  //ios::app-> append to end of file.//No need to append
	  //ios::binary-> file is binary not text.
	  //ios::out -> write to the file
	  myFile.open(fileName, ios::out|ios::binary);
}


/**
 * write the file.
 * @param -
 * 1. char data : data you want to write in the file.
 */
void writeFileByBytes(unsigned char data){
	//cout << "data " << (int)data << endl;
			  myFile << data;
}

/**
 * Close the file once all write operations done.
 */
void closeFile(){
	myFile.close();
}

/**
 * this function writes the 2-d Image data
 * in the file.
 */
void writeRawImage(unsigned char ***Imagedata, int size, int bitsperpixel){

	char opfilename[] = "deblocked.raw";
	writePrepare(opfilename);
	for(int i = 0; i < size; i++)
		for(int j = 0; j < size; j++)
			for(int k = 0; k < bitsperpixel; k++)
				writeFileByBytes(Imagedata[i][j][k]);
	closeFile();
	cout << "Deblocked file stored in *deblocked.raw*" << endl;

}

/**
 * This function reads only black and white image
 * and returns the data in 2-D unsigned char array
 */
unsigned char** readBWrawImage(char filename[], int size){

	FILE *file;
	// image data array. Assigning memory
	unsigned char* imgArray = new unsigned char[size*size];

	// read image "ride.raw" into image data matrix
	if (!(file = fopen(filename, "rb"))) {
		cout << "Cannot open file: " << filename << endl;
		exit(1);
	}
	fread(imgArray, sizeof(unsigned char), size * size, file);
	fclose (file);

	unsigned char **Imagedata = new unsigned char*[size];
	for(int i = 0; i < size; i++){
		Imagedata[i] = new unsigned char[size];
	}

	//Convert 1-d array element to 2-D
	int no = 0;
	for(int i = 0; i < size; i++)
		for(int j = 0; j < size; j++)
			Imagedata[i][j] = imgArray[no++];

	delete [] imgArray;

	return Imagedata;
}
