// this program recovers 50 JPG files from the input file card.raw
// by opening it and testing each contiguous block of 512 bytes
// for the JPG file 'signature' (first three bytes 0xff, 0xd8, 0xff) 


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#define ARRAY 512       // size of the block of bytes to copy from the input file

typedef uint8_t BYTE;   // BYTE is an 8-bit (= 1 byte) unsigned int, for storing
                        // individual bytes from the input file

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // remember filename
    char *infile = argv[1];

    // open input file 
    FILE *file = fopen(infile, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // *******************************************************************
    // **  SET UP THE ARRAYS, VARIABLES, AND FILENAMES THAT WILL BE     **
    // **  USED TO EXECUTE THE MAIN ALGORITHM LOOP                      **
    // *******************************************************************

    // set file position in the input file to the start
    fseek(file, 0, SEEK_SET);
    
    // create an array of 8-bit (= 1 byte) ints to store blocks of 512
    // bytes from the input file
    BYTE *jpgtest = malloc(ARRAY * sizeof(BYTE));
    
    // cretate a char array to store the output file filenames
    // (000.jpg, 001.jpg, ... 049.jpg)
    char *filename = malloc(8 * sizeof(char));
    
    // create int to store the incremental filename number (0, 1, ... 49).
    // start at -1 which will be a temp file, used for storing any
    // 'unneeded' bytes found before the first JPG signature
    int filenumber = -1;
    int *fn = &filenumber;
    
    // open the first output file (the temp file) with 'append' access
    // by storing the filename in the char array 'filename'
    // DO NOT NEED "&filename" BECAUSE MALLOC RETURNS THE ADDRESS OF THE
    // MEMORY IT HAS ALLOCATED, SO YOU DON'T NEED THE ADDRESS OF IT, JUST IT.
    sprintf(filename,"%03i.jpg", *fn);
    FILE *ptr = fopen(filename, "a");
    
    // create a copy of the temp file's name so it can be deleted when
    // it is finished with
    char *temp_file = malloc(8 * sizeof(char));
    strcpy(temp_file, filename);
    
    // loop to store each contiguous block of 512 bytes from the input file
    // in an array and check if it starts with the JPG signature.
    // if it does, close the output file that is already open and open the
    // next output file and write the current block of 512 bytes to it.
    // if it doesn't, add the current block of 512 bytes to the output file
    // that is already open.
    // this while loops continuously until the end-of-file check returns
    // nonzero, invoking the break;
    while(1)
    {
        
        // copy into the array 'jpgtest' the next block of 512 bytes
        // from the input file
        // DO NOT NEED "&jpgtest" BECAUSE MALLOC HAS RETURNED THE
        // ADDRESS OF THE MEMORY ALLOCATION TO jpptest
        fread(jpgtest, sizeof(BYTE), ARRAY, file);
        
        // if read past the end of the input file, quit this while loop 
        if (feof(file))
        {
            break;
        }
        
    
        // check the first three elements in the array for the JPG signature
        // 0xff, 0xd8, 0xff (or 255, 216, 255 in decimal).
        if ((jpgtest[0] == 0xff) && (jpgtest[1] ==0xd8) && (jpgtest[2] == 0xff))
        {
            // if true,
            // close the open output file as we are done writing to it
            fclose(ptr);
            // create and open the next output file
            ++*fn;
            sprintf(filename,"%03i.jpg", *fn);               
            ptr = fopen(filename, "a");
            // write the contents of the array to the output file
            fwrite(jpgtest, sizeof(BYTE), ARRAY, ptr);
        }
        
        else
        {
            // if false,
            // keep the existing output file open and
            // write the contents of the array to it 
            fwrite(jpgtest, sizeof(BYTE), ARRAY, ptr);
        }

    }
    
    // close the final *.jpg file that was created
    fclose(ptr);
    
    
    // close the input file
    fclose(file);
    
    // remove the temp file that stored the 'unneeded' data
    // and check removal was successfull
    int rem = remove(temp_file);
    if (rem != 0)
    {
        fprintf(stderr, "Could not remove temp file %s.\n", temp_file);
        return 2;
    }
    
    // free the memory that was allocated by malloc
    free(jpgtest);
    free(filename);
    free(temp_file);
    
    // success!
    return 0;
}