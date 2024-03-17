struct func_graph {
	func_graph(const std::ranges::range auto &f = std::ranges::empty_view<size_t>{}) {
		const vector<size_t> p(begin(f), end(f));
		const size_t n = size(p);
		if(n == 0) return ; //nothing bad just skip overhead
		assert(*std::ranges::max_element(p) < n);
		
		size_t vn = 0;
		vs_pos.resize(n);
		
		size_t gn = 0, g_none = -1;
		vector<size_t> g_id(n), g_par(n), ts;
		
		jump.resize(n);
		jump_len.resize(n);
		vector<bool> on_cyc(n, false);
		vector<size_t> used(n, n);
		for(size_t i=0; i<n; ++i) if(used[i] == n) {
			vector<size_t> path;
			for(size_t v=i; ; ) {
				used[v] = i;
				path.push_back(v);
				v = p[v];
				if(used[v] <= i) {
					if(used[v] == i) {
						const auto itv = find(begin(path), end(path), v);
						for(auto it = itv; it != end(path); ++it) {
							size_t x = *it;
							vs_pos[x] = vn++;
							jump[x] = v;
							jump_len[x] = end(path) - it;
							on_cyc[x] = true;
						}
						path.erase(itv, end(path));
					}
					break ;
				}
				used[v] = i;
			}
			for(size_t x : std::views::reverse(path)) {
				g_id[x] = gn++;
				g_par[g_id[x]] = on_cyc[p[x]] ? g_none : g_id[p[x]];
				ts.push_back(x);
			}
		}
		
		vector<vector<size_t>> g(gn + 1);
		std::ranges::replace(g_par, g_none, gn);
		for(size_t i=0; i<gn; ++i) g[g_par[i]].push_back(i);
		heavy_light_decomposition hld(g, gn);
		for(size_t i=0; i<n; ++i) if(!on_cyc[i]) vs_pos[i] = n - hld.index(g_id[i]);
		for(size_t v : ts) {
			if(index(p[v]) == index(v) + 1) {
				jump[v] = jump[p[v]];
				jump_len[v] = jump_len[p[v]] + 1;
			} else {
				jump[v] = p[v];
				jump_len[v] = 1;
			}
		}
	}
	
	size_t index(size_t v) const { return vs_pos[v]; }
	
	auto decompose(size_t v) const {
		vector<tuple<size_t,size_t,bool>> res;
		for(;; v = jump[v]) {
			size_t l = index(v);
			bool last = v == jump[v];
			res.emplace_back(l, l + jump_len[v], last);
			if(last) return res;
		}
	}
	
	private:
	vector<size_t> vs_pos, jump, jump_len;
};
