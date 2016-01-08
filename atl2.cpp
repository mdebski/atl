#include<cstdio>
#include<vector>
#include<algorithm>
#include<cmath>
#include<functional>
#include<fstream>
#include<set>
#include<iostream>
#include<limits>
#include<boost/container/flat_map.hpp>

using namespace std;

typedef unsigned NUM;
typedef double FLOAT;
const FLOAT MAX_DIVS = 30;

template<typename T> void dump_gammas(vector<T>& v, FLOAT n) {
  cout << "N: " << n << ";\t";
  T s = accumulate(v.begin(), v.end(), 0) / n;
  cout << "SUM: " << fixed << s << ";\t";
  for(int i=0;i<v.size();++i) {
    cout << fixed << v[i]/n;
    if(i != v.size() - 1)
      cout << ", ";
  }
  cout << endl;
}


int main(int argc, char* argv[]) {
  cout.precision(std::numeric_limits<FLOAT>::max_digits10);
  if(argc < 2) {
    printf("Usage: %s n primes_file_name\n", argv[0]);
  }

  NUM n = stoi(argv[1]);

  std::set<NUM> breaks;
  NUM c = 1000;
  while(c < n) {
    breaks.insert(c);
    c*=10;
  }
  for(int i=1;i<10;++i) {
    breaks.insert((n*i)/10);
  }
  breaks.insert(n);

  vector<NUM> largest_divisors;
  boost::container::flat_map<NUM, FLOAT> prime_logs;
  largest_divisors.resize(n+1);

  ifstream file(argv[2], ios::binary);
  file.read(reinterpret_cast<char*>(largest_divisors.data()),
            largest_divisors.size() * sizeof(NUM));
  file.close();

  for(NUM i=1;i<=n;++i) {
    if(largest_divisors[i] == i) {
      prime_logs[i] = log(i);
    }
  }

  vector<FLOAT> gamma;
  gamma.resize(MAX_DIVS, 0.0);

  for(NUM i=2;i<=n;++i) {
    NUM c = i;
    FLOAT li = log(i);

    for(int j=0;j<MAX_DIVS;++j) {
      FLOAT l = prime_logs[largest_divisors[c]] / li;
      gamma[j] += l;
      c /= largest_divisors[c];
    }

    if(breaks.find(i) != breaks.end())
      dump_gammas(gamma, i);
  }

}
