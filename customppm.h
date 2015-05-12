#ifndef CUSTOMPPM_H
#define CUSTOMPPM_H
#include <string>
#include <GL/glx.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;

typedef struct t_ppmimage {
	int width;
	int height;
	unsigned char *data;
} Ppmimage;


class customppm { 
	public:
	    Ppmimage *image;
	    
		Ppmimage *ppm6GetImage(char *filename)
		{
			int i, j, width, height, size, maxval, ntries;
			char ts[4096];
			//unsigned int rgb[3];
			unsigned char c;
			unsigned char *p;
			FILE *fpi;
			image = (Ppmimage *)malloc(sizeof(Ppmimage));
			if (!image) {
				printf("ERROR: out of memory\n");
				exit(EXIT_FAILURE);
			}
			fpi = fopen(filename, "r");
			if (!fpi) {
				printf("ERROR: cannot open file **%s** for reading.\n", filename);
				exit(EXIT_FAILURE);
			}
			image->data=NULL;
			fgets(ts, 4, fpi);
			if (strncmp(ts, "P6" ,2) != 0) {
				printf("ERROR: File is not ppm RAW format.\n");
				exit(EXIT_FAILURE);
			}
			//comments?
			while(1) {
				c = fgetc(fpi);
				if (c != '#')
					break;
				//read until newline
				ntries=0;
				while(1) {
					//to avoid infinite loop...
					if (++ntries > 10000) {
						printf("ERROR: too many blank lines in **%s**\n", filename);
						exit(EXIT_FAILURE);
					}
					c = fgetc(fpi);
					if (c == '\n')
						break;
				}
			}
			ungetc(c, fpi);
			fscanf(fpi, "%u%u%u", &width, &height, &maxval);
			//
			//get past any newline or carrage-return
			ntries=0;
			while(1) {
				//to avoid infinite loop...
				if (++ntries > 10000) {
					printf("ERROR: too many blank lines in **%s**\n", filename);
					exit(EXIT_FAILURE);
				}
				c = fgetc(fpi);
				if (c != 10 && c != 13) {
					ungetc(c, fpi);
					break;
				}
			}
			//
			size = width * height * 3;
			image->data = (unsigned char *)malloc(size);
			if (!image->data) {
				printf("ERROR: no memory for image data.\n");
				exit(EXIT_FAILURE);
			}
			image->width = width;
			image->height = height;
			p = (unsigned char *)image->data;
			for (i=0; i<height; i++) {
				for (j=0; j<width; j++) {
					*p = fgetc(fpi); p++;
					*p = fgetc(fpi); p++;
					*p = fgetc(fpi); p++;
				}
			}
			fclose(fpi);
			return image;
		}




		GLuint getPpm(string fi)
		{
			//load images into a ppm structure
			char* fil = (char*)fi.data();
			image = ppm6GetImage(fil);
			int w = image->width;
			int h = image->height;
			unsigned char tmpArr[w*h*3];
			unsigned char *t = image->data;
			unsigned char dataWithAlpha[w*h*4];

			for(int i=0; i<(w*h*3); i++){
				tmpArr[i] = *(t+i);
			}
			// aply the alpha channel
			for(int i=0; i<(w*h); i++){
				// coppy color to new array
				dataWithAlpha[i*4] = tmpArr[3 * i];
				dataWithAlpha[i*4 + 1] = tmpArr[ 3 * i + 1];
				dataWithAlpha[i*4 + 2] = tmpArr[ 3 * i + 2];
				// set alpha
				dataWithAlpha[i*4+3]=((int)tmpArr[i*3] | (int)tmpArr[i*3+1] | (int)tmpArr[i*3+2] );
			}


			GLuint returningTex;
			//
			glGenTextures(1, &returningTex);
			glBindTexture(GL_TEXTURE_2D, returningTex);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataWithAlpha);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			return returningTex;
		}
};

#endif
