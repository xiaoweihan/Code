#include "CudaParallel.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <cstdio>
//#define _USE_MATH_DEFINES
#include <cmath>
#include "Type.h"

//使用GPU计算 核函数
__global__ void CudaCaculateKernal(double* pX,double* pY,int nLength)
{
	if (nullptr == pX || nullptr == pY)
	{
		return;
	}
	//blockDim 线程块中的线程数
	//gridDim 网格中的线程块数
	//threadidx 线程块中的线程索引
	//blockidx 线程块在网格中的索引
	int nIndex = blockDim.x * blockIdx.x + threadIdx.x;


	if (nIndex < 0 || nIndex >= nLength)
	{
		return;
	}

	pY[nIndex] = sin(pX[nIndex]);
	printf("nIndex = %d.\n", nIndex);

}

//启动Cuda进行并行计算的函数
void CalculateSinValue(int nLength)
{
	//把主机内存中的数据拷贝到设备内存中
	//建立托管数组
	double* pCudaX = NULL;
	double* pCudaY = NULL;

	//托管数组(主机端与设备端都可以访问)
	cudaMallocManaged(&pCudaX, nLength * sizeof(double));
	cudaMallocManaged(&pCudaY, nLength * sizeof(double));

	double dStep = 2.0 * PI / nLength;

	for (int i = 0; i < nLength; ++i)
	{
		pCudaX[i] = i * dStep;
	}
	//构建并行计算网格数
	int n = (nLength + BLOCK_THREAD_NUM - 1) / BLOCK_THREAD_NUM;
	auto BlockSize = dim3(n, 1, 1);
	uint3 ThreadSize;
	ThreadSize.x = BLOCK_THREAD_NUM;
	ThreadSize.y = 1;
	ThreadSize.z = 1;
	//启动核函数
	CudaCaculateKernal <<<BlockSize, ThreadSize>>> (pCudaX, pCudaY, nLength);
	//等待所有网格中的线程结束
	cudaDeviceSynchronize();
	//释放内存
	cudaFree(pCudaX);
	cudaFree(pCudaY);
}

//GPU计算能力查询   计算能力从1.0开始
void QueryGPUCapcity(void)
{
	cudaDeviceProp prop;

	int count = 0;
	cudaGetDeviceCount(&count);
	printf("显卡所支持的cuda处理器数量：%d\n", count);
	for (int i = 0; i < count; ++i) 
	{
		cudaGetDeviceProperties(&prop, i);
		printf("----第%d个处理器的基本信息----\n", i + 1);
		printf("处理器名称：%s \n", prop.name);
		printf("计算能力：%d.%d\n", prop.major, prop.minor);
		printf("设备上全局内存总量：%dMB\n", prop.totalGlobalMem / 1024 / 1024);
		printf("设备上常量内存总量：%dKB\n", prop.totalConstMem / 1024);
		printf("一个线程块中可使用的最大共享内存：%dKB\n", prop.sharedMemPerBlock / 1024);
		printf("一个线程束包含的线程数量：%d\n", prop.warpSize);
		printf("一个线程块中可包含的最大线程数量：%d\n", prop.maxThreadsPerBlock);
		printf("多维线程块数组中每一维可包含的最大线程数量：(%d,%d,%d)\n", prop.maxThreadsDim[0],
			prop.maxThreadsDim[1], prop.maxThreadsDim[2]);
		printf("一个线程格中每一维可包含的最大线程块数量：(%d,%d,%d)\n", prop.maxGridSize[0],
			prop.maxGridSize[1], prop.maxGridSize[2]);
	}
}