#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int i = 0;
    int k = 0;
    int j = 0;
    int h = 0;
    int n = 0;

    do
    {
        h = get_int("Height: ");

    }    
    while (h < 1 || h > 8);
    n = h;
    for (i = 0; i < h; i++)
    {
        for (j = (n - 1); j > 0; j--)
        { 
            printf(" ");
        }
        for (k = 0; k <= i; k++)
        {
            printf("#");
        }
        printf("  ");
        for (k = 0; k <= i; k++)
        {
            printf("#");
        }    
        printf("\n");
        n--;
    }
}    
    

    
