import torch
import ai3
from bench import predict_show_time
from test import compare_tensors
from example.simple_conv import SimpleConvNet


def run():
    print('SIMPLE CREATED')
    input_data = torch.randn(100, 3, 224, 224)
    with torch.inference_mode():
        torch_model = SimpleConvNet()
        torch_model.eval()
        torch_out = predict_show_time(torch_model, input_data, "pytorch")
        assert (isinstance(torch_out, torch.Tensor))

        ai3_model = ai3.swap_backend(torch_model)
        ai3_out = predict_show_time(ai3_model, input_data, "ai3")
        compare_tensors(ai3_out, torch_out, "")


if __name__ == "__main__":
    run()
