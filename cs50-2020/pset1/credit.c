#include <stdio.h>
#include <cs50.h>
#include <math.h>


int card_flag(long card, int leng);
int check_sum(long card);
int lenght_card_num(long card);

long r = 0; 
long pot = 0;
int f = 0;


int main(void)
{
    long c = 0;
    int len = 0;
    int re = 0;

    do
    {
     c = get_long("Number: ");
        len = lenght_card_num(c);
    }
    while ((c%10) < 0);
    if (len >12 && len < 17)
    {
    re = check_sum(c);
        if (re == 1)
    {
        f = card_flag(c, len);    
            if (f == 1)
            {
                printf("VISA\n");
            }
            else if (f == 2)
            {
                printf("AMEX\n");
            }
             else if (f == 3)
            {
                printf("MASTERCARD\n");
            }
            else 
            {
                printf("INVALID\n");
            }
        }
        else 
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}

int lenght_card_num(long card)
{
    int x = 0;
    for (int i = 1; i < 20; i++)
    {
        if (card > 0)
        {
            pot = pow(10,i);
            r = card % pot;
            card = card - r;
            r = r/pow(10,(pot - 1));
            x++;
        }
    }
    return x;
}

int check_sum(long card)
{ 
    int t = 0;
    long c1 = card;
    int y = 0;
    int y2 = 0;
    r = card % 10;
    card = card - r;

    for (int i = 2 ; i < 17; i = i + 2)
    {
        if (card > 0)
        {
            pot = pow(10, i);
            r = card % pot;
            card = card - r;
            long n1 = pow(10,i-1);
            //sum double of remains
            r = 2*(r/n1);

            //two-digit number
            if (r > 9)
            {
                if (r == 10)
                {
                    y = y + 1;
                }
                else
                {
                    r = ((r % 10)+ 1);
                    y = y + r;
                }
            }
        //Only one number
        else
        {
            y = y + r;
        }
           
    }
  }
    for (int k = 1; k < 17; k = k + 2)
    {
    if (c1 > 0)
    {
        pot = pow(10, k);
        r = c1 % pot;
        c1 = c1 - r;
        r = r / pow(10,k - 1);
        y2 = y2 + r;
    }
}
    if ((y + y2) % 10 == 0)
    {
        t = 1;
    }
    else
    {
    t = 0;
    }
    return t;
}

int card_flag(long card, int leng)
{
    int t = 0;
    long c = card;
    for (int i = 0; i < leng + 1 ; i++)
    {
        pot = pow(10, i);
        r = c % pot;
        c = c - r;
        r = r / pow(10, i - 1);
    }
    if (r == 4)
    {
       t = 1;
    }
    else if (r == 3)
    {
        c = card;
        for (int i = 0; i < leng; i++)
        {
            pot = pow(10, i);
            r = c % pot;
            c = c - r;
            r = r / pow(10, i - 1);

        }
        if (r == 4 || r == 7)
        {
            t = 2;
        }
    }
    else if (r == 5)
    {
        c = card;
        for (int i = 0; i < leng; i++)
        {
            pot = pow(10, i);
            r = c % pot;
            c = c - r;
            r = r / pow(10, i - 1);
        }
        if (r >= 1 && r <= 5)
        {
            t = 3;  
        }
        else 
        {
            t = 0;
        }
    
    }
    return t;
}