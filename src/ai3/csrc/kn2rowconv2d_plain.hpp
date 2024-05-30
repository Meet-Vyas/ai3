#ifndef KN2ROW_CONV2D
#define KN2ROW_CONV2D

#include "tensor.hpp"
#include "utils.hpp"
#include <optional>
#include <vector>

template <typename dtype>
Tensor<dtype>
kn2row_conv2d(const Tensor<dtype> &input, const Tensor<dtype> &kernel,
              const std::optional<const Tensor<dtype>> &bias,
              const std::vector<int> &padding, const std::vector<int> &stride,
              const std::vector<int> &dilation) {
    const int input_channels = dims::input_channels(input.shape);
    const int input_height = dims::height(input.shape);
    const int input_width = dims::width(input.shape);

    const int kernel_height = dims::height(kernel.shape);
    const int kernel_width = dims::width(kernel.shape);

    const int output_channels = dims::out_channels(kernel.shape);

    const int output_height = dims::output_dim<dtype>(
        input_height, kernel_height, padding[0], dilation[0], stride[0], false);
    const int output_width = dims::output_dim<dtype>(
        input_width, kernel_width, padding[1], dilation[1], stride[1], false);

    Tensor<dtype> output =
        Tensor<dtype>({output_channels, output_height, output_width});

    for (int out_c = 0; out_c < output_channels; ++out_c) {
        for (int out_h = 0; out_h < output_height; ++out_h) {
            for (int out_w = 0; out_w < output_width; ++out_w) {
                dtype sum = 0;
                for (int in_c = 0; in_c < input_channels; ++in_c) {
                    for (int kern_r = 0; kern_r < kernel_height; ++kern_r) {
                        for (int kern_c = 0; kern_c < kernel_width; ++kern_c) {
                            int h_offset = out_h * stride[0] - padding[0] +
                                           kern_r * dilation[0];
                            int w_offset = out_w * stride[1] - padding[1] +
                                           kern_c * dilation[1];

                            if (h_offset >= 0 && h_offset < input_height &&
                                w_offset >= 0 && w_offset < input_width) {
                                sum += input.at(in_c, h_offset, w_offset) *
                                       kernel.at(out_c, in_c, kern_r, kern_c);
                            }
                        }
                    }
                }
                if (bias.has_value()) {
                    sum += bias.value().at(out_c);
                }
                output.at(out_c, out_h, out_w) = sum;
            }
        }
    }
    return output;
}
#endif