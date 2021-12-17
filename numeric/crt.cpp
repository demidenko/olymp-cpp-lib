struct crt_device {
	template<class T, int mod, int ...mods> 
	static T evaluate(const modint<mod> &a0, const modint<mods>& ...a) {
		return evaluate<T>(((a - (int)a0) * get_inv<mod,mods>())...) * T((int)mod) + T((int)a0);
	}
	
	template<class T, int mod> 
	static T evaluate(const modint<mod> &a0) {
		return T((int)a0);
	}
	
	template<class T, int ...mod>
	static vector<T> evaluate(const vector<modint<mod>>& ...a) {
		vector<T> res((size(a),...));
		for(size_t i=0; i<size(res); ++i) res[i] = evaluate<T>(a[i]...);
		return res;
	}
	
	private:
	template<int x, int mod> static modint<mod> get_inv() {
		static const auto inv = 1 / modint<mod>{x};
		return inv;
	}
};
