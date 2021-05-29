template<typename T>
struct persistent_array {
    struct node_base {
        virtual size_t length();
        virtual const T& get(size_t);
        virtual node_base* set_value(size_t, const T&);
    };
    struct node_leaf : node_base {
        node_leaf(const T& value): val(value) {}
        constexpr size_t length(){ return 1; }
        const T& get(size_t) { return val; }
        node_leaf* set_value(size_t, const T &value){ return new node_leaf(value); }
        private: T val;
    };
    struct node : node_base {
        node(node_base *pl, node_base *pr): l(pl), r(pr), sz(pl->length()+pr->length()) {}
        inline size_t length(){ return sz; }
        const T& get(size_t i) {
            if(size_t m = l->length(); i < m) return l->get(i);
            else return r->get(i-m);
        }
        node* set_value(size_t i, const T& value) {
            if(size_t m = l->length(); i < m) return new node(l->set_value(i,value), r);
            else return new node(l, r->set_value(i-m,value));
        }
        private:
        node_base *l, *r;
        size_t sz;
    };
    persistent_array(const vector<auto> &vec): root(build(begin(vec),end(vec))) { }
    const T& operator[](size_t i) const { return root->get(i); }
    persistent_array<T> set_value(size_t i, const T &value) const { return persistent_array(root->set_value(i,value)); }
    size_t size() const { return root ? root->length() : 0; }
    private:
    node_base *root;
    persistent_array(node_base *p): root(p) {}
    node_base* build(auto from, auto to) {
        if(from >= to) return nullptr;
        if(from+1 == to) return new node_leaf(*from);
        auto mid = from + (to - from)/2;
        return new node(build(from,mid),build(mid,to));
    }
};