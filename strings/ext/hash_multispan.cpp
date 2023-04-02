struct hash_multispan {
	hash_multispan() {}
	hash_multispan(const hash_span &sp): s{sp}, prefs{sp.length()} {}
	
	hash_multispan& operator+=(const hash_span &sp) {
		if(size_t sl = sp.length()) {
			s.push_back(sp);
			prefs.push_back(length() + sl);
		}
		return *this;
	}
	
	size_t length() const { return empty(prefs) ? 0 : prefs.back(); }
	
	char_t operator[](size_t i) const {
		assert(i < length());
		auto it = upper_bound(begin(prefs), end(prefs), i);
		size_t before = it == begin(prefs) ? 0 : *prev(it);
		return s[it - begin(prefs)][i - before];
	}
	
	hash_multispan subspan(size_t pos, size_t n) const {
		assert(pos + n <= length());
		hash_multispan res;
		for(auto &&sp : s) {
			if(n == 0) break;
			if(size_t sl = sp.length(); pos < sl) {
				size_t sz = min(sl - pos, n);
				res += sp.subspan(pos, sz);
				n -= sz;
				pos = 0;
			} else pos -= sl;
		}
		return res;
	}
	
	friend bool operator==(const hash_multispan &a, const hash_multispan &b) {
		if(a.length() != b.length()) return false;
		return *a == *b;
	}
	
	hashed operator*() const {
		hashed h;
		for(auto &sp : s) h += *sp;
		return h;
	}
	
	friend size_t lcp(const hash_multispan &a, const hash_multispan &b) {
		size_t i = 0, j = 0, pa = 0, pb = 0, res = 0;
		while(i<size(a.s) && j<size(b.s)) {
			auto &sa = a.s[i], &sb = b.s[j];
			size_t len = min(sa.length() - pa, sb.length() - pb);
			if(sa.subhash(pa, len) == sb.subhash(pb, len)) {
				res += len;
				pa += len; if(pa == sa.length()) ++i, pa = 0;
				pb += len; if(pb == sb.length()) ++j, pb = 0;
			} else {
				res += lcp(sa.subspan(pa, len), sb.subspan(pb, len));
				break ;
			}
		}
		return res;
	}
	
	friend size_t lcp(const hash_span &a, const hash_multispan &b) {
		size_t res = 0;
		for(auto &sb : b.s) {
			size_t len = min(a.length() - res, sb.length());
			if(len == 0) break ;
			if(a.subhash(res, len) == sb.subhash(0, len)) {
				res += len;
			} else {
				res += lcp(a.subspan(res, len), sb.subspan(0, len));
				break ;
			}
		}
		return res;
	}
	friend size_t lcp(const hash_multispan &a, const hash_span &b) { return lcp(b, a); }
	
	private: 
	vector<hash_span> s;
	vector<size_t> prefs;	
};
