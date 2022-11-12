/* SANYAM
 * GUPTA
 * sanyamgu
*/

#include <cmath>
#include <cuda.h>
#include <iostream>
#include <algorithm>
#include <functional>


__global__ 
void 
gaussianKernel(int n, float h, float* d_x, float* d_y){

    // index to be operated upon
    int gidx = threadIdx.x + blockIdx.x * blockDim.x;

    if (gidx < n){
        // buffer to load data
        extern __shared__ float buffer[];

        // temporarily maintain calcuate result
        float resultIndex = 0.0;

        //load data
        float val = d_x[gidx];
        __syncthreads();

        //since data volume is larger than block memory so load data in chunks
        for (int i = 0; i < gridDim.x; i++){
            buffer[threadIdx.x] = d_x[threadIdx.x + i * blockDim.x];
            __syncthreads();

            //calucuate khat with loaded data
            for (int j = 0; j < blockDim.x; j++)
                resultIndex += exp(-1 * pow(((val - buffer[j]) / h), 2) / 2);
        }

        //function value
        resultIndex /= (pow((2*3.14), 0.5) * n * h);

        //store back result
        d_y[gidx] = resultIndex;

    }

}

void 
gaussian_kde(int n, float h, std::vector<float>& x, std::vector<float>& y) {
    //declare block size
    const int block_size = 1024;
    // store value in dim3
    dim3 d_numBlocks(block_size);

    //size of shared memory
    const int aux_mem_size = block_size*sizeof(float);
    int total_size = n * sizeof(float);

    //number of blocks
    int num_blocks = (n + block_size - 1)/block_size;
    //store value in dim3
    dim3 d_numThreads(num_blocks);

    //allocate memory
    float* d_x;
    float* d_y;
    cudaMalloc(&d_x, total_size);
    cudaMalloc(&d_y, total_size);

    //transfer data to device
    cudaMemcpy(d_x, x.data(), total_size, cudaMemcpyHostToDevice);
    
    gaussianKernel<<<d_numThreads, d_numBlocks, aux_mem_size>>>(n, h, d_x, d_y);

    //transfer data from device
    cudaMemcpy(y.data(), d_y, total_size, cudaMemcpyDeviceToHost);


    // std::cout << "y : " ;
    // for (int i=0; i<n; i++){
    //     std::cout << y[i] << " ";
    // }
    // std::cout << std::endl;

    cudaFree(d_x);
    cudaFree(d_y);
}