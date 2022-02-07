template<class T, bool well_formed = false>
struct segment_tree {
	explicit segment_tree(size_t n = 0): d(tree_size(n)), t(d*2) {}
	segment_tree(size_t n, auto gen): segment_tree(n) {
		for(size_t i=0; i<n; ++i) t[i+d] = gen(i);
		for(size_t i=d; i-->1;) t[i] = T(t[i*2], t[i*2+1]);
	}
	
	void set_value(size_t i, const T &value) {
		for(t[i+=d] = value; i>>=1; ) t[i] = T(t[i*2], t[i*2+1]);
	}
	
	T operator()(size_t l, size_t r) const {
		const static T neutral{};
		T fl = neutral, fr = neutral;
		for(l+=d,r+=d; l<r; l>>=1,r>>=1) {
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
	size_t d;
	vector<T> t;
	static size_t tree_size(size_t n) { return well_formed ? (n>1 ? tree_size((n+1)/2)*2 : 1) : n; }
};
/* implement:
	struct node {
		node()
		node(const node &vl, const node &vr)
	};
*/
