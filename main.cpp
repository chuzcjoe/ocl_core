#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include "OclWrapper.h"
#include <CL/opencl.h>

int main() {
    OclWrapper* ocl = new OclWrapper();
    ocl->createOclRunTime();
    ocl->deviceInfo();
    delete ocl;
}