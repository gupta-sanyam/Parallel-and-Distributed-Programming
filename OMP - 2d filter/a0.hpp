/*  FIRST_NAME = Sanyam
 *  LAST_NAME = Gupta
 *  UBIT_NAME = sanyamgu
 */

#ifndef A0_HPP
#define A0_HPP

#include <vector>
#include <iostream>

float transformation(int index, int n, int m, const std::vector<float>& K, std::vector<float>& A){
    int inital, final, kInit;
    float sum, initSum, val;
    sum = initSum = 0;
    inital = (m * ((index/m) - 1)) + ((index%m) - 1);
    final = (m * ((index/m) + 2)) + ((index%m) - 1);

    for (int aCol=inital; aCol<final; aCol+=m){
        initSum = 0;
        for (int aRow=aCol; aRow<aCol+3; aRow++){
            kInit = (3* (aRow-aCol));
            for (int kIndex=kInit; kIndex<kInit+3; kIndex++){
                val = A[aRow] * K[kIndex];
                sum = sum + val;
                initSum = initSum + val;
            }
        }
    }
    return sum;

}

void filter_2d(int n, int m, const std::vector<float>& K, std::vector<float>& A) {
    std::vector<float> aDash(n*m);
    #pragma omp parallel for shared(K,A,n,m)
    for (int index=0; index< n*m; index++){
        // if corner indices, value is original
        if ((index/m == 0) | (index/m == n-1) | (index%m == 0) || (index%m == n-1)){
            aDash[index] = A[index];
        }
        else {
            aDash[index] = transformation(index, n, m, K, A);
        }
    }

    #pragma omp parallel for shared(A, aDash)
    for (int index=0; index<n*m; index++){
        A[index] = aDash[index];
    }

} // filter_2d

#endif // A0_HPP