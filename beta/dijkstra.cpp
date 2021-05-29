template<typename S, typename T>
auto dijkstra(const S &start,
    function<void(const S&, const function<void(const S&, const T&)>&)> gen
	//pass as [](const auto &s, auto &upd){... upd(to, edge_cost); ...}
){
    map<S, T> dist{{start,0}};
    set<pair<T,S>> q{{0,start}};
    T d;
    auto upd = [&dist, &q, &d](const S &v, const T &edge_cost) {
        const T cost = edge_cost + d;
        if(auto it = dist.find(v); it!=end(dist)) {
            if(T& cur = it->second; cost < cur) {
                auto e = q.extract({cur,v});
                e.value().first = cur = cost;
                q.insert(move(e));
            }
        } else q.emplace(dist[v] = cost, v);
    };
    while(!empty(q)) {
        auto p = move(q.extract(begin(q)).value());
        d = p.first;
        gen(p.second, upd);
    }
    return dist;
};