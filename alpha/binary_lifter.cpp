struct binary_lifter {
	binary_lifter(const std::ranges::sized_range auto &to = std::ranges::empty_view<size_t>{}) {
		const size_t n = size(to);
		vector<size_t> p(n+1, n);
		std::ranges::copy(to, begin(p));
		assert(*std::ranges::max_element(p) <= n);
		
		jumps_to_cyc.resize(n+1);
		cyc_pos.resize(n+1);
		
		vector<bool> used(n+1);
		vector<size_t> used_at(n+1, n);
		for(size_t i=0; i<=n; ++i) if(!used[i]) {
			vector<size_t> path;
			for(size_t v=i; ; ) {
				path.push_back(v);
				size_t t = v < n ? p[v] : n;
				if(used[t]) {
					if(used_at[t] == i) {
						auto it = find(begin(path), end(path), t);
						auto &cyc = cycs.emplace_back(it, end(path));
						for(size_t pos=0; pos<size(cyc); ++pos) {
							size_t x = cyc[pos];
							jumps_to_cyc[x] = {0, x};
							cyc_pos[x] = {size(cycs) - 1, pos};
						}
						path.erase(it, end(path));
					}
					break ;
				}
				used[v] = true;
				used_at[v] = i;
				v = t;
			}
			for(size_t x : std::views::reverse(path))
				(jumps_to_cyc[x] = jumps_to_cyc[p[x]]).first++;
		}
		
		size_t d = std::ranges::max_element(jumps_to_cyc)->first;
		size_t L = std::countr_zero(std::bit_ceil(d));
		if(!L) return ;
		jump.assign(L, vector(n+1, n));
		jump[0] = std::move(p);
		for(size_t h=1; h<L; ++h)
		for(size_t i=0; i<n; ++i) jump[h][i] = jump[h-1][jump[h-1][i]];
	}
	
	size_t kth_jump(size_t i, std::unsigned_integral auto k) const {
		if(auto [d, v] = jumps_to_cyc[i]; k < d) {
			for(size_t h=0; k>0; ++h, k>>=1) if(k&1) i = jump[h][i];
			return i;
		} else {
			k -= d;
			auto [row, pos] = cyc_pos[v];
			auto &cyc = cycs[row];
			return cyc[(pos + k) % size(cyc)];
		}
	}
	
	/*struct result { size_t first_false, last_true, jumps_to_false; };
	result first_false(size_t i, auto &&pred) const {
		if(!pred(i)) return { i, none, 0 };
		size_t jumps = 0;
		for(size_t h = L; h--; )
		if(size_t j = jump[h][i]; j != none && pred(j)) {
			i = j;
			jumps += size_t(1) << h;
		}
		return { jump[0][i], i, jumps+1 };
	}*/
	
	private:
	vector<vector<size_t>> jump;
	vector<vector<size_t>> cycs;
	vector<pair<size_t,size_t>> cyc_pos;
	vector<pair<size_t,size_t>> jumps_to_cyc;
};
