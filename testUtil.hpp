#ifndef TESTUTIL_HPP
#define TESTUTIL_HPP
#include <iostream>
#include <assert.h>
#include "testUtil.hpp"
using std::cerr;
using std::endl;

template <typename T>
void assertEqual(T lhs, T rhs)
{
    if (lhs != rhs)
    {
        cerr << "assertEqual failed : " << lhs << " " << rhs << endl;
        assert(false);
    }
}

#endif