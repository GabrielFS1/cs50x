#include <stdio.h>

int main(void)
{
    int i, x, y;
    int sum = 0;

    scanf("%d%d", &x, &y);

    for (i = y + 1; i < x; i++)
    {
        if (i % 2 != 0)
        {
            sum += i;
        }
    }
    printf("%d\n", sum);
}