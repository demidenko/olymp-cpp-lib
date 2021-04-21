struct binary_lifter {
    binary_lifter(const vector<int> &p) {
        int n = size(p);
        for(int i=0;i<n;++i) assert(p[i]==-1 || (0<=p[i] && p[i]<n && p[i]!=i)); //weak check
        for(L = 1; (1<<L)<n; ++L);
        jump.assign(L,vector<int>(n,-1));
        jump[0] = p;
        for(int h=1;h<L;++h){
            for(int i=0;i<n;++i){
                int j = jump[h-1][i];
                jump[h][i] = (j == -1 ? -1 : jump[h-1][j]);
            }
        }
    }
    
    //returns <first fail, count of jumps, last nonfail>
    tuple<int,int,int> first_fail(int i, function<bool(int)> &&is_fail) {
        if(is_fail(i)) return { i, 0, -1 };
        int jumps = 0;
        for(int h=L-1;h>=0;--h){
            int j = jump[h][i];
            if(j!=-1 && !is_fail(j)) i = j, jumps+=1<<h;
        }
        return { jump[0][i], jumps+1, i };
    }
    
    private:
    vector<vector<int>> jump;
    int L;
};