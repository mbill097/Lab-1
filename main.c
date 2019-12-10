/****************VALUE OF PI****************/

#include<stdio.h>
#include<math.h>
#include <omp.h>
#include <sys/time.h>
#include <stdlib.h>

/**Function that generates a random number*/
double getRand(unsigned int *seed) {
    return (double) rand_r(seed) * 2 / (double) (RAND_MAX) - 1;
}

/** Method to used to find the PI value using sequentail thread  **/
long double Calculate_Pi_Sequential(long n)
{
    unsigned int seed = (unsigned int) time(NULL);
    int Naccept=0,i,nt=10;
    double x , y;//x-coordinates, y-coordinates
    double result=0.00,s=0.00;
    //Renormalize the randomnumbers so that their values are from within [0,1]
    for(i=0;i<n;i+=nt){
        x = getRand(&seed);
        y = getRand(&seed);
        s= x * x + y * y;
        double s=pow(x,2)+pow(y,2);
        if (s<=1.0){
            Naccept++;
        }
        result=nt*(double)Naccept/n;
    }
    return result;
}

/** Method to used to find the PI value using mutiple threads   **/
long double Calculate_Pi_Parallel(long n)
{
    double result=0.0;
    //Parallel mutiple threads
#pragma omp parallel num_threads(20)
    {
        unsigned int seed = (unsigned int) time(NULL) + (unsigned int) 20;
        int Naccept=0,i;
        double x, y; //x-coordinates, y-coordinates
        double s=0.00,partial_result;
        int numthreads = omp_get_num_threads();
#pragma omp single
        //Renormalize the randomnumbers so that their values are from within [0,1]
        for(i=0;i<n;i+=numthreads){
            x = getRand(&seed);
            y = getRand(&seed);
            s=x * x + y * y;
            double s=pow(x,2)+pow(y,2);
            if (s<=1.0){
                Naccept++;
            }
            partial_result=numthreads*(double)Naccept/n;
         }
        result=partial_result;
/*#pragma omp critical
        result += partial_result;
        result=numthreads*partial_result;*/
    }

    return result;
}

void main(){
    struct timeval start, end;
    long  n;
    n = 10000000;
    gettimeofday(&start, NULL);
    printf("Timing sequentiona1...\n");
    long double sequential_pi = Calculate_Pi_Sequential(n);
    gettimeofday(&end, NULL);
    printf("Took %.6f seconds\n", end.tv_sec - start.tv_sec + (double) (end.tv_usec - start.tv_usec) / 1000000L);


    printf("Timing parallel...\n");
    gettimeofday(&start, NULL);
    long double parallel_pi = Calculate_Pi_Parallel(n);
    gettimeofday(&end, NULL);
    printf("Took %.6f seconds\n", end.tv_sec - start.tv_sec + (double) (end.tv_usec - start.tv_usec) / 1000000L);

    // This will print the result to 10 decimal places*/
    printf("pi = %.10Lf (sequential)\n",4 * sequential_pi);
    printf("pi = %.10Lf (parallel)", 4 * parallel_pi);

}

