/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				@xllzi
**
**
** DATE:        2026-03-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
    Color pixel = image->image[row][col];
    // count the number of lived neighbours
    // (x-1, y-1), (x, y-1), (x+1, y-1) ...
    uint32_t old_state_R = !!(pixel.R);
    uint32_t old_state_G = !!(pixel.G);
    uint32_t old_state_B = !!(pixel.B);
    uint32_t n_lived_R = 0;
    uint32_t n_lived_G = 0;
    uint32_t n_lived_B = 0;
    int x_bound = image->cols;
    int y_bound = image->rows;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;
            Color* neighbor = &(image->image[(row+dy+y_bound)%y_bound][(col+dx+x_bound)%x_bound]);
            n_lived_R += !!(neighbor->R);
            n_lived_G += !!(neighbor->G);
            n_lived_B += !!(neighbor->B);
        }
    }

    // 通过`n_lived`和`old_state`来确定新值在`rule`的哪一位上（从0开始）
    uint32_t which_bit;
    which_bit = n_lived_R + old_state_R * 9;
    pixel.R = (rule >> which_bit & 1)*255;
    which_bit = n_lived_G + old_state_G * 9;
    pixel.G = (rule >> which_bit & 1)*255;
    which_bit = n_lived_B + old_state_B * 9;
    pixel.B = (rule >> which_bit & 1)*255;
    return pixel;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
    Image *out_image = malloc(sizeof(Image));
    out_image->image = malloc((image->rows) * sizeof(Color*));
    out_image->cols = image->cols;
    out_image->rows = image->rows;
    for (int i = 0; i < out_image->rows; i++) {
        out_image->image[i] = malloc((out_image->cols)*sizeof(Color));
        for (int j = 0; j < out_image->cols; j++) {
            out_image->image[i][j] = evaluateOneCell(image, i, j, rule);
        }
    }

    return out_image;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
    if (argc != 3) {
        printf("    usage: ./gameOfLife filename rule\n"
               "   filename is an ASCII PPM file (type P3) with maximum value 255.\n"
               "   rule is a hex number beginning with 0x; Life is 0x1808.\n");
        return -1;
    }
    Image* in_image = readData(argv[1]);
    if (in_image == NULL) {
        return -1;
    }
    char* endptr;
    long rule = strtol(argv[2], &endptr, 16);
    if (endptr == argv[2] || *endptr != '\0') {
        printf("    usage: ./gameOfLife filename rule\n"
               "   filename is an ASCII PPM file (type P3) with maximum value 255.\n"
               "   rule is a hex number beginning with 0x; Life is 0x1808.\n");
        return -1;
    }
    if (rule <= 0x0 || rule >= 0x3FFFF) return -1;
    Image* out_image = life(in_image, rule); // here casting long to 32-bits unsigned int is safe
    writeData(out_image);
    freeImage(in_image);
    freeImage(out_image);
    return 0;
}
