#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include "OclWrapper.h"

/* OpenCL Header file */
#include <CL/opencl.h>

int main() {
    /* data on host */
    int *a = NULL;
    int *b = NULL;
    int *c = NULL;

    /* element numbers of each array */
    const int num_elements = 1000;

    /* array size */
    size_t data_size = sizeof (int) * num_elements;

    /* allocate mem for array */
    a = (int *) malloc (data_size);
    b = (int *) malloc (data_size);
    c = (int *) malloc (data_size);

    /* init array */
    for (int i = 0; i < num_elements; i++) {
        a[i] = i;
        b[i] = i;
        c[i] = a[i] + b[i];
    }


    OclWrapper* ocl = new OclWrapper();
    ocl->createOclRunTime();
    ocl->buildKernel("./kernels/vecAdd.cl", "vecss_add");

    cl_mem buffer_a, buffer_b, buffer_c;
    buffer_a = clCreateBuffer (ocl->m_context, CL_MEM_READ_ONLY, data_size, NULL, NULL);
    buffer_b = clCreateBuffer (ocl->m_context, CL_MEM_READ_ONLY, data_size, NULL, NULL);
    buffer_c = clCreateBuffer (ocl->m_context, CL_MEM_READ_WRITE, data_size, NULL, NULL);

    /* copy array a and b to buffer_a and buffer_b */
    clEnqueueWriteBuffer (ocl->m_queue, buffer_a, CL_FALSE, 0, data_size, a, 0, NULL, NULL);
    clEnqueueWriteBuffer (ocl->m_queue, buffer_b, CL_FALSE, 0, data_size, b, 0, NULL, NULL);

    clSetKernelArg (ocl->m_kernel, 0, sizeof (cl_mem), &buffer_a);
    clSetKernelArg (ocl->m_kernel, 1, sizeof (cl_mem), &buffer_b);
    clSetKernelArg (ocl->m_kernel, 2, sizeof (cl_mem), &buffer_c);

    size_t gs[1] = {num_elements};

    Timer t("vecAdd");
    t._Tic();
    ocl->runKernel(gs, nullptr, 1);
    clFinish(ocl->m_queue);
    t._Toc();
    t.profile();

    delete ocl;
    free (a);
    free (b);
    free (c);
}