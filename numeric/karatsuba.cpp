void multiply_brute(auto a, size_t n, auto b, size_t m, auto ret){
	fill(ret, ret+n+m-1, 0);
	for(size_t i=0; i<n; ++i)
	for(size_t j=0; j<m; ++j) ret[i+j] += a[i]*b[j];
}

void go_karatsuba(auto a, auto b, auto ret, size_t n){
	if(n<16){
		multiply_brute(a, n, b, n, ret);
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
vector<T> multiply(const vector<T> &a, const vector<T> &b){
	if(size(a) < size(b)) return multiply(b, a);
	if(empty(b)) return {};
	size_t n = size(a), m = size(b);
	if(1ULL*m*m < n){
		vector<T> ret(n+m-1);
		multiply_brute(begin(a), n, begin(b), m, begin(ret));
		return ret;
	}else{
		vector<T> b_ex(n);
		copy(begin(b), end(b), begin(b_ex));
		vector<T> ret(n*6);
		go_karatsuba(begin(a), begin(b_ex), begin(ret), n);
		return vector<T>(begin(ret), begin(ret)+n+m-1);
	}
}
