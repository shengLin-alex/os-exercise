#include "stdlib.h"
#include "omp.h"
#include "string.h"
#include "math.h"
#include "stdio.h"

int main(int argc, char** argv) {
    int amountOfPoints = 0;
    int pointsInCircle = 0;

    while (amountOfPoints <= 0) {
        printf("Amount of points:\n");
        scanf("%d", &amountOfPoints);
    }

#pragma omp parallel for num_threads(8)
    {
        for (int i = 0; i < amountOfPoints; i++) {
            double x = (double)rand() / RAND_MAX;
            double y = (double)rand() / RAND_MAX;

            // x^2 + y^2 < 1 means points in circle
            if (((x * x) + (y * y)) < 1) {
                pointsInCircle++;
            }
        }
    }

    double pi = 4.0 * pointsInCircle / amountOfPoints;
    printf("%d points; PI = %f", amountOfPoints, pi);

    return 0;
}