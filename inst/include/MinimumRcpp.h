#ifndef MINIMUM_RCPP_H
#define MINIMUM_RCPP_H

/* RcppCommon first so we can define our wrappers within the Rcpp namespace */
#include <RcppCommon.h>

/* Boost Headers */
#include <boost/geometry.hpp>
#include <boost/unordered_map.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

/* Typedefs for convenience */
typedef boost::geometry::model::d2::point_xy<double> Point;
typedef boost::geometry::model::polygon<Point> Polygon2D;

namespace Rcpp {
  template <> SEXP wrap( const std::vector<Polygon2D>& poly  );
}

/* Do not remove this inlcude header -- it is necessary to declare the namespace wrappers between RcppCommon and Rcpp */
#include <Rcpp.h>
#endif
