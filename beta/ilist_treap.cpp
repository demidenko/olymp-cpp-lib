template<class T>
struct ilist_treap {
	
	struct node {
		private: friend ilist_treap;
		node *l, *r, *p;
		const size_t priority;
		size_t sz;
		T *value;
		node(const T &x): node(new T(x)) {}
		node(T *ptr = nullptr): l(nullptr), r(nullptr), p(nullptr), priority(gen()), sz(1), value(ptr) { }
		static inline auto gen = mt19937_64(chrono::high_resolution_clock::now().time_since_epoch().count());
	};
	
	template<class V>
	struct node_iterator: public std::iterator<std::random_access_iterator_tag, V, size_t> {
		private: friend ilist_treap;
		node *t;
		node_iterator(node *ptr): t(ptr) { }
		public:
		node_iterator(): node_iterator(nullptr) { }
		V& operator*() { return *t->value; }
		bool operator==(const node_iterator &it) { return t == it.t; }
		bool operator!=(const node_iterator &it) { return t != it.t; }
		
		node_iterator& operator++() {
			t = t->r ? leftmost(t->r) : right_parent(t);
			return *this;
		}
		
		node_iterator& operator--() {
			t = t->l ? rightmost(t->l) : left_parent(t);
			return *this;
		}
		
		node_iterator& operator+=(size_t n) {
			for(; n--; t = right_parent(t))
				if(size_t sr = sz(t->r); n < sr) {
					t = nth(t->r, n);
					break ;
				} else n-=sr;
			return *this;
		}
		
		node_iterator& operator-=(size_t n) {
			for(; n--; t = left_parent(t))
				if(size_t sl = sz(t->l); n < sl) {
					t = nth(t->l, sl-n-1);
					break ;
				} else n-=sl;
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
		private: friend ilist_treap;
		extracted(node *ptr): t(ptr) {}
		node *t;
	};
	
	
	ilist_treap() {
		root = __end = new node();
	}
	
	explicit ilist_treap(size_t n, const T &value = {}) {
		node* nodes = init_nodes(n);
		for(size_t i=0; i<n; ++i) *nodes[i].value = value;
	}
	
	template<class _InputIterator, class = std::_RequireInputIter<_InputIterator>>
	ilist_treap(_InputIterator first, _InputIterator last) {
		size_t n = std::distance(first, last);
		node* nodes = init_nodes(n);
		for(size_t i=0; i<n; ++i, ++first) *nodes[i].value = *first;
	}
	
	ilist_treap(extracted e): ilist_treap(e.t) {}
	
	ilist_treap(const ilist_treap &a) = delete ;
	
	ilist_treap(ilist_treap &&a): root(exchange(a.root, nullptr)), __end(exchange(a.__end, nullptr)) {}
	
	ilist_treap& operator=(const ilist_treap &a) = delete ;
	ilist_treap& operator=(ilist_treap &&a) = default ;
	
	size_t size() const { return sz(root) - 1; }
	bool empty() const { return sz(root) == 1; }
	
	void clear() {
		split(__end);
		root = __end;
	}
	
	iterator begin() { return leftmost(root); }
	iterator end() { return __end; }
	const_iterator begin() const { return leftmost(root); }
	const_iterator end() const { return __end; }
	
	iterator at(size_t pos) { return nth(root, pos); }
	const_iterator at(size_t pos) const { return nth(root, pos); }
	
	T& operator[](size_t pos) { return *at(pos); }
	const T& operator[](size_t pos) const { return *at(pos); }
	
	void push_back(const T &x) { insert(__end, x); }
	iterator insert(iterator pos, const T &x) { return insert(pos, new node(x)); }
	iterator insert(iterator pos, iterator it) { return insert(pos, it.t); }
	iterator insert(iterator pos, extracted e) { return insert(pos, e.t); }
	iterator insert(iterator pos, ilist_treap &&a) {
		auto [v, aend] = split(a.__end);
		a.root = aend;
		return insert(pos, v);
	}
	
	extracted extract(iterator first, iterator last) {
		auto [l, suf] = split(first.t);
		auto [mid, r] = split(last.t);
		root = merge(l, r);
		return extracted(mid);
	}
	
	ilist_treap erase(iterator first, iterator last) {
		return ilist_treap(extract(first, last).t);
	}
	
	iterator erase(iterator it) {
		node *t = it.t, *p = t->p;
		assert(t != __end);
		node *&target = p ? ref_in_parent(t) : root;
		if(target = merge(t->l, t->r)) target->p = p;
		for(node *v = p; v; v = v->p) upd_sz(v);
		t->p = t->l = t->r = nullptr;
		t->sz = 1;
		return iterator(t);
	}
	
	private:
	node *root, *__end;
	
	ilist_treap(node *v): ilist_treap() {
		root = merge(v, __end);
	}
	
	node* init_nodes(size_t n) {
		node *nodes = new node[n+1];
		T *values = new T[n];
		for(size_t i=0; i<n; ++i) nodes[i].value = values + i;
		__end = nodes + n;
		root = build(nodes, __end + 1);
		return nodes;
	}
	
	iterator insert(iterator it, node *v) {
		if(sz(v) == 1) return insert_one(it.t, v);
		auto res = iterator(leftmost(v));
		auto [l, r] = split(it.t);
		if(sz(l) < sz(r)) l = merge(l, v); else r = merge(v, r);
		root = merge(l, r);
		return res;
	}
	
	iterator insert_one(node *s, node *v) {
		if(node *t = s; t->priority < v->priority) {
			while(t->p && t->p->priority < v->priority) t = t->p;
			if(node *p = t->p; p == nullptr) root = v;
			else ref_in_parent(t) = v, v->p = p, t->p = nullptr;
			auto [sl, sr] = split(s);
			set_left(v, sl);
			set_right(v, sr);
		} else 
		if(s->l == nullptr) set_left(s, v);
		else {
			for(t = s->l; t->r && t->r->priority > v->priority; t = t->r) ;
			set_left(v, t->r);
			set_right(t, v);
		}
		for(node *t = v; t; t = t->p) upd_sz(t);
		return iterator(v);
	}
	
	static inline size_t sz(node *t) { return t ? t->sz : 0; }
	static inline node*& ref_in_parent(node *t) { return t->p->l == t ? t->p->l : t->p->r; }
	static inline void set_left(node *v, node *to) { v->l = to; if(to) to->p = v; }
	static inline void set_right(node *v, node *to) { v->r = to; if(to) to->p = v; }
	static inline void upd_sz(node *t) { if(t) t->sz = sz(t->l) + sz(t->r) + 1; }
	
	static node* leftmost(node *t) {
		while(t->l) t = t->l;
		return t;
	}
	
	static node* rightmost(node *t) {
		while(t->r) t = t->r;
		return t;
	}
	
	static inline node* left_parent(node *t) {
		while(t->p->l == t) t = t->p;
		return t->p;
	}
	
	static inline node* right_parent(node *t) {
		while(t->p->r == t) t = t->p;
		return t->p;
	}
	
	static size_t get_pos(node *t) {
		size_t pos = sz(t->l);
		for(; t->p; t = t->p) if(t->p->r == t) pos += sz(t->p->l)+1;
		return pos;
	}
	
	static node* nth(node *v, size_t n) {
		assert(n < sz(v));
		for(;;) {
			size_t sl = sz(v->l);
			if(n == sl) return v;
			if(n < sl) v = v->l;
			else n -= sl+1, v = v->r;
		}
	}
	
	static node* merge(node *l, node *r) {
		if(l == nullptr) return r;
		if(r == nullptr) return l;
		if(l->priority > r->priority) {
			l->sz += r->sz;
			set_right(l, merge(l->r, r));
			return l;
		} else {
			r->sz += l->sz;
			set_left(r, merge(l, r->l));
			return r;
		}
	}
	
	//split by *s such right starts with *s
	static pair<node*,node*> split(node *s) {
		node *l = s->l, *r = s;
		if(s->l) s->l->p = nullptr, s->l = nullptr;
		upd_sz(r);
		for(bool f = true; s->p; ) {
			bool cur = s->p->l == s;
			s = s->p;
			if(f != cur) {
				if(cur) set_left(s, r), l->p = nullptr;
				else set_right(s, l), r->p = nullptr;
				f = cur;
			} 
			if(cur) r = s; else l = s;
			upd_sz(s);
		}
		return {l, r};
	}
	
	static node* build(node *l, node *r) {
		if(l == r) return nullptr;
		for(node *t = l++; ; ++l) if(l == r || t->priority < l->priority) {
			set_right(t, build(t+1, l));
			upd_sz(t);
			if(l == r) return t;
			set_left(l, t);
			t = l;
		}
	}
};
//template<class T> using ilist = ilist_treap<T>;
