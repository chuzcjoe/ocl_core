#include "OclWrapper.h"

OclWrapper::OclWrapper() {

}

OclWrapper::~OclWrapper() {

}

cl_device_id OclWrapper::getDevice() {
    
    clGetPlatformIDs (0, NULL, &num_platforms);
    m_platforms.resize(num_platforms);
    clGetPlatformIDs (num_platforms, m_platforms.data(), NULL);

    if (m_platforms.empty()) {
        std::cerr << "No platforms found!" << std::endl;
        exit(1);
    }

    clGetDeviceIDs(m_platforms[0], CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices);
    m_devices.resize(num_devices);
    clGetDeviceIDs(m_platforms[0], CL_DEVICE_TYPE_GPU, num_devices, m_devices.data(), NULL);

    if (m_devices.empty()) {
        std::cerr << "No devices found!" << std::endl;
        exit(1);
    } else {
        std::cout << "number of devices: " << m_devices.size() << std::endl;
    }

    return m_devices[0];
}

void OclWrapper::initContext() {
    m_context = clCreateContext(NULL, num_devices, m_devices.data(), NULL, NULL, NULL);
}

void OclWrapper::initQueue() {
    m_queue = clCreateCommandQueueWithProperties (m_context, m_devices[0], 0, NULL);;
}

void OclWrapper::buildProgramWithSource(const std::string& kernel_file, const std::string& kernel_name) {
    std::ifstream kernel(kernel_file);
    std::string src(std::istreambuf_iterator<char>(kernel), (std::istreambuf_iterator<char>()));
    kernel.close();

    const char* source = src.c_str();
    m_program = clCreateProgramWithSource(m_context, 1, &source, NULL, NULL);
    clBuildProgram(m_program, num_devices, m_devices.data(), NULL, NULL, NULL);
    m_kernel = clCreateKernel(m_program, kernel_name.c_str(), NULL);
}

void OclWrapper::runKernel(const size_t gs[], const size_t ls[], int dim) {
    clEnqueueNDRangeKernel(m_queue, m_kernel, dim, NULL, gs, ls, 0, NULL, NULL);
}

void OclWrapper::createOclRunTime() {
    this->getDevice();
    this->initContext();
    this->initQueue();
}

void OclWrapper::buildKernel(const std::string& kernel_file, const std::string& kernel_name) {
    this->buildProgramWithSource(kernel_file, kernel_name);
}