template<typename T>
vector<T> tree_dp_root_each(
	const auto &g,
	function<T(size_t)> init,
	function<T(T, T)> f
) {
	using it = typename vector<T>::iterator;
	function<void(it,it,T)> exclude = [&f,&exclude](it l, it r, const T& val) {
		if(l==r) return ;
		if(l+1 == r) { *l = val; return ; }
		auto m = l + (r-l)/2;
		T fl = accumulate(l,m,val,f);
		T fr = accumulate(m,r,val,f);
		exclude(l, m, fr);
		exclude(m, r, fl);
	};
	
	size_t n = size(g);
	vector<T> result(n), subtree(n);
	vector<bool> used(n);
	
	function<void(size_t)> go_sub = [&](size_t v) {
		used[v] = true;
		subtree[v] = init(v);
		for(size_t i : g[v]) if(!used[i]) {
			go_sub(i);
			subtree[v] = f(subtree[v], subtree[i]);
		}
	};
	
	function<void(size_t,size_t,T)> go = [&](size_t v, size_t p, T up) {
		used[v] = true;
		T dv = init(v);
		result[v] = p==v ? dv : f(dv, up);
		vector<T> a;
		for(size_t i : g[v]) if(i!=p) a.push_back(subtree[i]);
		exclude(ALL(a), dv);
		auto it = begin(a);
		for(size_t i : g[v]) if(i!=p) {
			go(i, v, p==v ? *it : f(*it,up));
			++it;
			result[v] = f(result[v], subtree[i]);
		}
	};
	
	for(size_t i=0;i<n;++i) if(!used[i]) go_sub(i);
	used.flip();
	for(size_t i=0;i<n;++i) if(!used[i]) go(i,i,{});
	
	return result;
}