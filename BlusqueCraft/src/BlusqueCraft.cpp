#include <iostream>
#include <fstream>
#include <omp.h>

#include "Application/Application.h"

int main(int argc, char** argv)
{
    auto const app = new BC::Application;
    app->Run();
    return 0;
// #pragma omp parallel num_threads(16)
//     {
//         std::cout << "hello world!\n";
//     }
//     return 0;
}