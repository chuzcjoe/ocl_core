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

    // Check if kernel is defined
    size_t size;
    clGetProgramInfo(m_program, CL_PROGRAM_BINARY_SIZES, sizeof(size_t), &size, nullptr);
    char* binary = new char[size];
    clGetProgramInfo(m_program, CL_PROGRAM_BINARIES, size, &binary, nullptr);

    std::string binaryString(binary, size);

    if (binaryString.find(kernel_name) == std::string::npos) {
        std::cerr << "\033[1;31mkernel is undefined\033[0m" << std::endl;
        assert(false);
    }

    m_kernel = clCreateKernel(m_program, kernel_name.c_str(), NULL);
}

void OclWrapper::runKernel(const size_t gs[], const size_t ls[], int dim) {
    clEnqueueNDRangeKernel(m_queue, m_kernel, dim, NULL, gs, ls, 0, NULL, NULL);
}

void OclWrapper::deviceInfo() {
    auto device = m_devices[0];

    char name[1024];
    clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(name), name, NULL);

    char vendor[1024];
    clGetDeviceInfo(device, CL_DEVICE_VENDOR, sizeof(vendor), vendor, NULL);

    char version[1024];
    clGetDeviceInfo(device, CL_DEVICE_VERSION, sizeof(version), version, NULL);

    size_t workItems[3];
    clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(workItems), workItems, NULL);

    size_t workGroups;
    clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(workGroups), &workGroups, NULL);

    cl_uint computeUnits;
    clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(computeUnits), &computeUnits, NULL);

    cl_ulong globalMemory;
    clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(globalMemory), &globalMemory, NULL);

    cl_ulong localMemory;
    clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(localMemory), &localMemory, NULL);


    std::cout << "\tOpenCL Device Info:"
    << "\n\tName: " << name
    << "\n\tVendor: " << vendor
    << "\n\tVersion: " << version
    << "\n\tMax size of work-items: (" << workItems[0] << "," << workItems[1] << "," << workItems[2] << ")"
    << "\n\tMax size of work-groups: " << workGroups
    << "\n\tNumber of compute units: " << computeUnits
    << "\n\tGlobal memory size (bytes): " << globalMemory
    << "\n\tLocal memory size (bytes): " << localMemory
    << std::endl;
}

void OclWrapper::createOclRunTime() {
    this->getDevice();
    this->initContext();
    this->initQueue();
}

void OclWrapper::buildKernel(const std::string& kernel_file, const std::string& kernel_name) {
    this->buildProgramWithSource(kernel_file, kernel_name);
}

void OclWrapper::oclFinish() {
    if (!m_queue)
        return;
    
    clFinish(m_queue);
}