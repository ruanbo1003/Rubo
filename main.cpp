/*
 * main.cpp
 *
 *      Author: ruanbo
 */



#include "tests/tests.hpp"
#include "comm/Log.hpp"
#include <string>
#include "util/Time.hpp"


int main(int argc, const char *argv[])
{
    std::string param = "";
    if(argc > 1)
    {
        param = std::string(argv[1]);
    }

    test_entrys(param);

    Log(" ==== end of main ==== ");
    return 0;
}





