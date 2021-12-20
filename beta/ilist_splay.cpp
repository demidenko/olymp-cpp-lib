template<class T>
struct ilist_splay {

	struct node {
		private: friend ilist_splay;
		node *l, *r, *p;
		size_t sz;
		T *value;
		node(const T &x): node(new T(x)) {}
		node(T *ptr = nullptr): l(nullptr), r(nullptr), p(nullptr), sz(1), value(ptr) { }
	};

	template<class V>
	struct node_iterator: public std::iterator<std::random_access_iterator_tag, V, size_t> {
		private: friend ilist_splay;
		node *t;
		node_iterator(node *ptr): t(ptr) { }
		public:
		node_iterator(): node_iterator(nullptr) { }
		V& operator*() { return *t->value; }
		bool operator==(const node_iterator &it) { return t == it.t; }
		bool operator!=(const node_iterator &it) { return t != it.t; }

		node_iterator& operator++() {
			for(;; rotate_big(t, t->p, t->p->p)) {
				if(t->r) return t = leftmost(t->r), *this;
				if(t->p->l == t) return t = t->p, *this;
			}
		}

		node_iterator& operator--() {
			for(;; rotate_big(t, t->p, t->p->p)) {
				if(t->l) return t = rightmost(t->l), *this;
				if(t->p->r == t) return t = t->p, *this;
			}
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


	ilist_splay(): __end(new node()), __size(0) {}

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
	
	ilist_splay(ilist_splay &&a): __end(exchange(a.__end, nullptr)), __size(a.__size) {}
	
	ilist_splay& operator=(const ilist_splay &a) = delete ;
	ilist_splay& operator=(ilist_splay &&a) = default ;

	size_t size() const { return __size; }
	bool empty() const { return __size == 0; }

	void clear() {
		split(__end);
		__size = 0;
	}

	iterator begin() { return leftmost(splay(__end)); }
	iterator end() { return __end; }
	const_iterator begin() const { return leftmost(splay(__end)); }
	const_iterator end() const { return __end; }

	iterator at(size_t pos) { return nth(splay(__end), pos); }
	const_iterator at(size_t pos) const { return nth(splay(__end), pos); }

	T& operator[](size_t pos) { return *at(pos); }
	const T& operator[](size_t pos) const { return *at(pos); }

	void push_back(const T &x) { insert(__end, x); }
	iterator insert(iterator pos, const T &x) { return insert(pos, new node(x)); }
	iterator insert(iterator pos, iterator it) { return insert(pos, it.t); }
	iterator insert(iterator pos, extracted e) { return insert(pos, e.t); }
	iterator insert(iterator pos, ilist_splay &&a) { return insert(pos, split(a.__end).first); }

	extracted extract(iterator first, iterator last) {
		auto [pref, r] = split(last.t);
		auto [l, mid] = split(first.t);
		set_left(r, l);
		upd_sz(r);
		__size -= sz(mid);
		return extracted(mid);
	}
	
	ilist_splay erase(iterator first, iterator last) {
		return ilist_splay(extract(first, last).t);
	}

	iterator erase(iterator it) {
		node *t = splay(it.t);
		assert(t != __end);
		node *l = t->l, *r = t->r;
		t->sz = 1;
		t->r = t->l = r->p = nullptr;
		if(l) {
			r = leftmost(r);
			set_left(r, l);
			upd_sz(r);
		}
		--__size;
		return iterator(t);
	}
	
	private:
	node * __end;
	size_t __size;
	
	ilist_splay(node *v): ilist_splay() {
		__size = sz(v);
		set_left(__end, v);
		upd_sz(__end);
	}

	node* init_nodes(size_t n) {
		node *nodes = new node[n+1];
		T *values = new T[n];
		for(size_t i=0; i<n; ++i) nodes[i].value = values + i;
		__end = nodes + n;
		__size = n;
		build(nodes, __end + 1);
		return nodes;
	}

	iterator insert(iterator it, node *v) {
		__size += sz(v);
		auto [l, r] = split(it.t);
		v = leftmost(v);
		set_left(v, l);
		upd_sz(v);
		set_left(r, v);
		upd_sz(r);
		return iterator(v);
	}

	static inline size_t sz(node *t) { return t ? t->sz : 0; }
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
			else n -= sl+1, v = v->r;
		}
	}

	static void upd_after_rotate(node *x, node *y, node *p) {
		if(p) p->l == x ? set_left(p, y) : set_right(p, y); else y->p = nullptr;
		upd_sz(x);
		upd_sz(y);
	}

	static void rotate_left(node *x) {
		node *p = x->p, *r = x->r;
		set_right(x, r->l);
		set_left(r, x);
		upd_after_rotate(x, r, p);
	}

	static void rotate_right(node *x) {
		node *p = x->p, *l = x->l;
		set_left(x, l->r);
		set_right(l, x);
		upd_after_rotate(x, l, p);
	}
	
	static void rotate_big(node *x, node *p, node *g) {
		if(node *gg = g->p) gg->l == g ? set_left(gg, x) : set_right(gg, x); else x->p = nullptr;
		if(g->l == p) {
			if(p->l == x) set_left(g, p->r), set_left(p, x->r), set_right(p, g), set_right(x, p);
			else set_left(g, x->r), set_right(p, x->l), set_left(x, p), set_right(x, g);
		} else {
			if(p->l == x) set_right(g, x->l), set_left(p, x->r), set_right(x, p), set_left(x, g);
			else set_right(g, p->l), set_right(p, x->l), set_left(p, g), set_left(x, p);
		}
		upd_sz(g);
		upd_sz(p);
		upd_sz(x);
	}

	static node* splay(node *x) {
		while(x->p) {
			node *p = x->p, *g = p->p;
			if(g) rotate_big(x, p, g);
			else if(p->l == x) rotate_right(p);
			else rotate_left(p);
		}
		return x;
	}

	static pair<node*,node*> split(node *s) {
		node *l = splay(s)->l;
		if(l) {
			l->p = s->l = nullptr;
			upd_sz(s);
		}
		return {l, s};
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
//template<class T> using ilist = ilist_splay<T>;