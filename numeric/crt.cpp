struct crt_device {
	crt_device(){};
	crt_device(const vector<int> &mods): mods(mods), invs(size(mods)) {
		for(size_t i=0; i<size(mods); ++i) {
			tmod = mods[i];
			invs[i].resize(i);
			for(size_t j=0; j<i; ++j) invs[i][j] = *modint<(tmod)>{mods[j]}.inverse().value();
		}
	}
	
	template<class T>
	T evaluate(vector<int> a) const {
		assert(size(a) == size(mods));
		for(size_t i=0; i<size(a); ++i) {
			tmod = mods[i];
			modint<(tmod)> cur{a[i]};
			for(size_t j=0; j<i; ++j) cur -= a[j], cur *= invs[i][j];
			a[i] = *cur;
		}
		T res = 0;
		for(size_t i=size(a); i--; ) res *= mods[i], res += a[i];
		return res;
	}
	
	template<class T, int mod0, int ...mod> 
	static T evaluate(const modint<mod0> &a0, const modint<mod>& ...a) {
		if constexpr (sizeof...(a) == 0) return T(*a0); else
		return evaluate<T>(((a - *a0) * inv<mod0, mod>)...) * T(mod0) + T(*a0);
	}
	
	template<class T, int mod0, int ...mod>
	static vector<T> evaluate(const vector<modint<mod0>> &a0, const vector<modint<mod>>& ...a) {
		vector<T> res(size(a0)); assert(((size(a) == size(a0)) && ...));
		for(size_t i=0; i<size(res); ++i) res[i] = evaluate<T>(a0[i], a[i]...);
		return res;
	}
	
	private: 
	vector<int> mods;
	vector<vector<int>> invs;
	
	template<int x, int mod> static inline const auto inv =  1 / modint<mod>{x};
	static inline int tmod;
};
