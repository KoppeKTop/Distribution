#include <iostream>
#include <vector>
#include "GraphField.h"
#include "GraphFieldTest.h"
#include "Coord.h"
#include "Rect.h"
#include "Indexer.h"

//using namespace std;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;

typedef vector<Coord<double> > SphereVec;

bool is_overlapped(const dCoord & sph1, const dCoord & sph2)
{
	double r = 0;
	for (int d = 0; d < dCoord::GetDefDims()-1; ++d) {
		double r_d = (sph1[d] + sph2[d]);
		r += r_d * r_d;
	}
	double r_sum = sph1[dCoord::GetDefDims()-1] + sph2[dCoord::GetDefDims()-1];
	return r < r_sum * r_sum;
}

const int divisions = 7;

CoordVec * get_map()
{
	static CoordVec * map = NULL;
	if (map) {
		return map;
	}
	map = new CoordVec;
	for (int; <#condition#>; <#increment#>) {
		<#statements#>
	}

void blank_space(GraphField & fld, dCoord & center)
{
	
}


double getVolume(const SphereVec & spheres, const Rect & box, double poreSize)
{
	iCoord fld_size;
	
	for (int dim = 0; dim < iCoord::GetDefDims(); ++dim) {
		fld_size[dim] = (int)((box.maxCoord[dim] - box.minCoord[dim]) * divisions / poreSize + 1);
	}
	
	GraphField fld(fld_size);
	// Mom, forgive me!
	if (iCoord::GetDefDims() == 2) {
		for (double i = divisions / 2.0; i < fld_size[0]-divisions/2.0; i+=1) {
			for (double j = divisions / 2.0; j < fld_size[1] - divisions/2.0; j+=1) {
				dCoord curr_coord(i, j, divisions/2.0);
				bool over = false;
				for (SphereVec::iterator sph = spheres.begin(); sph != spheres.end(); ++sph) {
					if (is_overlapped(*sph, curr_coord)) {
						over = true;
						break;
					}
				}
				if (!over) {
					blank_space(fld, curr_coord);
				}
			}
		}
	} else {
		for (int i = divisions / 2; i < fld_size[0]-divisions/2; ++i) {
			for (int j = divisions / 2; j < fld_size[1] - divisions/2; ++j) {
				for (int k = divisions / 2; k < fld_size[2] - divisions/2; ++k) {
					
				}
			}
		}
	}

	delete fld_size_vec;
}

vector<double> getDistribution(const & SphereVec spheres, double minPores, double maxPores, double h)
{
	Rect box;
	const double BIG_NUMBER = 1e8;
	const double SMALL_NUMBER = -BIG_NUMBER;
	for (int dim = 0; dim < iCoord::GetDefDims(); ++dim) {
		box.minCoord[dim] = BIG_NUMBER;
		box.maxCoord[dim] = SMALL_NUMBER;
	}
	for (SphereVec::iterator pnt = spheres.begin(); pnt != spheres.end(); ++pnt) {
		for (int dim = 0; dim < iCoord::GetDefDims(); ++dim) {
			if (pnt->GetCoord(dim) - pnt->GetCoord(iCoord::GetDefDims() - 1) < box.minCoord[dim]) {
				box.minCoord[dim] = pnt->GetCoord(dim) - pnt->GetCoord(iCoord::GetDefDims() - 1);
			} else if (pnt->GetCoord(dim) + pnt->GetCoord(iCoord::GetDefDims() - 1) > box.maxCoord[dim] {
				box.maxCoord[dim] = pnt->GetCoord(dim) + pnt->GetCoord(iCoord::GetDefDims() - 1);
			}
		}
	}
	
	vector<double> result;
	for (double poreSize = minPores; poreSize <= maxPores; poreSize += h) {
		cout << "For pore size " << poreSize;
		result.push_back(getVolume(spheres, poreSize));
		cout << " " << result.back() << endl;
	}
	return result;
}

int main (int argc, char * const argv[]) {
	iCoord::SetDefDims(2);
	TestField();
    return 0;
}
