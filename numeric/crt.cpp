struct crt_device {
	template<class T, int mod0, int ...mod> 
	static T evaluate(const modint<mod0> &a0, const modint<mod>& ...a) {
		return evaluate<T>(((a - (int)a0) * get_inv<mod0, mod>())...) * T((int)mod0) + T((int)a0);
	}
	
	template<class T, int mod> static T evaluate(const modint<mod> &a0) {
		return T((int)a0);
	}
	
	template<class T, class mint0, class ...mint>
	static vector<T> evaluate(const vector<mint0> &a0, const vector<mint>& ...a) {
		vector<T> res(size(a0)); assert(((size(a) == size(a0)) && ...));
		for(size_t i=0; i<size(res); ++i) res[i] = evaluate<T>(a0[i], a[i]...);
		return res;
	}
	
	private: template<int x, int mod> static inline auto get_inv() {
		static const auto inv = 1 / modint<mod>{x};
		return inv;
	}
};
