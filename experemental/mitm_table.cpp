template<typename T, T(*f)(const T&, const T&), T (*get_neutral)() >
struct mitm_table {
	vector<vector<T>> t;
	
	template<typename C>
	mitm_table(const vector<C> &vals) {
		size_t n = size(vals), L = 1;
		while((1<<L) < n) ++L;
		t.resize(L);
		
		size_t d = 1<<L;
		
		T neutral = get_neutral();
		auto &v = t[0];
		v.assign(d, neutral);
		copy(begin(vals), end(vals), begin(v));
		
		for(size_t p=4, k=1; k<L; ++k, p<<=1){
			auto &cur = t[k];
			cur.assign(begin(v), end(v));
			for(size_t l = 0; l<d; l+=p){
				size_t mid = l+(p>>1);
				cur[mid-1] = v[mid-1];
				for(size_t i=mid-1; i>l; --i) cur[i-1] = f(v[i-1], cur[i]);
				cur[mid] = v[mid];
				for(size_t i=mid; i+1<l+p; ++i) cur[i+1] = f(cur[i], v[i+1]);
			}
		}
	}
	
	
	T operator()(size_t l, size_t r){
		const static T neutral = get_neutral();
		if(l>=r) return neutral;
		--r;
		if(l == r) return t[0][l];
		int h = 31 - __builtin_clz(l^r);
		return f(t[h][l], t[h][r]);
	}
};