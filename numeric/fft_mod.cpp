namespace FFT{
	const size_t MAXN = (1 << 19);
	mint pws[MAXN + 1];

	void init_fft(){
		assert(mod == 998244353);
		pws[MAXN] = 805775211;
		for(size_t i = MAXN>>1; i >= 1; i>>=1){
			pws[i] = pws[i*2] * pws[i*2];
		}
	}
	
	void fft(vector<mint> &a, vector<mint> &ans, size_t n, size_t l, size_t cl, size_t step){
		if (n == 1) { ans[l] = a[cl]; return; }
		n>>=1;
		fft(a, ans, n, l, cl, step*2);
		fft(a, ans, n, l+n, cl+step, step*2);
		mint cw = 1, gw = pws[n*2];
		for(size_t i = l; i < l + n; i++){
			mint u = ans[i], v = cw*ans[i+n];
			ans[i] = u + v;
			ans[i+n] = u - v;
			cw *= gw;
		}
	}
	
	void fft_inv(vector<mint> &a, vector<mint> &ans, size_t n){
		fft(a, ans, n, 0, 0, 1);
		reverse(begin(ans)+1,end(ans));
		auto div = 1m / n;
		for(auto &val : ans) val*=div;
	}
};


vector<mint> multiply(const vector<mint> &a, const vector<mint> &b){
	if(size(a) < size(b)) return multiply(b, a);
	if(empty(b)) return {};
	size_t n = size(a), m = size(b), d = 1;
	while(d<n+m-1) d<<=1; assert(d<=FFT::MAXN);
	static bool init_called = false;
	if(!init_called) FFT::init_fft(), init_called = true;
	vector<mint> t(d), fa(d), fb(d);
	copy(begin(b),end(b),begin(t));
	FFT::fft(t, fb, d, 0, 0, 1);
	copy(begin(a),end(a),begin(t));
	FFT::fft(t, fa, d, 0, 0, 1);
	for(size_t i=0;i<d;++i) fa[i]*=fb[i];
	FFT::fft_inv(fa, t, d);
	t.resize(n+m-1);
	return t;
}