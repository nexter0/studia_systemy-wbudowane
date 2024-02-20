#include <stdio.h>
#include <stdlib.h>

int main()
{
    int b, x, idx;
    int binaryArr[16];
    printf("Enter a number: ");
    scanf("%d", &x);
    printf("Bit number: ");
    scanf("%d", &b);
    idx = 0;
    while(x > 0)
    {
        binaryArr[idx] = x % 2;
        x >>= 1;
        idx++;
    }

    if (b > idx - 1)
    {
        printf("No such bit.");
        return 0;
    }

    printf("%d", binaryArr[b]);
    printf("\n");

    for (int j = idx - 1; j >= 0; j--)
    {
       printf("%d", binaryArr[j]);
    }

    return 0;
}
