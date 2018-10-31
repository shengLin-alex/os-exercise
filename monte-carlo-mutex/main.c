#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int pointsPerThread = 0;
int pointsInCircle = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* run(void* x) {
    int pointsInCirclePerThread = 0;
    unsigned int randState = rand();

    for (int i = 0; i < pointsPerThread; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;

        // x^2 + y^2 < 1 means points in circle
        if (((x * x) + (y * y)) < 1) {
            pointsInCirclePerThread++;
        }
    }

    // lock and accumulate points in circle per thread to global pointsInCircle
    pthread_mutex_lock(&mutex);
    pointsInCircle += pointsInCirclePerThread;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char** argv) {
    srand((unsigned)time(NULL));

    int threadCount = 0;
    int amountOfPoints = 0;

    while (amountOfPoints <= 0 && threadCount <= 0) {
        printf("Amount of points:\n");
        scanf("%d", &amountOfPoints);
        printf("The number of thread:\n");
        scanf("%d", &threadCount);
    }

    pointsPerThread = amountOfPoints / threadCount;
    
    pthread_t* threads = malloc(threadCount * sizeof(pthread_t));
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    for (int i = 0; i < threadCount; i++) {
        pthread_create(&threads[i], &attr, run, NULL);
    }

    for (int i = 0; i < threadCount; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    free(threads);

    double pi = 4.0 * pointsInCircle / amountOfPoints;
    printf("%d points; PI = %f", amountOfPoints, pi);

    return 0;
}