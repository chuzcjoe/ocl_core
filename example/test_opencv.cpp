#include "log.h"
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <stdlib.h>
#include "OclWrapper.h"
#include <CL/opencl.h>

int main() {
    cv::Mat img, gray;
    img = cv::imread("./img/lena.png", 1);
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::imwrite("./result/lena_gray.png", gray);

    return 1;
}