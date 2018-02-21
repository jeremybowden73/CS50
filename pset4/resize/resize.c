/**
 * Resize a BMP by a factor of n, where is n is an integer 1 to 100.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    // check if enlarging factor is within range
    int n = atoi(argv[1]);
    
    if (n <1 || n>100)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\nn must be 1 to 100\n");
        return 1;
    }

    //remember enlarging factor; create pointer pn
    //int *pn = &n;
    
    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 1;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 1;
    }

    // read infile's BITMAPFILEHEADER
    // create structure variable bf of data type BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    // create structure variable bi of data type BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 1;
    }

    // create header structure variables for the output file
    BITMAPFILEHEADER bfout = bf;
    BITMAPINFOHEADER biout = bi;
    
    // determine padding for scanlines in input file
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    // determine padding for scanlines in resized output file
    int resize_padding = (4 - ((bi.biWidth*n) * sizeof(RGBTRIPLE)) % 4) % 4;

    // resize the outfile bitmap using members biWidth, biHeight,
    // biSizeImage and bfSize in structure variables biout and bfout
    biout.biWidth *= n;
    biout.biHeight *= n;
    biout.biSizeImage = ((abs(bi.biHeight) * n) * (((bi.biWidth*n)*sizeof(RGBTRIPLE) + resize_padding)));
    bfout.bfSize = (biout.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));

    // write outfile's BITMAPFILEHEADER
    fwrite(&bfout, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&biout, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
            // create structure variable array "triple" of data type RGBTRIPLE.
            // number of elements = infile width * enlarging factor n.
            // this array will be the "resized version" of the scanline. 
            RGBTRIPLE triple[bi.biWidth*n];
            
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage for infile's RGBTRIPLE
                RGBTRIPLE triple_in;

                // read RGB triple from infile
                fread(&triple_in, sizeof(RGBTRIPLE), 1, inptr);

                // copy the infile pixel n times to the array "triple".
                // each time it is copied, copy R,G, & B member data from the
                // infile RGBTRIPLE triple_in
                for (int k = j*n ; k < (n*(j+1)) ; k++)
                {
                    triple[k].rgbtBlue = triple_in.rgbtBlue;
                    triple[k].rgbtGreen = triple_in.rgbtGreen;
                    triple[k].rgbtRed = triple_in.rgbtRed;
                }
            }
            
            // write the array "triple" (plus padding if required) to the outfile n times.
            // this copies the outfile scanline 'down' the height of the resized bitmap n times.
            for (int m = 0; m < n; m++)
            {
                // write RGB triple array, to the outfile
                fwrite(&triple, sizeof(RGBTRIPLE), (bi.biWidth*n), outptr);
                
                // add required padding to the output file scanline
                for (int k = 0; k < resize_padding; k++)
                {
                    fputc(0x00, outptr);
                }
            }
        
        
        // skip over padding in input file, if any
        fseek(inptr, padding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);
    
    // success
    return 0;
}
