// Name: Faruk
// Surname: KAPLAN
// Student ID: 21050111026

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
    double rowProduct;
    double sum = 0.0;

    for (int col = 0; col < H; col++) {
        rowProduct = 1.0;
        
        for (int row = 0; row < (W - 3); row += 4) {
            if(row == 0) {
                rowProduct = (A / (data[col][row] + B));
            } else {
                rowProduct *= (A / (data[col][row] + B));
            }
            
            rowProduct *= (A / (data[col][row + 1] + B));
            rowProduct *= (A / (data[col][row + 2] + B));
            rowProduct *= (A / (data[col][row + 3] + B));
        }

        sum += rowProduct;
    }
    
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
    printf("compute1() results is %f \n", productSum1);
    printf("compute2() results is %f \n", productSum2);

    printf("compute1() took %f seconds to execute \n", cpu_time_used1);
    printf("compute2() took %f seconds to execute \n", cpu_time_used2);
    
    printf("Speedup factor is %.3fx \n", cpu_time_used1 / cpu_time_used2);
    
    return 0;
}
// 1,319 max one