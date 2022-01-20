auto parallel_binary_search(size_t queries_count, size_t versions_count, auto action) {
	using ask_type = function<bool(size_t)>;
	struct holder {
		static void ensure(bool cond, const char *msg) { if(!cond) __throw_logic_error(msg); }
		struct range { size_t l, r, qi; };
		vector<range> ranges;
		vector<size_t> result;
		holder(size_t q, size_t n): ranges(q), result(q), n(n) {
			for(size_t i=0; i<q; ++i) ranges[i] = {0, n+1, i};
		}
		void set_ask(ask_type f) {
			ensure(!ask.has_value(), "ask already set in this scope");
			ask = f;
			iter = begin(ranges);
			current_version = 0;
			calc();
		}
		void finish() {
			ensure(current_version == n, "not all versions are commited");
			ask.reset();
			auto it = begin(ranges);
			for(auto &t : ranges) if(auto &[l, r, qi] = t; l < r) *it++ = t; else result[qi] = r;
			ranges.erase(it, end(ranges));
		}
		void commit() {
			ensure(ask.has_value(), "ask is not set");
			current_version++;
			calc();
		}
		private: size_t n, current_version;
		optional<ask_type> ask;
		typename decltype(ranges)::iterator iter, rter;
		void calc() {
			ensure(current_version <= n, "too many versions");
			if(iter == end(ranges)) return ;
			const size_t m = (iter->l + iter->r) >> 1;
			if(m > current_version) return ;
			for(rter = iter; rter != end(ranges) && rter->l == iter->l; ++rter);
			for(auto jter = rter; iter != jter; ) 
				for(size_t qi = iter->qi, start_qi = qi; ; ) {
					if((*ask)(qi)) swap(iter->qi, qi), (iter++)->r = m;
					else swap((--jter)->qi, qi), jter->l = m+1;
					if(qi == start_qi) break ;
				}
			iter = rter;
		}
	};
	
	struct version_controller {
		explicit version_controller(holder &h): h(h), ask(h) {}
		void commit() { h.commit(); }
		private: holder &h;
		struct helper {
			helper(holder &h): h(h) {}
			void operator=(ask_type f) { h.set_ask(f); }
			private: holder &h;
		};
		public: helper ask;
	};
	
	holder h(queries_count, versions_count);
	while(!empty(h.ranges)) {
		action(version_controller{h});
		h.finish();
	}
	
	return h.result;
}
