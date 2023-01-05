auto parallel_binary_search(size_t queries_count, size_t versions_count, auto &&action) {
	auto ensure = [](bool cond, const char *msg) { if(!cond) __throw_logic_error(msg); };
	using pred_t = function<bool(size_t)>;
	
	struct assign_helper {
		assign_helper(function<void(pred_t)> f): set_pred(f) {}
		void operator=(pred_t pred) { set_pred(pred); }
		private: function<void(pred_t)> set_pred;
	};
	struct vc { assign_helper pred; function<void()> commit; };
	
	struct range { size_t l, r, qi; };
	vector<range> ranges(queries_count);
	for(size_t i=0; i<queries_count; ++i) ranges[i] = {0, versions_count + 1, i};
	
	vector<size_t> result(queries_count);
	while(!empty(ranges)) {
		optional<pred_t> pred;
		size_t current_version = 0;
		auto it = begin(ranges);
		auto calc = [&] {
			ensure(pred.has_value(), "pred is not set");
			ensure(current_version <= versions_count, "too many versions");
			if(it == end(ranges)) return ;
			const size_t m = (it->l + it->r) / 2, pl = it->l;
			if(m > current_version) return ;
			for(auto mt = it; it != end(ranges) && it->l == pl; ++it)
				if((*pred)(it->qi)) it->l = m + 1; 
				else it->r = m, swap(*it, *mt), ++mt;
		};
		action(vc {
			assign_helper([&](pred_t f) {
				ensure(!pred.has_value(), "pred already set in this scope");
				pred = f;
				calc();
			}),
			[&] { ++current_version; calc(); }
		});
		ensure(current_version == versions_count, "not all versions are commited");
		it = begin(ranges);
		for(auto &t : ranges) if(t.l < t.r) *it++ = t; else result[t.qi] = t.r;
		ranges.erase(it, end(ranges));
	}
	
	return result;
}
