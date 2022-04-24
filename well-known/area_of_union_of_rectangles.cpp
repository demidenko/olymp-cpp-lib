ll area_of_union_of_rectangles(
	//all inclusive
	const vector<tuple<int,int,int,int>> &rects
) {
	struct add_op {
		int a;
		add_op(int add): a(add) {}
		add_op(const add_op &x, const add_op &y): a(x.a+y.a) {}
		decltype(auto) slice(size_t start, size_t end) const { return *this; }
	};
	
	struct icnt {
		int x, cnt;
		icnt(): icnt(0,0) {}
		icnt(int value, int __count = 1): x(value), cnt(__count) { }
		icnt(const icnt &a, const icnt &b) {
			x = min(a.x, b.x);
			cnt = 0;
			if(a.x == x) cnt += a.cnt;
			if(b.x == x) cnt += b.cnt;
		}
		icnt(const icnt &a, const add_op &o, size_t): x(a.x+o.a), cnt(a.cnt) {}
	};
	
	vector<int> ys;
	for(auto [ax, ay, bx, by] : rects) {
		ys.push_back(ay);
		ys.push_back(by+1);
	}
	if(empty(ys)) return 0;
	
	sort(begin(ys), end(ys));
	ys.resize(unique(begin(ys), end(ys)) - begin(ys));
	const int H = ys.back() - ys[0];
	
	segment_tree<icnt,add_op> t(size(ys)-1, [&](size_t i) {
		return icnt{0, ys[i+1]-ys[i]};
	});
	
	vector<tuple<int,int,int,int>> events;
	events.reserve(size(rects)*2);
	for(auto [ax, ay, bx, by] : rects) {
		size_t sl = lower_bound(begin(ys), end(ys), ay) - begin(ys);
		size_t sr = upper_bound(begin(ys), end(ys), by) - begin(ys);
		events.emplace_back(ax, +1, sl, sr);
		events.emplace_back(bx+1, -1, sl, sr);
	}
	sort(begin(events), end(events), [&](auto &e1, auto &e2) {
		return get<0>(e1) < get<0>(e2);
	});
	
	ll area = 0, lx = 0;
	for(auto [x, tp, sl, sr] : events) {
		auto cur = t();
		ll filled = cur.x == 0 ? H - cur.cnt : H;
		area += filled * (x - lx);
		t.apply(sl, sr, tp);
		lx = x;
	}
	
	return area;
}
