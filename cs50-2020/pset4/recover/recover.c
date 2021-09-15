#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    
    BYTE bytes[512];
  
    int num_elemens = 0;
    char namejpg[8];
    int i = 0;

    if (argc != 2)
    {
        printf("Usage ./recover image");
        return 1;
    }
    
    char *read_file = argv[1];
    
    //open card file to read
    FILE *file = fopen(read_file, "r");
    if (!file)
    {
        return 1;
    }
    
    //run until the end of the file
    while (true)
    {
        //Read one chunk of 512 Bytes and return 0 in the end of the file
        num_elemens = fread(bytes, sizeof(BYTE), 512, file);
        
        if (num_elemens == 0)
        {
            break;
        }
        
        //check if it is a JPEG 
        if (bytes[0] == 0xff && bytes[1] == 0xd8 && bytes[2] == 0xff && (bytes[3] & 0xf0) == 0xe0)
        { 
            //check if it is the first image
            if (i == 0)
            {
                sprintf(namejpg, "%03i.jpg", i);
                FILE *img = fopen(namejpg, "w");
                fwrite(bytes, sizeof(BYTE), 512, img);
                i++;
                fclose(img);
            }
            //create another image file
            else 
            {
                sprintf(namejpg, "%03i.jpg", i);
                FILE *img = fopen(namejpg, "w");
                fwrite(bytes, sizeof(BYTE), 512, img);
                i++;
                fclose(img);
            }
        }
        //if the chunk is not the start of a JPEG
        else
        {
            //write below if an image is already open
            if (i > 0)
            {
                FILE *img = fopen(namejpg, "a");
                fwrite(bytes, sizeof(BYTE), 512, img);
                fclose(img);
            }
        }
    }
    return 0;
}
 