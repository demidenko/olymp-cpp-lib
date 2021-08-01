template<class T, bool well_formed = false>
struct segment_tree {
	segment_tree(size_t n, function<T(size_t)> gen) {
		if constexpr (!well_formed) d = n;
		else for(d=1; d<n; d<<=1);
		t.assign(d*2, {});
		for(size_t i=0;i<n;++i) t[i+d] = gen(i);
		for(size_t i=d;i-->1;) t[i] = T(t[i*2], t[i*2+1]);
	}
	
	void set_value(size_t i, const T &value) {
		for(t[i+=d] = value; i>>=1; ) t[i] = T(t[i*2], t[i*2+1]);
	}
	
	T operator()(size_t l, size_t r) const {
		const static T neutral{};
		T fl = neutral, fr = neutral;
		for(l+=d,r+=d; l<r; l>>=1,r>>=1){
			if(l&1) fl = T(fl, t[l]), ++l;
			if(r&1) --r, fr = T(t[r], fr);
		}
		return T(fl, fr);
	}
	
	decltype(auto) operator()() const {
		if constexpr (well_formed) return t[1];
		else return operator()(0, d);
	}
	
	private:
	vector<T> t;
	size_t d;
};
/*
implement:
struct node {
	node()
	node(const node &l, const node &r)
};
*/