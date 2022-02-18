namespace kihash {
	using char_t = int32_t;
	struct hash_t {
		static constexpr uint64_t M = (uint64_t(1)<<61) - 1;
		hash_t(): x(0) {}
		hash_t(uint64_t val): x(val < M ? val : val%M) {}
		#define hash_t_op(O, E, F) hash_t& operator E(const hash_t &b) { F return *this; } friend hash_t operator O(hash_t a, const hash_t &b) { return a E b; }
		hash_t_op(*, *=, x = mul(x,b.x); )
		hash_t_op(+, +=, x+=b.x; if(x>=M) x-=M; )
		hash_t_op(-, -=, if(x<b.x) x+=M-b.x; else x-=b.x; )
		bool operator==(const hash_t &b) const { return x == b.x; }
		bool operator!=(const hash_t &b) const { return x != b.x; }
		uint64_t operator*() const { return x; }
		private: uint64_t x;
		static uint64_t mul(uint64_t a, uint64_t b) {
			uint64_t l1 = (uint32_t)a, h1 = a >> 32, l2 = (uint32_t)b, h2 = b >> 32;
			uint64_t l = l1 * l2, m = l1 * h2 + l2 * h1, h = h1 * h2;
			uint64_t ret = (l & M) + (l >> 61) + (h << 3) + (m >> 29) + (m << 35 >> 3) + 1;
			ret = (ret & M) + (ret >> 61);
			ret = (ret & M) + (ret >> 61);
			return ret - 1;
		}
	};
	
	const hash_t X = uint64_t(309935741)<<32 | mt19937(chrono::high_resolution_clock::now().time_since_epoch().count())() | 1;
	
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
		hash_view(char_t ch): hash_view(ch, 1) {}
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
		char_t operator[](size_t i) const { return data.at(i); }
		private: 
		vector<hash_t> suf;
		vector<char_t> data;
		static inline vector<hash_t> xpow = {1};
		static void expand_xpow(size_t n) {
			xpow.reserve(n);
			while(size(xpow) <= n) xpow.push_back(xpow.back() * X);
		}
	};
	
	struct hash_span {
		hash_span(): p(nullptr) {}
		hash_span(const hasher &s, size_t i, size_t n): p(&s), offset(i), len(n) { assert(i + n <= s.length()); }
		size_t start() const { return offset; }
		size_t length() const { return len; }
		char_t operator[](size_t i) const { return (*p)[offset + i]; }
		hash_t substr(size_t pos, size_t n) const { return p->substr(offset + pos, n); }
		hash_view subview(size_t pos, size_t n) const { return p->subview(offset + pos, n); }
		hash_span subspan(size_t pos, size_t n) const { return {*p, offset + pos, n}; }
		friend auto operator+(const hash_span &a, const hash_span &b) { return a.subview(0, a.len) + b.subview(0, b.len); }
		friend bool operator==(const hash_span &a, const hash_span &b) {
			return a.len == b.len && a.substr(0, a.len) == b.substr(0, b.len);
		}
		friend size_t lcp(const hash_span &a, const hash_span &b) {
			size_t l = 1, r = min(a.len, b.len) + 1;
			while(l < r) if(size_t m=(l+r)/2; a.substr(0,m)==b.substr(0,m)) l = m+1; else r = m;
			return l - 1;
		}
		friend bool operator<(const hash_span &a, const hash_span &b) {
			size_t i = lcp(a, b);
			return i < b.len && (i==a.len || a[i] < b[i]);
		}
		private:
		const hasher *p;
		size_t offset, len;
	};
	
	hash_span hasher::subspan(size_t pos, size_t n) const { return {*this, pos, n}; }
}
//using namespace kihash;
