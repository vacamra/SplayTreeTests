/*
 * Author: Mirek Kratochvil <kratochvil@ksi.mff.cuni.cz>
 * Last Modification: 2016/04/26
 * License: CC 3.0 BY-NC (http://creativecommons.org/)
 *
 * PRNG generator based on RC4.
 */

#ifndef rc4prng_h
#define rc4prng_h

#include <vector>

template<class inttype = unsigned char, int bits = 8, int disc_bytes = 0>
class rc4prng
{
	std::vector<inttype> S;
	inttype I, J;
	inttype mask;
public:
	void init () {
		size_t Ssize = 1 << bits;
		I = J = 0;
		S.resize (Ssize);
		mask = ~ (inttype) 0;
		if ( (inttype) (1 << bits)) mask %= 1 << bits;
		for (size_t i = 0; i < Ssize; ++i) S[i] = (inttype)i;
	}

	rc4prng() {
		init();
	}

	void clear() {
		init();
	}

	template<class K>
	void load_key (const K*begin, const K*end) {
		inttype j, t;
		size_t i;
		const K *keypos;

		//eat whole key iteratively, even if longer than permutation
		for (; begin < end; begin += mask + 1) {
			j = 0;
			for (i = 0, keypos = begin;
			     i <= mask;
			     ++i, ++keypos) {
				if (keypos >= end) keypos = begin; //rotate
				j = (j + S[i] + (inttype) (*keypos)) & mask;
				t = S[j];
				S[j] = S[i];
				S[i] = t;
			}
		}

		gen (disc_bytes, nullptr);
	}

	inttype gen() {
		I = (I + 1) & mask;
		J = (J + S[I]) & mask;

		register inttype t;
		t = S[J];
		S[J] = S[I];
		S[I] = t;

		return S[ (S[I] + S[J]) & mask];
	}

	inline void gen (size_t n, inttype*out) {
		if (out)
			for (size_t i = 0; i < n; ++i) out[i] = gen();
		else
			for (size_t i = 0; i < n; ++i) gen();
	}

	inline void gen (size_t n, std::vector<inttype>&out) {
		out.resize (n);
		gen (n, & (out[0]));
	}

	template<typename T>
	inline void gen (T&a) {
		gen (sizeof (T) / sizeof (inttype), (inttype*) &a);
	}

	inline uint64_t random () {
		uint64_t r;
		gen (r);
		return r;
	}

	inline uint64_t random (uint64_t max) {
		return random() % max;
	}
};

#endif
