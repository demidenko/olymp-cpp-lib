template<typename T>
vector<T> tree_dp_root_each(
	const graph &g,
	function<T(size_t)> single,
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
	vector<bool> used(n);
	vector<size_t> par(n, -1);
	
	vector<T> res(n), up(n);
	
	auto calc = [&](const size_t root) {
		vector<size_t> q = {root};
		for(size_t k=0; k<size(q); ++k) {
			size_t v = q[k];
			used[v] = true;
			res[v] = single(v);
			for(size_t i : g[v]) if(!used[i]) {
				par[i] = v;
				q.push_back(i);
			}
		}
		
		for(size_t k=size(q)-1; k>0; --k) {
			size_t v = q[k];
			res[par[v]] = f(res[par[v]], res[v]);
		}
		
		for(size_t v : q) {
			vector<T> a;
			for(size_t i : g[v]) if(i!=par[v]) a.push_back(res[i]);
			exclude(ALL(a), single(v));
			auto it = begin(a);
			for(size_t i : g[v]) if(i!=par[v]) {
				up[i] = par[v]==-1 ? *it : f(*it,up[v]);
				res[i] = f(res[i], up[i]);
				++it;
			}
		}
	};
	
	for(size_t i=0;i<n;++i) if(!used[i]) calc(i);
	
	return res;
}
