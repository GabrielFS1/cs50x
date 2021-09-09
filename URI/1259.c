#include <stdio.h>

int print_even(int arr[], int n)
{
    int i = 0;
    int smll_i = 0;
    int even_num;

    while (1)
    {
        // Look for any even number
        even_num = -1;
        for (i = 0; i < n; i++)
        {
            if (arr[i] % 2 == 0)
            {
                even_num = arr[i];
                smll_i = i;
                break;
            }
        }
        // the array does not have any even number 
        if (even_num == -1)
        {
            return 0;
        }
        
        // find the smaller even number
        for (i = 1; i < n; i++)
        {
            if (arr[i] % 2 == 0 && arr[i] <= even_num)
            {
                even_num = arr[i];
                smll_i = i;
            }
        }
        printf("%d\n", even_num);
        arr[smll_i] = -1;
    }
}

int print_odd(int arr[], int n)
{
    int i = 0;
    int smll_i = 0;
    int odd_num;

    while (1)
    {
        // Look for any odd number
        odd_num = -1;
        for (i = 0; i < n; i++)
        {
            if (arr[i] % 2 != 0 && arr[i] != -1)
            {
                odd_num = arr[i];
                smll_i = i;
                break;
            }
        }
        // the array does not have any odd number 
        if (odd_num == -1)
        {
            return 0;
        }
        
        // find the smaller odd number
        for (i = 1; i < n; i++)
        {
            if (arr[i] % 2 != 0 && arr[i] >= odd_num && arr[i] != -1)
            {
                odd_num = arr[i];
                smll_i = i;
            }
        }
        printf("%d\n", odd_num);
        arr[smll_i] = -1;
    }
}

int main(void)
{
    int i, n;

    scanf("%d", &n);

    int nums[n];

    for (i = 0; i < n; i++)
    {
        scanf("%d", &nums[i]);
    }

    print_even(nums, n);
    print_odd(nums, n);
    printf("\n");
}