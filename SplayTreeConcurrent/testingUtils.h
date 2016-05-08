#pragma once
#include "Settings.h"
#include <iostream>
#include <unordered_set>
#include "splayset_concurrent.hpp"

inline void my_assert(bool expresssion)
{
	if (!expresssion) {
		std::cout << "Problem" << std::endl;
	}
}

inline void my_assert(bool expression, const char* message)
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
void testUSnSt(const std::unordered_set<T>& us, splayset<T>& st)
{
	for (const auto& item : us) {
		auto res = st.find(item);
		my_assert(res && res->data == item);
	}
}

//Expects node to have parent
template <typename T, typename Comp>
size_t subTreeCountAndCheckParent(typename splayset<T, Comp>::node* node)
{
	static Comp comp;
	if (!node) return 0;
	my_assert(node->P->L == node || node->P->R == node, "father doesn't recognize his son");
	my_assert(node->P->L == node ? comp(node->data, node->P->data) : comp(node->P->data, node->data), "BST rule violation");
	return subTreeCountAndCheckParent<T, Comp>(node->L) + 1 + subTreeCountAndCheckParent<T, Comp>(node->R);
}

template <typename T, typename Comp>
void testTreeConsistency(splayset<T, Comp>& st, size_t expectedNodeCount)
{
	using node = typename splayset<T, Comp>::node;
	node* root = st.root;
	size_t count = subTreeCountAndCheckParent<T, Comp>(root->L) + 1 + subTreeCountAndCheckParent<T, Comp>(root->R);
	my_assert(count == expectedNodeCount, "node count doesn't match");
}


