#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include "GraphField.h"
#include "BitField.h"
#include "PlainField.h"
#include "GraphFieldTest.h"
#include "Coord.h"
#include "Rect.h"
#include "Indexer.h"
#include "CalcPlan.h"
#include <omp.h>
#include <boost/thread/mutex.hpp>

//using namespace std;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::ifstream;
using std::ios;
using boost::mutex;

typedef vector<dCoord > SphereVec;

#define SQR(x) (x)*(x)

bool is_overlapped(const dCoord & sph1, const dCoord & sph2)
{
    double r = 0;
    for (int d = 0; d < dCoord::GetDefDims()-1; ++d) {
        double r_d = (sph1[d] - sph2[d]);
        r += r_d * r_d;
    }
    double r_sum = SQR(sph1[dCoord::GetDefDims()-1] + sph2[dCoord::GetDefDims()-1]);
    const double EPS = -1e-4;
    return (r_sum-r) > EPS;
}

int divisions = 30;

CoordVec * get_map(double radius, double a)
// radius – радиус сферы
// a – сторона куба (квадрата)
{
    CoordVec * map = NULL;
    
    dCoord centre;
    int divCnt = ceil(radius/a);
    double centreCoord = divCnt / 2.0;
    for (int d = 0; d < dCoord::GetDefDims()-1; ++d) {
        centre[d] = centreCoord;
    }
    centre[dCoord::GetDefDims()-1] = radius;
    
    map = new CoordVec;
    vector<int> sz(iCoord::GetDefDims(), divCnt);
    Indexer indx(sz);
    dCoord curr_coord;
    iCoord curr_icoord;
    while (!indx.is_last()) {
        vector<int> curr_vec = indx.curr();
        for (int d = 0; d < curr_vec.size(); ++d) {
            curr_coord[d] = curr_vec[d] + 0.5;
            curr_icoord[d] = curr_vec[d] - divCnt/2;
        }
        if (is_overlapped(centre, curr_coord)) {
            map->push_back(curr_icoord);
        }
        indx.next();
    }
    return map;
}

template <typename FieldType>
void blank_space(FieldType & fld, const dCoord & center, const CoordVec * map)
{
    iCoord icenter;
    for (int d = 0; d < iCoord::GetDefDims(); ++d) {
        icenter[d] = (int)center[d];
    }
    int ind = 0;
    const int map_size = map->size();
    for (ind = 0; ind < map_size; ++ind) {
        fld.Set(icenter + map->at(ind));
    }
}

bool is_point_overlap_spheres(const SphereVec & spheres, const dCoord & curr_coord)
{
    for (int sph = 0; sph < spheres.size(); ++sph) {
        if (is_overlapped(spheres[sph], curr_coord)) {
            return true;
        }
    }
    return false;
}

template <typename FieldType>
double getVolume(const SphereVec & spheres, const Rect & box, double poreSize, double sq_len, FieldType & fld)
{
    iCoord fld_size = fld.size();
    dCoord zero_pnt;
    dCoord scale;
    mutex io_mutex;
    
    int prev_cells_cnt = fld.CountSet();
    
    double radius = poreSize/2.0;
    int divCnt = ceil(poreSize/sq_len);
    printf("Division count = %d\n", divCnt);
    for (int dim = 0; dim < iCoord::GetDefDims(); ++dim) {
        scale[dim] = sq_len;
        zero_pnt[dim] = box.minCoord[dim];
    }
    scale[iCoord::GetDefDims()] = 1; // scale of radius
    
    CoordVec * map = get_map(radius, sq_len);
    // Mom, forgive me!
    if (iCoord::GetDefDims() == 2) {
        for (double i = divCnt / 2.0; i <= fld_size[0]-divCnt/2.0; i+=1) {
            for (double j = divCnt / 2.0; j <= fld_size[1] - divCnt/2.0; j+=1) {
                dCoord curr_coord(i, j, radius);
                if (!is_point_overlap_spheres(spheres, curr_coord * scale + zero_pnt)) {
                    blank_space(fld, curr_coord, map);
                }
            }
        }
    } else {
        const int x_steps_cnt = fld_size[0] - divCnt;
//#pragma omp parallel for
        for (int i = 0; i <= x_steps_cnt; i += 1) {
            double x = i + divCnt / 2.0;
            for (double j = divCnt / 2.0; j <= fld_size[1] - divCnt/2.0; j += 1) {
                for (double k = divCnt / 2.0; k <= fld_size[2] - divCnt/2.0; k += 1) {
                    dCoord curr_coord(x, j, k, radius);
                    
                    if (!is_point_overlap_spheres(spheres, curr_coord * scale + zero_pnt)) {
                        blank_space(fld, curr_coord, map);
                    }
                }
            }
            // for debug, verbose view:
            io_mutex.lock();
            {
                cout << "Line " << i << " of " << x_steps_cnt << " in thread " << 
                omp_get_thread_num() << endl;
            }
            io_mutex.unlock();
        }
    }
    
    delete map;
    double one_cell_vol = pow(sq_len, iCoord::GetDefDims());
    int cells_cnt = fld.CountSet() - prev_cells_cnt;
    return cells_cnt * one_cell_vol;
}

vector<double> getDistribution(const SphereVec & spheres, double minPores, double maxPores, double h)
{
    Rect box;
    const double BIG_NUMBER = 1e8;
    const double SMALL_NUMBER = -BIG_NUMBER;
    for (int dim = 0; dim < iCoord::GetDefDims(); ++dim) {
        box.minCoord[dim] = BIG_NUMBER;
        box.maxCoord[dim] = SMALL_NUMBER;
    }
    for (int pnt = 0; pnt < spheres.size(); ++pnt) {
        for (int dim = 0; dim < iCoord::GetDefDims(); ++dim) {
            if (spheres[pnt].GetCoord(dim) - spheres[pnt].GetCoord(dCoord::GetDefDims() - 1) < box.minCoord[dim]) {
                box.minCoord[dim] = spheres[pnt].GetCoord(dim) - spheres[pnt].GetCoord(dCoord::GetDefDims() - 1);
            }
            if (spheres[pnt].GetCoord(dim) + spheres[pnt].GetCoord(dCoord::GetDefDims() - 1) > box.maxCoord[dim]) {
                box.maxCoord[dim] = spheres[pnt].GetCoord(dim) + spheres[pnt].GetCoord(dCoord::GetDefDims() - 1);
            }
        }
    }
    
    vector<double> result;
    double sq_len = h/divisions;
    // Create one field per all sizes
    iCoord fld_size;
    size_t full_size = 1;
    for (int dim = 0; dim < iCoord::GetDefDims(); ++dim) {
        fld_size[dim] = (int)((box.maxCoord[dim] - box.minCoord[dim]) / sq_len + 1);
        full_size *= fld_size[dim];
    }
    // maximum size for plain field of chars = 2 Gb
    Field<char> * fld = NULL;
    if (full_size < ((long)2 << 30))  {
        fld = new PlainField<char>(fld_size);
    }   else if (full_size < ((long)2 << 33) )   {
        // maximum size for BitField = 16 Gbit
        fld = new BitField<char>(fld_size);
    }   else    {
        fld = new GraphField<char>(fld_size);
    }
    cout << "Field size: " << fld_size << endl;
    
    vector<double> poreSizes;
    for (double poreSize = minPores; poreSize <= maxPores; poreSize += h) {
        poreSizes.push_back(poreSize);
    }
    for (int poreSizeIdx = poreSizes.size()-1; poreSizeIdx >= 0; --poreSizeIdx) {
        cout << "For pore size " << poreSizes[poreSizeIdx] << endl;
        result.push_back(getVolume(spheres, box, poreSizes[poreSizeIdx], sq_len, fld));
        cout << "Result: " << result.back() << endl;
    }
    return result;
}

bool exists(const char *fname)
{
    if( access( fname, F_OK ) != -1 ) {
        return true;
    } else {
        return false;
    }
}

template <typename T>
void load_coords(const char * filename, SphereVec * v)
{
    if (filename == NULL || v == NULL) {
        fprintf(stderr, "Load coords error! Wrong args\n");
        exit(10);
    }
    
    FILE *ifile;
    T *buffer;
    unsigned long fileLen;
    int type_size = sizeof(T);
    
    ifile = fopen(filename, "rb");
    if (!ifile) {
        fprintf(stderr, "No such file: %s\n", filename);
        exit(11);
    }
    
    fseek(ifile, 0, SEEK_END);
    fileLen=ftell(ifile);
    fseek(ifile, 0, SEEK_SET);
    
    if (fileLen % type_size != 0 || fileLen/type_size % dCoord::GetDefDims() != 0) {
        fprintf(stderr, "Wrong file: %s\n", filename);
        exit(12);
    }
    
    buffer=(T *)malloc(dCoord::GetDefDims() * type_size);
    if (!buffer)
    {
        fprintf(stderr, "Memory error!");
        fclose(ifile);
        exit(13);
    }
    
    v->clear();
    while (fread(buffer, dCoord::GetDefDims(), type_size, ifile)) {
        dCoord curr;
        for (int i = 0; i < dCoord::GetDefDims(); ++i) {
            curr[i] = (double)buffer[i];
        }
        v->push_back(curr);
    }
    
    fclose(ifile);
    
    cout << v->size() << " points added\n";
}

int main (int argc, char * const argv[]) {
    // iCoord::SetDefDims(2);
    // TestField();
    iCoord::SetDefDims(3);
    dCoord::SetDefDims(iCoord::GetDefDims()+1);
    SphereVec v;
    TCalcPlan plan(argc, argv);
    divisions = plan.divisions;
    
    if (plan.is_float) {
        load_coords<float>(plan.filename, &v);
    } else {
        load_coords<double>(plan.filename, &v);
    }
    
    vector<double> distr = getDistribution(v, 1.0, 50.0, 1.0);
    for (int i =0 ; i < distr.size(); ++i) {
        cout << distr[i] << " ";
    }
    cout << endl;
    return 0;
}
