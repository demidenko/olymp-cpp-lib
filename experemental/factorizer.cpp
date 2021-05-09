struct factorizer {
	vector<int> primes, md;
	factorizer(int n): md(n+1) {
		for(int i=2;i<=n;++i){
			if(!md[i]) primes.push_back(md[i]=i);
			for(int p : primes){
				if(p > md[i] || p*i > n) break;
				md[i*p] = p;
			}
		}
	}
	
	auto factorize(int x) {
		vector<pair<int,int>> res;
		while(x>1){
			int p = md[x], q = 0;
			do ++q, x/=p; while(x%p==0);
			res.emplace_back(p,q);
		}
		return res;
	}
	
	auto divisors(int x) {
		static int divs[1500], dn;
		divs[0] = dn = 1;
		for(int p=0,k,l; x!=1; x/=p){
			if(int c=md[x];c!=p) k=dn, l=0, p=c;
			for(int i=k;i--;) divs[dn++] = divs[l++]*p;
		}
		return vector<int>(divs, divs+dn);
	}
	
};