template<class F, class C>
struct mincost_flow_graph {
	mincost_flow_graph(size_t n): mincost_flow_graph(n, 0, n-1) {}
	mincost_flow_graph(size_t n, size_t s, size_t t): g(n), S(s), T(t), p(n) {}
	
	void add_edge(size_t from, size_t to, F cap, C cost) {
		assert(0 <= from && from < size(g));
		g[from].push_back(size(edges));
		edges.push_back({to, 0, cap, cost});
		assert(0 <= to && to < size(g));
		g[to].push_back(size(edges));
		edges.push_back({from, 0, 0, -cost});
	}
	
	struct result { F flow; C cost; };
	result push(F flow = numeric_limits<F>::max()) {
		vector<size_t> fr(size(p), -1);
		vector<C> d(size(p), numeric_limits<C>::max());
		priority_queue<pair<C,size_t>, vector<pair<C,size_t>>, greater<pair<C,size_t>>> q;
		q.emplace(d[S] = 0, S);
		while(!empty(q)) {
			auto [di, i] = q.top();
			q.pop();
			if(di > d[i]) continue ;
			for(size_t k : g[i]) if(edges[k].rem() > 0) {
				size_t j = edges[k].to;
				C dist = d[i] + edges[k].cost + p[i] - p[j];
				if(dist < d[j]) q.emplace(d[j] = dist, j), fr[j] = k;
			}
		}
		
		if(fr[T] == -1) return {0, 0};
		
		for(size_t k = fr[T]; k != -1; k = fr[edges[k^1].to])
			flow = std::min(flow, edges[k].rem());
		
		C cost = 0;
		for(size_t k = fr[T]; k != -1; k = fr[edges[k^1].to]) {
			cost += edges[k].cost * C(flow);
			edges[k].flow += flow;
			edges[k^1].flow -= flow;
		}
		
		for(size_t i = 0; i < size(p); ++i) p[i] += d[i];
		return {flow, cost};
	}
	
	optional<C> mincost(F flow) {
		C cost = 0;
		while(flow > 0) 
			if(auto [f, c] = push(flow); f == 0) return nullopt;
			else flow -= f, cost += c;
		return cost;
	}
	
	result mincost_maxflow() {
		F flow = 0;
		C cost = 0;
		for(;;) {
			result r = push();
			if(r.flow == 0) return {flow, cost};
			flow += r.flow;
			cost += r.cost;
		}
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
