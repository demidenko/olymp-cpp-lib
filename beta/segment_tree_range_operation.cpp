template<class T, class O>
struct segment_tree {
	segment_tree(size_t n, function<T(size_t)> gen): d(n), t(d*2, {{}, nullopt}) {
		if(n) build(0, d, n=1, gen);
	}
	
	void apply(size_t l, size_t r, const O &operation) {
		if(r > d) r = d;
		if(l < r) apply(l, r, operation, 0, d, 1);
	}
	
	T operator()(size_t l, size_t r) const {
		if(r > d) r = d;
		return l < r ? query(l, r, 0, d, 1) : T{};
	}
	
	const T& operator()() const { return t[1].first; }
	
	private:
	const size_t d;
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
		if(i == l && j == r) {
			cover(v, operation, r-l);
			return ;
		}
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
	
	T query(size_t i, size_t j, size_t l, size_t r, size_t v) const {
		if(i == l && j == r) return t[v].first;
		push(v, r-l);
		size_t m = (l+r) >> 1, vr = v + (m-l)*2;
		if(j <= m) return query(i, j, l, m, v+1);
		if(i >= m) return query(i, j, m, r, vr);
		return T(query(i,m,l,m,v+1), query(m,j,m,r,vr));
	}
	
	size_t build(size_t l, size_t r, size_t &v, auto gen) {
		size_t cur = v++;
		if(l+1 == r) t[cur].first = gen(l); else {
			size_t m = (l+r) >> 1, vl = build(l, m, v, gen), vr = build(m, r, v, gen);
			t[cur].first = T(t[vl].first, t[vr].first);
		}
		return cur;
	}
};

/* implement:
	struct operation {
		operation()
		operation(const operation &a, const operation &b)
		auto split(size_t sl, size_t sr) const { return pair<const operation&,const operation&>{*this,*this}; }
	};
	struct node {
		node()
		node(const node &l, const node &r)
		node(const node &t, const operation &o, size_t length)
	}
*/
