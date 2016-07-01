#include <stdio.h>
#include <CL/cl.h>

static const int DATA_SIZE = 20;

int main(int argc, char**argv){

	/*Fill data*/
	int *hInputData = NULL;
	hInputData = (int*)malloc(DATA_SIZE * sizeof(int));
	
	for (int i = 0; i < DATA_SIZE; i++){
		hInputData = i;
	}

	/*Status variable for checks*/
	cl_int status;

	/*Find Platforms*/
	cl_platform_id platform;
	status = clGetPlatformIDs(1, &platform, NULL);
	
	/*Find Device*/
	cl_device_id device;
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device, NULL);

	/*Create Context*/
	cl_context context;
	context = clCreateContext(NULL, 1, &device, NULL, NULL, &status);

	/*Create Command Queue*/
	cl_command_queue command_queue;
	command_queue = clCreateCommandQueueWithProperties(context, device, 0, &status);

	
}
