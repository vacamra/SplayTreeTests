// Lukas Jendele DU3 NPRG051 2015/16
#ifndef splayset_included
#define splayset_included

#include <functional>

template<typename T, typename Comp = std::less<T> >
class splayset
{
private:
	Comp comp;

	struct node {
		node *L, *R;
		node *P;
		T data;
		node (const T& init = T())
			: L (nullptr), R (nullptr), P (nullptr),
			  data (init) {}
	};

	node *root;
	unsigned long p_size;

	void left_rotate (node *x) {
		node *y = x->R;
		if (y) {
			x->R = y->L;
			if (y->L) y->L->P = x;
			y->P = x->P;
		}

		if (!x->P) root = y;
		else if (x == x->P->L) x->P->L = y;
		else x->P->R = y;
		if (y) y->L = x;
		x->P = y;
	}

	void right_rotate (node *x) {
		node *y = x->L;
		if (y) {
			x->L = y->R;
			if (y->R) y->R->P = x;
			y->P = x->P;
		}

		if (!x->P) root = y;
		else if (x == x->P->L) x->P->L = y;
		else x->P->R = y;
		if (y) y->R = x;
		x->P = y;
	}

	void splay (node *x) {
		while (x->P) {
			if (!x->P->P) {
				if (x->P->L == x) right_rotate (x->P);
				else left_rotate (x->P);
			} else if (x->P->L == x && x->P->P->L == x->P) {
				right_rotate (x->P->P);
				right_rotate (x->P);
			} else if (x->P->R == x && x->P->P->R == x->P) {
				left_rotate (x->P->P);
				left_rotate (x->P);
			} else if (x->P->L == x && x->P->P->R == x->P) {
				right_rotate (x->P);
				left_rotate (x->P);
			} else {
				left_rotate (x->P);
				right_rotate (x->P);
			}
		}
	}

	node* subtree_minimum (node *u) {
		while (u->L) u = u->L;
		return u;
	}

	node* subtree_maximum (node *u) {
		while (u->R) u = u->R;
		return u;
	}
public:
	splayset() : root (nullptr), p_size (0) {}
	splayset (const splayset&) = delete;
	~splayset() {
		clear();
	}

	void insert (const T &data) {
		node *z = root;
		node *p = 0;

		while (z) {
			p = z;
			if (comp (z->data, data)) z = z->R;
			else if (comp (data, z->data)) z = z->L;
			else {
				splay (z);
				root = z;
				return;
			}
		}

		z = new node (data);
		z->P = p;

		if (!p) root = z;
		else if (comp (p->data, z->data)) p->R = z;
		else p->L = z;

		splay (z);
		root = z;
		p_size++;
	}

	node* find (const T &data) {
		node *z = root;
		if (!z) return 0;
		for (;;) {
			if (comp (z->data, data)) {
				if (!z->R) {
					splay (z);
					root = z;
					return nullptr;
				}
				z = z->R;
			} else if (comp (data, z->data)) {
				if (!z->L) {
					splay (z);
					root = z;
					return nullptr;
				}
				z = z->L;
			} else {
				splay (z);
				root = z;
				return z;
			}
		}
	}

	void erase (const T &key) {
		node *z = find (key);
		if (!z) return;
		//z is now root.
		if (!z->R) {
			root = z->L; //z was maximum
			if (root) root->P = nullptr;
		} else {
			node *y = subtree_minimum (z->R);
			splay (y);
			root = y;
			//y->L is now z (by definition of zigzag operations)
			//you can assert(y->L->R==nullptr)
			y->L = y->L->L;
			if (y->L) y->L->P = y;
		}

		delete z;
		p_size--;
	}

	bool empty() const {
		return root == nullptr;
	}

	unsigned long size() const {
		return p_size;
	}

	void clear() {
		while (root) {
			if (root->L) {
				right_rotate (root);
				root = root->P;
			} else {
				root = root->R;
				delete root->P;
				root->P = nullptr;
			}
		}
	}

	void sync() {
		/*
		 * This function will be called as a "flush", with an intention
		 * to remove inter-thread inconsistencies.
		 */
	}
};

#endif // splayset
