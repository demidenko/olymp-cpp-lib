auto parallel_binary_search(size_t qn, size_t n, auto action) {
	using ask_type = function<bool(size_t)>;
	struct holder {
		size_t qn, n, current_version;
		vector<size_t> result;
		vector<tuple<size_t,size_t,size_t>> ranges;
		holder(size_t qn, size_t n): qn(qn), n(n), result(qn), ranges(qn) {
			for(size_t i=0; i<qn; ++i) ranges[i] = {0, n+1, i};
		}
		ask_type ask;
		typename decltype(ranges)::iterator iter;
		void reset() {
			size_t sz = 0;
			for(const auto &[l, r, qi] : ranges) {
				if(l < r) ranges[sz++] = {l, r, qi};
				else result[qi] = r;
			}
			ranges.resize(sz);
			sort(begin(ranges),end(ranges),[](const auto &a, const auto &b){ return get<0>(a) < get<0>(b); });
			iter = begin(ranges);
			current_version = 0;
			calc();
		}
		void commit() {
			current_version++;
			calc();
		}
		void calc() {
			assert(current_version <= n);
			for(; iter != end(ranges); ++iter) {
				auto &[l, r, qi] = *iter;
				size_t m = (l + r) >> 1;
				assert(m >= current_version);
				if(m > current_version) break;
				if(ask(qi)) r = m; else l = m+1;
			}
		}
	};
	
	struct helper {
		helper(holder &h): h(h), ask(h) {}
		void commit() { h.commit(); }
		private: holder &h;
		struct subhelper {
			subhelper(holder &h): h(h) {}
			void operator=(ask_type f) {
				h.ask = f;
				h.reset();
			}
			private: holder &h;
		};
		public: subhelper ask;
	};
	
	holder h(qn, n);
	while(!empty(h.ranges)) action(helper{h});
	
	return h.result;
}
