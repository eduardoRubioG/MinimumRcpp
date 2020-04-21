#ifndef FINDPARETOSET__
#define FINDPARETOSET__
#include <Rcpp.h>
using namespace Rcpp;

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/adapted/boost_tuple.hpp>

BOOST_GEOMETRY_REGISTER_BOOST_TUPLE_CS(cs::cartesian)
typedef boost::tuple<double, double> Point;
typedef boost::geometry::model::polygon<Point, true, true> Polygon;

namespace Rcpp {

  // as<>() converter from R --> Boost.Geometry's Polygon Type
  template <> Polygon as(SEXP pointsMatrixSEXP){
    NumericMatrix pointsMatrix(pointsMatrixSEXP);
    Polygon poly;
    for( int i = 0; i < pointsMatrix.nrow(); i++ ){
      double x = pointsMatrix( i , 0 );
      double y = pointsMatrix( i , 1 );
      Point p(x,y);
      poly.outer().push_back(p);
    }
    return (poly);
  }

  // wrap() converter for Boost.Geometry's Polygon --> R(cpp) matrix
  // So now Rcpp NumericMatrix can be converted to/from SEXP type
  template <> SEXP wrap( const Polygon& poly ){
    const std::vector<Point>& points = poly.outer();
    NumericMatrix rmat(points.size(), 2);
    for( unsigned int i = 0; i < points.size(); ++i ){
      const Point& p = points[i];
      rmat(i,0) = p.get<0>();
      rmat(i,1) = p.get<1>();
    }
    return Rcpp::wrap(rmat);
  }
} // End the Rcpp Namespace wrapper definition here

//' Find the pareto set, or the convex hull of the input data
//' @param idealPoints ...
// [[Rcpp::export]]
NumericMatrix findParetoSet( SEXP idealPoints ){
  Polygon poly = as<Polygon>(idealPoints);
  Polygon hull;
  boost::geometry::convex_hull(poly, hull);
  return wrap(hull);
}
#endif

