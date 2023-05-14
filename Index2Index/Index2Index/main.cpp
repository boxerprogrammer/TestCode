#include<iostream>
#include<vector>
#include<algorithm>


using namespace std;

int main() {
	vector<int> org = { 1,2,3,4,5,6,7,8,9 };
	vector<int> buff;
	buff.reserve(org.size() * 2);
	for (auto it = org.begin(); it != org.end();++it) {
		if (it != org.begin()) {
			buff.push_back(*(it-1));
			buff.push_back(*it);
		}
	}
	for_each(buff.begin(), buff.end(), [](const auto b) {cout << b << endl; });
}