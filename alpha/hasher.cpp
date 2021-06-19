namespace kihash {
	constexpr uint64_t M = (uint64_t(1)<<61) - 1;
	
	uint64_t mul(uint64_t a, uint64_t b) {
		uint64_t l1 = (uint32_t)a, h1 = a >> 32, l2 = (uint32_t)b, h2 = b >> 32;
		uint64_t l = l1 * l2, m = l1 * h2 + l2 * h1, h = h1 * h2;
		uint64_t ret = (l & M) + (l >> 61) + (h << 3) + (m >> 29) + (m << 35 >> 3) + 1;
		ret = (ret & M) + (ret >> 61);
		ret = (ret & M) + (ret >> 61);
		return ret - 1;
	}
	
	struct hash_t {
		hash_t(): x(0) {}
		hash_t(int32_t val): x(val<0 ? val+M : val) { }
		void operator*=(const hash_t &b){ x = mul(x,b.x); }
		void operator+=(const hash_t &b){ x+=b.x; if(x>=M) x-=M; }
		void operator-=(const hash_t &b){ if(b.x>x) x+=M-b.x; else x-=b.x; }
		friend hash_t operator*(hash_t a, const hash_t &b){ a*=b; return a; }
		friend hash_t operator+(hash_t a, const hash_t &b){ a+=b; return a; }
		friend hash_t operator-(hash_t a, const hash_t &b){ a-=b; return a; }
		friend bool operator==(const hash_t &a, const hash_t &b){ return a.x==b.x; }
		friend bool operator!=(const hash_t &a, const hash_t &b){ return a.x!=b.x; }
		friend ostream& operator<<(ostream &o, const hash_t &h){ return o<<h.x; }
		uint64_t to_uint() { return x; }
		int64_t to_int() { return x; }
		private: uint64_t x;
	};
	
	hash_t pow(hash_t x, uint64_t n) {
		hash_t res = 1;
		for(; n; n>>=1, x*=x) if(n&1) res*=x;
		return res;
	}
	
	const hash_t X = 309935741 +  
		int32_t(mt19937(chrono::high_resolution_clock::now().time_since_epoch().count())() >> 2);
	
	
	hash_t hash(const string &s) {
		hash_t h = 0;
		for(size_t i=size(s); i--; ) h = h*X + s[i];
		return h;
	}
	
	struct hash_view {
		size_t length;
		hash_t h, px;
		hash_view(): length(0), h(0), px(1) {}
		hash_view(char ch): length(1), h(ch), px(X) {}
		hash_view(const string &s): length(size(s)), h(hash(s)), px(pow(X,length)) {}
		void operator+=(const hash_view &a) {
			length+=a.length;
			h+=a.h*px;
			px*=a.px;
		}
		friend hash_view operator+(hash_view a, const hash_view &b) { a+=b; return a; }
		friend bool operator==(const hash_view &a, const hash_view &b) { return a.h==b.h && a.length==b.length; }
	};
	
	struct hasher {
		static inline vector<hash_t> px = {1};
		vector<hash_t> suf;
		
		hasher(const string &s) {
			size_t n = size(s);
			suf.resize(n+1);
			for(size_t i=n; i--; ) suf[i] = suf[i+1]*X + s[i];
			if(size_t pn = size(px); pn < n+1) {
				px.resize(n+1);
				for(size_t i=pn; i<=n; ++i) px[i] = px[i-1]*X;
			}
		}
		
		hash_t substr(size_t pos, size_t n) {
			return suf[pos] - suf[pos+n]*px[n];
		}
	};
}
