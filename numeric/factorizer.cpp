struct factorizer {
	vector<int> minp, primes;
	explicit factorizer(int n): minp(n+1) {
		for(int i=2; i<=n; ++i) {
			if(!minp[i]) minp[i] = primes.emplace_back(i);
			for(int p : primes) 
				if(p <= minp[i] && i*p <= n) minp[i*p] = p;
				else break;
		}
	}
	
	auto factorization(int x) const {
		static pair<int,int> f[11], *e;
		for(e = f; x>1; ++e) {
			auto &[p, q] = *e = {minp[x], 0};
			do ++q, x/=p; while(minp[x] == p);
		}
		return vector(f, e);
	}
	
	auto divisors(int x) const {
		static int divs[1500], dn;
		for(int p=0,l=dn=divs[0]=1; x>1; x/=p) {
			if(p != minp[x]) p = minp[x], l = 0;
			for(int r=dn; l<r; ) divs[dn++] = divs[l++] * p;
		}
		return vector(divs, divs+dn);
	}
};
