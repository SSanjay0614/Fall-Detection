#include <string.h>

void score(double * input, double * output) {
    double var0[2];

    if (input[1] <= 3.6754653) {
        if (input[6] > 2.312645) {
            if (input[0] > 11.492050) {
                if (input[7] > 0.543256) {
                    if (input[4] > 63.56845) {
                        if (input[8] <= 17.153267) {
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
