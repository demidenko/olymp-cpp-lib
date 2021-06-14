template<typename ...T>
struct graph_t {
	using E = conditional_t<sizeof...(T), tuple<size_t,T...>, size_t>;
	graph_t(size_t n = 0): g(n) {}
	void add_dir_edge(size_t from, size_t to, const T&... args) {
		assert(from<size() && to<size());
		g[from].emplace_back(to, args...);
	}
	void add_edge(size_t x, size_t y, const T&... args) {
		add_dir_edge(x, y, args...);
		add_dir_edge(y, x, args...);
	}
	const vector<E>& operator[](size_t i) const { return g[i]; }
	size_t size() const { return g.size(); }
	private: vector<vector<E>> g;
};
using graph = graph_t<>;