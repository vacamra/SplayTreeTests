#pragma once
#include <iostream>
#include <concurrent_unordered_set.h>
#include <unordered_set>
#include "splayset_concurrent.hpp"

inline void my_assert(bool expresssion)
{
	if (!expresssion) {
		std::cout << "Problem" << std::endl;
	}
}


template <typename T>
void testCSnSt(const concurrency::concurrent_unordered_set<T>& cs, splayset<T>& st)
{
	for (const auto& item : cs) {
		auto res = st.find(item);
		my_assert(res && res->data == item);
	}
}

template <typename T>
void testUSnSt(const std::unordered_set<T>& us, splayset<T>& st)
{
	for (const auto& item : us) {
		auto res = st.find(item);
		my_assert(res && res->data == item);
	}
}

