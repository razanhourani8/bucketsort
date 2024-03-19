#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Bucket {
    int num_elems;
    int index;
    int start;
};

int cmp(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int main(int argc, char *argv[]) {
    int *arr, *temp;
    int size, num_buckets, i, width, j, limit;
    struct Bucket *buckets;
    clock_t start_time, end_time;
    float total_time;

    printf("Enter the length of the array to sort: \n");
    if (scanf("%d", &size) != 1){
        printf("Error\n");
        return -1;
    }
    printf("Enter the number of buckets: \n");
    if (scanf("%d", &num_buckets) != 1){
        printf("Error\n");
        return -1;
    }
    limit = 100000;
    width = (int)limit / num_buckets;
    arr = (int *)malloc(sizeof(int) * size);
    buckets = (struct Bucket *)calloc(num_buckets, sizeof(struct Bucket));

    for (i = 0; i < size; i++) {
        arr[i] = rand() % limit;
    }

    start_time = clock();
    
    for (i = 0; i < size; i++) {
        j = arr[i] / width;
        if (j > num_buckets - 1)
            j = num_buckets - 1;
        buckets[j].num_elems++;
    }
    
    for (i = 1; i < num_buckets; i++) {
        buckets[i].index = buckets[i - 1].index + buckets[i - 1].num_elems;
        buckets[i].start = buckets[i - 1].start + buckets[i - 1].num_elems;
    }
    
    int b_index;
    for (i = 0; i < size; i++) {
        j = arr[i] / width;
        if (j > num_buckets - 1)
            j = num_buckets - 1;
        b_index = buckets[j].index++;
        arr[b_index] = arr[i];
    }

    for (i = 0; i < num_buckets; i++) {
        qsort(arr + buckets[i].start, buckets[i].num_elems, sizeof(int), cmp);
    }

    end_time = clock();
    total_time = ((float)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("Sorted array: \n");
    for (i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    printf("Sorting took %f seconds\n", total_time);

    int sorted = 1;
    for (i = 0; i < size - 1; i++) {
        if (arr[i] > arr[i + 1])
            sorted = 0;
    }
    if (!sorted)
        printf("The data is not sorted\n");

    free(arr);
    free(buckets);

    return 0;
}
