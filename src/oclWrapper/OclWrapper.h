#ifndef __OCLWRAPPER_H__
#define __OCLWRAPPER_H__

#include <assert.h>
#include <mutex>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <fstream>
#include <iostream>
#include <string>
#include <CL/opencl.h>


class OclWrapper {
public:
    cl_uint num_platforms = 0;
    cl_uint num_devices = 0;

    std::vector<cl_device_id> m_devices;
    std::vector<cl_platform_id> m_platforms;
    cl_context m_context;
    cl_command_queue m_queue;
    cl_program m_program;
    cl_kernel m_kernel;
public:
    OclWrapper();
    ~OclWrapper();

    cl_device_id getDevice();
    void initContext();
    void initQueue();
    void buildProgramWithSource(const std::string& kernel_file, const std::string& kernel_name);
    void runKernel(const size_t gs[], const size_t ls[], int dim);

    void createOclRunTime();
    void deviceInfo();
    void buildKernel(const std::string& kernel_file, const std::string& kernel_name);
    void oclFinish();
};

#endif