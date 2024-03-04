using uint128_t = __uint128_t;
template<class T> using square_t = conditional_t<sizeof(T) < sizeof(uint64_t), uint64_t, uint128_t>;

template<uint32_t... A, std::unsigned_integral T>
bool __miller_rabin(T n) {
	assert(n > 1 && n % 2 == 1);
	auto test = [n, z=std::countr_zero(n-1)](square_t<T> a) {
		square_t<T> r = 1; 
		for(T d = n>>z; d; d>>=1, a = a*a %n) if(d&1) r = r*a %n;
		if(r == 1) return true;
		for(uint32_t s=z; s--; r = r*r %n) if(r == n-1) return true;
		return false;
	};
	return ((A >= n || test(A)) && ...);
}

template<std::integral T>
bool is_prime(T n) {
	if(n < 2 || n % 2 == 0) return n == 2;
	if constexpr (is_signed_v<T>) return is_prime<make_unsigned_t<T>>(n);
	else if constexpr (sizeof(T) > sizeof(uint32_t)) {
		if(n <= numeric_limits<uint32_t>::max()) return is_prime<uint32_t>(n);
		return __miller_rabin<2, 325, 9375, 28178, 450775, 9780504, 1795265022>(n);
	} else return __miller_rabin<2, 7, 61>(n);
}

template<std::integral T>
void __get_factors(T n, auto &p) {
	assert(n > 0 && n % 2 == 1);
	if constexpr (is_signed_v<T>) return __get_factors<make_unsigned_t<T>>(n, p);
	if constexpr (sizeof(T) > sizeof(uint32_t)) {
		if(n <= numeric_limits<uint32_t>::max()) return __get_factors<uint32_t>(n, p);
	}
	
	if(n == 1) return ;
	if(is_prime(n)) { p.push_back(n); return ; }
	
	static mt19937_64 rnd(chrono::steady_clock::now().time_since_epoch().count());
	for(;;) {
		auto &&g = [n, a=rnd()%(n-1)+1](square_t<T> x) { return (x*x + a) %n; };
		for(T x = rnd()%(n-3)+3, y = g(x); x != y; x=g(x), y=g(g(y)))
		if(const T d = std::gcd(x<y ? y-x : x-y, n); d > 1)
			return __get_factors<T>(d, p), __get_factors<T>(n / d, p);
	}
}

template<std::integral T>
vector<T> factors(T n) {
	assert(n > 0);
	vector<T> f;
	while(n%2 == 0) n/=2, f.push_back(2);
	for(T p = 3; p <= 37 && p*p <= n; p += 2) 
		while(n%p == 0) n/=p, f.push_back(p);
	__get_factors(n, f);
	return f;
}
