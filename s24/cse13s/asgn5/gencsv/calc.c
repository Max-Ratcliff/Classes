#include "mathlib.h"
#include "messages.h"
#include "operators.h"
#include "stack.h"

#include <math.h>
#include <stdio.h>

int main() {
    //sin
    FILE *fp = fopen("sin_graph.csv", "w");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    fprintf(fp, "Input,Output\n");

    for (double x = -100.0; x <= 100.0; x += 1.0) {
        double my_sin = Sin(x);
        double math_sin = sin(x);
        double result = fabs(my_sin - math_sin);
        fprintf(fp, "%.20f,%.20f\n", x, result);
    }

    fclose(fp);

    //cos
    fp = fopen("cos_graph.csv", "w");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    fprintf(fp, "Input,Output\n");

    for (double x = -100.0; x <= 100.0; x += 1.0) {
        double my_cos = Cos(x);
        double math_cos = cos(x);
        double result = fabs(my_cos - math_cos);
        fprintf(fp, "%.20f,%.20f\n", x, result);
    }

    fclose(fp);

    //tan
    fp = fopen("tan_graph.csv", "w");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    fprintf(fp, "Input,Output\n");

    for (double x = -100.0; x <= 100.0; x += 1.0) {
        double my_tan = Tan(x);
        double math_tan = tan(x);
        double result = fabs(my_tan - math_tan);
        fprintf(fp, "%.20f,%.20f\n", x, result);
    }

    fclose(fp);

    return 0;
}
