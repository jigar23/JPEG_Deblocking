JPEG_Deblocking
===============

Deblocking filter | DCT | PSNR

There are total of 3 operations that can be performed.

1. DCT operations

   Run the file *DctOperations* and follow its usage.
   e.g. Run the file as:
   DctOperations -i lena.raw -q 60

2. JPEG deblocking

   Run the file *JPEG_Deblocking* and follow its usage.
   e.g. Run the file as:
   JPEG_Deblocking -i pepper.raw -c 1
   
   CHECK OUTPUT IN MATLAB
   -Once you get the desired output, you can check the quality of the output
    images by running the matlab file *checkImageBlocking.m*.
   -Enter the desired filenames in this file and you get the compared image output.

3. Calculate MSE and PSNR

   Run the file *PSNR* and follow its usage.
   e.g. Run the file as:
   PSNR -i resources/original.dat -o resources/quantized.dat
