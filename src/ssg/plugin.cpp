//
// Created by Nick on 1/4/2017.
//
#include "plugin.h"

#include <iostream>

namespace ssg
{
    void IPlugin::Test() {
        std::cout << "This is a test of Plugin!";
    }
}