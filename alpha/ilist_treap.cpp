template<class T>
struct ilist_treap {
	
	struct node {
		node *l, *r, *p;
		size_t sz, priority;
		T* value;
		node(const T &x): node(new T(x)) {}
		
		private:
		friend ilist_treap;
		node(T* ptr = nullptr): l(nullptr), r(nullptr), p(nullptr), sz(1), value(ptr) {
			static mt19937_64 gen(chrono::high_resolution_clock::now().time_since_epoch().count());
			priority = gen();
		}
		
		~node() { if(value != nullptr) delete value; }
	};
	
	struct iterator: public std::iterator<std::random_access_iterator_tag, T> {
		friend ilist_treap;
		private: node *t;
		public:
		iterator(node *ptr = nullptr): t(ptr) { }
		T& operator*() { return *t->value; }
		bool operator==(const iterator& it) { return t == it.t; }
		bool operator!=(const iterator& it) { return t != it.t; }
		
		size_t operator-(const iterator &it) const {
			return get_pos(t) - get_pos(it.t);
		}
		
		iterator& operator++() {
			if(t->r) t = leftmost(t->r);
			else t = right_parent(t);
			return *this;
		}
		
		iterator& operator--() {
			if(t->l) t = rightmost(t->l);
			else t = left_parent(t);
			return *this;
		}
		
		iterator& operator+=(size_t n) {
			while(n--) {
				if(size_t sr = sz(t->r); sr>n) {
					t = nth(t->r, n);
					break ;
				} else n-=sr;
				t = right_parent(t);
			}
			return *this;
		}
		
		iterator operator-=(size_t n) {
			while(n--) {
				if(size_t sl = sz(t->l); sl>n) {
					t = nth(t->l, sl-n-1);
					break ;
				} else n-=sl;
				t = left_parent(t);
			}
			return *this;
		}
		
		iterator operator+(size_t n) const { return iterator(t)+=n; }
		iterator operator-(size_t n) const { return iterator(t)-=n; }
	};
	
	
	ilist_treap() {
		root = __end = new node();
	}
	
	explicit ilist_treap(size_t n, const T &value = {}): ilist_treap() {
		while(n--) push_back(value);
	}
	
	ilist_treap(std::initializer_list<T> values): ilist_treap() { 
		for(auto&& x : values) push_back(x);
	}
	
	ilist_treap(const ilist_treap &a) = delete ;
	
	ilist_treap(ilist_treap &&a) = default ;
	
	ilist_treap& operator=(const ilist_treap &a) = delete ;
	
	ilist_treap& operator=(ilist_treap &&a) {
		root = exchange(a.root, nullptr);
		__end = exchange(a.__end, nullptr);
		return *this;
	}
	
	size_t size() const { return sz(root) - 1; }
	iterator begin() const { return iterator(leftmost(root)); }
	iterator end() const { return __end; }
	
	void clear() {
		clear(split(__end).first);
		root = __end;
	}
	
	~ilist_treap() { clear(root); }
	
	iterator at(size_t pos) {
		return iterator(nth(root, pos));
	}
	
	T& operator[](size_t pos) {
		assert(pos < size());
		return *at(pos);
	}
	
	void push_back(const T &x) { insert(__end, x); return ; }
	
	iterator insert(iterator it, const T &x) {
		return insert(it, new node(x));
	}
	
	iterator insert(iterator pos, ilist_treap &&a) {
		auto [v, aend] = split(a.__end);
		a.root = aend;
		return insert(pos, v);
	}
	
	iterator insert(iterator pos, iterator it) {
		return insert(pos, it.t);
	}
	
	ilist_treap erase(iterator first, iterator last) {
		auto [l, suf] = split(first.t);
		auto [mid, r] = split(last.t);
		merge(l, r, root);
		return ilist_treap(mid);
	}
	
	iterator erase(iterator it) {
		node *t = it.t, *p = t->p;
		assert(t != __end);
		node *&target = p ? ref_in_parent(t) : root;
		merge(t->l, t->r, target);
		if(target) target->p = p;
		for(node *v = p; v; v = v->p) upd_sz(v);
		t->p = t->l = t->r = nullptr;
		t->sz = 1;
		return iterator(t);
	}
	
	
	private:
	node *root, *__end;
	
	ilist_treap(node *v): ilist_treap() {
		merge(v, __end, root);
	}
	
	iterator insert(iterator it, node *v) {
		if(sz(v) == 1) return insert_one(it.t, v);
		auto res = iterator(leftmost(v));
		auto [l, r] = split(it.t);
		if(sz(l) < sz(r)) merge(l, v, l); else merge(v, r, r);
		merge(l, r, root);
		return res;
	}
	
	iterator insert_one(node *s, node *v) {
		if(s->priority < v->priority) {
			node *t = s;
			while(t->p && t->p->priority < v->priority) t = t->p;
			if(node *p = t->p; p == nullptr) root = v;
			else ref_in_parent(t) = v, v->p = p, t->p = nullptr;
			tie(v->l, v->r) = split(s);
		} else 
		if(s->l == nullptr) s->l = v, v->p = s;
		else {
			for(node *t = s->l; ; t = t->r){
				if(t->r == nullptr || t->r->priority < v->priority) {
					if(t->r) v->l = t->r, t->r->p = v;
					t->r = v;
					v->p = t;
					break ;
				}
			}
		}
		for(node *t = v; t; t = t->p) upd(t);
		return iterator(v);
	}
	
	static inline size_t sz(node *t) { return t ? t->sz : 0; }
	static inline node*& ref_in_parent(node *t) { return t->p->l == t ? t->p->l : t->p->r; }
	static void upd(node *t) {
		if(t == nullptr) return ;
		t->sz = 1;
		if(t->l) t->l->p = t, t->sz += t->l->sz;
		if(t->r) t->r->p = t, t->sz += t->r->sz;
	}
	static inline void upd_sz(node *t) {
		if(t) t->sz = (t->l ? t->l->sz : 0) + (t->r ? t->r->sz : 0) + 1;
	}
	
	static inline node* leftmost(node *t) {
		while(t->l) t = t->l;
		return t;
	}
	static inline node* rightmost(node *t) {
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
	
	static node* nth(node *v, size_t n) {
		assert(n<sz(v));
		for(;;) {
			size_t sl = sz(v->l);
			if(n == sl) return v;
			if(n < sl) v = v->l;
			else n-=sl+1, v = v->r;
		}
	}
	static size_t get_pos(node *t) {
		assert(t != nullptr);
		size_t pos = sz(t->l);
		while(t->p) {
			if(t->p->r == t) pos+=sz(t->p->l)+1;
			t = t->p;
		}
		return pos;
	}
	
	static void merge(node *l, node *r, node *&t) {
		if(l == nullptr) t = r; else
		if(r == nullptr) t = l; else {
			if(l->priority > r->priority) {
				merge(l->r, r, l->r);
				l->r->p = t = l;
			} else {
				merge(l, r->l, r->l);
				r->l->p = t = r;
			}
			upd_sz(t);
		}
	}
	
	//split by *s such right starts with *s
	static pair<node*,node*> split(node *s) {
		assert(s!=nullptr);
		node *l = s->l, *r = s;
		if(s->l) s->l->p = nullptr, s->l = nullptr;
		upd_sz(r);
		for(bool f = true; s->p; ) {
			bool cur = s->p->l == s;
			s = s->p;
			if(f != cur) {
				if(cur) s->l = r, l->p = nullptr;
				else s->r = l, r->p = nullptr;
				f = cur;
			} 
			if(cur) r = s; else l = s;
			upd(s);
		}
		return {l, r};
	}
	
	static void clear(node *v) {
		if(v == nullptr) return ;
		clear(v->l);
		clear(v->r);
		delete v;
	}
};
