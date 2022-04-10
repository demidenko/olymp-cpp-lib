template<class F, class C>
struct mcmf_graph {
	mcmf_graph(size_t n): mcmf_graph(n, 0, n-1) {}
	mcmf_graph(size_t n, size_t s, size_t t): g(n), S(s), T(t), p(n) {}
	
	void add_edge(size_t from, size_t to, F cap, C cost) {
		assert(0 <= from && from < size(g));
		g[from].push_back(size(edges));
		edges.push_back({to, 0, cap, cost});
		assert(0 <= to && to < size(g));
		g[to].push_back(size(edges));
		edges.push_back({from, 0, 0, -cost});
	}
	
	pair<F, C> push() { return push(numeric_limits<F>::max()); }
	pair<F, C> push(const F &flow_limit) {
		const size_t n = size(p);
		vector<size_t> fr(n, -1);
		vector<C> d(n, numeric_limits<C>::max());
		set<pair<C,size_t>> q{{d[S] = 0, S}};
		while(!empty(q)) {
			const size_t i = begin(q)->second;
			q.erase(begin(q));
			for(size_t k : g[i]) if(edges[k].rem() > 0) {
				size_t j = edges[k].to;
				C dist = d[i] + edges[k].cost + p[i] - p[j];
				if(dist < d[j]) {
					q.erase({d[j], j});
					q.emplace(d[j] = dist, j);
					fr[j] = k;
				}
			}
		}
		if(fr[T] == -1) return {0, 0};
		
		F flow = flow_limit;
		for(size_t v = T; v != S; v = edges[fr[v]^1].to) 
			flow = min(flow, edges[fr[v]].rem());
		
		C cost = 0;
		for(size_t v = T; v != S; ) {
			size_t k = fr[v];
			cost += edges[k].cost * C(flow);
			edges[k].flow += flow;
			edges[k^1].flow -= flow;
			v = edges[k^1].to;
		}
		
		for(size_t i=0; i<n; ++i) p[i] += d[i];
		
		return {1, cost};
	}
	
	private:
	vector<vector<size_t>> g;
	size_t S, T;
	vector<C> p;
	struct edge {
		size_t to;
		F flow, cap;
		C cost;
		F rem() { return cap - flow; }
	};
	vector<edge> edges;
};
