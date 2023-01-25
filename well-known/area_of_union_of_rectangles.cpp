//requires @aggregator_range_operation
template<class S, class D = int, class P>
S area_of_union_of_rectangles(
	//[ax, ay, bx, by] all inclusive
	const vector<tuple<P,P,P,P>> &rects
) {
	struct add_op {
		D a;
		add_op(D add): a{add} {}
		add_op(const add_op &x, const add_op &y): a{x.a+y.a} {}
		decltype(auto) slice(size_t, size_t) const { return *this; }
	};
	
	struct min_cnt {
		D m;
		P cnt;
		min_cnt(): min_cnt(0,0) {}
		min_cnt(D val, P cnt): m{val}, cnt{cnt} {}
		min_cnt(const min_cnt &a, const min_cnt &b): m{min(a.m,b.m)}, cnt{0} {
			if(a.m == m) cnt += a.cnt;
			if(b.m == m) cnt += b.cnt;
		}
		void apply(const add_op &o, size_t) { m += o.a; }
	};
	
	vector<P> ys;
	for(auto [ax, ay, bx, by] : rects) {
		assert(ax <= bx && ay <= by);
		ys.push_back(ay);
		ys.push_back(by+1);
	}
	if(empty(ys)) return 0;
	
	sort(begin(ys), end(ys));
	ys.erase(unique(begin(ys), end(ys)), end(ys));
	
	aggregator<min_cnt,add_op> t(size(ys)-1, [&](size_t i) {
		return min_cnt(0, ys[i+1]-ys[i]);
	});
	
	vector<tuple<P,int,size_t,size_t>> events;
	events.reserve(size(rects)*2);
	for(auto [ax, ay, bx, by] : rects) {
		size_t sl = lower_bound(begin(ys), end(ys), ay) - begin(ys);
		size_t sr = upper_bound(begin(ys), end(ys), by) - begin(ys);
		events.emplace_back(ax, +1, sl, sr);
		events.emplace_back(bx+1, -1, sl, sr);
	}
	sort(begin(events), end(events), [](auto &e1, auto &e2) {
		return get<0>(e1) < get<0>(e2);
	});
	
	S area = 0; 
	P H = ys.back() - ys[0], px = 0;
	for(auto [x, sig, sl, sr] : events) {
		auto &cur = t();
		area += S(x - px) * S(cur.m == 0 ? H - cur.cnt : H);
		t.apply(sl, sr, sig);
		px = x;
	}
	
	return area;
}
