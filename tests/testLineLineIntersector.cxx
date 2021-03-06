#include <mntLineLineIntersector.h>
#undef NDEBUG // turn on asserts
#include <cassert>
#include <cmath>

void test1() {
    // standard
    double tol = 1.e-10;
    double p0[] = {0., 0.};
    double p1[] = {2., 0.};
    double q0[] = {1., -1.};
    double q1[] = {1., 2.};
    LineLineIntersector lli;
    lli.setPoints(p0, p1, q0, q1);
    Vector<double> xi = lli.getSolution();
    assert(abs(xi[0] - 1./2.) < tol);
    assert(abs(xi[1] - 1./3.) < tol);
}

void test2() {
    // degenerate solution
    double tol = 1.e-10;
    double p0[] = {0., 0.};
    double p1[] = {2., 0.};
    double q0[] = {0., 0.};
    double q1[] = {1., 0.};
    LineLineIntersector lli;
    lli.setPoints(p0, p1, q0, q1);
    double det = lli.getDet();
    assert(abs(det) < tol);
    assert(lli.hasSolution(tol));
}

void test3() {
    // no solution
    double tol = 1.e-10;
    double p0[] =   {0., 0.};
    double p1[] =   {2., 0.};
    double q0[] =   {0., 1.};
    double q1[] =   {1., 1.};
    LineLineIntersector lli;
    lli.setPoints(p0, p1, q0, q1);
    double det = lli.getDet();
    assert(abs(det) < tol);
    assert(! lli.hasSolution(tol));
}


void testNoOverlap() {
    // no solution
    double tol = 1.e-10;
    double p0[] =   {0., 0.};
    double p1[] =   {M_PI/2., 0.};
    double q0[] =   {-2., 0.};
    double q1[] =   {-1., 0.};
    LineLineIntersector lli;
    lli.setPoints(p0, p1, q0, q1);
    double det = lli.getDet();
    assert(abs(det) < tol);
    assert(! lli.hasSolution(tol));
}


void testNoOverlap2() {
    // no solution
    double tol = 1.e-10;
    double p0[] =   {M_PI /2., 0.};
    double p1[] =   {0., 0.};
    double q0[] =   {-2., 0.};
    double q1[] =   {-1., 0.};
    LineLineIntersector lli;
    lli.setPoints(p0, p1, q0, q1);
    double det = lli.getDet();
    assert(abs(det) < tol);
    assert(! lli.hasSolution(tol));
}


void testPartialOverlap() {
    // no solution
    double tol = 1.e-10;
    double p0[] = {0., 0.};
    double p1[] =  {M_PI /2., 0.};
    double q0[] =  {-2., 0.};
    double q1[] =  {0.5, 0.};
    LineLineIntersector lli;
    lli.setPoints(p0, p1, q0, q1);
    double det = lli.getDet();
    assert(abs(det) < tol);
    assert(lli.hasSolution(tol));
    std::pair< Vector<double>, Vector<double> > p = lli.getBegEndPoints();
    Vector<double> pa = p.first;
    Vector<double> pb = p.second;

    Vector<double> dp10(2);
    Vector<double> dpap0(2);
    Vector<double> dpbq1(2);
    Vector<double> u(2);
    for (size_t i = 0; i < 2; ++i) {
        dp10[i] = p1[i] - p0[i];
        dpap0[i] = pa[i] - p0[i];
        dpbq1[i] = pb[i] - q1[i];
        u[i] = p1[i] - p0[i];
    }
    u /= dot(dp10, dp10);

    assert(std::abs(dot(dpap0, u)) < tol);
    assert(std::abs(dot(dpbq1, u)) < tol);
}


void testPartialOverlap2() {
    // no solution
    double tol = 1.e-10;
    double p0[] =  {0., 0.};
    double p1[] =  {M_PI /2., 0.};
    double q0[] =  {0.1, 0.};
    double q1[] =  {M_PI , 0.};
    LineLineIntersector lli;
    lli.setPoints(p0, p1, q0, q1);
    double det = lli.getDet();
    assert(std::abs(det) < tol);
    assert(lli.hasSolution(tol));
    std::pair< Vector<double>, Vector<double> > p = lli.getBegEndPoints();
    Vector<double> pa = p.first;
    Vector<double> pb = p.second;

    Vector<double> dp10(2);
    Vector<double> dpaq0(2);
    Vector<double> dpbp1(2);
    Vector<double> u(2);
    for (size_t i = 0; i < 2; ++i) {
        dp10[i] = p1[i] - p0[i];
        dpaq0[i] = pa[i] - q0[i];
        dpbp1[i] = pb[i] - p1[i];
        u[i] = p1[i] - p0[i];
    }
    u /= sqrt(dot(dp10, dp10));

    assert(abs(dot(dpaq0, u)) < tol);
    assert(abs(dot(dpbp1, u)) < tol);
}

void testPartialOverlap3() {
    // no solution
    double tol = 1.e-10;
    double p0[] =   {M_PI /2., 0.};
    double p1[] =   {0., 0.};
    double q0[] =   {0.1, 0.};
    double q1[] =   {M_PI , 0.};
    LineLineIntersector lli;
    lli.setPoints(p0, p1, q0, q1);
    double det = lli.getDet();
    assert(abs(det) < tol);
    assert(lli.hasSolution(tol));
    std::pair< Vector<double>, Vector<double> > pab = lli.getBegEndPoints();
    Vector<double> pa = pab.first;
    Vector<double> pb = pab.second;

    Vector<double> u(2);
    Vector<double> dpap0(2);
    Vector<double> dpbq0(2);
    Vector<double> dp10(2);
    for (size_t i = 0; i < 2; ++i) {
        u[i] = p1[i] - p0[i];
        dpap0[i] = pa[i] - p0[i];
        dpbq0[i] = pb[i] - q0[i];
        dp10[i] = p1[i] - p0[i];
    }
    u /= sqrt(dot(dp10, dp10));
    assert(abs(dot(dpap0, u)) < tol);
    assert(abs(dot(dpbq0, u)) < tol);
}


void testQInsideP() {
    // no solution
    double tol = 1.e-10;
    double p0[] =   {0., 0.};
    double p1[] =   {1., 0.};
    double q0[] =   {0.1, 0.};
    double q1[] =   {0.8, 0.};
    LineLineIntersector lli;
    lli.setPoints(p0, p1, q0, q1);
    double det = lli.getDet();
    assert(abs(det) < tol);
    assert(lli.hasSolution(tol));
    std::pair< Vector<double>, Vector<double> > pab = lli.getBegEndPoints();
    Vector<double> pa = pab.first;
    Vector<double> pb = pab.second;

    Vector<double> u(2);
    Vector<double> dp10(2);
    Vector<double> dpaq0(2);
    Vector<double> dpbq1(2);
    for (size_t i = 0; i < 2; ++i) {
        u[i] = p1[i] - p0[i];
        dp10[i] = p1[i] - p0[i];
        dpaq0[i] = pa[i] - q0[i];
        dpbq1[i] = pb[i] - q1[i];
    }
    u /= sqrt(dot(dp10, dp10));
    assert(abs(dot(dpaq0, u)) < tol);
    assert(abs(dot(dpbq1, u)) < tol);
}

void testPInsideQ() {
    // no solution
    double tol = 1.e-10;
    double p0[] =   {0.1, 0.};
    double p1[] =   {0.9, 0.};
    double q0[] =   {0., 0.};
    double q1[] =   {1., 0.};
    LineLineIntersector lli;
    lli.setPoints(p0, p1, q0, q1);
    double det = lli.getDet();
    assert(abs(det) < tol);
    assert(lli.hasSolution(tol));
    std::pair< Vector<double>, Vector<double> > pab = lli.getBegEndPoints();
    Vector<double> pa = pab.first;
    Vector<double> pb = pab.second;

    Vector<double> u(2);
    Vector<double> dp10(2);
    Vector<double> dpap0(2);
    Vector<double> dpbp1(2);
    for (size_t i = 0; i < 2; ++i) {
        u[i] = p1[i] - p0[i];
        dp10[i] = p1[i] - p0[i];
        dpap0[i] = pa[i] - p0[i];
        dpbp1[i] = pb[i] - p1[i];
    }
    u /= sqrt(dot(dp10, dp10));
    assert(abs(dot(dpap0, u)) < tol);
    assert(abs(dot(dpbp1, u)) < tol);
}


int main(int argc, char** argv) {

    test1();
    test2();
    test3();
    testNoOverlap();
    testNoOverlap2();
    testPartialOverlap();
    testPartialOverlap2();
    testPartialOverlap3();
    testQInsideP();
    testPInsideQ();

    return 0;
}
