#ifndef MINIMUM_RCPP
#define MINIMUM_RCPP
#include "../inst/include/MinimumRcpp.h"

namespace Rcpp {
  // wrap() converter for Boost.Geometry's Polygon --> R(cpp) matrix
  // So now Rcpp NumericMatrix can be converted to/from SEXP type
  template <> SEXP wrap( const std::vector<Polygon2D>& poly  ){
    Rcpp::List result;
    Rcpp::DataFrame D;
    /* Generate a DF for each polygon in the polygon vector */
    for( int i = 0; i < poly.size(); i++ ){
      NumericVector x;
      NumericVector y;
      std::vector<double> xvec;
      std::vector<double> yvec;
      xvec.reserve(boost::geometry::num_points(poly[i]));
      yvec.reserve(boost::geometry::num_points(poly[i]));
      for(auto it = boost::begin(boost::geometry::exterior_ring(poly[i])); it != boost::end(boost::geometry::exterior_ring(poly[i])); ++it){
        double xval = boost::geometry::get<0>(*it);
        double yval = boost::geometry::get<1>(*it);
        xvec.push_back(xval);
        yvec.push_back(yval);
      }
      /* Push polygon into List */
      Rcpp::DataFrame D = Rcpp::DataFrame::create( Named("x") = xvec, Named("y") = yvec );
      result.push_back(D);
    }
    /* Return list of DF */
    return Rcpp::wrap(result);
  }
}

#endif
