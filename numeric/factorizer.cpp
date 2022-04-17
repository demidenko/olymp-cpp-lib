struct factorizer {
	vector<int> primes, md;
	factorizer(int n): md(n+1) {
		for(int i=2; i<=n; ++i) {
			if(!md[i]) md[i] = primes.emplace_back(i);
			for(int p : primes) {
				if(p > md[i] || i*p > n) break;
				md[i*p] = p;
			}
		}
	}
	
	auto factorize(int x) {
		static pair<int,int> f[11], *e;
		for(e = f; x>1; ++e) {
			int p = md[x], q = 0;
			do ++q, x/=p; while(md[x]==p);
			*e = {p, q};
		}
		return vector<pair<int,int>>(f, e);
	}
	
	auto divisors(int x) {
		static int divs[1500], dn;
		divs[0] = dn = 1;
		for(int p=0,k=0,l=0; x>1; x/=p) {
			if(int c=md[x]; c!=p) p = c, k = dn, l = 0;
			for(int i=k;i--;) divs[dn++] = divs[l++] * p;
		}
		return vector<int>(divs, divs+dn);
	}
};
