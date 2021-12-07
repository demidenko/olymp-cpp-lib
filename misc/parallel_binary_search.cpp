auto parallel_binary_search(size_t q, size_t n, auto action) {
	using ask_type = function<bool(size_t)>;
	struct holder {
		static void check(bool cond, const char *msg) { if(!cond) __throw_logic_error(msg); }
		vector<size_t> result;
		vector<tuple<size_t,size_t,size_t>> ranges;
		holder(size_t q, size_t n): result(q), ranges(q), q(q), n(n) {
			for(size_t i=0; i<q; ++i) ranges[i] = {0, n+1, i};
		}
		void set_ask(ask_type f) {
			ask = f;
			iter = begin(ranges);
			current_version = 0;
			calc();
		}
		void finish() {
			check(current_version == n, "not all versions are commited");
			size_t sz = 0;
			for(const auto &[l, r, qi] : ranges) {
				if(l < r) ranges[sz++] = {l, r, qi};
				else result[qi] = r;
			}
			ranges.resize(sz);
		}
		void commit() {
			current_version++;
			calc();
		}
		private: 
		size_t q, n, current_version;
		ask_type ask;
		typename decltype(ranges)::iterator iter;
		void calc() {
			check(current_version <= n, "too many versions");
			if(iter == end(ranges)) return ;
			size_t m = (get<0>(*iter) + get<1>(*iter)) >> 1;
			if(m > current_version) return ;
			auto rter = iter; 
			while(rter != end(ranges) && get<0>(*iter) == get<0>(*rter)) ++rter;
			for(auto jter = rter; iter != jter; ) {
				size_t qi = get<2>(*iter), start_qi = qi;
				do {
					if(ask(qi)) {
						swap(get<2>(*iter), qi);
						get<1>(*iter++) = m;
					} else {
						swap(get<2>(*--jter), qi);
						get<0>(*jter) = m+1;
					}
				} while(qi != start_qi);
			}
			iter = rter;
		}
	};
	
	struct version_controller {
		version_controller(holder &h): h(h), ask(h) {}
		void commit() {
			holder::check(ask.is_set, "ask is not set");
			h.commit();
		}
		private: holder &h;
		struct helper {
			helper(holder &h): h(h), is_set(false) {}
			void operator=(ask_type f) {
				holder::check(!is_set, "ask already set in this scope");
				is_set = true;
				h.set_ask(f);
			}
			private: holder &h; bool is_set;
			friend version_controller;
		};
		public: helper ask;
	};
	
	holder h(q, n);
	while(!empty(h.ranges)) {
		action(version_controller{h});
		h.finish();
	}
	
	return h.result;
}
