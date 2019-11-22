#include <cmath>
#include <iostream>

using namespace std;

double f(double x) {
	return exp(-x * x);
}

double simpson2ndRule(double a, double b, int k) {
	double n = 3*k;
	double h = (b - a) / n;
	double integral = f(a) + f(b);
	for (int i = 1; i < n; i++) {
		if (i % 3 == 0) {
			integral += 2 * f(a + i*h);
		} else {
			integral += 3 * f(a + i*h);
		}
	}

	integral *= (3.0/8.0) * h;
	return integral;
}

int main() {
	double a, b;
	int k;
	cin >> a >> b >> k;
	cout << simpson2ndRule(a, b, k) << endl;
	return 0;
}
