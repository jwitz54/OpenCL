#include <stdio.h>
#include <CL/cl.h>

#include "utils.c"

static const int DATA_SIZE = 20;

int main(int argc, char**argv){

	const int data_size = 20;
	const int work_size = 5;

	/*Fill data*/
	int *hInputData = NULL;
	hInputData = (int*)malloc(DATA_SIZE * sizeof(int));
		
	int i;
	for (i = 0; i < DATA_SIZE; i++){
		hInputData[i] = i;
	}

	int *hOutputData = (int*)malloc(DATA_SIZE * sizeof(int));

	/*Status variable for checks*/
	cl_int status;

	/*Find Platforms*/
	cl_platform_id platform;
	status = clGetPlatformIDs(1, &platform, NULL);
	check(status);
	
	/*Find Device*/
	cl_device_id device;
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device, NULL);
	check(status);

	/*Create Context*/
	cl_context context;
	context = clCreateContext(NULL, 1, &device, NULL, NULL, &status);
	check(status);

	/*Create Command Queue*/
	cl_command_queue cmdQueue;
	cmdQueue = clCreateCommandQueueWithProperties(context, device, 0, &status);
	check(status);

	/*Create buffer for input data*/
	cl_mem bufInputData;
	bufInputData = clCreateBuffer(context, CL_MEM_READ_ONLY, DATA_SIZE * sizeof(int), NULL, &status);
	check(status);

	/*Create buffer for output data*/
	cl_mem bufOutputData;
	bufOutputData = clCreateBuffer(context, CL_MEM_WRITE_ONLY, DATA_SIZE * sizeof(int), NULL, &status);
	check(status);

	/*Write input data to device*/
	status = clEnqueueWriteBuffer(cmdQueue, bufInputData, CL_TRUE, 0, DATA_SIZE * sizeof(int), hInputData, 0, NULL, NULL);
	check(status);

	/*Initialize output histo*/
	int zero = 0;
	status = clEnqueueFillBuffer(cmdQueue, bufOutputData, &zero, sizeof(int), 0, DATA_SIZE * sizeof(int), 0, NULL, NULL);
	check(status);

	/*Create Prog*/
	char *programSource = readFile("reduce.cl"); //find readfile
	size_t programSourceLen = strlen(programSource);
	cl_program program = clCreateProgramWithSource(context, 1, (const char**)&programSource, &programSourceLen, &status);
	check(status);

	status = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
	if (status != CL_SUCCESS){
		printCompilerError(program, device);
		exit(-1);
	}

	/*Create Kernel*/
	cl_kernel kernel; 
	kernel = clCreateKernel(program, "reduce", &status);
	check(status);

	/*Set kernel arguments (in, out, num)*/
	//int elements = DATA_SIZE;

	status  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufInputData);
	status |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufOutputData);
	status |= clSetKernelArg(kernel, 2, sizeof(int), &data_size);
	status |= clSetKernelArg(kernel, 3, sizeof(int), &work_size);
	check(status);

	/*Define global and work group size*/
	size_t globalWorkSize[1];
	globalWorkSize[0] = DATA_SIZE;

	size_t localWorkSize[1];
	localWorkSize[0] = 4;

	/*Enqueue Kernel*/
	status = clEnqueueNDRangeKernel(cmdQueue, kernel, 1, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);

	/*Read output*/
	status = clEnqueueReadBuffer(cmdQueue, bufOutputData, CL_TRUE, 0, sizeof(int) * DATA_SIZE, hOutputData, 0, NULL, NULL);
	check(status);

	printf("Result: %i \n", hOutputData[0]);

	return (0);	
}
