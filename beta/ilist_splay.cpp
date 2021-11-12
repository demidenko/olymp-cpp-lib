template<class T>
struct ilist_splay {

	struct node {
		node *l, *r, *p;
		size_t sz;
		T *value;
		node(const T &x): node(new T(x)) {}

		private:
		friend ilist_splay;
		node(T *ptr = nullptr): l(nullptr), r(nullptr), p(nullptr), sz(1), value(ptr) { }
	};

	template<class V>
	struct node_iterator: public std::iterator<std::random_access_iterator_tag, V, size_t> {
		friend ilist_splay;
		private: node *t;
		node_iterator(node *ptr): t(ptr) { }
		public:
		node_iterator(): node_iterator(nullptr) { }
		V& operator*() { return *t->value; }
		bool operator==(const node_iterator &it) { return t == it.t; }
		bool operator!=(const node_iterator &it) { return t != it.t; }

		node_iterator& operator++() {
			splay(t);
			t = leftmost(t->r);
			return *this;
		}

		node_iterator& operator--() {
			splay(t);
			t = rightmost(t->l);
			return *this;
		}

		node_iterator& operator+=(size_t n) {
			if(n > 0) splay(t), t = nth(t->r, n-1);
			return *this;
		}

		node_iterator& operator-=(size_t n) {
			if(n > 0) splay(t), t = nth(t->l, sz(t->l)-n);
			return *this;
		}

		node_iterator operator+(size_t n) const { return node_iterator(t)+=n; }
		node_iterator operator-(size_t n) const { return node_iterator(t)-=n; }

		size_t operator-(const node_iterator &it) const {
			return get_pos(t) - get_pos(it.t);
		}
	};

	using iterator = node_iterator<T>;
	using const_iterator = node_iterator<const T>;

	struct extracted {
		private: friend ilist_splay;
		extracted(node *ptr): t(ptr) {}
		node *t;
	};


	ilist_splay(): __end(new node()) {}

	explicit ilist_splay(size_t n, const T &value = {}) {
		node* nodes = init_nodes(n);
		for(size_t i=0; i<n; ++i) *nodes[i].value = value;
	}

	template<class _InputIterator, class = std::_RequireInputIter<_InputIterator>>
	ilist_splay(_InputIterator first, _InputIterator last) {
		size_t n = std::distance(first, last);
		node* nodes = init_nodes(n);
		for(size_t i=0; i<n; ++i, ++first) *nodes[i].value = *first;
	}
	
	ilist_splay(extracted e): ilist_splay(e.t) {}
	
	ilist_splay(const ilist_splay &a) = delete ;
	
	ilist_splay(ilist_splay &&a) = default ;
	
	ilist_splay& operator=(const ilist_splay &a) = delete ;
	
	ilist_splay& operator=(ilist_splay &&a) {
		__end = exchange(a.__end, nullptr);
		return *this;
	}

	size_t size() const { return sz(splay(__end)) - 1; } //not O(1) !!!
	bool empty() const { return __end->p == nullptr && __end->l == nullptr; }

	void clear() { split(__end); }

	iterator begin() { return leftmost(splay(__end)); }
	iterator end() { return __end; }
	const_iterator begin() const { return leftmost(splay(__end)); }
	const_iterator end() const { return __end; }

	iterator at(size_t pos) { return nth(splay(__end), pos); }
	const_iterator at(size_t pos) const { return nth(splay(__end), pos); }

	T& operator[](size_t pos) { return *at(pos); }
	const T& operator[](size_t pos) const { return *at(pos); }

	void push_back(const T &x) { insert(__end, x); }

	iterator insert(iterator pos, const T &x) {
		return insert(pos, new node(x));
	}
	
	iterator insert(iterator pos, iterator it) {
		return insert(pos, it.t);
	}

	iterator insert(iterator pos, extracted e) {
		return insert(pos, e.t);
	}
	
	iterator insert(iterator pos, ilist_splay &&a) {
		return insert(pos, split(a.__end).first);
	}

	extracted extract(iterator first, iterator last) {
		auto [pref, r] = split(last.t);
		auto [l, mid] = split(first.t);
		set_left(r, l);
		upd_sz(r);
		return extracted(mid);
	}
	
	ilist_splay erase(iterator first, iterator last) {
		return ilist_splay(extract(first, last).t);
	}

	iterator erase(iterator it) {
		node *t = splay(it.t);
		node *l = t->l, *r = t->r;
		if(l) t->l = l->p = nullptr;
		if(r) t->r = r->p = nullptr;
		t->sz = 1;
		merge(l, r);
		return iterator(t);
	}
	
	
	private:
	node * __end;
	
	ilist_splay(node *v): ilist_splay() {
		set_left(__end, v);
		upd_sz(__end);
	}

	iterator insert(iterator it, node *v) {
		auto [l, r] = split(it.t);
		v = leftmost(v);
		set_left(v, l);
		upd_sz(v);
		set_left(r, v);
		upd_sz(r);
		return iterator(v);
	}


	static inline size_t sz(node *t) { return t ? t->sz : 0; }
	//static inline node*& ref_in_parent(node *t) { return t->p->l == t ? t->p->l : t->p->r; }
	static inline void set_left(node *v, node *to) { v->l = to; if(to) to->p = v; }
	static inline void set_right(node *v, node *to) { v->r = to; if(to) to->p = v; }
	static inline void upd_sz(node *t) { if(t) t->sz = sz(t->l) + sz(t->r) + 1; }

	static node* leftmost(node *t) {
		while(t->l) rotate_right(t), t = t->p;
		return t;
	}
	static node* rightmost(node *t) {
		while(t->r) rotate_left(t), t = t->p;
		return t;
	}

	static size_t get_pos(node *t) {
		splay(t);
		return sz(t->l);
	}

	static node* nth(node *v, size_t n) {
		assert(n < sz(v));
		for(;;) {
			size_t sl = sz(v->l);
			if(n == sl) return splay(v);
			if(n < sl) v = v->l;
			else n-=sl+1, v = v->r;
		}
	}

	static void upd_after_rotate(node *x, node *y, node *p) {
		if(p == nullptr) y->p = nullptr;
		else if(p->l == x) set_left(p, y);
		else set_right(p, y);
		upd_sz(x);
		upd_sz(y);
	}

	static void rotate_right(node *x) {
		node *p = x->p, *y = x->l, *a = y->r;
		set_left(x, a);
		set_right(y, x);
		upd_after_rotate(x, y, p);
	}

	static void rotate_left(node *x) {
		node *p = x->p, *y = x->r, *a = y->l;
		set_right(x, a);
		set_left(y, x);
		upd_after_rotate(x, y, p);
	}

	static node* splay(node *x) {
		while(x->p != nullptr) {
			node *p = x->p, *g = p->p;
			if(g == nullptr) {
				if(p->l == x) rotate_right(p);
				else rotate_left(p);
			} else 
			if(g->l == p) {
				if(p->l == x) rotate_right(g), rotate_right(p);
				else rotate_left(p), rotate_right(g);
			} else {
				if(p->l == x) rotate_right(p), rotate_left(g);
				else rotate_left(g), rotate_left(p);
			}
		}
		return x;
	}

	static pair<node*,node*> split(node *s) {
		splay(s);
		node *l = s->l;
		if(l != nullptr) {
			l->p = s->l = nullptr;
			upd_sz(s);
		}
		return {l, s};
	}

	static node* merge(node *l, node *r) {
		if(l == nullptr) return r;
		if(r == nullptr) return l;
		node *t = leftmost(r);
		set_left(t, l);
		upd_sz(t);
		return t;
	}

	node* init_nodes(size_t n) {
		node *nodes = new node[n+1];
		T *values = new T[n];
		for(size_t i=0; i<n; ++i) nodes[i].value = values + i;
		__end = nodes + n;
		build(nodes, __end + 1);
		return nodes;
	}

	static node* build(node *l, node *r) {
		size_t n = r - l;
		if(n == 0) return nullptr;
		node *mid = l + n/2;
		set_left(mid, build(l, mid));
		set_right(mid, build(mid+1, r));
		upd_sz(mid);
		return mid;
	}
};
