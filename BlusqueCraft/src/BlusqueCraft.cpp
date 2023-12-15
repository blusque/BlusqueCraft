#include <iostream>
#include <fstream>

#include "Application/Application.h"

int main(int argc, char** argv)
{
    auto const app = new BC::Application;
    app->Run();
    return 0;
}