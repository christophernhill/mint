#include <mntLineLineIntersector.h>
#include <vtkUnstructuredGrid.h>
#include <vtkCellLocator.h>
#include <map>
#include <algorithm>

struct TCmpFunctor {
    TCmpFunctor(const std::vector<double>& ts) {
        this->tVals = ts;
    }
    bool operator()(size_t i, size_t j) {
        return (this->tVals[i] < this->tVals[j]);
    }
    std::vector<double> tVals;
};


struct PolysegmentIter {

    /**
     * Constructor
     * @param grid instance of vtkUnstructuredGrid
     * @param locator vtkCellLocator instance attached to the above grid
     * @param p0 start point
     * @param p1 end point
     */
    PolysegmentIter(vtkUnstructuredGrid* grid, vtkCellLocator* locator, 
                    const double p0[], const double p1[]);

    /**
     * Get the integrated linear parametric coordinates
     * @return value
     */
    double getIntegratedParamCoord() const;

    /**
     * Reset the counter
     */
    void reset();


    /**
     * Next segment
     */
    void next();


    /**
     * Get the current cell Id
     * @return index
     */
    vtkIdType getCellId() const;


    /**
     * Get the current cell parametric coordinates at the beginning of segment
     * @return 2d array
     */
    const std::vector<double>& getBegCellParamCoord() const;


    /**
     * Get the current cell parametric coordinates at the end of segment
     * @return 2d array
     */
    const std::vector<double>& getEndCellParamCoord() const;


    /**
     * Get the current cell parametric coordinates at the beginning of segment
     * @return number
     */
    double getBegLineParamCoord() const;
        

    /**
     * Get the current cell parametric coordinates at the end of segment
     * @return number
     */
    double getEndLineParamCoord() const;

    /**
     * Get the coefficient accounting for duplicates
     * @return coefficient
     */
    double getCoefficient() const;
 

    /**
     * Get the current index
     * @return index
     */
    size_t getIndex() const;


    void __assignCoefficientsToSegments();

    /**
     * Collect all the intersection points
     * @param pBeg starting point
     * @param pEnd end point
     * @param cIds cell Ids (output)
     * @param lambRay line paramatric coordinates (output)
     * @param points intersection points (output)
     */
    void __collectIntersectionPoints(const double pBeg[], 
                                     const double pEnd[],
                                     std::vector<vtkIdType>& cIds,
                                     std::vector<double>& lambRays,
                                     std::vector< std::vector<double> >& points);

    /**
     * Collect and store all the line-grid intersection points
     * @param p0 starting point of the line
     * @param p1 end point of the line 
     */
    void __collectLineGridSegments(const double p0[],
                                   const double p1[]);


    // cell Ids for each intersection point
    std::vector<vtkIdType> cellIds;
    
    // 2d cell parametric coordinates for each intersection point
    std::vector< std::vector<double> > xis;

    // 1d line parametric coordinates for each intersection point
    std::vector<double> ts;

    // grid cell Ids for each segment
    std::vector<vtkIdType> segCellIds;

    // starten parametric line coordinates
    std::vector<double> segTas;
    std::vector<double> segTbs;

    // start/end cell parametric coordinates
    std::vector< std::vector<double> > segXias;
    std::vector< std::vector<double> > segXibs;

    // duplicity coefficient
    std::vector<double> segCoeffs;

    vtkUnstructuredGrid* grid;

    vtkCellLocator* locator;

    double eps;
    double eps100;
    double tol;
    double totalT;

    size_t index;
    size_t numSegs;

};
