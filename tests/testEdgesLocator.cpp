#include <mntUgridEdgeReader.h>
#include <mntEdgesLocator.h>
#undef NDEBUG // turn on asserts
#include <cassert>

void test4() {

    UgridEdgeReader uer;
    uer.load("${CMAKE_SOURCE_DIR}/data/cs_4.nc");
    std::cout << "Number of edges: " << uer.getNumberOfEdges() << '\n';
    double xmin[3], xmax[3];
    uer.getRange(xmin, xmax);
    std::cout << "Domain range: " << xmin[0] << ',' << xmin[1] << ',' << xmin[2] << " -> "
                                  << xmax[0] << ',' << xmax[1] << ',' << xmax[2] << '\n';


    EdgesLocator el;
    el.setRange(xmin, xmax);
    int numEdgesPerBucket = 1;
    el.build(uer.getEdgePoints(), numEdgesPerBucket);

    double p0[3], p1[3];

    size_t ncases = 3;
    double pab[] = {-50., -90., 0.,  -50., +90., 0.,    // line is outside
                    -90.,   0., 0.,  -90.,   0., 0.,    // line has zero length
                      0., -90., 0.,  360., +90., 0.};   // line traverses domain

    for (size_t icase = 0; icase < ncases; ++icase) {

        double* pa = &pab[icase*6 + 0];
        double* pb = &pab[icase*6 + 3];

        std::set<vtkIdType> edgeIds = el.getEdgesAlongLine(pa, pb);

        std::cout << "Found " << edgeIds.size() << " edges that likely intersect line "
              << pa[0] << ',' << pa[1] << ',' << pa[2] << " -> "
              << pb[0] << ',' << pb[1] << ',' << pb[2] << '\n';

        for (auto iet = edgeIds.begin(); iet != edgeIds.end(); ++iet) {
            uer.getEdge(*iet, p0, p1);
            std::cout << "edge Id " << *iet << " points " << p0[0] << ',' << p0[1] << " -> " << p1[0] << ',' << p1[1] << '\n';
        }
    }
}

int main(int argc, char** argv) {

	test4();

    return 0;
}
