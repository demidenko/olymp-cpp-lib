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
	
	const hash_t X = uint64_t(309935741)<<32 | mt19937(chrono::steady_clock::now().time_since_epoch().count())() | 1;
	
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
	struct hashed {
		hashed(): h(0), px(1), len(0) {}
		hashed(const hash_t &h, size_t len): hashed(h, len, pow_of_X(len)) {}
		hashed(const string &s): hashed(hash(s), size(s)) {}
		hashed(char_t ch): hashed(ch, 1, X) {}
		void operator+=(const hashed &a) { h += a.h*px; px *= a.px; len += a.len; }
		void operator+=(char_t ch) { h += ch*px; px *= X; ++len; }
		friend hashed operator+(hashed a, auto &&b) { a += b; return a; }
		bool operator==(const hashed &b) const { return h == b.h && len == b.len; }
		hash_t operator*() const { return h; }
		size_t length() const { return len; }
		private: hash_t h, px;
		size_t len;
		hashed(hash_t h, size_t len, hash_t px): h(h), px(px), len(len) {}
		friend hasher;
	};
	
	struct hasher {
		template<class Iter> hasher(Iter first, Iter last): suf(distance(first,last)+1), data(first,last) {
			expand_xpow(size(data));
			for(size_t i=size(data); i--; ) suf[i] = suf[i+1]*X + data[i];
		}
		hasher(const string &str): hasher(begin(str), end(str)) {}
		hasher(): hasher(""s) {}
		hash_t subhash(size_t pos, size_t n) const {
			assert(pos + n < size(suf));
			return suf[pos] - suf[pos+n]*xpow[n];
		}
		hashed substr(size_t pos, size_t n) const { return {subhash(pos,n), n, xpow[n]}; }
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
		hash_t subhash(size_t pos, size_t n) const { return p->subhash(offset + pos, n); }
		hashed substr(size_t pos, size_t n) const { return p->substr(offset + pos, n); }
		hash_span subspan(size_t pos, size_t n) const { return {*p, offset + pos, n}; }
		hashed operator*() const { return substr(0, len); }
		bool operator==(const hash_span &s) const { return s.len == len && s.subhash(0, len) == subhash(0, len); }
		friend size_t lcp(const hash_span &a, const hash_span &b) {
			size_t l = 1, r = min(a.len, b.len) + 1;
			while(l < r) if(size_t m=(l+r)/2; a.subhash(0,m)==b.subhash(0,m)) l = m+1; else r = m;
			return l - 1;
		}
		private:
		const hasher *p;
		size_t offset, len;
	};
	
	hash_span hasher::subspan(size_t pos, size_t n) const { return {*this, pos, n}; }
	
	auto operator<(auto &&a, auto &&b) -> decltype(lcp(a,b), a[a.length()] == b[b.length()]) {
		size_t i = lcp(a, b);
		return i < b.length() && (i == a.length() || a[i] < b[i]);
	}
}
using namespace kihash;
