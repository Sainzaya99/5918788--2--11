#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long count = 0;

int greater(int a, int b)
{
    count++;
    return a > b;
}

void insertion_sort(int arr[], int n)
{
    int i, j, key;

    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && greater(arr[j], key)) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void shell_sort(int arr[], int n, int gaps[], int gap_count)
{
    int g, h, i, j, key;

    for (g = 0; g < gap_count; g++) {
        h = gaps[g];
        for (i = h; i < n; i++) {
            key = arr[i];
            j = i - h;
            while (j >= 0 && greater(arr[j], key)) {
                arr[j + h] = arr[j];
                j -= h;
            }
            arr[j + h] = key;
        }
    }
}

int make_original_gaps(int n, int gaps[])
{
    int cnt = 0;
    int h;

    for (h = n / 2; h >= 1; h /= 2)
        gaps[cnt++] = h;
    return cnt;
}

int make_knuth_gaps(int n, int gaps[])
{
    int seq[50];
    int cnt = 0;
    int h = 1;
    int i;

    while (h < n) {
        seq[cnt++] = h;
        h = 3 * h + 1;
    }
    for (i = 0; i < cnt; i++)
        gaps[i] = seq[cnt - 1 - i];
    return cnt;
}

int is_sorted(int arr[], int n)
{
    int i;

    for (i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1])
            return 0;
    }
    return 1;
}

int cmp_int(const void* a, const void* b)
{
    int x = *(const int*)a;
    int y = *(const int*)b;

    return (x > y) - (x < y);
}

long long measure(int original[], int answer[], int n, int method)
{
    int* data = (int*)malloc(sizeof(int) * n);
    int gaps[50];
    int gap_count = 0;
    long long result;

    memcpy(data, original, sizeof(int) * n);  

    if (method == 1)
        gap_count = make_original_gaps(n, gaps);
    else if (method == 2)
        gap_count = make_knuth_gaps(n, gaps);

    count = 0;                                
    if (method == 0)
        insertion_sort(data, n);
    else
        shell_sort(data, n, gaps, gap_count);
    result = count;

    if (!is_sorted(data, n) || memcmp(data, answer, sizeof(int) * n) != 0) {
        printf("정렬 실패! (n = %d, method = %d)\n", n, method);
        exit(1);
    }

    free(data);
    return result;
}

int main(void)
{
    int sizes[6] = { 1000, 2000, 4000, 8000, 16000, 20000 };
    int s, i, n;
    int* original;
    int* answer;

    srand(2026);  

    for (s = 0; s < 6; s++) {
        n = sizes[s];
        original = (int*)malloc(sizeof(int) * n);
        answer = (int*)malloc(sizeof(int) * n);

        for (i = 0; i < n; i++)
            original[i] = rand() % 1000001;

        memcpy(answer, original, sizeof(int) * n);
        qsort(answer, n, sizeof(int), cmp_int);

        printf("Data Size : %d\n\n", n);
        printf("Insertion Sort       : %lld comparisons\n",
            measure(original, answer, n, 0));
        printf("Shell Sort (Original): %lld comparisons\n",
            measure(original, answer, n, 1));
        printf("Shell Sort (Knuth)   : %lld comparisons\n\n",
            measure(original, answer, n, 2));

        free(original);
        free(answer);
    }
    return 0;
}