#ifndef KINTERSECTION_CPP
#define KINTERSECTION_CPP
#include <Rcpp.h>
#include <stdio.h>

// STL headers
#include <vector>
#include <list>

// Boost headers
#include <boost/geometry.hpp>
#include <boost/unordered_map.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

typedef boost::geometry::model::d2::point_xy<double> Point;
typedef boost::geometry::model::polygon<Point> Polygon2D;
using namespace Rcpp;

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
      /* Push polygon DF into List */
      Rcpp::DataFrame D = Rcpp::DataFrame::create( Named("x") = xvec, Named("y") = yvec );
      result.push_back(D);
    }
    /* Return list of DF */
    return Rcpp::wrap(result);
  }
}

/* Internal CPP Protoype Declaration */
template<class Container>
std::vector<Polygon2D> simplifyAndMergePolygons( Container& c, const double epsilon );
template<class Container>
std::list<Polygon2D> simplifyPolygons(Container& c, const double epsilon);
void mergePolygonsPass(std::list<Polygon2D>& c, const double epsilon);

/* Inline referencers */
inline Polygon2D& polygonRef(Polygon2D& polygon) noexcept{
  return polygon;
}

inline Polygon2D& polygonRef(std::pair<const std::set<std::size_t>, Polygon2D>& kvp) noexcept{
  return kvp.second;
}

// [[Rcpp::export]]
Rcpp::List kintersection( Rcpp::List polyList, const int k, const double epsilon ){
  /* Error checking before anything begins */
  if( k == 0 )
    Rcpp::stop("Invalid Argument: Zero-intersection is illegal");
  if( polyList.size() == 0 )
    Rcpp::stop("Invalid Argument: No polygons in list");
  if( epsilon < 0.0 )
    Rcpp::stop("Invalid Argument: Negative epsilon is not permitted");
  if( k == 1 ) {
    /* Merge polygons */
  }

  /* Parse the list of DataFrames into a vector of Polygons */
  std::vector<Polygon2D> local_plist;
  local_plist.reserve(polyList.size());
  Rcpp::DataFrame D;
  NumericVector x;
  NumericVector y;
  Point p;
  for( int i = 0; i < polyList.size(); i++ ){
    Polygon2D poly;
    D = as<DataFrame>(polyList(i));
    x = D["x"];
    y = D["y"];
    Rcout << "POLYGON " << i+1 << std::endl;
    for( int c = 0; c < x.size(); c++ ){
      boost::geometry::set<0>(p,x(c));
      boost::geometry::set<1>(p,y(c));
      boost::geometry::append(poly.outer(), p);
      Rcout << x(c) << " " << y(c) << std::endl;
    }
    local_plist.push_back(poly);
  }

  /* Data parsing complete -- check for intersections */
  Rcpp::List result;
  std::vector<Polygon2D> cresult;
  const auto n = polyList.size();
  if( k > n ){
    return result;
  } else if ( k == 2 ){
    /* Memory optimized variant for k == 2 */
    for( std::size_t i = 0; i < n; ++i ){
      for (std::size_t j = i + 1; j < n; ++j) {
        const auto initialResultSize = result.size();

        /* Obtain new intersection */
        boost::geometry::intersection(local_plist[i], local_plist[j], cresult);

        /* Skip empty intersection */
        const auto resultSizeIncrement = result.size() - initialResultSize;
        if (resultSizeIncrement == 0) continue;

        /* Error handling for the non convex polygons */
        /* Intersection of two convex polygons is either empty or another convex polygon */
        if( resultSizeIncrement != 1)
          Rcpp::stop("Invalid Argument: There is a non-convex polygon");
      }
    }
    if( cresult.size() > 1 ){
      return wrap(simplifyAndMergePolygons( cresult, epsilon ));
    }
  } else {
    /* General variant for k > 2 */
    boost::unordered_map<std::set<std::size_t>, Polygon2D> a, b;
    for (std::size_t i = 0; i < n; ++i)
      a.emplace(std::set<std::size_t> { i }, local_plist[i]);
      {
        std::list<Polygon2D> output;
        for (std::size_t i = 1; i < k && !a.empty(); ++i) {
          for (const auto& intersection : a) {
            for (std::size_t j = 0; j < n; ++j) {
              // Skip this polygon if it is already part of this intersection
              if (intersection.first.count(j) > 0) continue;

              // Skip candidate intersection if it is already known
              auto key = intersection.first;
              key.insert(j);
              if (b.count(key) > 0) continue;

              // Obtain new intersection
              output.clear();
              boost::geometry::intersection(intersection.second, local_plist[j], output);

              // Skip empty intersection
              if (output.empty()) continue;

              // Intersection of two convex polygons is either empty or another convex polygon.
              // So if we get more than one polygon in the intersection this means
              // there are some non-convex polygons, which is error.
              if (output.size() != 1)
                throw std::invalid_argument("There is non-convex polygon");

              // Store new intersection
              b.emplace(std::move(key), std::move(output.back()));
            }
          }
          // Prepare for next iteration
          a.swap(b);
          b.clear();
        }
      }
      if( !a.empty() ){
        if(a.size() > 1){
          return wrap(simplifyAndMergePolygons( a, epsilon ));
        } else {
          cresult.push_back(std::move(a.begin()->second));
        }
      }
  }
  return wrap(cresult);
}

template<class Container>
std::vector<Polygon2D> simplifyAndMergePolygons( Container& c, const double epsilon ){
  auto unions = simplifyPolygons(c, epsilon);
  while(true) {
    const auto sizeBeforeMerge = unions.size();
    mergePolygonsPass(unions, epsilon);
    if (unions.size() == sizeBeforeMerge) break;
  }
  // Move to resulting container
  std::vector<Polygon2D> result;
  if (!unions.empty()) {
    result.reserve(unions.size());
    for (auto& u: unions) {
      result.push_back(std::move(u));
    }
  }
  return result;
}

template<class Container>
std::list<Polygon2D> simplifyPolygons(Container& c, const double epsilon){
  std::list<Polygon2D> result;
  if (epsilon > 0.0) {
    for (auto& e: c) {
      Polygon2D simplifiedPolygon;
      auto& polygon = polygonRef(e);
      boost::geometry::simplify(polygon, simplifiedPolygon, epsilon);
      result.push_back(std::move(simplifiedPolygon));
    }
  } else {
    for (auto& e: c) {
      result.push_back(std::move(polygonRef(e)));
    }
  }
  return result;
}

void mergePolygonsPass(std::list<Polygon2D>& c, const double epsilon){
  // Enumerate remaining polygons
  auto unionsEnd = ++c.begin();
  auto itp = unionsEnd;
  while (itp != c.end()) {
    // Try to merge current polygon with known unions
    auto& polygon = *itp;
    bool merged = false;

    for (auto itu = c.begin(); itu != unionsEnd; ++itu) {
      auto& union_ = *itu;
      const auto sizeBeforeMerge = c.size();
      boost::geometry::union_(*itu, polygon, c);
      const auto deltaSize = c.size() - sizeBeforeMerge;

      if (deltaSize == 1) {
        // Polygon merged - simpilfy it, update union and move forward
        // to a next polygon.
        Polygon2D simplifiedPolygon;
        boost::geometry::simplify(c.back(), simplifiedPolygon, epsilon);
        std::swap(union_, simplifiedPolygon);

        c.pop_back();
        merged = true;
        ++itp;
        break;
      } else if (deltaSize == 2) {
        // Polygon not merged - dismiss merge results
        c.pop_back();
        c.pop_back();
      } else if(deltaSize > 2) {
        Rcpp::stop("Logic Error: Unexpected result of polygon union");
      }
    }
    if (!merged) {
      // Polygon could not be merged with any of present unions.
      // Therefore the only option now is to add it as a separate union.
      if (itp != unionsEnd) {
        unionsEnd = c.insert(unionsEnd, Polygon2D());
        std::swap(*unionsEnd, *itp);
        itp = c.erase(itp);
      } else {
        ++itp;
      }
      ++unionsEnd;
    }
  }
  // Retain only unions
  c.erase(unionsEnd, c.end());
}

#endif // KINTERSECTION_CPP
