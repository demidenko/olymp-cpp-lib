
int root2(ll a){
	ll b = sqrt(a);
	while((b+1LL)*(b+1LL)<=a) ++b;
	return b;
}

int root3(ll a){
	ll b = cbrt(a);
	while((b+1LL)*(b+1LL)*(b+1LL)<=a) ++b;
	return b;
}

void getPrimes(int n, vector<int> &pr){
	vector<int> md(n+1);
	pr.clear();
	for(int i=2;i<=n;++i){
		if(!md[i]) pr.push_back(md[i]=i);
		for(int p : pr){
			if(p>md[i] || 1LL*p*i>n) break;
			md[p*i] = p;
		}
	}
}

struct countOfPrimes{
	vector<int> pr;
	
	countOfPrimes(ll n){
		getPrimes(root2(n+1)*2, pr);
		prepi.assign(pr.back()+1, 0);
		int sum = 0;
		for(int i=0,j=0;i<=pr.back();++i){
			if(i==pr[j]) sum++, j++;
			prepi[i] = sum;
		}
		int sn = min(1LL<<17, n), sm = min((int)pr.size(), 1<<6);
		mem.assign(sm, vector<int>(sn));
		for(int j=0;j<sm;++j)
		for(int i=0;i<sn;++i){
			if(j==0) mem[j][i] = i; else
			mem[j][i] = mem[j-1][i] - mem[j-1][i/pr[j-1]];
		}
	}

	///count of numbers 1<=X<=n whose minimal prime divisor >= pr[m]
	vector<vector<int>> mem;
	ll f(ll n, int m){
		if(m==0) return n;
		if(pr[m]>n) return 1;
		if(m<mem.size() && n<mem[m].size()) return mem[m][(int)n];
		if(n<1LL*pr[m]*pr[m]) return pi(n)-pi(pr[m-1])+1;
		return f(n, m-1) - f(n/pr[m-1], m-1);
	}

	///count of prime numbers 1<=X<=n
	vector<int> prepi;
	ll pi(ll n){
		if(n<=pr.back()) return prepi[(int)n];
		int m = upper_bound(ALL(pr), root3(n)) - pr.begin();
		assert(1LL*pr[m]*pr[m]*pr[m]>n);
		ll res = f(n,m) + pi(pr[m-1]) - 1;
		for(int k=m;;++k){
			ll p = pr[k];
			if(p*p>n) break;
			assert(n/p >= pr[k]);
			ll val = pi(n/p) - pi(p-1);
			res-=val;
		}
		return res;
	}
	
	ll operator()(ll n){
		return pi(n);
	}
};

template<typename T>
struct sumOfPrimes{
	vector<int> pr;
	
	//1 + 2 + ... + n
	static T sar(ll n){
		if(!n) return 0;
		if(n&1) return sar(n-1) + n;
		T m = n/2;
		return sar(n/2)*2 + m*m;
	}
	
	sumOfPrimes(ll n){
		getPrimes(root2(n+1)*2, pr);
		prespi.assign(pr.back()+1, 0);
		T sum = 0;
		for(int i=0,j=0;i<=pr.back();++i){
			if(i==pr[j]) sum+=pr[j++];
			prespi[i] = sum;
		}
		int sn = min(1LL<<17, n), sm = min((int)pr.size(), 1<<6);
		mem.assign(sm, vector<T>(sn));
		for(int j=0;j<sm;++j)
		for(int i=0;i<sn;++i){
			if(j==0) mem[j][i] = sar(i); else
			mem[j][i] = mem[j-1][i] - mem[j-1][i/pr[j-1]]*pr[j-1];
		}
	}

	///sum of numbers 1<=X<=n whose minimal prime divisor >= pr[m]
	vector<vector<T>> mem;
	T sf(ll n, int m){
		if(m==0) return sar(n);
		if(pr[m]>n) return 1;
		if(m<mem.size() && n<mem[m].size()) return mem[m][(int)n];
		if(n<1LL*pr[m]*pr[m]) return spi(n)-spi(pr[m-1])+1;
		return sf(n, m-1) - sf(n/pr[m-1], m-1)*pr[m-1];
	}

	///sum of prime numbers 1<=X<=n
	vector<T> prespi;
	T spi(ll n){
		if(n<=pr.back()) return prespi[(int)n];
		int m = upper_bound(ALL(pr), root3(n)) - pr.begin();
		assert(1LL*pr[m]*pr[m]*pr[m]>n);
		T res = sf(n,m) + spi(pr[m-1]) - 1;
		for(int k=m;;++k){
			ll p = pr[k];
			if(p*p>n) break;
			assert(n/p >= pr[k]);
			T val = spi(n/p) - spi(p-1);
			res-=val*p;
		}
		return res;
	}
	
	T operator()(ll n){
		return spi(n);
	}
};