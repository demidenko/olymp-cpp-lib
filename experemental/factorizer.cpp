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
	
	vector<pair<int,int>> factorize(int x){
		vector<pair<int,int>> res;
		while(x>1){
			int p = md[x], q = 0;
			do ++q, x/=p; while(x%p==0);
			res.emplace_back(p,q);
		}
		return res;
	}
	
	vector<int> divisors(int x){
		static int divs[1500], dn;
		divs[0] = dn = 1;
		for(int pr=0,p,k,l; x!=1; x/=p,pr=p){
			if((p=md[x])!=pr) k=dn, l=0;
			for(int i=0;i<k;++i) divs[dn++] = divs[l++]*p;
		}
		return vector<int>(divs, divs+dn);
	}
	
};