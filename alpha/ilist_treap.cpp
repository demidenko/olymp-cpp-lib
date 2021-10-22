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
				if(size_t sr = sz(t->l); sr>n) {
					t = nth(t->l, sr-n-1);
					break ;
				} else n-=sr;
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
	
	ilist_treap(size_t n, const T &value = {}): ilist_treap() {
		for(; n>0; --n) push_back(value);
	}
	
	ilist_treap(std::initializer_list<T> values): ilist_treap() { 
		for(auto&& x : values) push_back(x);
	}
	
	ilist_treap(const ilist_treap &a) = delete ;
	
	ilist_treap(ilist_treap &&a) = default ;
	
	ilist_treap& operator=(const ilist_treap &a) {
		clear();
		root = copy(a.root);
		__end = rightmost(root);
		return *this;
	}
	
	ilist_treap& operator=(ilist_treap &&a) {
		root = exchange(a.root, nullptr);
		__end = exchange(a.__end, nullptr);
		return *this;
	}
	
	size_t size() const { return sz(root) - 1; }
	iterator begin() const { return iterator(leftmost(root)); }
	iterator end() const { return __end; }
	
	void clear() {
		clear(split(root, __end).first);
		root = __end;
	}
	
	~ilist_treap() { clear(root); }
	
	iterator at(size_t pos) const {
		return iterator(nth(root, pos));
	}
	
	T& operator[](size_t pos) const {
		assert(pos < size());
		return *at(pos);
	}
	
	iterator insert(iterator it, const T &x) {
		return insert(it, new node(x));
	}
	
	void push_back(const T &x) { insert(__end, x); }
	
	iterator insert(iterator it, ilist_treap &&a) {
		auto [v, aend] = split(a.root, a.__end);
		a.root = aend;
		return insert(it, v);
	}
	
	ilist_treap erase(iterator first, iterator last) {
		auto [l, suf] = split(root, first.t);
		auto [mid, r] = split(suf, last.t);
		merge(l, r, root);
		return ilist_treap(mid);
	}
	
	
	private:
	node *root, *__end;
	
	ilist_treap(node *v): ilist_treap() {
		merge(v, __end, root);
	}
	
	iterator insert(iterator it, node *v) {
		auto res = iterator(leftmost(v));
		auto [l, r] = split(root, it.t);
		merge(l, v, l);
		merge(l, r, root);
		return res;
	}
	
	static inline size_t sz(node *t) { return t ? t->sz : 0; }
	static void upd(node *t) {
		if(t == nullptr) return ;
		t->sz = 1;
		if(t->l) t->l->p = t, t->sz += t->l->sz;
		if(t->r) t->r->p = t, t->sz += t->r->sz;
	}
	
	static inline node* leftmost(node *t) {
		while(t->l) t = t->l;
		return t;
	}
	static inline node* rightmost(node *t) {
		while(t->r) t = t->r;
		return t;
	}
	static node* left_parent(node *t) {
		while(t->p->l == t) t = t->p;
		return t->p;
	}
	static node* right_parent(node *t) {
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
				t = l;
			} else {
				merge(l, r->l, r->l);
				t = r;
			}
			upd(t);
		}
	}
	
	//split *t by *s such right starts with *s
	static pair<node*,node*> split(node *t, node *s) {
		assert(s!=nullptr);
		node *l = s->l, *r = s;
		if(s->l) s->l->p = nullptr, s->l = nullptr;
		upd(r);
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
	
	static node* copy(node *v) {
		if(v == nullptr) return nullptr;
		node *res = v->value ? new node(*v->value) : new node();
		res->l = copy(v->l);
		res->r = copy(v->r);
		upd(res);
		return res;
	}
};
