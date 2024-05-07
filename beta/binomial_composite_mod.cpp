template<class bmint, class CRT>
struct binomial {
	struct factorial_device {
		uint32_t p, mod;
		vector<bmint> f;
		
		factorial_device(uint32_t p, uint32_t q): p(p), mod(pow(p, q)), f(mod) {
			assert(bmint::get_mod() % mod == 0);
			bmint prod = 1;
			for(uint32_t k=0; k<mod; ++k) {
				if(k % p != 0) prod *= k;
				f[k] = prod;
			}
		}
		
		pair<bmint, uint64_t> operator()(uint64_t n) const {
			bmint res = 1;
			uint64_t q = 0;
			bool fq = 0;
			while(n > 1) {
				fq ^= (n / mod) &1;
				res *= f[n % mod];
				n /= p;
				q += n;
			}
			return {p > 2 && fq ? -res : res, q};
		}
	};
	
	binomial() {
		vector<int32_t> mods;
		for(uint32_t x=bmint::get_mod(), p=2; x>1; ++p) {
			if(p * p > x) p = x;
			if(x % p == 0) {
				uint32_t q = 0;
				do ++q, x /= p; while(x % p == 0);
				fs.emplace_back(p, q);
				mods.push_back(fs.back().mod);
			}
		}
		crt = CRT{mods};
	}
	
	bmint operator()(uint64_t n, uint64_t k) const {
		if(n < k) return 0;
		
		vector<int32_t> noms, denoms;
		for(auto &f : fs) {
			auto [x1, q1] = f(n);
			auto [x2, q2] = f(k);
			auto [x3, q3] = f(n-k);
			
			bmint nom = x1 * pow(bmint{f.p}, q1 - q2 - q3);
			bmint denom = x2 * x3;
			
			noms.push_back(*nom % f.mod);
			denoms.push_back(*denom % f.mod);
		}
		
		bmint nom = crt.template evaluate<int32_t>(noms);
		bmint denom = crt.template evaluate<int32_t>(denoms);
		return nom / denom;
	}
	
	private:
	CRT crt;
	vector<factorial_device> fs;
};
