#include <iostream>
#include <vector>
#include "GraphField.h"
#include "GraphFieldTest.h"
#include "Coord.h"
#include "Rect.h"

//using namespace std;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;

double getVolume(CoordVec points, vector<double> radius, Rect box, double poreSize)
{
	const int divisions = 7;
	
	GraphField fld();
}

vector<double> getDistribution(CoordVec points, vector<double> radius, double minPores, double maxPores, double h)
{
//	Rect box;
//	const double BIG_NUMBER = 1e8;
//	const double SMALL_NUMBER = -BIG_NUMBER;
//	for (int dim = 0; dim < iCoord::GetDefDims(); ++dim) {
//		box.minCoord[dim] = SMALL_NUMBER;
//		box.maxCoord[dim] = BIG_NUMBER;
//	}
//	int ind = 0;
//	for (CoordVec::iterator pnt = points.begin(); pnt != points.end(); ++pnt, ++ind) {
//		for (int dim = 0; dim < iCoord::GetDefDims(); ++dim) {
//			if (pnt->GetCoord()) {
//				<#statements#>
//			}
//		}
//	}
//	vector<double> result;
//	for (double poreSize = minPores; poreSize <= maxPores; poreSize += h) {
//		cout << "For pore size " << poreSize;
//		result.push_back(getVolume(points, radius, poreSize));
//		cout << " " << result.back() << endl;
//	}
//	return result;
}

int main (int argc, char * const argv[]) {
	iCoord::SetDefDims(2);
	TestField();
    return 0;
}
