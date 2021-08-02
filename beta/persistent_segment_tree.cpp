template<class T>
struct persistent_segment_tree {
	struct node {
		node *l, *r;
		T value;
		node(const T &value): l(nullptr), r(nullptr), value(value) {}
		node(node *l, node *r): l(l), r(r), value(l->value,r->value) {}
	};
	
	T operator()(size_t l, size_t r) { return calc(root, sz, l, r); };
	persistent_segment_tree set_value(size_t i, const T &value) { return {set_value(root, sz, i, value), sz}; }
	
	persistent_segment_tree(const vector<auto> &values): root(build(begin(values),end(values))), sz(size(values)) {}
	explicit persistent_segment_tree(size_t n=0, const T &value={}): persistent_segment_tree(vector(n,value)) {}
	
	private:
	node *root;
	size_t sz;
	persistent_segment_tree(node *root, size_t sz): root(root), sz(sz) {}
	node* build(auto from, auto to) {
		if(from >= to) return nullptr;
		if(from+1 == to) return new node(*from);
		auto mid = from + (to - from)/2;
		return new node(build(from,mid), build(mid,to));
	}
	node* set_value(node *t, size_t sz, size_t i, const T &value) {
		if(sz == 1) return new node(value);
		size_t m = sz / 2;
		if(i < m) return new node(set_value(t->l, m, i, value), t->r);
		return new node(t->l, set_value(t->r, sz-m, i-m, value));
	}
	T calc(node *t, size_t sz, size_t i, size_t j) {
		if(i==0 && j==sz) return t->value;
		size_t m = sz / 2;
		if(j<=m) return calc(t->l, m, i, j);
		if(i>=m) return calc(t->r, sz-m, i-m, j-m);
		return T(calc(t->l, m, i, m), calc(t->r, sz-m, 0, j-m));
	}
};
