template<class T> struct persique {
	/*
		persistent deque 
		invented by babin74
	*/
	private:
	struct node_base {
		virtual size_t size();
		virtual const T& get_value(size_t i);
		virtual node_base* push_front(const T &x);
		virtual node_base* push_back(const T &x);
		virtual node_base* pop_front();
		virtual node_base* pop_back();
	};
	struct node_empty;
	struct node_one;
	
	struct node: node_base {
		node_base *p0, *p1;
		const size_t sz;
		size_t size() { return sz; }
		node(node_base *q0, node_base *q1): p0(q0), p1(q1), sz(p0->size() + p1->size()) {}
		const T& get_value(size_t i) {
			assert(i < sz);
			return i%2 ? p1->get_value(i>>1) : p0->get_value(i>>1);
		}
		node *push_front(const T &x) {
			return new node(p1->push_front(x), p0);
		}
		node *push_back(const T &x) {
			return sz % 2 ? new node(p0, p1->push_back(x)) : new node(p0->push_back(x), p1);
		}
		node_base* pop_front() {
			if(sz == 2) return p1;
			return new node(p1, p0->pop_front());
		};
		node_base* pop_back() {
			if(sz == 2) return p0;
			return sz % 2 ? new node(p0->pop_back(), p1) : new node(p0, p1->pop_back());
		}
	};
	
	struct node_one: node_base {
		const T value;
		node_one(const T &x): value(x) {}
		size_t size() { return 1; }
		const T& get_value(size_t) { return value; }
		node* push_front(const T &x) { return new node(new node_one(x), this); }
		node* push_back(const T &x) { return new node(this, new node_one(x)); }
		node_empty* pop_front() { return empty_node; }
		node_empty* pop_back() { return empty_node; }
	};
	
	struct node_empty: node_base {
		size_t size() { return 0; }
		const T& get_value(size_t) { assert(false); }
		node_one* push_front(const T &x) { return new node_one(x); }
		node_one* push_back(const T &x) { return new node_one(x); }
		node_base* pop_front() { assert(false); }
		node_base* pop_back() { assert(false); }
	};
	static inline node_empty *empty_node = new node_empty();
	
	node_base *p;
	persique(node_base *t): p(t) {}
	
	public:
	persique(): p(empty_node) {}
	
	size_t size() const { return p->size(); }
	bool empty() const { return p->size() == 0; }
	
	persique push_front(const T &x) const { return p->push_front(x); }
	persique push_back(const T &x) const { return p->push_back(x); }
	persique pop_front() const { return p->pop_front(); }
	persique pop_back() const { return p->pop_back(); }
	
	const T& operator[](size_t i) const { return p->get_value(i); }
	const T& front() const { return operator[](0); }
	const T& back() const { return operator[](size()-1); }
};
