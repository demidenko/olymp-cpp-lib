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
	
	const hash_t X = 309935741 +  
		int32_t(mt19937(chrono::high_resolution_clock::now().time_since_epoch().count())() >> 2);
	
	
	vector<hash_t> xpow = {1};
	void expand_xpow(size_t n) {
		if(size_t pn = size(xpow); pn < n+1) {
			xpow.resize(n+1);
			for(size_t i=pn; i<=n; ++i) xpow[i] = xpow[i-1]*X;
		}
	}
	
	hash_t hash(const string &s) {
		hash_t h = 0;
		for(size_t i=size(s); i--; ) h = h*X + s[i];
		return h;
	}
	
	struct hash_view {
		hash_t h;
		size_t length;
		hash_view(): h(0), length(0) {}
		hash_view(const hash_t &h, size_t len): h(h), length(len) {}
		hash_view(char ch): h(ch), length(1) {}
		hash_view(const string &s): h(hash(s)), length(size(s)) {}
		void operator+=(const hash_view &a) {
			expand_xpow(length);
			h+=a.h*xpow[length];
			length+=a.length;
		}
		friend hash_view operator+(hash_view a, const hash_view &b) { a+=b; return a; }
		friend bool operator==(const hash_view &a, const hash_view &b) { return a.h==b.h && a.length==b.length; }
	};
	
	struct hasher {
		hasher(const string &s) {
			size_t n = size(s);
			expand_xpow(n);
			suf.resize(n+1);
			for(size_t i=n; i--; ) suf[i] = suf[i+1]*X + s[i];
		}
		
		hash_t substr(size_t pos, size_t n) {
			return suf[pos] - suf[pos+n]*xpow[n];
		}
		
		hash_view substr_view(size_t pos, size_t n) {
			return {substr(pos,n), n};
		}
		
		private:
		vector<hash_t> suf;
	};
}
