/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				@xllzi
**
**
** DATE:        2025-12-09
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
Image *readData(char *filename) 
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("no such file\n");
        return NULL;
    }
    
    // read header
    char ppm_type[3];
    fscanf(fp, "%s", ppm_type);
    if (strcmp(ppm_type, "P3") != 0) {
        fclose(fp);
        return NULL;
    }
    int width, height;
    fscanf(fp, "%d %d", &width, &height);
    int max_value;
    fscanf(fp, "%d", &max_value);

    Image *image = malloc(sizeof(Image));
    image->rows = height;
    image->cols = width;
    image->image = malloc(height*sizeof(Color*));
    for (int i = 0; i < height; i++) {
        image->image[i] = malloc(width*sizeof(Color));
        for (int j = 0; j < width; j++) {
            Color *pixel = &image->image[i][j];
            fscanf(fp, "%hhu %hhu %hhu", &pixel->R, &pixel->G, &pixel->B);
        }
    }
    fclose(fp);
    return image;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
    printf("P3\n%d %d\n255\n", image->rows, image->cols);
    for (int i = 0; i < image->rows; i++) {
        for (int j = 0; j < (image->cols)-1; j++) {
            Color *pixel = &image->image[i][j];
            printf("%3hhu %3hhu %3hhu", pixel->R, pixel->G, pixel->B);
            printf("   ");
        }
        // print the last one of a row
        Color *pixel = &image->image[i][(image->cols)-1];
        printf("%3hhu %3hhu %3hhu", pixel->R, pixel->G, pixel->B);
        printf("\n");
    }
}

//Frees an image
void freeImage(Image *image)
{
    uint32_t rows = image->rows;
    for (int i = 0; i < rows; i++) {
        free(image->image[i]);
    }
    free(image->image);
    free(image);
}
