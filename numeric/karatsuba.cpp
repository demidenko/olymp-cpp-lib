void go_karatsuba(auto a, auto b, auto ret, size_t n){
	if(n<8){
		fill(ret, ret+n*2-1, 0);
		for(size_t i=0; i<n; ++i)
		for(size_t j=0; j<n; ++j) ret[i+j] += a[i]*b[j];
	}else
	if(n&1){
		go_karatsuba(a,b,ret,n-1);
		ret[n*2-3] = 0;
		for(size_t i=0; i<n-1; ++i) ret[i+n-1] += a[i]*b[n-1] + b[i]*a[n-1];
		ret[n*2-2] = a[n-1]*b[n-1];
	}else{
		auto 	ar = a, al = a+n/2, 
				br = b, bl = b+n/2,
				asum = ret+n*5, bsum = asum+n/2,
				x1 = ret, x2 = x1+n, x3 = x2+n;
		for(size_t i=0; i<n/2; ++i){
			asum[i] = al[i] + ar[i];
			bsum[i] = bl[i] + br[i];
		}
		go_karatsuba(ar, br, x1, n/2);
		go_karatsuba(al, bl, x2, n/2);
		go_karatsuba(asum, bsum, x3, n/2);
		for(size_t i=0; i<n; ++i) x3[i] -= x1[i]+x2[i];
		for(size_t i=0; i<n; ++i) ret[i+n/2]+=x3[i];
	}
	ret[n*2-1] = 0;
}

template<class T>
vector<T> multiply(const vector<T> &_a, const vector<T> &_b){
	if(empty(_a) || empty(_b)) return {};
	auto n = max(size(_a), size(_b));
	vector<T> a(n), b(n);
	copy(begin(_a), end(_a), begin(a));
	copy(begin(_b), end(_b), begin(b));
	vector<T> ret(n*6);
	go_karatsuba(begin(a), begin(b), begin(ret), n);
	return vector<T>(begin(ret), begin(ret)+size(_a)+size(_b)-1);
}
