namespace kihash {
	struct hash_t {
		static constexpr uint64_t M = (uint64_t(1)<<61) - 1;
		hash_t(): x(0) {}
		hash_t(int32_t val): x(val<0 ? val+M : val) {}
		#define hash_t_op(O, E, F) hash_t& operator E(const hash_t &b) { F return *this; } friend hash_t operator O(hash_t a, const hash_t &b) { return a E b; }
		hash_t_op(*, *=, x = mul(x,b.x); )
		hash_t_op(+, +=, x+=b.x; if(x>=M) x-=M; )
		hash_t_op(-, -=, if(b.x>x) x+=M-b.x; else x-=b.x; )
		bool operator==(const hash_t &b) const { return x == b.x; }
		bool operator!=(const hash_t &b) const { return x != b.x; }
		uint64_t operator*() const { return x; }
		private: uint64_t x;
		uint64_t mul(uint64_t a, uint64_t b) {
			uint64_t l1 = (uint32_t)a, h1 = a >> 32, l2 = (uint32_t)b, h2 = b >> 32;
			uint64_t l = l1 * l2, m = l1 * h2 + l2 * h1, h = h1 * h2;
			uint64_t ret = (l & M) + (l >> 61) + (h << 3) + (m >> 29) + (m << 35 >> 3) + 1;
			ret = (ret & M) + (ret >> 61);
			ret = (ret & M) + (ret >> 61);
			return ret - 1;
		}
	};
	
	const hash_t X = 309935741 +  
		int32_t(mt19937(chrono::high_resolution_clock::now().time_since_epoch().count())() >> 2);
	
	hash_t pow_of_X(size_t n) {
		hash_t p = 1, a = X;
		for(; n; n>>=1, a*=a) if(n&1) p*=a;
		return p;
	}
	
	hash_t hash(const string &s) {
		hash_t h;
		for(size_t i=size(s); i--; ) h = h*X + s[i];
		return h;
	}
	
	struct hasher; struct hash_span;
	struct hash_view {
		hash_t h;
		size_t length;
		hash_view(): h(0), length(0), px(1) {}
		hash_view(const hash_t &h, size_t len): hash_view(h, len, pow_of_X(len)) {}
		hash_view(const string &s): hash_view(hash(s), size(s)) {}
		hash_view(int32_t ch): hash_view(ch, 1) {}
		void operator+=(const hash_view &a) {
			h += a.h * px;
			length += a.length;
			px *= a.px;
		}
		friend hash_view operator+(hash_view a, const hash_view &b) { a+=b; return a; }
		bool operator==(const hash_view &b) const { return h == b.h && length == b.length; }
		uint64_t operator*() const { return *h; }
		private: hash_t px;
		hash_view(hash_t h, size_t len, hash_t px): h(h), length(len), px(px) {}
		friend hasher;
	};
	
	struct hasher {
		hasher(): hasher(""s) {}
		hasher(const auto &s): suf(size(s)+1), data(begin(s), end(s)) {
			expand_xpow(size(s));
			for(size_t i=size(s); i--; ) suf[i] = suf[i+1]*X + s[i];
		}
		hash_t substr(size_t pos, size_t n) const {
			assert(pos + n < size(suf));
			return suf[pos] - suf[pos+n]*xpow[n];
		}
		hash_view subview(size_t pos, size_t n) const { return {substr(pos,n), n, xpow[n]}; }
		hash_span subspan(size_t pos, size_t n) const;
		size_t length() const { return size(data); }
		int32_t operator[](size_t i) const { return data.at(i); }
		private: 
		vector<hash_t> suf;
		vector<int32_t> data;
		static inline vector<hash_t> xpow = {1};
		static void expand_xpow(size_t n) {
			xpow.reserve(n);
			while(size(xpow) <= n) xpow.push_back(xpow.back() * X);
		}
	};
	
	struct hash_span {
		hash_span(const hasher &s, size_t l, size_t n): s(s), start(l), len(n) { assert(start + len <= s.length()); }
		size_t length() const { return len; }
		int32_t operator[](size_t i) const { return s[start + i]; }
		hash_view subview(size_t pos, size_t n) const { return s.subview(start + pos, n); }
		hash_span subspan(size_t pos, size_t n) const { return {s, start+pos, n}; }
		friend size_t lcp(const hash_span &a, const hash_span &b) {
			size_t l = 1, r = min(a.len, b.len) + 1, m;
			while(l < r) if(m=(l+r)/2; a.subview(0,m)==b.subview(0,m)) l = m+1; else r = m;
			return l - 1;
		}
		friend bool operator==(const hash_span &a, const hash_span &b) {
			return a.subview(0, a.len) == b.subview(0, b.len);
		}
		friend bool operator<(const hash_span &a, const hash_span &b) {
			size_t i = lcp(a, b);
			return i < b.len && (i==a.len || a[i] < b[i]);
		}
		private:
		const hasher &s;
		size_t start, len;
	};
	
	hash_span hasher::subspan(size_t pos, size_t n) const { return {*this, pos, n}; }
}
