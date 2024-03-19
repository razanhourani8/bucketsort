#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <sys/time.h>
#include <pthread.h>
#include "mysort.h"

struct Node {
    float value;
    struct Node *next;
};

int bucketSize;
struct Node **bucketHeads;
float maxValue = -INFINITY;
float minValue = INFINITY;
int threadCount = 2;

void *bucketSort(void *arg);
struct Node *insertionSort(struct Node *list);

int pthreadSort(int numElements, float *data) {

    for (int i = 0; i < numElements; i++) {
        if (maxValue < data[i]) {
            maxValue = data[i];
        }
        if (minValue > data[i]) {
            minValue = data[i];
        }
    }
    if (minValue == maxValue) {
        return 0;
    }

    bucketSize = numElements / 2;
    bucketHeads = (struct Node **)malloc(sizeof(struct Node*) * numElements);
    for (int i = 0; i < bucketSize; i++) {
        bucketHeads[i] = NULL;
    }


    float width = maxValue - minValue + 0.00001;
    struct Node *node = NULL;
    for (int i = 0; i < numElements; i++) {
        node = (struct Node *)malloc(sizeof(struct Node));
        int index = int((bucketSize * data[i]) / (maxValue + 0.01));
        node->value = data[i];
        node->next = bucketHeads[index];
        bucketHeads[index] = node;
    }


    pthread_t pthread[threadCount];
    int *threadArgs = (int *)malloc(sizeof(int) * threadCount);
    for (int i = 0; i < threadCount; i++) {
        threadArgs[i] = i;
        pthread_create(&pthread[i], NULL, bucketSort, (void *)&threadArgs[i]);
    }
    for (int i = 0; i < threadCount; i++) {
        pthread_join(pthread[i], NULL);
    }
    free(threadArgs);

    // Combine sorted buckets
    node = NULL;
    int dataIndex = 0;
    for (int b = 0; b < bucketSize; b++) {
        node = bucketHeads[b];
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
    int start = *threadID * bucketSize;
    int end = start + bucketSize;
    for (int i = start; i < end; i++) {
        bucketHeads[i] = insertionSort(bucketHeads[i]);
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
