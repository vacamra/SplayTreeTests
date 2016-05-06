#include <iostream>
#include "splayset_serial.hpp"
#include "stopwatch.hpp"
#include "Settings.h"
#include "rc4prng.hpp"

void test_group_biased(uint64_t start, splayset_serial<uint64_t>& st)
{
	for (size_t i = 0; i < group_accesses; i++)
	{
		st.find(i);
	}
}

int main_serial_biasedData()
{
	splayset_serial<uint64_t> st;
		
	bpp::Stopwatch stopwatch(true);

	for (size_t i = 0; i < concurrency_ * item_count; ++i)
		st.insert(i);

	stopwatch.stop();
	std::cout << stopwatch.getMiliseconds() << std::endl;
	std::cout << "Tree size:" << st.size() << std::endl;

	stopwatch.start();

	for (size_t tid = 0; tid < concurrency_; ++tid)
		for (size_t grp = 0; grp < group_count; ++grp)
			test_group_biased(0, st);

	stopwatch.stop();
	std::cout << stopwatch.getMiliseconds() << std::endl;
	std::cout << "Tree size:" << st.size() << std::endl;

	return 0;
}