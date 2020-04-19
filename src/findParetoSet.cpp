#include <Rcpp.h>
using namespace Rcpp;

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/adapted/boost_tuple.hpp>

BOOST_GEOMETRY_REGISTER_BOOST_TUPLE_CS(cs::cartesian)
typedef boost::tuple<double, double> point;
typedef boost::geometry::model::polygon<point, true, true> polygon;

namespace Rcpp {

  // as<>() converter from R --> Boost.Geometry's Polygon Type
  template <> polygon as(SEXP pointsMatrixSEXP){
    NumericMatrix pointsMatrix(pointsMatrixSEXP);
    polygon poly;
    for( int i = 0; i < pointsMatrix.nrow(); i++ ){
      double x = pointsMatrix( i , 0 );
      double y = pointsMatrix( i , 1 );
      point p(x,y);
      poly.outer().push_back(p);
    }
    return (poly);
  }

  // wrap() converter for Boost.Geometry's polygon --> R(cpp) matrix
  // So now Rcpp NumericMatrix can be converted to/from SEXP type
  template <> SEXP wrap( const polygon& poly ){
    const std::vector<point>& points = poly.outer();
    NumericMatrix rmat(points.size(), 2);
    for( unsigned int i = 0; i < points.size(); ++i ){
      const point& p = points[i];
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
  polygon poly = as<polygon>(idealPoints);
  polygon hull;
  boost::geometry::convex_hull(poly, hull);
  return wrap(hull);
}


