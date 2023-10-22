#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include "OclWrapper.h"
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
    ocl->buildKernel("./kernels/vecAdd.cl", "vec_add1");

    cl_mem buffer_a, buffer_b, buffer_c;
    buffer_a = clCreateBuffer (ocl->m_context, CL_MEM_READ_ONLY, data_size, NULL, NULL);
    buffer_b = clCreateBuffer (ocl->m_context, CL_MEM_READ_ONLY, data_size, NULL, NULL);
    buffer_c = clCreateBuffer (ocl->m_context, CL_MEM_READ_WRITE, data_size, NULL, NULL);

    /* copy array a and b to buffer_a and buffer_b */
    clEnqueueWriteBuffer (ocl->m_queue, buffer_a, CL_FALSE, 0, data_size, a, 0, NULL, NULL);
    clEnqueueWriteBuffer (ocl->m_queue, buffer_b, CL_FALSE, 0, data_size, b, 0, NULL, NULL);

    int idx = 0;
    clSetKernelArg (ocl->m_kernel, idx++, sizeof (cl_mem), &buffer_a);
    clSetKernelArg (ocl->m_kernel, idx++, sizeof (cl_mem), &buffer_b);
    clSetKernelArg (ocl->m_kernel, idx++, sizeof (cl_mem), &buffer_c);

    size_t gs[1] = {num_elements};

    Timer t("vecAdd");
    t._Tic();
    ocl->runKernel(gs, nullptr, 1);
    ocl->oclFinish();
    t._Toc();
    t.profile();

    int* tmp = (int*) malloc(data_size);
    clEnqueueReadBuffer(ocl->m_queue, buffer_c, CL_TRUE, 0, data_size, tmp, 0, NULL, NULL);

    for (int i = 0; i < num_elements; i++) {
        if (tmp[i] != c[i])
            std::cout << "error at index: " << i << ", tmp: " << tmp[i] << ", c: " << c[i] << std::endl; 
    }

    delete ocl;
    free (a);
    free (b);
    free (c);
}