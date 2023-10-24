#include "log.h"
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <stdlib.h>
#include "OclWrapper.h"
#include <CL/opencl.h>

int main() {
    
    cv::Mat img, gray;
    
    img = cv::imread("./img/lena.png");
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    gray.convertTo(gray, CV_32F);

    int height = gray.rows;
    int width = gray.cols;
    int totalElements = gray.total();
    std::cout << "total elements: " << totalElements << std::endl;

    OclWrapper* ocl = new OclWrapper();
    ocl->createOclRunTime();
    // ocl->deviceInfo();
    ocl->buildKernel("./kernels/filters.cl", "gaussian_filter");

    cl_mem buffer_in, buffer_out;
    cv::Mat blurred(height, width, CV_32FC1);
    cv::Mat out(height, width, CV_32FC1);
    buffer_in = clCreateBuffer(ocl->m_context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, totalElements * sizeof(float), gray.data, nullptr);
    buffer_out = clCreateBuffer(ocl->m_context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, totalElements * sizeof(float), nullptr, nullptr);

    int idx = 0;
    clSetKernelArg(ocl->m_kernel, idx++, sizeof (cl_mem), &buffer_in);
    clSetKernelArg(ocl->m_kernel, idx++, sizeof (cl_mem), &buffer_out);
    clSetKernelArg(ocl->m_kernel, idx++, sizeof (cl_int), &width);
    clSetKernelArg(ocl->m_kernel, idx++, sizeof (cl_int), &height);

    size_t gs[2] = {(size_t)width, (size_t)height};

    Timer t("gaussianFilter");
    t._Tic();
    ocl->runKernel(gs, nullptr, 2);
    ocl->oclFinish();
    t._Toc();
    t.profile();

    clEnqueueReadBuffer(ocl->m_queue, buffer_out, CL_TRUE, 0, totalElements * sizeof(float), blurred.data, 0, NULL, NULL);

    cv::imwrite("./result/gaussian_blur.png", blurred);

    delete ocl;
    
}