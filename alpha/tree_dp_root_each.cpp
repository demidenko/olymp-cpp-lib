template<typename T>
vector<T> tree_dp_root_each(
	const auto &g,
	function<T(size_t)> init,
	function<T(T, T)> f
) {
	size_t n = size(g);
	vector<T> result(n), sub(n);
	
	function<void(size_t,size_t)> go_sub = [&](size_t v, size_t p) {
		sub[v] = init(v);
		for(size_t i : g[v]) if(i!=p) {
			go_sub(i, v);
			sub[v] = f(sub[v], sub[i]);
		}
	};
	
	using it = typename vector<T>::iterator;
	function<void(it,it,T)> exclude = [&](it l, it r, const T& val) {
		if(l==r) return ;
		if(l+1 == r) { *l = val; return ; }
		auto m = l + (r-l)/2;
		T fl = accumulate(l,m,val,f);
		T fr = accumulate(m,r,val,f);
		exclude(l, m, fr);
		exclude(m, r, fl);
	};
	
	function<void(size_t,size_t,T)> go = [&](size_t v, size_t p, T up) {
		T dv = init(v);
		result[v] = p==v ? dv : f(dv, up);
		vector<T> a;
		for(size_t i : g[v]) if(i!=p) a.push_back(sub[i]);
		exclude(ALL(a), dv);
		auto it = begin(a);
		for(size_t i : g[v]) if(i!=p) {
			go(i, v, p==v ? *it : f(*it,up));
			result[v] = f(result[v], sub[i]);
			++it;
		}
	};
	
	go_sub(0,0);
	go(0,0,{});
	
	return result;
}
