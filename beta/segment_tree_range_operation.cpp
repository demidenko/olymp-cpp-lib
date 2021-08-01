template<class T, class O>
struct segment_tree {
	segment_tree(size_t n, function<T(size_t)> gen) {
		for(d=1; d<n; d<<=1);
		t.assign(d*2, {{}, nullopt});
		for(size_t i=0;i<n;++i) t[i+d].first = gen(i);
		for(size_t i=d;i-->1;) t[i].first = T(t[i*2].first, t[i*2+1].first);
	}
	
	void apply(size_t l, size_t r, const O &operation) {
		if(r>d) r = d;
		if(l<r) apply(l, r, operation, 0, d, 1);
	}
	
	T operator()(size_t l, size_t r) {
		if(r>d) r = d;
		if(l>=r) return {};
		return query(l, r, 0, d, 1);
	}
	
	const T& operator()() const { return t[1].first; }
	
	private:
	vector<pair<T,optional<O>>> t;
	size_t d;
	
	void cover(size_t v, const O &operation, size_t length) {
		t[v].first = T(t[v].first, operation, length);
		if(auto &op = t[v].second) op = O(*op, operation);
		else op = operation;
	}
	
	void push(size_t v, size_t length) {
		if(auto &op = t[v].second) {
			auto [opl, opr] = op->split(length/2, length/2);
			cover(v*2, opl, length/2);
			cover(v*2+1, opr, length/2);
			op.reset();
		}
	}
	
	void apply(size_t i, size_t j, const O &operation, size_t l, size_t r, size_t v) {
		if(i==l && j==r) {
			cover(v, operation, r-l);
			return ;
		}
		push(v, r-l);
		size_t m = (l+r)>>1;
		if(j<=m) apply(i, j, operation, l, m, v*2); else
		if(i>=m) apply(i, j, operation, m, r, v*2+1); else {
			auto [opl, opr] = operation.split(m-i, j-m);
			apply(i, m, opl, l, m, v*2);
			apply(m, j, opr, m, r, v*2+1);
		}
		t[v].first = T(t[v*2].first, t[v*2+1].first);
	}
	
	T query(size_t i, size_t j, size_t l, size_t r, size_t v) {
		if(i==l && j==r) return t[v].first;
		push(v, r-l);
		size_t m = (l+r)>>1;
		if(j<=m) return query(i, j, l, m, v*2);
		if(i>=m) return query(i, j, m, r, v*2+1);
		return T(query(i,m,l,m,v*2), query(m,j,m,r,v*2+1));
	}
};
/*
implement:
struct operation {
	operation(const operation &first, const operation &second)
	pair<operation, operation> split(size_t pref, size_t suf) const { return {*this, *this}; }
};
struct node {
	node()
	node(const node &l, const node &r)
	node(const node &v, const operation &o, size_t len)
};
*/
