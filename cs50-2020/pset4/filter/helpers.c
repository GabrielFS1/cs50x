#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0, h = height; i < h; i++)
    {
        for (int j = 0, w = width; j < w; j++)
        {
            //takes each pixel colors
            float r = image[i][j].rgbtRed;
            float b = image[i][j].rgbtBlue;
            float g = image[i][j].rgbtGreen;

            float average = (r + g + b) / 3;

            //all pixels are given the same value
            image[i][j].rgbtRed = round(average);
            image[i][j].rgbtBlue = round(average);
            image[i][j].rgbtGreen = round(average);
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0, h = height; i < h; i++)
    {
        for (int j = 0, w = width; j < w; j++)
        {
            //takes each pixel colors
            float r = image[i][j].rgbtRed;
            float g = image[i][j].rgbtGreen;
            float b = image[i][j].rgbtBlue;

            float sepiaRed = (.393 * r) + (.769 * g) + (.189 * b);
            float sepiaGreen = (.349 * r) + (.686 * g) + (.168 * b);
            float sepiaBlue = (.272 * r) + (.534 * g) + (.131 * b);

            //if greater than 8 bits
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            image[i][j].rgbtRed = round(sepiaRed);
            image[i][j].rgbtGreen = round(sepiaGreen);
            image[i][j].rgbtBlue = round(sepiaBlue);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0, h = height; i < h; i++)
    {
        for (int j = 0, w = width; j < w; j++)
        {
            //if width is even
            if (width % 2 == 0)
            {
                if (j < (width / 2))
                {
                    //change all pixel
                    RGBTRIPLE tmp[height][width];
                    tmp[i][j] = image[i][j];
                    image[i][j] = image[i][(width - 1) - j];
                    image[i][(width - 1) - j] = tmp[i][j];
                }

            }
            else
            {
                if (j < ((width + 1) / 2))
                {
                    //don't change middle pixel 
                    RGBTRIPLE tmp[height][width];
                    tmp[i][j] = image[i][j];
                    image[i][j] = image[i][(width - 1) - j];
                    image[i][(width - 1) - j] = tmp[i][j];
                }
            }
        }
    }
    return;
}
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //averages
    float averager = 0;
    float averageb = 0;
    float averageg = 0;

    int gaph = 0;
    int gapw = 0;
    float celln = 0;

    int ctx = 0;
    int ctk = 0;
    
    
    RGBTRIPLE original_image[height][width];
    
    for (int b = 0, n = height; b < n; b++)
    {
        for (int q = 0, z = width; q < z; q++)    
        {
            //copy original image values
            original_image[b][q].rgbtRed = image[b][q].rgbtRed;
            original_image[b][q].rgbtGreen = image[b][q].rgbtGreen;
            original_image[b][q].rgbtBlue = image[b][q].rgbtBlue;
        }
    }
    for (int i = 0, h = height; i < h; i++)
    {
        for (int j = 0, w = width; j < w; j++)
        {
            averager = 0;
            averageb = 0;
            averageg = 0;
        
            //upper left corner
            if (i == 0 && j == 0)
            {
                ctk = 0;
                
                ctx = 0;
                
                gaph = 2;
                
                gapw = 2;
                
                celln = 4;
            }
            //upper middle
            else if (i == 0 && j > 0 && j < (w - 1))
            {
                
                ctk = 0;
                
                ctx = -1;
                
                gaph = 2;
                
                gapw = 2;
                
                celln = 6;
            }
            //lower middle
            else if (i == (h - 1) && j > 0 && j < (w - 1))
            {
                ctk = -1;
                
                ctx = -1;
                
                gaph = 1;
                
                gapw = 2;
                
                celln = 6;
            }
            //left side
            else if (i > 0 && i < (h - 1) && j == 0)
            {
                ctk = -1;
                ctx = 0;
                gaph = 2;
                gapw = 2;
                celln = 6;
            }
            //right side
            else if (i > 0 && i < (h - 1) && j == (w - 1))
            {
                ctk = -1;
                ctx = -1;
                gaph = 2;
                gapw = 1;
                celln = 6;
            }
            //top right corner
            else if (i == 0 && j == (w - 1))
            {
                ctk = 0;
                
                ctx = -1;
                
                gaph = 2;
                
                gapw = 1;
                
                celln = 4;
            }
            //inside the edge
            else if (i > 0 && i < (h - 1) && j > 0 && j < (w - 1))            
            {
                ctk = -1;
                
                ctx = -1;
                
                gaph = 2;
                
                gapw = 2;
                
                celln = 9;
            }
            //lower left corner
            else if (i == (h - 1) && j == (w - 1))
            {
                ctk = -1;
                
                ctx = -1;
                
                gaph = 1;
                
                gapw = 1;
                
                celln = 4;
            }
            //lower right corner
            else if (i == (h - 1) && j == 0)
            {
                ctk = -1;
                
                ctx = 0;
                
                gaph = 1;
                
                gapw = 2;
                
                celln = 4;
            }
            for (int k = ctk; k < gaph; k++)
            {
                for (int x = ctx; x < gapw; x++)
                {
                    //sum of each color
                    averager += original_image[i + k][j + x].rgbtRed;
                    averageb += original_image[i + k][j + x].rgbtBlue;
                    averageg += original_image[i + k][j + x].rgbtGreen;
                }
            }
            //averaging
            averager = averager / celln;
            averageb = averageb / celln;
            averageg  = averageg / celln;

            image[i][j].rgbtRed = round(averager);
            image[i][j].rgbtBlue = round(averageb);
            image[i][j].rgbtGreen = round(averageg);
        }
    }
    return;
}

