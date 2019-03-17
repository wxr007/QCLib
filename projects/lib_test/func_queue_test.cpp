#include <stdio.h>
#include <boost/bind.hpp>
#include "containers_define.h"

void func_test()
{
	printf("func_test\n");
}
void func_test_1(int num)
{
	printf("func_test_1:%d\n",num);
}

int main(int argc, char* argv[])
{
	QCOOL::FunctionQueue	func_list;

	func_list.AddFunc(boost::bind(func_test));
	func_list.AddFunc(boost::bind(func_test_1,4));

	func_list.CallAllFunc();
}