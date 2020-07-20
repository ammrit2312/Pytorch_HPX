
#include <torch/torch.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <hpx/hpx_main.hpp>
#include <hpx/include/iostreams.hpp>
#include <hpx/hpx.hpp>
#include <hpx/hpx_init.hpp>
#include <hpx/async_local/dataflow.hpp>
#include <hpx/include/lcos.hpp>
#include <hpx/include/parallel_generate.hpp>
#include <hpx/include/parallel_sort.hpp>

#include <chrono>


using namespace std::chrono;

#define POLY_DEGREE 4



torch::Tensor batch_x, batch_y;
auto W_target = torch::randn({POLY_DEGREE, 1}) * 5;
auto b_target = torch::randn({1}) * 5;



// Builds features i.e. a matrix with columns [x, x^2, x^3, x^4].
torch::Tensor make_features(torch::Tensor x) {
  x = x.unsqueeze(1);
  std::vector<torch::Tensor> xs;
  for (int64_t i = 0; i < POLY_DEGREE; ++i)
    xs.push_back(x.pow(i + 1));
  return torch::cat(xs, 1);
}

// Approximated function.
torch::Tensor f(
    torch::Tensor x,
    torch::Tensor W_target,
    torch::Tensor b_target) {
  return x.mm(W_target) + b_target.item();
}

// Creates a string description of a polynomial.
std::string poly_desc(torch::Tensor W, torch::Tensor b) {
  auto size = W.size(0);
  std::ostringstream stream;

  stream << "y = ";
  for (int64_t i = 0; i < size; ++i)
    stream << W[i].item<float>() << " x^" << size - i << " ";
  stream << "+ " << b[0].item<float>();
  return stream.str();
}
  struct retv{
    torch::Tensor x_try, y_try;
  };
// Builds a batch i.e. (x, f(x)) pair.
  /*for hpx
struct retv get_batch(

    torch::Tensor W_target,
    torch::Tensor b_target,
    int64_t batch_size = 32) {
  auto random = torch::randn({batch_size});
  hpx::future<torch::Tensor> f1 = hpx::async(make_features, random);
  auto x = f1.get();
  hpx::future<torch::Tensor> f2 = hpx::async(f,x, W_target, b_target);
  auto y = f2.get();
  
  return retv{x,y};
}*/

void get_batch(int64_t batch_size = 32)
{
  auto random = torch::randn({batch_size});
  batch_x = make_features(random);
  batch_y = f(batch_x, W_target, b_target);
  //return std::make_pair(x, y);
}

int main() {

  auto start = high_resolution_clock::now();

  

  // Define the model and optimizer
  auto fc = torch::nn::Linear(W_target.size(0), 1);
  torch::optim::SGD optim(fc->parameters(), .1);

  float loss = 0;
  int64_t batch_idx = 0;

  while (++batch_idx) {
    // Get data

    //torch::Tensor batch_x, batch_y;


    hpx::future<void> f22 = hpx::async(get_batch,32);//, W_target, b_target);
   // hpx::future<struct retv> f1 = hpx::async(get_batch,W_target, b_target);
f22.get();
   // struct retv name = f1.get();
    //batch_x = name.x_try;
    //batch_y = name.y_try;
    // Reset gradients
    optim.zero_grad();

    // Forward pass
    auto output = torch::smooth_l1_loss(fc(batch_x), batch_y);
    loss = output.item<float>();

    // Backward pass
    output.backward();

    // Apply gradients
    optim.step();

    // Stop criterion
    if (loss < 1e-3f)
      break;
    if(batch_idx == 300)
      break;
  }

  hpx::cout << "Loss: " << loss << " after " << batch_idx << " batches"
            << hpx::endl;
  hpx::future<std::string> f2 = hpx::async(poly_desc,fc->weight.view({-1}), fc->bias);
  hpx::cout << "==> Learned function:\t"
            << f2.get() << hpx::endl;
  hpx::future<std::string> f3 = hpx::async(poly_desc, W_target.view({-1}), b_target);
  hpx::cout << "==> Actual function:\t"
            << f3.get() << hpx::endl;


  hpx::cout << "\n";
  hpx::cout << "\n";
  auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    
    std::cout << "Time taken by function: "
    << duration.count() << " microseconds" << std::endl;
  return 0;
}
