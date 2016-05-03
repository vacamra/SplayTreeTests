//
//#include "splayset_serial.hpp"
//#include "splayset_concurrent.hpp"
//#include "stopwatch.hpp"
//#include "rc4prng.hpp"
//
//#include <cassert>
//#include <concurrent_unordered_set.h>
//#include <mutex>
//#include <fstream>
//#include <iostream>
//#include <algorithm>
//#include <sstream>
//#include <cstdint>
//#include <thread>
//
//#ifdef _DEBUG
//const size_t concurrency_ = 4,
//item_count = 200,
//group_size = 10,
//group_accesses = 20,
//group_count = 10;
//#else
//const size_t concurrency_ = 4,
//item_count = 200000,
//group_size = 100,
//group_accesses = 2000,
//group_count = 1000;
//#endif // _DEBUG
//
//const char rand_seed[] = "test ukolu";
//
//void my_assert(bool expresssion)
//{
//	if (!expresssion) {
//		std::cout << "Problem" << std::endl;
//	}
//}
//
//template <typename T>
//void testCSnSt(const Concurrency::concurrent_unordered_set<T>& cs, splayset<T>& st)
//{
//	for (const auto& item : cs) {
//		auto res = st.find(item, false);
//		my_assert(res && res->data == item);
//	}
//}
//
//void test_group_parallel_stupid(rc4prng<>& rng, std::mutex& mtx, std::vector<std::string>& results, splayset_serial<uint64_t>&st)
//{
//	uint64_t begin = rng.random(), mult = rng.random();
//	for (size_t i = 0; i < group_accesses; ++i) {
//		std::lock_guard<std::mutex> locker(mtx);
//		int64_t query = begin + mult * rng.random(group_size);
//		auto result = st.find(query);
//		std::stringstream stream;
//		stream << query << "," << (result ? result->data : 0);
//		results.push_back(stream.str());
//	}
//}
//
//void test_group_parallel(rc4prng<>& rng, Concurrency::concurrent_unordered_set<uint64_t>& cs, splayset<uint64_t>&st)
//{
//	uint64_t begin = rng.random(), mult = rng.random();
//	for (size_t i = 0; i < group_accesses; ++i) {
//		int64_t query = begin + mult * rng.random(group_size);
//		auto result = st.find(query);
//
//		// compare with unordered_set
//		auto cs_result = cs.find(query);
//		my_assert((cs_result == cs.end()) == (result == nullptr));
//		my_assert(!result || result->data == *cs_result);
//	}
//}
//
//int main_parallel_stupid()
//{
//	splayset_serial<uint64_t> st;
//	std::mutex mtx;
//
//	rc4prng<> thread_rng[concurrency_];
//	for (size_t tid = 0; tid < concurrency_; ++tid)
//		thread_rng[tid].load_key(rand_seed, rand_seed + 10 - tid); // :]
//
//	std::vector<std::vector<std::string>> results;
//	for (size_t i = 0; i < concurrency_; i++) {
//		results.push_back(std::vector<std::string>());
//	}
//
//	std::thread thread[concurrency_];
//	bpp::Stopwatch stopwatch(true);
//
//	for (size_t tid = 0; tid < concurrency_; ++tid)
//		thread[tid] = std::thread([tid, &thread_rng, &st, &mtx]()
//	{
//		for (size_t i = 0; i < item_count; ++i) {
//			std::lock_guard<std::mutex> locker(mtx);
//			st.insert(thread_rng[tid].random());
//		}
//	});
//
//	for (size_t tid = 0; tid < concurrency_; ++tid)
//		thread[tid].join();
//
//	stopwatch.stop();
//	std::cout << stopwatch.getMiliseconds() << std::endl;
//	std::cout << "Tree size:" << st.size() << std::endl;
//	stopwatch.start();
//
//	for (size_t tid = 0; tid < concurrency_; ++tid)
//		thread[tid] = std::thread([tid, &thread_rng, &st, &mtx, &results]()
//	{
//		for (size_t grp = 0; grp < group_count; ++grp)
//			test_group_parallel_stupid(thread_rng[tid], mtx, results[tid], st);
//	});
//
//	for (size_t tid = 0; tid < concurrency_; ++tid)
//		thread[tid].join();
//
//	stopwatch.stop();
//	std::cout << stopwatch.getMiliseconds() << std::endl;
//	std::cout << "Tree size:" << st.size() << std::endl;
//
//	for (size_t i = 0; i < concurrency_; i++) {
//		std::stringstream str; str << "parallel_naive_" << i << ".out";
//		std::ofstream file(str.str());
//		std::sort(results[i].begin(), results[i].end());
//		for (auto& res : results[i]) {
//			file << res;
//		}
//		file.close();
//	}
//
//	return 0;
//}
//
//
//int main_parallel()
//{
//	splayset<uint64_t> st;
//	Concurrency::concurrent_unordered_set<uint64_t> cs;
//	splayset<uint64_t> st2;
//
//	st2.insert(5);
//	st2.insert(7);
//	st2.insert(10);
//	st2.insert(8);
//	st2.insert(6);
//	st2.insert(3);
//	st2.insert(2);
//	st2.insert(1);
//
//	auto res = st2.find(2);
//	res = st2.find(2);
//	res = st2.find(2);
//	res = st2.find(2);
//	res = st2.find(10);
//
//	st2.erase(2);
//	st2.erase(8);
//	st2.erase(7);
//	res = st2.find(2);
//	res = st2.find(8);
//	res = st2.find(10);
//	st2.erase(5);
//	st2.erase(10);
//	st2.erase(6);
//	st2.erase(3);
//	st2.erase(1);
//
//
//
//	rc4prng<> thread_rng[concurrency_];
//	for (size_t tid = 0; tid < concurrency_; ++tid)
//		thread_rng[tid].load_key(rand_seed, rand_seed + 10 - tid); // :]
//
//	std::vector<std::vector<std::string>> results;
//	for (size_t i = 0; i < concurrency_; i++) {
//		results.push_back(std::vector<std::string>());
//	}
//
//	std::thread thread[concurrency_];
//	bpp::Stopwatch stopwatch(true);
//
//	for (size_t tid = 0; tid < concurrency_; ++tid)
//		thread[tid] = std::thread([tid, &thread_rng, &st, &cs]()
//	{
//		for (size_t i = 0; i < item_count; ++i) {
//			int64_t val = thread_rng[tid].random();
//			st.insert(val);
//			cs.insert(val);
//		}
//	});
//
//	for (size_t tid = 0; tid < concurrency_; ++tid)
//		thread[tid].join();
//
//	stopwatch.stop();
//	std::cout << stopwatch.getMiliseconds() << std::endl;
//	std::cout << "Tree size:" << st.size() << std::endl;
//	stopwatch.start();
//
//	for (size_t tid = 0; tid < concurrency_; ++tid)
//		thread[tid] = std::thread([tid, &thread_rng, &st, &cs]()
//	{
//		for (size_t grp = 0; grp < group_count; ++grp)
//			test_group_parallel(thread_rng[tid], cs, st);
//	});
//
//	for (size_t tid = 0; tid < concurrency_; ++tid)
//		thread[tid].join();
//
//	stopwatch.stop();
//	std::cout << stopwatch.getMiliseconds() << std::endl;
//	std::cout << "Tree size:" << st.size() << std::endl;
//
//	testCSnSt(cs, st);
//
//	return 0;
//}