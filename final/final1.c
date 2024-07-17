#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#define min(a,b) (((a)<(b)) ? (a) : (b))
#define max(a,b) (((a)>(b)) ? (a) : (b))

#define W 512
#define H 16384
#define N (W*H) 
#define S 11


double compute1(unsigned char* data)
{
    double* product = (double*)malloc(sizeof(double) * W);
    for (int x=0; x<W; x++) { 
        product[x] = 0.0;
    }

    for (int x=0; x<W; x++) {
        for (int y=0; y<H; y++) {
            double minValue = 1.0/0.0;
            for (int yy=y-S; yy<=y+S; yy++) {
                int yp = min(max(yy, 0), H-1);
                minValue = min(minValue, sqrt(data[yp * W + x]));
            }
            
            double maxValue = -1.0/0.0;
            for (int yy=y-S; yy<=y+S; yy++) {
                int yp = min(max(yy, 0), H-1);
                maxValue = max(maxValue, sqrt(data[yp * W + x]));
            }
            
            product[x] = max(product[x], minValue * maxValue);
        }
    }    
    
    double maximumProduct = 0;
    for (int x=0; x<W; x++) {
        maximumProduct = max(maximumProduct, product[x]);
    }

    free(product);

    return maximumProduct;
}

double compute2(unsigned char* data) 
{
    double* product = (double*)malloc(sizeof(double) * W);
    double temporal = 0.0;

    for (int x=0; x<W; x++) { 
        
        for(int y = 0; y < 12; y++) {
            double minValue = 1.0/0.0;
            double maxValue = -1.0/0.0;
            
            for (int yy=y-S; yy<=y+S; yy++) { 
                minValue = min(minValue, sqrt(data[x]));
                maxValue = max(maxValue, sqrt(data[x]));
            }
            
        }
        
        for(int y = 12; y < H; y++) {
            double minValue = 1.0/0.0;
            double maxValue = -1.0/0.0;
            
            for (int yy=y-S; yy<=y+S; yy++) { 
                int temp = yy * W + x;
                minValue = min(minValue, sqrt(data[temp]));
                maxValue = max(maxValue, sqrt(data[temp]));
            }
            
            temporal = max(temporal, minValue * maxValue);
        }
        
        product[x] = temporal;
    }    
    
    double maximumProduct = 0;
    for (int x=0; x<W-7; x+=8) {
        maximumProduct = max(maximumProduct, product[x]);
        maximumProduct = max(maximumProduct, product[x + 1]);
        maximumProduct = max(maximumProduct, product[x + 2]);
        maximumProduct = max(maximumProduct, product[x + 3]);
        maximumProduct = max(maximumProduct, product[x + 4]);
        maximumProduct = max(maximumProduct, product[x + 5]);
        maximumProduct = max(maximumProduct, product[x + 6]);
        maximumProduct = max(maximumProduct, product[x + 7]);
    }

    free(product);

    return maximumProduct;
}

unsigned char* createData(int seed) 
{
    srand(seed);
    
    unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char) * N);
    
    for (int i=0; i<N; i++) {
        data[i] = (unsigned char)(rand());
    }

    return data;
}


int main()
{
    int seed = time(0);
    
    const int experimentCount = 5;

    // compute-1
    unsigned char* data1 = createData(seed);
    clock_t start1 = clock();
    double product1 = 0.0;
    for (int i=0; i<experimentCount; i++) {
        product1 += compute1(data1);
    }
    clock_t executionTime1 = clock() - start1;
    double cpu_time_used1 = (((double)executionTime1 / experimentCount) / CLOCKS_PER_SEC);
    product1 /= experimentCount;

    // compute-2
    unsigned char* data2 = createData(seed);
    clock_t start2 = clock();
    double product2 = 0.0;
    for (int i=0; i<experimentCount; i++) {
        product2 += compute2(data2);
    }
    clock_t executionTime2 = clock() - start2;
    double cpu_time_used2 = (((double)executionTime2 / experimentCount) / CLOCKS_PER_SEC);
    product2 /= experimentCount;

    // free allocated memory
    free(data1);
    free(data2);


    // show results
    printf("compute1() results is %.4lf \n", product1);
    printf("compute2() results is %.4lf \n", product2);

    printf("compute1() took %.4f seconds to execute \n", cpu_time_used1);
    printf("compute2() took %.4f seconds to execute \n", cpu_time_used2);
    
    printf("Speedup factor is %.2fx ", cpu_time_used1 / cpu_time_used2);
    
    return 0;
}
