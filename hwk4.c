// John Chang
// Reads, edits, and writes bitmap files.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>



/**
 * USE ONE fread statement and use pointers.
 * cast char pointer to int pointer at the address to read the int.
 * sizee has to be same.
 * only bmp files
 *
 */

//prototypes
void getDimensions(unsigned char *file, int width, int height);
void checkType(unsigned char *file);
void getHeader(unsigned char *file, unsigned char *write);
void blend(unsigned char* blended, unsigned char* image1, unsigned char* image2, int size);
void checkFile(char *theFile);
unsigned long get_file_size(char *s);
void checker(unsigned char* write, unsigned char* file1, unsigned char* file2, int size, int width, int height);


// main method
int main () {
    int size = 0;
    int width = 0;
    int height = 0;

//if(argc != 3) {
//    perror("invalid number of arguments");
//    exit(-1);
//}
//    argv++;
    char *file1 = "in1.bmp";
//    argv++;
	char *file2 = "in2.bmp";

    checkFile(file1);
    checkFile(file2);
	FILE *infile1 = fopen(file1, "rb");
	FILE *infile2 = fopen(file2, "rb");

	unsigned long size1 = get_file_size(file1);
	unsigned long size2 = get_file_size(file2);


	if(size1 != size2) {
		perror("Sizes of files are not the same.");
		exit(-1);
	}


	unsigned char *fileRead = malloc(get_file_size(file1));
	unsigned char *fileRead2 = malloc(get_file_size(file2));
	unsigned char *fileWrite = malloc(get_file_size(file1));
	unsigned char *fileWrite2 = malloc(get_file_size(file2));

	fread(fileRead, 1, size1, infile1);
	fread(fileRead2, 1, size1, infile2);

    unsigned char *read = fileRead;
    unsigned char *read2 = fileRead2;
    unsigned char *write = fileWrite;
    unsigned char *write2 = fileWrite2;

    checkType(read);
    checkType(read2);

	FILE *outfile1 = fopen("blend.bmp", "wb");
	FILE *outfile2 = fopen("checker.bmp", "wb");

    getDimensions(read + 18, width, height);

	getHeader(read, write);
    getHeader(read2,write2);

    blend(write, read + 54, read2 + 54, (int) size1 - 54);

	checker(write2, read + 54, read2 +54, (int) size1 - 54, width, height);


	// writes the pixel data into checker file
	fwrite(fileWrite, 1, size1, outfile1);
    fwrite(fileWrite2, 1, size1, outfile2);

    free(fileRead);
	free(fileRead2);
	free(fileWrite);
	free(fileWrite2);

	// closes all files
	fclose(infile1);
	fclose(infile2);
	fclose(outfile1);
	fclose(outfile2);

	return 0;
}


/**
 * Blends two images.
 */
void blend(unsigned char* write, unsigned char* file1, unsigned char* file2, int size) {
    unsigned char *image1 = file1;
    unsigned char *image2 = file2;
	for(int i = 0; i < size; i++) {
		*write = (*image1 + *image2) / 2;
        write++;
        image1++;
        image2++;
	}
}

/**
 * Checks if file exists or is empty.
 * Returns file if exists and not empty.
*/
void checkFile(char *theFile) {
	char *temp = theFile;
	FILE *file = fopen(temp, "r");
	if(file == NULL) {
		perror("Error: file doses not exist.");
		exit(-1);
	}
	fclose(file);
}

/**
 * Stores a header for bitmap file.
 */
void getHeader(unsigned char *file, unsigned char *write) {
    unsigned char *temp = file;
    for(int i = 0; i < 54; i++) {
        *write = *temp;
        write++;
        temp++;
    }
}

/**
 * Checks the size of file.
 * returns the sizze
 */
unsigned long get_file_size(char *s) {
	long result = -1;
	struct stat file_info;
	if (stat(s, &file_info) > -1) {
		result = (unsigned long)(file_info.st_size);
	}
	return (unsigned long)result;
}

/**
 * Checks the file type.
 */
void checkType(unsigned char *file) {
    unsigned char *temp = file;
    int result = 0;
    if(*temp != 'B') {
        result++;
    }
    temp++;
    if(*temp != 'M') {
        result++;
    }
    if(result > 0) {
        perror("File is not a bmp file");
        exit(-1);
    }

}

/**
 * Gets the width and height of the file.
 */
void getDimensions(unsigned char *file, int width, int height) {
    unsigned char *temp = file;
    width = *((int*) temp);
    temp += 4;
    height = *((int*) temp);
}

/**
 * Creates bmp file that alternates between two images.
 */
void checker(unsigned char* write, unsigned char* file1, unsigned char* file2, int size, int width, int height) {
    int flip = -1;
    for(int i = 0; i < width; i++) {
		if(i % (width / 8) == 0) {
						flip = flip*-1;
		}
		for(int j = 0; j < (height * 3); j++) {
			if(j % (height * 3 / 8) == 0) {
				flip = flip * -1;
			}
			if(flip < 0) {
				*write = *file2;

			}else {
				*write = *file1;
			}
            write++;
            file1++;
            file2++;
		}
	}
}
