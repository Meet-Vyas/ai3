import torch
import ai3
import torchvision
from bench import predict_show_time
from test import compare_tensors


def run():
    print('VGG Swapping Layers')
    input_data = torch.randn(1, 3, 224, 224)
    with torch.inference_mode():
        vgg16 = torchvision.models.vgg16()
        vgg16.eval()
        torch_out = predict_show_time(vgg16, input_data, "pytorch")

        ai3_vgg16 = ai3.swap_conv2d(vgg16)
        assert (isinstance(torch_out, torch.Tensor))
        ai3_out = predict_show_time(ai3_vgg16, input_data, "ai3")
        compare_tensors(ai3_out, torch_out, "")


if __name__ == "__main__":
    run()
