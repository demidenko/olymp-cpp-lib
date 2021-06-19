namespace hashing {
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
		hash_t(int32_t val): x(val) { if(x<0) x+=M; }
		void operator*=(const hash_t &b){ x = mul(x,b.x); }
		void operator+=(const hash_t &b){ x+=b.x; if(x>=M) x-=M; }
		void operator-=(const hash_t &b){ if(b.x>x) x+=M-b.x; else x-=b.x; }
		friend hash_t operator*(hash_t a, const hash_t &b){ a*=b; return a; }
		friend hash_t operator+(hash_t a, const hash_t &b){ a+=b; return a; }
		friend hash_t operator-(hash_t a, const hash_t &b){ a-=b; return a; }
		friend bool operator==(const hash_t &a, const hash_t &b){ return a.x==b.x; }
		friend bool operator!=(const hash_t &a, const hash_t &b){ return a.x!=b.x; }
		uint64_t to_uint() { return x; }
		int64_t to_int() { return x; }
		private: uint64_t x;
	};
	
	const uint64_t X = 309935741 +  
		(mt19937(chrono::high_resolution_clock::now().time_since_epoch().count())()>>1);
	
	struct hasher {
		vector<hash_t> suf;
		static inline vector<hash_t> px = {1};
		
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
