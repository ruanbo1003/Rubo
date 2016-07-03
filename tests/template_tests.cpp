/*
 * template_tests.cpp
 *
 *      Author: ruanbo
 */

#include "comm/global.hpp"
#include <iostream>
#include "template_head.hpp"




void template_test_entry()
{
    TAPtr<SItemPtr> foo3(new TA<SItemPtr>());
    foo3->set(1100);

    SItemPtr item(new SItem());
    item->show();
    foo3->add(item);

    TAPtr<SItemPtr> cp = foo3;
}


