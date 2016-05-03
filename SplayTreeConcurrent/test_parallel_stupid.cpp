#include "Settings.h"
#include "rc4prng.hpp"
#include "stopwatch.hpp"
#include "splayset_serial.hpp"

#include <iostream>
#include <mutex>

void test_group_parallel_stupid(rc4prng<>& rng, std::mutex& mtx, splayset_serial<uint64_t>&st)
{
	uint64_t begin = rng.random(), mult = rng.random();
	for (size_t i = 0; i < group_accesses; ++i) {
		std::lock_guard<std::mutex> locker(mtx);
		int64_t query = begin + mult * rng.random(group_size);
		auto result = st.find(query);
	}
}


int main_parallel_stupid()
{
	splayset_serial<uint64_t> st;
	std::mutex mtx;

	rc4prng<> thread_rng[concurrency_];
	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread_rng[tid].load_key(rand_seed, rand_seed + 10 - tid); // :]

	std::vector<std::vector<std::string>> results;
	for (size_t i = 0; i < concurrency_; i++) {
		results.push_back(std::vector<std::string>());
	}

	std::thread thread[concurrency_];
	bpp::Stopwatch stopwatch(true);

	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread[tid] = std::thread([tid, &thread_rng, &st, &mtx]()
	{
		for (size_t i = 0; i < item_count; ++i) {
			std::lock_guard<std::mutex> locker(mtx);
			st.insert(thread_rng[tid].random());
		}
	});

	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread[tid].join();

	stopwatch.stop();
	std::cout << stopwatch.getMiliseconds() << std::endl;
	std::cout << "Tree size:" << st.size() << std::endl;
	stopwatch.start();

	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread[tid] = std::thread([tid, &thread_rng, &st, &mtx, &results]()
	{
		for (size_t grp = 0; grp < group_count; ++grp)
			test_group_parallel_stupid(thread_rng[tid], mtx, st);
	});

	for (size_t tid = 0; tid < concurrency_; ++tid)
		thread[tid].join();

	stopwatch.stop();
	std::cout << stopwatch.getMiliseconds() << std::endl;
	std::cout << "Tree size:" << st.size() << std::endl;

	return 0;
}
