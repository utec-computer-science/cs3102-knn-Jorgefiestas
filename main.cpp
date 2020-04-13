#include <iostream>
#include <vector>
#include <time.h>
#include <utility>
#include <algorithm>
#include <set>
#include <cmath>
#include <chrono> 
#include <stdio.h>
#include <unistd.h>

template <typename T>
class CartesianCoord {
	public:
		typedef T value_t;
		value_t x;
		value_t y;
		
		CartesianCoord(value_t x = 0, value_t y = 0) :
			x(x), y(y) {}

		~CartesianCoord(void) {
		}

		template<typename _T>
		friend std::ostream& operator <<(std::ostream &out, const CartesianCoord<_T> c){
			out << "(" << c.x << "," << c.y << ")";
			return out;
		}
};


typedef CartesianCoord<int> coord_t;

float euclideanDistance(coord_t p1, coord_t p2) {
	float dx = (float) (p1.x - p2.x);
	float dy = (float) (p1.y - p2.y);

	return sqrt(dx * dx + dy * dy);
}

std::vector<coord_t> knn (int k, const std::vector<coord_t> &points, const coord_t &q) {
	std::vector<std::pair<float, size_t>> distances;

	for (size_t i = 0; i < points.size(); i++) {
		coord_t coor = points[i];
		float dist = euclideanDistance(coor, q);
		std::pair<float, size_t> distPoint = {dist, i};
		distances.emplace_back(distPoint);
	}

	std::nth_element(
			distances.begin(), 
			distances.begin() + k - 1, 
			distances.end()
			);
	
	std::vector<coord_t> nns(k);
	for (size_t i = 0; i < k; i++) {
		nns[i] = points[distances[i].second];
	}
	
	return nns;
}


int main() {
	srand (time(NULL));
	
	std::vector<coord_t> points;
	for (int i = 0; i < 10000000; i++) {
		points.push_back(coord_t(rand() % 1000, rand() % 1000));
	}
		
	std::vector<coord_t> knns;

	auto start = std::chrono::high_resolution_clock::now(); 
	knns = knn(3, points, coord_t(100, 200));
	auto stop = std::chrono::high_resolution_clock::now(); 

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	std::cout << "Time: " << std::endl;
	std::cout << duration.count() << std::endl;
	std::cout << "knns" << std::endl;
	for (auto kit = knns.begin(); kit != knns.end(); kit++) {
		std::cout << *kit << std::endl; 
	}

	return 0;
}
