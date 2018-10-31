#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int pointsPerThread = 0;
int pointsInCircle = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* run(void* randSeed) {
    int pointsInCirclePerThread = 0;

    for (int i = 0; i < pointsPerThread; i++) {
        // use rand_r, cause rand_r is thread-safe.
        double x = (double)rand_r(&((unsigned int*)randSeed)[0]) / RAND_MAX;
        double y = (double)rand_r(&((unsigned int*)randSeed)[1]) / RAND_MAX;

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
    
    pthread_t* threads = (pthread_t*)malloc(threadCount * sizeof(pthread_t));
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    unsigned int* randSeed = (unsigned int*)malloc(2 * sizeof(unsigned int));
    for (int i = 0; i < threadCount; i++) {
        randSeed[0] = rand();
        randSeed[1] = rand();
        pthread_create(&threads[i], &attr, run, randSeed);
    }

    for (int i = 0; i < threadCount; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    free(threads);
    free(randSeed);

    double pi = 4.0 * pointsInCircle / amountOfPoints;
    printf("%d points; PI = %f", amountOfPoints, pi);

    return 0;
}