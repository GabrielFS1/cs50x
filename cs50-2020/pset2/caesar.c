#import <stdio.h>
#import <cs50.h>
#import <ctype.h> 
#import <stdlib.h>
#import <string.h>

int main(int argc, string argv[])
{
    //variables
    char c1;
    int key = 0;
    int key1 = 0;

    //invalid command    
    if (argc != 2)
    {
        printf("Usage ./ceasar key\n");
        return 1;
    }
    //key verification
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (isdigit(argv[1][i]) == 0)
        {
            printf("Usage ./ceasar key\n");
            return 1;
        }
    }
    //convert key to a number
    key = atoi(argv[1]);
    
    //get the text
    string txt = get_string("Plaint: ");
    
    //Output text
    printf("ciphertext: ");
    
    for (int j = 0, n = strlen(txt); j < n; j++)
    {
        //rewrites special characters or numbers 
        if (txt[j] >= 32 && txt[j] <= 64)
        {
            c1 = txt[j];
        }
        //encrypt capital letters
        else if (isupper(txt[j]))
        {
            key1 = key;
            c1 = txt[j];
            for (int x = 0; x < key1; x++)
            {
                if (c1 < 90)
                {
                    c1++;
                }
                else
                {
                    c1 = 65;
                }
            }
        }
        //encrypt lowercase letters
        else 
        {
            c1 = txt[j];
            key1 = key;
            for (int z = 0; z < key1; z++)
            {
                if (c1 < 122)
                {
                    c1++;
                }
                else
                {
                    c1 = 97;
                }
                
            }
           
            
        }
        printf("%c", c1);
    }
    printf("\n");
return 0;
}
