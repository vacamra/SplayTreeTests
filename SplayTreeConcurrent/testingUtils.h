#pragma once
#include <iostream>
#include <concurrent_unordered_set.h>
#include <unordered_set>
//Don't try this at home - for root access
#define private public 
#include "splayset_concurrent.hpp"

inline void my_assert(bool expresssion)
{
	if (!expresssion) {
		std::cout << "Problem" << std::endl;
	}
}

inline void my_assert(bool expression, char* message)
{
	if (!expression)
	{
		std::cout << "Problem: " << message << std::endl;
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
void testUSnSt(const std::unordered_set<T> us, splayset<T>& st)
{
	for (const auto& item : us) {
		auto res = st.find(item);
		my_assert(res && res->data == item);
	}
}

//Expects node to have parent
template <typename T>
size_t subTreeCountAndCheckParent(typename splayset<T>::node* node)
{
	if (!node) return 0;
	my_assert(node->P->L == node || node->P->R == node, "father doesn't recognize his son");
	return subTreeCountAndCheckParent<T>(node->L) + 1 + subTreeCountAndCheckParent<T>(node->R);
}

template <typename T>
void testTreeConsistency(splayset<T>& st, size_t expectedNodeCount)
{
	using node = typename splayset<T>::node;
	node* root = st.root;
	size_t count = subTreeCountAndCheckParent<T>(root->L) + 1 + subTreeCountAndCheckParent<T>(root->R);
	my_assert(count == expectedNodeCount, "node count doesn't match");
}


