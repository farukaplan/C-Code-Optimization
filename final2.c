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
    double maximumProduct = 0;

    for (int y=0; y<H; y++) {
        int y1 = max(y-S, 0);
        int y2 = min(y+S, H-1);
        
        for (int x=0; x<W; x+=4) {
            unsigned char minValue0 = 255;
            unsigned char minValue1 = 255;
            unsigned char minValue2 = 255;
            unsigned char minValue3 = 255;
            
            unsigned char maxValue0 = 0;
            unsigned char maxValue1 = 0;
            unsigned char maxValue2 = 0;
            unsigned char maxValue3 = 0;
            
            int i = y1 * W + x;
            for (int yy=y1; yy<=y2; yy++) {
                minValue0 = min(minValue0, data[i]);
                maxValue0 = max(maxValue0, data[i]);

                minValue1 = min(minValue1, data[i+1]);
                maxValue1 = max(maxValue1, data[i+1]);

                minValue2 = min(minValue2, data[i+2]);
                maxValue2 = max(maxValue2, data[i+2]);

                minValue3 = min(minValue3, data[i+3]);
                maxValue3 = max(maxValue3, data[i+3]);
                
                i+= W;
            }

            double product0 = sqrt((unsigned int)minValue0 * (unsigned int)maxValue0);
            double product1 = sqrt((unsigned int)minValue1 * (unsigned int)maxValue1);
            double product2 = sqrt((unsigned int)minValue2 * (unsigned int)maxValue2);
            double product3 = sqrt((unsigned int)minValue3 * (unsigned int)maxValue3);

            maximumProduct = max(maximumProduct, product0);
            maximumProduct = max(maximumProduct, product1);
            maximumProduct = max(maximumProduct, product2);
            maximumProduct = max(maximumProduct, product3);
        }
    }    

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
    
    const int experimentCount = 10;

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
