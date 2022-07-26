struct crt_device {
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
	
	private: template<int x, int mod> static inline const auto inv =  1 / modint<mod>{x};
};
