template<class S, class W>
auto dijkstra(const S &start, auto &&gen) {
	/*unordered_*/map<S, pair<W,S>> dist{{start,{0,start}}};
	set<pair<W,S>> q{{0,start}};
	while(!empty(q)) {
		auto [dv, v] = move(q.extract(begin(q)).value());
		gen(as_const(v), [&dist, &q, &dv, &v](const S &t, W cost) {
			const W cur = dv + cost;
			if(auto it = dist.find(t); it == end(dist)) {
				dist.emplace(t, pair{cur, v});
				q.emplace(cur, t);
			} else if(auto &[dt, how] = it->second; cur < dt) {
				auto e = q.extract({dt, t});
				e.value().first = dt = cur;
				how = v;
				q.insert(move(e));
			}
		});
	}
	return dist;
}; //pass gen as [&](auto &v, auto &&upd) {/*upd(to, edge_cost)*/}
