#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <sys/time.h>
#include <pthread.h>

struct Node {
    float value;
    struct Node *next;
};

int numBuckets;
struct Node **bucketArray;
float maxValue = -INFINITY;
float minValue = INFINITY;
int numThreads = 2;

void *bucketSort(void *arg);
struct Node *insertionSort(struct Node *list);

int sortArray(int numElements, float *data) {
    // Finding the min and max values in the data
    for (int i = 0; i < numElements; i++) {
        if (maxValue < data[i]) {
            maxValue = data[i];
        }
        if (minValue > data[i]) {
            minValue = data[i];
        }
    }
    // If all elements are the same, no sorting is needed
    if (minValue == maxValue) {
        return 0;
    }

    numBuckets = numElements / 2;
    bucketArray = (struct Node **)malloc(sizeof(struct Node*) * numElements);
    for (int i = 0; i < numBuckets; i++) {
        bucketArray[i] = NULL;
    }

    // Creating buckets and distributing data into them
    float width = maxValue - minValue + 0.00001;
    struct Node *node = NULL;
    for (int i = 0; i < numElements; i++) {
        node = (struct Node *)malloc(sizeof(struct Node));
        int index = (int)((numBuckets * data[i]) / (maxValue + 0.01));
        node->value = data[i];
        node->next = bucketArray[index];
        bucketArray[index] = node;
    }

    // Creating threads to sort buckets
    pthread_t threadArray[numThreads];
    int *threadArgs = (int *)malloc(sizeof(int) * numThreads);
    for (int i = 0; i < numThreads; i++) {
        threadArgs[i] = i;
        pthread_create(&threadArray[i], NULL, bucketSort, (void *)&threadArgs[i]);
    }
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threadArray[i], NULL);
    }
    free(threadArgs);

    // Merging sorted buckets back into the data array
    node = NULL;
    int dataIndex = 0;
    for (int b = 0; b < numBuckets; b++) {
        node = bucketArray[b];
        while (node != NULL) {
            data[dataIndex] = node->value;
            dataIndex++;
            node = node->next;
        }
    }
    return 0;
}

void *bucketSort(void *arg) {
    int *threadID = (int *)arg;
    int start = *threadID * numBuckets;
    int end = start + numBuckets;
    for (int i = start; i < end; i++) {
        bucketArray[i] = insertionSort(bucketArray[i]);
    }
    return NULL;
}

struct Node *insertionSort(struct Node *head) {
    if (head == NULL) {
        return head;
    }

    struct Node *ptr, *prev, *node, *cur, *dummy;
    ptr = head;
    dummy = (struct Node *)malloc(sizeof(struct Node));
    dummy->value = -INFINITY;
    dummy->next = NULL;

    while (ptr != NULL) {
        node = ptr;
        ptr = ptr->next;
        node->next = NULL;

        prev = dummy;
        cur = dummy->next;

        while (cur != NULL && cur->value < node->value) {
            prev = cur;
            cur = cur->next;
        }

        node->next = prev->next;
        prev->next = node;
    }

    head = dummy->next;
    free(dummy);
    return head;
}
