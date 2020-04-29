// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// findParetoSet
DataFrame findParetoSet(SEXP idealPoints);
RcppExport SEXP _MinimumRcpp_findParetoSet(SEXP idealPointsSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type idealPoints(idealPointsSEXP);
    rcpp_result_gen = Rcpp::wrap(findParetoSet(idealPoints));
    return rcpp_result_gen;
END_RCPP
}
// kintersection
Rcpp::List kintersection(Rcpp::List polyList, const int k, const double epsilon);
RcppExport SEXP _MinimumRcpp_kintersection(SEXP polyListSEXP, SEXP kSEXP, SEXP epsilonSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< Rcpp::List >::type polyList(polyListSEXP);
    Rcpp::traits::input_parameter< const int >::type k(kSEXP);
    Rcpp::traits::input_parameter< const double >::type epsilon(epsilonSEXP);
    rcpp_result_gen = Rcpp::wrap(kintersection(polyList, k, epsilon));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_MinimumRcpp_findParetoSet", (DL_FUNC) &_MinimumRcpp_findParetoSet, 1},
    {"_MinimumRcpp_kintersection", (DL_FUNC) &_MinimumRcpp_kintersection, 3},
    {NULL, NULL, 0}
};

RcppExport void R_init_MinimumRcpp(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
