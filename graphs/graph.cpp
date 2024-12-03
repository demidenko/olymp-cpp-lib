template<class... T> struct edge_t : public tuple<size_t,T...> {
	edge_t(auto&&... args): tuple<size_t,T...>(args...) {}
	operator size_t() const { return get<0>(*this); }
};
template<class... T> struct std::tuple_size<edge_t<T...>> : tuple_size<tuple<size_t,T...>> {};
template<size_t I, class... T> struct std::tuple_element<I,edge_t<T...>> : tuple_element<I,tuple<size_t,T...>> {};
#if __glibcxx_tuple_like
template<class... T> constexpr bool std::__is_tuple_like_v<edge_t<T...>> = true;	
#endif

template<class... T> struct graph_t {
	using E = conditional_t<sizeof...(T), edge_t<T...>, size_t>;
	explicit graph_t(size_t n): g(n) {}
	void add_dir_edge(size_t from, size_t to, const T&... args) {
		assert(from<size() && to<size());
		g[from].emplace_back(to, args...);
	}
	void add_edge(size_t i, size_t j, const T&... args) {
		add_dir_edge(i, j, args...);
		add_dir_edge(j, i, args...);
	}
	const vector<E>& operator[](size_t i) const { return g[i]; }
	size_t size() const { return g.size(); }
	private: vector<vector<E>> g;
};
using graph = graph_t<>;
