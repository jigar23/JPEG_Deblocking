//============================================================================
// Name        : Dct_operations.cpp
// Author      : Jigar Gada
// Description : This files computes the DCT of the *square* image having dimensions as
// 				 multiples of 8. It takes Quality factor as a parameter and displays
//			 	 the first 8x8 block as output for the value of entered quality factor.
//				 The displayed output can be changed by changing the values in the loop
// 				 at line 82 in this code.
//Last updated : 03/01/14
//============================================================================
#include <iostream>
#include "FileIO/fileIO.h"
#include "DCT/Dct.h"
#include <stdlib.h>
#include <string.h>
#include "definition.h"

int main(int argc,char *argv[]){
	char Imagefilename[NAME_MAXIMUM];
	int QF = 50;
	char option;
	char *programname = *argv;

	/* if no options entered, list all of the defaults */
		  if (argc == 1) {

		    printf("%s\n",USAGE);
		    printf("\nOPTIONS   DESCRIPTIONS                       DEFAULTS\n");
		    printf("-i        input image		                    \n");
		    printf("-q        quality factor                       	%d\n",QF);
		    printf("\n");
		    printf("blocks are of size 8x8 only \n");
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
					case 'q':
						sscanf(*argv,"%d",&QF);
						break;
					default:
						fprintf(stderr, "%s: %c: %s\n", programname, option,
								NOTOPTION);
						exit(1);
						break;
					}
				} else {
					fprintf(stderr, "%s %s %s\n", USAGE, programname,HOWTOUSE_DCT);
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

//---START THE MAIN PROGRAM AFTER REVECING THE ARGUMENTS PROPERLY---//
//	char Imagefilename[] = "resources/lena.raw";
	int size = sqrt(getFileSize(Imagefilename));

	unsigned char **Imgdata = readBWrawImage(Imagefilename, size);
	//Step 2. Subtract 128 from each pixel.
	char **imageData = subtract128(Imgdata, size);

	//Step 3. Get the DCT coefficients with quantization
	char **D = computeDCTwithQuantization(imageData,size,QF);

	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++)
			cout << (int)D[i][j] << " ";
		cout << endl;
	}

	cout << "ENd of PRogram." << endl;
}
