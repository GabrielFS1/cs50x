#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int letter_count(string str);

int word_count(string str);

int sentence_count(string str);

int main(void)
{
    //variables
    float letters = 0;
    float words = 0;
    float sentences = 0;
    float index = 0;
    
    //prompt user a text
    string txt = get_string("Text: ");

    //function to count letters
    letters = letter_count(txt);
    
    //function to count words
    words = word_count(txt);
    
    //function to count sentence
    sentences = sentence_count(txt);
    
    //average numbers of letters per 100 words
    float L = (letters / words) * 100;
    
    //average numbers of sentences per 100 words
    float S = (sentences / words) * 100;

    //Coleman-Liau index
    index = (0.0588 * L) - (0.296 * S) - 15.8;
    
    if (index < 1)
    {
        printf("Before Grade 1");
    }
    else if (index > 16)
    {
        printf("Grade 16+");
    }
    else
    {
        printf("Grade %.0f", round(index));
    }
    printf("\n");
}

int letter_count(string str)
{
    int l = 0;
    for (int i = 0, n = strlen(str); i < n; i++)
    {
        if (toupper(str[i]) >= 'A' && toupper(str[i]) <= 'Z')
        {
            l++;
        }
    }
    return l;
}

int word_count(string str)
{
    int w = 0;
    for (int i = 0, n = strlen(str); i < n; i++)
    {
        if (str[i] == ' ')
        {
            w++;
        }
    }
    return w + 1;
}
int sentence_count(string str)
{
    int s = 0;
    for (int i = 0, n = strlen(str); i < n; i++)
    {
        if (str[i] == '.' || str[i] == '!' || str[i] == '?')
        {
            s++;
        }
    }
    return s;
}