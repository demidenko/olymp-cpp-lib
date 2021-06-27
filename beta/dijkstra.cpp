template<class S, class T>
auto dijkstra(const S &start,
	function<void(const S&, function<void(const S&, T)>)> gen
	//pass as [](const auto &v, auto upd){... upd(to, edge_cost); ...}
) {
	map<S, pair<T,S>> dist{{start,{0,start}}};
	set<pair<T,S>> q{{0,start}};
	T dv; S v;
	auto upd = [&dist, &q, &dv, &v](const S &t, T edge_cost) {
		const T cur = dv + edge_cost;
		if(auto it = dist.find(t); it!=end(dist)) {
			if(auto &[dt, how] = it->second; cur < dt) {
				auto e = q.extract({dt,t});
				e.value().first = dt = cur;
				how = v;
				q.insert(move(e));
			}
		} else {
			dist[t] = {cur, v};
			q.emplace(cur, t);
		}
	};
	while(!empty(q)) {
        tie(dv, v) = move(q.extract(begin(q)).value());
		gen(v, upd);
	}
	return dist;
};
