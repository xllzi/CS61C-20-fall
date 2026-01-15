/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color evaluateOnePixel(Image *image, int row, int col)
{
    Color pixel;
    uint8_t B_value = image->image[row][col].B;
    if (B_value & 1) {
        pixel.R = 255;
        pixel.G = 255;
        pixel.B = 255;
    } else {
        pixel.R = 0;
        pixel.G = 0;
        pixel.B = 0;
    }
    return pixel;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
    Image *out_image = malloc(sizeof(Image));
    out_image->image = malloc((image->rows) * sizeof(Color*));
    out_image->cols = image->cols;
    out_image->rows = image->rows;
    for (int i = 0; i < out_image->rows; i++) {
        out_image->image[i] = malloc((out_image->cols)*sizeof(Color));
        for (int j = 0; j < out_image->cols; j++) {
            out_image->image[i][j] = evaluateOnePixel(image, i, j);
        }
    }

    return out_image;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
    Image *in_image = readData(argv[1]);
    if (in_image == NULL) {
        return -1;
    }
    Image *out_image = steganography(in_image); 
    writeData(out_image);
    freeImage(in_image);
    freeImage(out_image);
}
