#include <string.h>

void score(double * input, double * output) {
    double var0[2];

    if (input[1] <= 2.0) {
        if (input[6] > 1.5) {
            if (input[0] > 11.0) {
                if (input[7] > 0.5) {
                    if (input[4] > 60.0) {
                        if (input[8] <= 15.0) {
                            memcpy(var0, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var0, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    } else {
                        memcpy(var0, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                } else {
                    memcpy(var0, (double[]){1.0, 0.0}, 2 * sizeof(double));
                }
            } else {
                memcpy(var0, (double[]){1.0, 0.0}, 2 * sizeof(double));
            }
        } else {
            memcpy(var0, (double[]){1.0, 0.0}, 2 * sizeof(double));
        }
    } else {
        memcpy(var0, (double[]){1.0, 0.0}, 2 * sizeof(double));
    }

    memcpy(output, var0, 2 * sizeof(double));
}