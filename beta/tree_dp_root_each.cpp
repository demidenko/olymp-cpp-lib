template<typename T>
vector<T> tree_dp_root_each(
	const graph &g,
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
	vector<size_t> par(n, -1);
	vector<bool> used(n);
	
	vector<T> result(n), subtree(n);
	function<void(size_t)> go_sub = [&](size_t v) {
		used[v] = true;
		subtree[v] = init(v);
		for(size_t i : g[v]) if(!used[i]) {
			par[i] = v;
			go_sub(i);
			subtree[v] = f(subtree[v], subtree[i]);
		}
	};
	
	function<void(size_t,T)> go = [&](size_t v, T up) {
		vector<T> a;
		for(size_t i : g[v]) if(i!=par[v]) a.push_back(subtree[i]);
		exclude(ALL(a), init(v));
		auto it = begin(a);
		for(size_t i : g[v]) if(i!=par[v]) {
			go(i, par[v]==-1 ? *it : f(*it,up));
			++it;
		}
		result[v] = par[v]==-1 ? subtree[v] : f(subtree[v], up);
	};
	
	for(size_t i=0;i<n;++i) if(!used[i]) {
		go_sub(i);
		go(i,{});
	}
	
	return result;
}
