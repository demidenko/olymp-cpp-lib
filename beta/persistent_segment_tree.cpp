template<class T>
struct persistent_segment_tree {
	struct node {
		node *l, *r;
		size_t sz;
		T value;
		node(const T &val): value(val), sz(1), l(nullptr), r(nullptr) {}
		node(node *l, node *r): l(l), r(r), sz(l->sz+r->sz), value(l->value,r->value) {}
		node* set_value(size_t i, const T &x) {
			assert(i < sz);
			if(sz == 1) return new node(x);
			if(i < l->sz) return new node(l->set_value(i, x), r);
			return new node(l, r->set_value(i-l->sz, x));
		}
		T calc(size_t i, size_t j) {
			assert(0<=i && i<j && j<=sz);
			if(i==0 && j==sz) return value;
			size_t m = sz / 2;
			if(j<=m) return l->calc(i, j);
			if(i>=m) return r->calc(i-m, j-m);
			return T(l->calc(i, m), r->calc(0, j-m));
		}
	};
	
	T operator()(size_t l, size_t r) { return root->calc(l, r); };
	persistent_segment_tree set_value(size_t i, const T &value) { return root->set_value(i, value); }
	
	persistent_segment_tree(const vector<auto> &vals): root(build(begin(vals),end(vals))) {}
	explicit persistent_segment_tree(size_t n, const T &val = {}): persistent_segment_tree(vector(n,val)) {}
	
	private:
	node *root;
	persistent_segment_tree(node *root): root(root) {}
	node* build(auto from, auto to) {
		if(from >= to) return nullptr;
		if(from+1 == to) return new node(*from);
		auto mid = from + (to - from)/2;
		return new node(build(from,mid), build(mid,to));
	}
};
