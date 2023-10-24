__kernel void gaussian_filter(__global float* input, __global float* output, int width, int height) {
    int x = get_global_id(0);
    int y = get_global_id(1);

    const float filter[9] = {
        1.0f / 16, 2.0f / 16, 1.0f / 16,
        2.0f / 16, 4.0f / 16, 2.0f / 16,
        1.0f / 16, 2.0f / 16, 1.0f / 16
    };

    int half_kernel_size = 1;

    float sum = 0.0f;
    for (int i = -half_kernel_size; i <= half_kernel_size; ++i) {
        for (int j = -half_kernel_size; j <= half_kernel_size; ++j) {
            int xOffset = x + i;
            int yOffset = y + j;

            // Ensure the offset is within the image boundaries
            xOffset = clamp(xOffset, 0, width - 1);
            yOffset = clamp(yOffset, 0, height - 1);

            // Calculate the index in the 1D array
            int index = yOffset * width + xOffset;

            // Apply the Gaussian kernel
            sum += filter[(i + half_kernel_size) * 3 + (j + half_kernel_size)] * input[index];
        }
    }

    // Write the result to the output image
    output[y * width + x] = sum;
}
