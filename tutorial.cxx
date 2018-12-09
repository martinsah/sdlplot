#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <fftw3.h>
#include <complex>
#include <boost/program_options.hpp>
#include <iostream>
#include <iomanip>
#include "sdlplot.h"
#include "TutorialConfig.h"

#define N 1024

using namespace std::complex_literals;
using namespace std;
using namespace boost::program_options;

void on_input(std::string s)
{
  std::cout << "On input: " << s << '\n';
}

int main(int argc, char *argv[])
{
  fprintf(stdout, "Version %d.%d\n", Tutorial_VERSION_MAJOR, Tutorial_VERSION_MINOR);
  try
  {
    options_description desc{"Options"};
    desc.add_options()("help,h", "Help screen")("fs", value<float>()->default_value(1e6f), "Sample Rate")("input", value<std::string>()->notifier(on_input), "Input Filename");

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);

    if (vm.count("help"))
      std::cout << desc << '\n';
    else if (vm.count("fs"))
      std::cout << "fs specified as : " << vm["fs"].as<float>() << '\n';
    else if (vm.count("input"))
      std::cout << "input file: " << vm["input"].as<std::string>() << '\n';
  }
  catch (const error &ex)
  {
    std::cerr << ex.what() << '\n';
  }

  fftw_complex *in, *out;
  fftw_plan p;

  for (int x = 0; x < argc; x++)
  {
    printf("\n[%d] == %s", x, argv[x]);
  }
  
  in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
  out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
  p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

  
//  complex<double> bbb = 1i + 1;
//  in[0][0] = reinterpret_cast<double(&)[2]>(bbb)[0];
//  in[0][1] = reinterpret_cast<double(&)[2]>(bbb)[1];
  srand(1);

  for (int n = 0; n < N; n++)
  {
    in[n][0] = ((float)rand()) / RAND_MAX;
    in[n][1] = ((float)rand()) / RAND_MAX;
  }
  fftw_execute(p);

  
  sdlplot pp;
  pp.axis.xmax = M_PI;
  pp.axis.xmin = -M_PI;
  pp.axis.ymin = -1.2;
  pp.axis.ymax = 1.2;
  pp.line.connect_the_dots = true;

  for(int n=0;n<100;n++){
    datapoint pt;
    pt.x = ((double)n-50.0)/100.0 * 2*M_PI;
    pt.y = sin(pt.x);
    pp.line.points.push_back(pt);
    
  }

  pp.makewindow();

  while(1){
    if(pp.run()){
      cout << "\nquit!";
      break;
    }

  }

  fftw_destroy_plan(p);
  fftw_free(in);
  fftw_free(out);

  return 0;
}
