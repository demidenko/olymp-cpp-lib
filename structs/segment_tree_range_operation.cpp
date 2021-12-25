template<class T, class O>
struct segment_tree {
	explicit segment_tree(size_t n = 0, function<T(size_t)> gen = [](size_t){ return T{}; })
		: d(n), t(d*2, {{}, nullopt}) { if(n) build(0, d, n=1, gen); }
	
	void apply(size_t l, size_t r, const O &operation) {
		if(r > d) r = d;
		if(l < r) apply(l, r, operation, 0, d, 1);
	}
	
	void set_value(size_t i, const T &value) {
		assert(i < d);
		set_value(i, value, 0, d, 1);
	}
	
	T operator()(size_t l, size_t r) const {
		if(r > d) r = d;
		return l < r ? query(l, r, 0, d, 1) : T{};
	}
	
	const T& operator()() const { return t[1].first; }
	
	private:
	size_t d;
	mutable vector<pair<T,optional<O>>> t;
	
	void cover(size_t v, const O &operation, size_t length) const {
		t[v].first = T(t[v].first, operation, length);
		if(auto &op = t[v].second) op = O(*op, operation);
		else op = operation;
	}
	
	void push(size_t v, size_t length) const {
		if(auto &op = t[v].second) {
			size_t sl = length >> 1;
			auto&& [opl, opr] = op->split(sl, length - sl);
			cover(v+1, opl, sl);
			cover(v+sl*2, opr, length - sl);
			op.reset();
		}
	}
	
	void apply(size_t i, size_t j, const O &operation, size_t l, size_t r, size_t v) {
		if(i == l && j == r) { cover(v, operation, r-l); return ; }
		push(v, r-l);
		size_t m = (l+r) >> 1, vr = v + (m-l)*2;
		if(j <= m) apply(i, j, operation, l, m, v+1); else
		if(i >= m) apply(i, j, operation, m, r, vr); else {
			auto&& [opl, opr] = operation.split(m-i, j-m);
			apply(i, m, opl, l, m, v+1);
			apply(m, j, opr, m, r, vr);
		}
		t[v].first = T(t[v+1].first, t[vr].first);
	}
	
	void set_value(size_t i, const T &value, size_t l, size_t r, size_t v) {
		if(l+1 == r) { t[v].first = value; return ; }
		push(v, r-l);
		size_t m = (l+r) >> 1, vr = v + (m-l)*2;
		if(i < m) set_value(i, value, l, m, v+1);
		else set_value(i, value, m, r, vr);
		t[v].first = T(t[v+1].first, t[vr].first);
	}
	
	T query(size_t i, size_t j, size_t l, size_t r, size_t v) const {
		if(i == l && j == r) return t[v].first;
		push(v, r-l);
		size_t m = (l+r) >> 1, vr = v + (m-l)*2;
		if(j <= m) return query(i, j, l, m, v+1);
		if(i >= m) return query(i, j, m, r, vr);
		return T(query(i,m,l,m,v+1), query(m,j,m,r,vr));
	}
	
	void build(size_t l, size_t r, size_t &vn, auto gen) {
		size_t v = vn++, m = (l+r) >> 1;
		if(l+1 == r) { t[v].first = gen(l); return ; }
		build(l, m, vn, gen); build(m, r, vn, gen);
		t[v].first = T(t[v+1].first, t[v+(m-l)*2].first);
	}
};

/* implement:
	struct operation {
		operation(const operation &op, const operation &op2)
		auto split(size_t sl, size_t sr) const { return pair<const operation&,const operation&>{*this,*this}; }
	};
	struct node {
		node()
		node(const node &vl, const node &vr)
		node(const node &v, const operation &op, size_t length)
	};
*/
