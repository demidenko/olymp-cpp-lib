struct graph {
	graph(size_t n): g(n) {}
	void add_dir_edge(size_t from, size_t to) {
		g[from].push_back(to);
	}
	const vector<size_t> operator[](size_t i) const {
		return g[i];
	}
	size_t size() const { return g.size(); }
	private:
	vector<vector<size_t>> g;
};

vector<size_t> strong_connected_components(const graph &g) {
	size_t n = g.size(), tn = 0, cn = 0;
	vector<size_t> tin(n), h(n), comp(n), s;
	function<void(size_t)> css = [&](size_t v) {
		h[v] = tin[v] = ++tn;
		s.push_back(v);
		for(size_t i : g[v]){
			if(!tin[i]) css(i);
			if(tin[i] && !comp[i]) h[v] = min(h[v], h[i]);
		}
		if(h[v]==tin[v]) for(++cn; !comp[v]; s.pop_back()) comp[s.back()] = cn;
	};
	for(size_t i=0; i<n; ++i) if(!tin[i]) css(i);
	for(size_t &x : comp) x = cn-x;
	return comp;
};

struct sat2 {
	
	struct item {
		sat2 &s; size_t i; bool value;
		item(sat2& s, size_t i, bool value = true): s(s), i(i), value(value) {}
		void operator||(const item &other) {
			s.add_or(i, value, other.i, other.value);
		}
		item operator!() { return item(s, i, !value); }
	};
	
	item operator[](size_t i) { return item(*this, i); }
	
	sat2(size_t sz): n(sz), g(n*2) { }
	void add_or(size_t i, bool fi, size_t j, bool fj) {
		g.add_dir_edge(i+n*fi, j+n*!fj);
		g.add_dir_edge(j+n*fj, i+n*!fi);
	}
	bool solve(vector<bool> &res) {
		auto cp = strong_connected_components(g);
		res.resize(n);
		for(size_t i=0; i<n; ++i){
			size_t x = cp[i], y = cp[i+n];
			if(x == y) return false;
			res[i] = x > y;
		}
		return true;
	}
	private:
	const size_t n;
	graph g;
};