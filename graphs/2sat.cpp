struct sat2 {
	//sat2 a(n); a[i] || !a[j];
	struct item {
		sat2 &s; size_t i; bool value;
		void operator||(const item &other) {
			s.add_or(i, value, other.i, other.value);
		}
		item operator!() { return {s, i, !value}; }
	};
	item operator[](size_t i) { return {*this, i, true}; }
	
	sat2(size_t sz): n(sz), g(n*2) { }
	void add_or(size_t i, bool fi, size_t j, bool fj) {
		g.add_dir_edge(i+n*fi, j+n*!fj);
		g.add_dir_edge(j+n*fj, i+n*!fi);
	}
	optional<vector<bool>> solve() {
		vector<bool> res(n);
		auto cp = strong_connected_components(g);
		for(size_t i=0; i<n; ++i){
			size_t x = cp[i], y = cp[i+n];
			if(x == y) return nullopt;
			res[i] = x > y;
		}
		return res;
	}
	private:
	const size_t n;
	graph g;
};
