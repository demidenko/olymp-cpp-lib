template<class T>
struct persistent_aggregator {
	struct node {
		node *l, *r;
		T value;
		node(const T &value): l(nullptr), r(nullptr), value(value) {}
		node(node *l, node *r): l(l), r(r), value(l->value,r->value) {}
	};
	
	persistent_aggregator(): persistent_aggregator(nullptr, 0) {}
	persistent_aggregator(size_t n, auto&& gen): persistent_aggregator(build(0, n, gen), n) {}
	
	T operator()(size_t l, size_t r) const { return l < r ? calc(root, sz, l, r) : T{}; };
	persistent_aggregator set_value(size_t i, const T &value) const { return {set_value(root, sz, i, value), sz}; }
	
	private:
	node *root;
	size_t sz;
	persistent_aggregator(node *root, size_t sz): root(root), sz(sz) {}
	node* build(auto l, auto r, auto &&gen) {
		if(l >= r) return nullptr;
		if(l+1 == r) return new node(gen(l));
		auto mid = l + (r - l)/2;
		return new node(build(l, mid, gen), build(mid, r, gen));
	}
	node* set_value(node *t, size_t sz, size_t i, const T &value) const {
		if(sz == 1) return new node(value);
		size_t m = sz / 2;
		if(i < m) return new node(set_value(t->l, m, i, value), t->r);
		return new node(t->l, set_value(t->r, sz-m, i-m, value));
	}
	T calc(node *t, size_t sz, size_t i, size_t j) const {
		if(i == 0 && j == sz) return t->value;
		size_t m = sz / 2;
		if(j <= m) return calc(t->l, m, i, j);
		if(i >= m) return calc(t->r, sz-m, i-m, j-m);
		return T(calc(t->l, m, i, m), calc(t->r, sz-m, 0, j-m));
	}
};
