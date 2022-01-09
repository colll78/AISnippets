#include "pch.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <iterator>
#include <ctime>
#include <cstdlib>
#include <random>
#include <tuple>

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

bool fair_division(int num_people, int target_sum, int allowed_shifts, vi beads) {
	
	vector<ll> prefix_sum(beads.size() + 1, 0);
	for (int i = 1; i <= beads.size(); i++) {
		prefix_sum[i] = prefix_sum[i - 1] + beads[i - 1];
	}
	
	/*
	cout << "Beads: ";
	copy(beads.begin(), beads.end(), ostream_iterator<int>(cout, " "));
	cout << "\n";
	cout << "Prefix_sum: ";
	copy(prefix_sum.begin(), prefix_sum.end(), ostream_iterator<int>(cout, " "));
	*/
	int shifts = 0, num_p = num_people;
	for (int j = 1, i = 1; j <= beads.size() - allowed_shifts; j++) {
		//cout << "\n";
		//cout << prefix_sum[j + shifts] - prefix_sum[i - 1];
		//cout << "\n";
		if (prefix_sum[j + shifts] - prefix_sum[i - 1] > target_sum) {
			if (++shifts > allowed_shifts) {
				return false;
			} 
			j = 0, i = 1, num_people = num_p;
		}
		else if (prefix_sum[j + shifts] - prefix_sum[i - 1] == target_sum) {
			//cout << "Target Sum hit \n";
			i = j + 1;
			if (--num_people == 0) {
				return true;
			}
		}
	}

	return false;
} 

/*
int main2() {
	vi numbers = { 1, 7, 3, 4, 5 };
	int x = 0;
	*(numbers.data()) = 2;
	cout << numbers[0];
	return 0;
}
*/

void fuzz_test();

// 3 11 1 14 7 7 6 8 15 6 5 5 10
int main(){
	int num_people, num_beads;
	cin >> num_people >> num_beads;

	// fuzz_test();

	vi beads(num_beads);
	int sum = 0;
	for (int i = 0; i < num_beads; i++) {
		cin >> beads[i];
		sum += beads[i];
	}

	if (sum % num_people == 0) {
		int target_sum = sum / num_people;

		vi modified_beads;
		//auto [a, b] = tuple{ 1 , 2 };
		{
			int i = beads.size() - 1;
			ll end_sum = beads[i];
			for (; i > 0 && end_sum + beads[0] <= target_sum; end_sum += beads[--i])
				modified_beads.push_back(beads[i]);
		}
		//copy(modified_beads.begin(), modified_beads.end(), ostream_iterator<int>(cout, " "));

		int allowed_shifts = modified_beads.size();
		reverse(modified_beads.begin(), modified_beads.end());

		modified_beads.insert(
			modified_beads.end(),
			make_move_iterator(beads.begin()),
			make_move_iterator(beads.end())
		);

		//copy(modified_beads.begin(), modified_beads.end(), ostream_iterator<int>(cout, " "));

		if (fair_division(num_people, target_sum, allowed_shifts, modified_beads)) {
			cout << "YES";
		}
		else {
			cout << "NO";
		}
	} 
	else {
		cout << "NO";
	}
	return 0;
}

void fuzz_test() {
	random_device rd;
	default_random_engine generator(rd());
	uniform_int_distribution<long long> num_bead_distribution(1, 1e7);
	uniform_int_distribution<long long> distribution(1, 1e9);
	int num_beads = num_bead_distribution(generator);
	vector<ll> beads(num_beads);
	for (int i = 0; i < num_beads; i++) {
		beads[i] = distribution(generator);
	}

	srand(time(0));
	int num_people = rand() % 10000000;

	ofstream fuzz_data("fuzz_data.txt");
	fuzz_data << num_people << " " << num_beads << "\n";
	copy(beads.begin(), beads.end(), ostream_iterator<int>(fuzz_data, " "));
}