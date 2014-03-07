//============================================================================
// Name        : JPEG_deblocking.cpp
// Author      : Jigar Gada
// Copyright   : Your copyright notice
// Description : This file does some post processing on the JPEG file
//			   to remove the blocks of pixel data and smoothen it.
//			   Ref: Sung Deuk Kim, et all. "A Deblocking Filter with Two Separate
//			   Modes in Block-Based Video Coding", Feb 1999.
//			   The variables used in this function are exactly the same as the
//			   paper so easy to understand the code.
//Last updated : 03/01/14
//============================================================================

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "FileIO/fileIO.h"
#include "Deblocking/deblock.h"
#include "YUV/yuv.h"
#include "definition.h"
void printData(float ***arr, int size, int bitsperpixel);

using namespace std;

int main(int argc,char *argv[]) {

	char Imagefilename[NAME_MAXIMUM];
	int color = 0;
	char option;
	char *programname = *argv;

	/* if no options entered, list all of the defaults */
		  if (argc == 1) {

		    printf("%s\n",USAGE);
		    printf("\nOPTIONS   DESCRIPTIONS                       DEFAULTS\n");
		    printf("-i        Input Image file (raw)                   \n");
		    printf("-c        color			                  %d\n",color);
		    printf("\n");
		    printf("Please Enter -c 1 for colored images \n");
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
						strncpy(Imagefilename, *argv, NAME_MAXIMUM);
						break;
					case 'c':
						sscanf(*argv,"%d",&color);
						break;
					default:
						fprintf(stderr, "%s: %c: %s\n", programname, option,
								NOTOPTION);
						exit(1);
						break;
					}
				} else {
					fprintf(stderr, "%s %s %s\n", USAGE, programname,HOWTOUSE_JPEG);
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

	//--------USER INPUTS-------------
//	char Imagefilename[] = "resources/clock_70.raw";
//	bool color = false;
	//----------xxxxxxxxxxxxx----------
//---START THE MAIN PROGRAM AFTER REVECING THE ARGUMENTS PROPERLY---//

	int bitPerPixel = 1, size;
	if(color) bitPerPixel = 3;

	//Assuming a square image, get the size of row/column
	if(color)
		size = sqrt((getFileSize(Imagefilename))/3);
	else
		size = sqrt(getFileSize(Imagefilename));

	//read image and convert into 3-d array
	float ***Imgdata = readRawImage(Imagefilename, size, bitPerPixel);

	//RGb to YUV conversion
	if(color) 	rgb2yuv(Imgdata,size);

	//do the image deblocking in the horizontal direction
	deblockJpegImageH(Imgdata,size);

	//do the image deblocking in the vertical direction
	deblockJpegImageV(Imgdata,size);

	//YUV to RGB
	if(color) 	yuv2rgb(Imgdata,size);

	//Convert float data to char
	unsigned char ***MImgdata = floatToChar(Imgdata,size,bitPerPixel);

	//write the deblocked image.
	writeRawImage(MImgdata, size,bitPerPixel);

	cout << endl <<"End of program" << endl;
	return 0;
}

void printData(float ***arr, int size, int bitsperpixel){
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			for(int k = 0; k < bitsperpixel; k++)
				cout << round(arr[i][j][k]) << " ";
//			cout << endl;
		}
			cout << endl;
	}
}
