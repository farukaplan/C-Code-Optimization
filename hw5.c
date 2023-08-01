#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define W 512
#define H 16000
#define N 50
#define A 74.5
#define B 51.0

double compute1(unsigned char data[H][W]) 
{
    double *rowProduct = malloc(sizeof(double) * H);
    for (int k=0; k<H; k++) {
        rowProduct[k] = 1.0;
    }

    for (int i=0; i<W; i++) {
        for (int j=0; j<H; j++) {
            rowProduct[j] *= (A / (data[j][i] + B));
        }
    }

    double sum = 0.0;
    for (int k=0; k<H; k++) {
        sum += rowProduct[k];
    }
    
    free(rowProduct);
    
    return sum;
}

double compute2(unsigned char data[H][W]) 
{    
    double *logValues = malloc(sizeof(double) * N);
    for (int k=0; k<N; k++) {
        logValues[k] = log(A / (k + B));
    }
    
    double sum = 0.0;
    for (int j=0; j<H; j+=8) {
        double rowProduct1 = 0.0;
        double rowProduct2 = 0.0;
        double rowProduct3 = 0.0;
        double rowProduct4 = 0.0;
        double rowProduct5 = 0.0;
        double rowProduct6 = 0.0;
        double rowProduct7 = 0.0;
        double rowProduct8 = 0.0;
        unsigned char* dataPtr1 = data[j];
        unsigned char* dataPtr2 = data[j+1];
        unsigned char* dataPtr3 = data[j+2];
        unsigned char* dataPtr4 = data[j+3];
        unsigned char* dataPtr5 = data[j+4];
        unsigned char* dataPtr6 = data[j+5];
        unsigned char* dataPtr7 = data[j+6];
        unsigned char* dataPtr8 = data[j+7];
        for (int i=0; i<W; i++) {
            rowProduct1 += logValues[*dataPtr1++];
            rowProduct2 += logValues[*dataPtr2++];
            rowProduct3 += logValues[*dataPtr3++];
            rowProduct4 += logValues[*dataPtr4++];
            rowProduct5 += logValues[*dataPtr5++];
            rowProduct6 += logValues[*dataPtr6++];
            rowProduct7 += logValues[*dataPtr7++];
            rowProduct8 += logValues[*dataPtr8++];
        }
        sum += exp(rowProduct1) + exp(rowProduct2) + exp(rowProduct3) + exp(rowProduct4)
             + exp(rowProduct5) + exp(rowProduct6) + exp(rowProduct7) + exp(rowProduct8);
    }
    
    free(logValues);

    return sum;
}

void initializeData(unsigned char data[H][W]) 
{
    for (int i=0; i<W; i++) {
        for (int j=0; j<H; j++) {
            data[j][i] = rand() % N;
        }
    }
}

int main()
{
    // initialize
    int seed = time(0);
    unsigned char data[H][W];
    int experimentCount = 4;

    double productSum1 = 0.0;
    double productSum2 = 0.0;
    double cpu_time_used1 = 0.0;
    double cpu_time_used2 = 0.0;

    for (int t=0; t<experimentCount; t++) {
        // compute-1
        srand(seed + t);
        initializeData(data);
        
        clock_t start1 = clock();
        for (int k=0; k<experimentCount; k++) {
            productSum1 += compute1(data);
        }
        clock_t executionTime1 = clock() - start1;
        cpu_time_used1 += (((double)executionTime1) / CLOCKS_PER_SEC);
    
        // compute-2
        srand(seed + t);
        initializeData(data);
    
        clock_t start2 = clock();
        for (int k=0; k<experimentCount; k++) {
            productSum2 += compute2(data);
        }
        clock_t executionTime2 = clock() - start2;
        cpu_time_used2 += (((double)executionTime2) / CLOCKS_PER_SEC);
    }
    
    double normalizer = 1.0 / (experimentCount * experimentCount);
    productSum1 *= normalizer;
    productSum2 *= normalizer;
    cpu_time_used1 *= normalizer;
    cpu_time_used2 *= normalizer;
    
    // show results
    printf("compute1() results is %.3f \n", productSum1);
    printf("compute2() results is %.3f \n", productSum2);

    printf("compute1() took %f seconds to execute \n", cpu_time_used1);
    printf("compute2() took %f seconds to execute \n", cpu_time_used2);
    
    printf("Speedup factor is %.3fx \n", cpu_time_used1 / cpu_time_used2);
    
    return 0;
}