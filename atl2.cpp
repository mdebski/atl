#include<cstdio>
#include<vector>
#include<algorithm>
#include<cmath>
#include<functional>
#include<fstream>

using namespace std;

int main(int argc, char* argv[]) {
  if(argc < 2) {
    printf("Usage: %s n primes_file_name\n", argv[0]);
  }

  typedef unsigned NUM;
  NUM n = stoi(argv[1]);
  double sum;
  double sqr;

  vector<NUM> largest_divisors;
  largest_divisors.resize(n+1);

  for(int div_num=1;div_num<=1;++div_num) {

    ifstream file(argv[2], ios::binary);
    file.read(reinterpret_cast<char*>(largest_divisors.data()),
              largest_divisors.size() * sizeof(NUM));
    file.close();

    sum = 0; sqr = 0;

    for(int i=2;i<=n;++i) {
      NUM c = i;
      for(int j=1;j<div_num;++j) {
        c /= largest_divisors[c];
      }
      double l = log(largest_divisors[c]) / log(i);
      sum += l / n;
      sqr += l*l / n;
    }
    printf("%d %lf %lf\n", div_num, sum, sqr);
  }

}
