#include "gtest.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	//testing::GTEST_FLAG(output) = "xml:./../../"; // 生成 xml 的结果文件
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
