#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

int amountOfPoints = 0;
int pointsInCircle = 0;

void* run(void* x) {
    for (int i = 0; i < amountOfPoints; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;

        // x^2 + y^2 < 1 means points in circle
        if (((x * x) + (y * y)) < 1) {
            pointsInCircle++;
        }
    }

    return NULL;
}

int main(int argc, char** argv) {
    srand((unsigned)time(NULL));
    pthread_t thread;

    while (amountOfPoints <= 0) {
        printf("Amount of points:\n");
        scanf("%d", &amountOfPoints);
    }

    pthread_create(&thread, NULL, &run, NULL);
    pthread_join(thread, NULL);

    double pi = 4.0 * pointsInCircle / amountOfPoints;
    printf("%d points; PI = %f", amountOfPoints, pi);

    return 0;
}