template<class T>
struct ilist_splay {
	
	struct node {
		private: friend ilist_splay;
		node *l, *r, *p;
		size_t sz;
		T *value;
		node(T *ptr = nullptr): l(nullptr), r(nullptr), p(nullptr), sz(1), value(ptr) { }
	};
	
	template<class V>
	struct node_iterator: public std::iterator<std::random_access_iterator_tag, V, ptrdiff_t> {
		private: friend ilist_splay; node *t;
		node_iterator(node *ptr): t(ptr) { }
		public: node_iterator(): node_iterator(nullptr) { }
		V& operator*() { return *t->value; }
		bool operator==(const node_iterator &it) const { return t == it.t; }
		bool operator!=(const node_iterator &it) const { return t != it.t; }
		bool operator<(const node_iterator &it) const { return position(t) < position(it.t); }
		
		node_iterator& operator++() {
			for(;; rotate_big(t)) {
				if(t->r) return t = leftmost(t->r), *this;
				if(t->p->l == t) return t = t->p, *this;
			}
		}
		
		node_iterator& operator--() {
			for(;; rotate_big(t)) {
				if(t->l) return t = rightmost(t->l), *this;
				if(t->p->r == t) return t = t->p, *this;
			}
		}
		
		node_iterator operator++(int) { node* r = t; ++*this; return r; }
		node_iterator operator--(int) { node* r = t; --*this; return r; }
		
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
		
		ptrdiff_t operator-(const node_iterator &it) const {
			return position(t) - position(it.t);
		}
		
		ilist_splay& get_ilist_ref() {
			return *((ilist_splay<T>*)(rightmost(splay(t))->value));
		}
	};
	
	using iterator = node_iterator<T>;
	using const_iterator = node_iterator<const T>;
	
	struct extracted {
		private: friend ilist_splay;
		extracted(node *ptr): t(ptr) {}
		node *t;
	};
	
	
	ilist_splay(): __end(make_end_node(new_node(),this)), __size(0) {}
	ilist_splay(extracted e): ilist_splay(e.t) {}
	explicit ilist_splay(size_t n, const T &value = {}): ilist_splay() { resize(n, value); }
	
	template<class I, class = enable_if_t<is_convertible_v<typename iterator_traits<I>::iterator_category, bidirectional_iterator_tag>>>
	ilist_splay(I first, I last): ilist_splay() { assign(first, last); }
	
	ilist_splay(ilist_splay &&a) noexcept { *this = std::move(a); }
	ilist_splay& operator=(ilist_splay &&a) noexcept {
		__end = make_end_node(exchange(a.__end, nullptr), this);
		__size = a.__size;
		return *this;
	}
	
	ilist_splay(const ilist_splay &a): ilist_splay() { *this = a; }
	ilist_splay& operator=(const ilist_splay &a) {
		size_t n = a.__size; resize(n);
		for(auto v = __end, i = a.__end; n--; ) move_prev(v), move_prev(i), *v->value = *i->value;
		return *this;
	}
	
	size_t size() const { return __size; }
	bool empty() const { return __size == 0; }
	
	iterator begin() { return leftmost(splay(__end)); }
	iterator end() { return __end; }
	const_iterator begin() const { return leftmost(splay(__end)); }
	const_iterator end() const { return __end; }
	
	iterator at(size_t pos) { return nth(splay(__end), pos); }
	const_iterator at(size_t pos) const { return nth(splay(__end), pos); }
	
	T& operator[](size_t pos) { return *at(pos); }
	const T& operator[](size_t pos) const { return *at(pos); }
	
	void clear() { resize(0); }
	void resize(size_t n) { if(n > __size) resize_more(n, T{}); else if(n < __size) resize_less(n); }
	void resize(size_t n, const T &value) { if(n > __size) resize_more(n, value); else if(n < __size) resize_less(n); }
	
	auto assign(auto first, auto last) -> decltype(*--last, void()) {
		resize(std::distance(first, last));
		for(node *v = __end; first != last; ) move_prev(v), *v->value = *--last;
	}
	void assign(size_t n, const T &value) {
		for(auto [v, i] = pair(__end, __size); i--; ) move_prev(v), *v->value = value;
		resize(n, value);
	}
	
	void push_back(const T &x) { insert(__end, x); }
	iterator insert(iterator pos, const T &x) { return insert(pos, new_node(x)); }
	iterator insert(iterator pos, iterator it) { return insert(pos, it.t); }
	iterator insert(iterator pos, extracted e) { return e.t ? insert(pos, splay(e.t)) : pos; }
	iterator insert(iterator pos, ilist_splay &&a) { return insert(pos, a.extract(a.begin(),a.end())); }
	
	extracted extract(iterator first, iterator last) {
		auto [l, suf] = split(first.t);
		auto [mid, r] = split(last.t);
		set_left(r, l);
		upd_sz(r);
		__size -= sz(mid);
		return extracted(mid);
	}
	
	iterator erase(iterator it) {
		assert(it.t != __end);
		node *t = splay(it.t), *l = t->l, *r = t->r;
		t->r = t->l = t->p = r->p = nullptr;
		upd_sz(t);
		if(l) {
			r = leftmost(r);
			set_left(r, l);
			upd_sz(r);
		}
		--__size;
		return it;
	}
	
	void remove(iterator it) { remove_node(erase(it).t); }
	void remove(iterator first, iterator last) { ilist_splay(extract(first, last)).clear(); }
	ilist_splay erase(iterator first, iterator last) { return extract(first, last); }
	
	iterator partition_point(auto pred) {
		auto last_false = __end;
		for(node* v = splay(__end)->l, *nxt; v; v = nxt) {
			if(pred(std::as_const(*v->value))) nxt = v->r;
			else last_false = v, nxt = v->l;
			if(nxt == nullptr) splay(v);
		}
		return last_false;
	}
	
	private:
	node * __end;
	size_t __size;
	
	ilist_splay(node *v): ilist_splay() {
		__size = sz(v);
		set_left(__end, v);
		upd_sz(__end);
	}
	
	iterator insert(iterator it, node *v) {
		if(v == nullptr) return it;
		__size += sz(v);
		auto [l, r] = split(it.t);
		v = leftmost(v);
		set_left(v, l);
		set_left(r, v);
		upd_sz(v, r);
		return iterator(v);
	}
	
	void resize_less(const size_t n) { assert(n <= __size);
		auto [v, t] = split(nth(splay(__end), n));
		for(auto [r, i] = pair(__end, __size-n); i--; ) move_prev(r), remove_node(r);
		set_left(splay(__end), v);
		upd_sz(__end);
		__size = n;
	}
	
	void resize_more(const size_t n, const T &value) { assert(n >= __size);
		node *v = leftmost(build(n - __size, value));
		set_left(v, split(__end).first);
		set_left(__end, v);
		upd_sz(v, __end);
		__size = n;
	}
	
	static inline size_t sz(node *t) { return t ? t->sz : 0; }
	static inline void set_left(node *v, node *to) { v->l = to; if(to) to->p = v; }
	static inline void set_right(node *v, node *to) { v->r = to; if(to) to->p = v; }
	static inline void upd_sz(auto... t) { ((t->sz = sz(t->l) + sz(t->r) + 1), ...); }
	
	static node* leftmost(node *t) {
		while(t->l) rotate_right(t), t = t->p;
		return t;
	}
	
	static node* rightmost(node *t) {
		while(t->r) rotate_left(t), t = t->p;
		return t;
	}
	
	static inline ptrdiff_t position(node *t) { return sz(splay(t)->l); }
	
	static node* nth(node *v, size_t n) {
		assert(n < sz(v));
		for(size_t sl; ; v = n<sl ? v->l : (n-=sl+1, v->r)) if(n == (sl=sz(v->l))) return splay(v);
	}
	
	static void move_prev(node *&v) {
		if(v->l) for(v = v->l; v->r; v = v->r);
		else { while(v->p->l == v) v = v->p; v = v->p; }
	}
	
	static inline void upd_after_rotate(node *x, node *y, node *p) {
		if(p) p->l == x ? set_left(p, y) : set_right(p, y); else y->p = nullptr;
		upd_sz(x, y);
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
	
	static void rotate_big(node *x) {
		auto p = x->p, g = p->p, gg = g->p;
		if(gg) gg->l == g ? set_left(gg, x) : set_right(gg, x); else x->p = nullptr;
		if(g->l == p) {
			if(p->l == x) set_left(g, p->r), set_left(p, x->r), set_right(p, g), set_right(x, p);
			else set_left(g, x->r), set_right(p, x->l), set_left(x, p), set_right(x, g);
		} else {
			if(p->l == x) set_right(g, x->l), set_left(p, x->r), set_right(x, p), set_left(x, g);
			else set_right(g, p->l), set_right(p, x->l), set_left(p, g), set_left(x, p);
		}
		upd_sz(g, p, x);
	}
	
	static node* splay(node *x) {
		while(node *p = x->p) p->p ? rotate_big(x) : p->l == x ? rotate_right(p) : rotate_left(p);
		return x;
	}
	
	static pair<node*,node*> split(node *s) {
		node *l = splay(s)->l;
		if(l) l->p = s->l = nullptr, upd_sz(s);
		return {l, s};
	}
	
	static node* build(size_t n, const T &value) {
		if(n-- == 0) return nullptr;
		node *v = new_node(value);
		set_left(v, build(n/2, value));
		set_right(v, build(n-n/2, value));
		upd_sz(v);
		return v;
	}
	
	static node* make_end_node(node *end, ilist_splay *list) { end->value = (T*)list; return end; }
	static node* new_node(const T &x) { auto t = new_node(); t->value = new_value(x); return t; }
	static node* new_node() { auto v = pointers_manager<node>::new_ptr(); *v = node(); return v; }
	static T* new_value(const T &x) { auto v = pointers_manager<T>::new_ptr(); *v = x; return v; }
	static void remove_node(node *v) {
		if(v->value) pointers_manager<T>::free_ptr(v->value);
		pointers_manager<node>::free_ptr(v);
	}
	
	template<class R, size_t pool_sz_ext = (1<<15)> struct pointers_manager {
		static inline vector<R*> ptr_pool = {};
		static R* new_ptr() {
			if(ptr_pool.empty()) {
				ptr_pool.resize(pool_sz_ext);
				R *ptrs = new R[pool_sz_ext];
				for(size_t i=pool_sz_ext; i--; ) ptr_pool[i] = ptrs + i;
			}
			R *ptr = ptr_pool.back();
			ptr_pool.pop_back();
			return ptr;
		}
		static void free_ptr(R *ptr) { ptr_pool.push_back(ptr); }
	};
	
	public: ~ilist_splay() { if(__end) { clear(); __end->value = nullptr; remove_node(__end); __end = nullptr; } }
};
//template<class T> using ilist = ilist_splay<T>;
