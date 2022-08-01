struct factorizer {
	vector<int> minp, primes;
	explicit factorizer(size_t n): minp(n+1) {
		for(size_t i=2; i<=n; ++i) {
			if(!minp[i]) minp[i] = primes.emplace_back(i);
			for(int p : primes) 
				if(p <= minp[i] && i*p <= n) minp[i*p] = p;
				else break;
		}
	}
	
	auto factorize(int x) const {
		static pair<int,int> f[11], *e;
		for(e = f; x>1; ++e) {
			int p = minp[x], q = 0;
			do ++q, x/=p; while(minp[x]==p);
			*e = {p, q};
		}
		return vector<pair<int,int>>(f, e);
	}
	
	auto divisors(int x) const {
		static int divs[1500], dn;
		divs[0] = dn = 1;
		for(int p=0,l=0; x>1; x/=p) {
			if(int c=minp[x]; c!=p) p = c, l = 0;
			for(int r=dn; l<r; ) divs[dn++] = divs[l++] * p;
		}
		return vector<int>(divs, divs+dn);
	}
};
