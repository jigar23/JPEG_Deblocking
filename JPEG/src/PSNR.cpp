/*
 * PSNR.cpp
 *
 *  Created on: Feb 27, 2014
 *      Author: jigar
 */

#include <iostream>
using namespace std;
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "FileIO/fileIO.h"
#include "definition.h"
unsigned char* fileRead(char filename[], int fullsize);
float getPSNR(unsigned char* im1, unsigned char* im2, int size);

int main(int argc,char *argv[]){

		char originalFile[NAME_MAXIMUM];
		char fileNameAfterProcessing[NAME_MAXIMUM];
		char option;
		char *programname = *argv;

		/* if no options entered, list all of the defaults */
			  if (argc == 1) {

			    printf("%s\n",USAGE);
			    printf("\nOPTIONS   DESCRIPTIONS                       DEFAULTS\n");
			    printf("-i        Input file		                   \n");
			    printf("-o        quantized file		                \n");
			    printf("\n");
			    printf("Both options are compulsory. \n");
			    fflush(stdout);
			    exit(0);
			  }


			  /* read and interpret command line arguments */
			while (--argc && ++argv) {
				if (*argv[0] == '-' && strlen(*argv) == 2) { /* each option has 1 letter */
					option = *++argv[0];
					if (--argc && ++argv) { /* examine the option */
						switch (option) { /* examine the option letter */
						case 'i':
							strncpy(originalFile, *argv, NAME_MAXIMUM);
							break;
						case 'o':
							strncpy(fileNameAfterProcessing, *argv, NAME_MAXIMUM);
							break;
						default:
							fprintf(stderr, "%s: %c: %s\n", programname, option,
									NOTOPTION);
							exit(1);
							break;
						}
					} else {
						fprintf(stderr, "%s %s %s\n", USAGE, programname,HOWTOUSE_PSNR);
						exit(2);
					}
				}

				else if (*argv[0] == '-') { /* user entered unknown option */
					++argv[0];
					fprintf(stderr, "%s: %s: %s\n", programname, *argv, NOTOPTION);
					exit(3);
				} else { /* user entered unknown string */
					fprintf(stderr, "%s: %s: %s\n", programname, *argv, NOTOPTION);
					exit(4);
				}
		}

//	char originalFile[] = "resources/couple.256";
//	char fileNameAfterProcessing[] = "resources/couple_5bitquantizedOutput.dat";

	int fullsize = getFileSize(originalFile);

	//Read the 2 files
	unsigned char* im1 = fileRead(originalFile,fullsize);
	unsigned char* im2 = fileRead(fileNameAfterProcessing,fullsize);

	//Get the PSNR
	float PSNR = getPSNR(im1,im2,fullsize);

	cout << "PSNR is : " << PSNR << endl;

}

/**
 * This function computed the PSNR given two data files:
 * File1: Original File
 * File2: File after Processing
 */
float getPSNR(unsigned char* im1, unsigned char* im2, int size){
	float PSNR, MSE = 0;
	int max = 0;
	for(int i = 0; i < size; i++){
		//Compute the Max Value
		if(im1[i] > max) max = im1[i];
		//Compute the MSE
		MSE += pow((im2[i] - im1[i]),2);
	}

	MSE /= size;
	cout << "MSE is : " << MSE << endl;
	cout << "MAX value in image is : " << max << endl;
	PSNR = 10*log10f(max*max/MSE);
	return PSNR;
}

/**
 * This function reads the file and stores it in 1-D unsigned char array
 */
unsigned char* fileRead(char filename[], int fullsize){
	FILE *file;
	// image data array. Assigning memory
	unsigned char* imgArray = new unsigned char[fullsize];

	// read image "ride.raw" into image data matrix
	if (!(file = fopen(filename, "rb"))) {
		cout << "Cannot open file: " << filename << endl;
		exit(1);
	}
	fread(imgArray, sizeof(unsigned char), fullsize, file);
	fclose (file);
	return imgArray;
}
