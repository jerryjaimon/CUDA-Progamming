#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "Inversion_CUDA.h"
#include "stdio.h";
__global__ void Inversion_CUDA(unsigned char* Image, int Channels);


void Image_Inversion_CUDA(unsigned char* Input_Image, int Height, int Width, int Channels) {
	unsigned char* Dev_Input_Image = NULL;
	cudaMalloc((void**)&Dev_Input_Image, Height * Width * Channels);
	cudaMemcpy(Dev_Input_Image, Input_Image, Height * Width * Channels, cudaMemcpyHostToDevice);
	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);

	dim3 Grid_Image(Width, Height);

	cudaEventRecord(start); {
	Inversion_CUDA << <Grid_Image, 1 >> > (Dev_Input_Image, Channels);
	}
	cudaEventRecord(stop);
	float ms = 0.0f;
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&ms, start, stop);
	printf("\n\tElapsed GPU time : %fms", ms);

	cudaMemcpy(Input_Image, Dev_Input_Image, Height * Width * Channels, cudaMemcpyDeviceToHost);
	cudaFree(Dev_Input_Image);

	cudaError_t error = cudaGetLastError();
	if (error != cudaSuccess)
	{
		// print the CUDA error message and exit
		printf("CUDA error: %s\n", cudaGetErrorString(error));
	}
}

__global__ void Inversion_CUDA(unsigned char* Image, int Channels) {
	int x = blockIdx.x;
	int y = blockIdx.y;
	int idx = (x + y * gridDim.x) * Channels;

	for (int i = 0; i < Channels; i++) {
		Image[idx + i] = 255 - Image[idx + i];
	}
}
