struct binary_lifter {
	binary_lifter(const std::ranges::sized_range auto &f = std::ranges::empty_view<size_t>{}) {
		const size_t n = size(f);
		if(n == 0) return ;
		
		const vector<size_t> p(begin(f), end(f));
		assert(*std::ranges::max_element(p) < n);
		
		size_t vn = 0;
		vs.resize(n);
		
		size_t gn = 0, g_none = -1;
		vector<size_t> g_id(n), g_par(n);
		
		jumps_to_cyc.assign(n, 0);
		first_cyc_v.resize(n);
		cyc_pos.resize(n);
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
							first_cyc_v[x] = x;
							cyc_pos[x] = {size(cycs) - 1, it - itv};
						}
						path.erase(itv, end(path));
					}
					break ;
				}
				used[v] = i;
			}
			for(size_t x : std::views::reverse(path)) {
				first_cyc_v[x] = first_cyc_v[p[x]];
				jumps_to_cyc[x] = jumps_to_cyc[p[x]] + 1;
				g_id[x] = gn++;
				g_par[g_id[x]] = jumps_to_cyc[p[x]] == 0 ? g_none : g_id[p[x]];
			}
		}
		
		assert(vn + gn == n);
		vector<vector<size_t>> g(gn + 1);
		std::ranges::replace(g_par, g_none, gn);
		for(size_t i=0; i<gn; ++i) g[g_par[i]].push_back(i);
		heavy_light_decomposition hld(g, gn);
		for(size_t i=0; i<n; ++i) if(jumps_to_cyc[i] > 0) vs[n - hld.index(g_id[i])] = i;
		vs_pos.resize(n);
		for(size_t i=0; i<n; ++i) vs_pos[vs[i]] = i;
		jump.resize(n);
		for(size_t i=n-1; i>=vn; --i) {
			size_t v = vs[i];
			if(vs_pos[p[v]] == i + 1) jump[v] = jump[p[v]];
			else jump[v] = p[v];
		}
	}
	
	size_t index(size_t v) const { return vs_pos[v]; }
	
	size_t kth_jump(size_t i, std::unsigned_integral auto k) const {
		if(size_t d = jumps_to_cyc[i]; k < d) {
			for(;;) {
				size_t j = jump[i];
				size_t len = jumps_to_cyc[i] - jumps_to_cyc[j];
				if(k < len) return vs[vs_pos[i] + k];
				k -= len;
				i = j;
			}
		} else {
			auto [ck, pos] = cyc_pos[first_cyc_v[i]];
			auto [cl, clen] = cycs[ck];
			return vs[cl + (pos + (k - d)) % clen];
		}
	}
	
	private:
	vector<size_t> vs, vs_pos;
	vector<pair<size_t,size_t>> cycs, cyc_pos;
	vector<size_t> jumps_to_cyc, first_cyc_v;
	vector<size_t> jump;
};
