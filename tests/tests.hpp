/*
 * tests.hpp
 *
 *      Author: ruanbo
 */

#ifndef TESTS_TESTS_HPP_
#define TESTS_TESTS_HPP_

#include <string>

void thread_tests();
void thread_tests2();


void ringbuff_tests();

void smart_ptr_tests();

void server_tests(const std::string& param);

void db_tests();

void msg_tests();

void reids_tests(const std::string& param);

void template_test_entry();

void json_tests();

void time_tests();

void test_entrys(const std::string& param)
{
//    thread_tests();
//    thread_tests2();

//    ringbuff_tests();

//    smart_ptr_tests();

    server_tests(param);

//    db_tests();

//    msg_tests();

//    reids_tests();

//    template_test_entry();

//    json_tests();

//    time_tests();
}





#endif /* TESTS_TESTS_HPP_ */
