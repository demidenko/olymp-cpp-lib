struct func_graph {
	func_graph(const std::ranges::sized_range auto &f = std::ranges::empty_view<size_t>{}) {
		const size_t n = size(f);
		if(n == 0) return ;
		
		const vector<size_t> p(begin(f), end(f));
		assert(*std::ranges::max_element(p) < n);
		
		size_t vn = 0;
		vs.resize(n);
		
		size_t gn = 0, g_none = -1;
		vector<size_t> g_id(n), g_par(n);
		
		cyc_pos.assign(n, {-1, -1});
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
						cycs.emplace_back(vn, end(path) - itv);
						for(auto it = itv; it != end(path); ++it) {
							size_t x = *it;
							vs[vn++] = x;
							cyc_pos[x] = {size(cycs) - 1, it - itv};
						}
						path.erase(itv, end(path));
					}
					break ;
				}
				used[v] = i;
			}
			for(size_t x : std::views::reverse(path)) {
				g_id[x] = gn++;
				g_par[g_id[x]] = on_cyc(p[x]) ? g_none : g_id[p[x]];
			}
		}
		
		assert(vn + gn == n);
		vector<vector<size_t>> g(gn + 1);
		std::ranges::replace(g_par, g_none, gn);
		for(size_t i=0; i<gn; ++i) g[g_par[i]].push_back(i);
		heavy_light_decomposition hld(g, gn);
		for(size_t i=0; i<n; ++i) if(!on_cyc(i)) vs[n - hld.index(g_id[i])] = i;
		vs_pos.resize(n);
		for(size_t i=0; i<n; ++i) vs_pos[vs[i]] = i;
		jump.resize(n);
		jump_len.resize(n);
		for(size_t i=n-1; i>=vn; --i) {
			size_t v = vs[i];
			if(vs_pos[p[v]] == i + 1) {
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
		vector<pair<size_t,size_t>> res;
		for(;;) {
			if(on_cyc(v)) {
				auto [ck, pos] = cyc_pos[v];
				auto [cl, clen] = cycs[ck];
				res.emplace_back(cl + pos, cl + clen);
				if(pos == 0) return res;
				v = vs[cl];
			} else {
				res.emplace_back(vs_pos[v], vs_pos[v] + jump_len[v]);
				v = jump[v];
			}
		}
	}
	
	size_t kth_jump(size_t i, std::unsigned_integral auto k) const {
		auto ranges = decompose(i);
		for(size_t p=0; p<size(ranges)-1; ++p) {
			auto [l, r] = ranges[p];
			if(r - l > k) return vs[l + k];
			k -= r - l;
		}
		auto [l, r] = ranges.back();
		k %= r - l;
		return vs[l + k];
	}
	
	private:
	vector<size_t> vs, vs_pos;
	vector<pair<size_t,size_t>> cycs, cyc_pos;
	vector<size_t> jump, jump_len;
	bool on_cyc(size_t v) const { return cyc_pos[v].first != -1; }
};
