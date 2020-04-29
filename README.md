# MinimumRcpp

## Workflow instructions: 
* Once the repo is cloned, open the Rstudio project
* On the build tab, press ```more > Clean and Rebuild``` 
* In the RStudio Console, enter the following command: ```devtools::load_all(".")``` 
* The two functions found in ```src/``` should now be globally available in the project 

### In order to export functions: 
Ensure that, when a new function is written in the `src/` directory, to add an `//' @export` to the function Roxygen2 directives in order for the functions to properly export into the NAMESPACE. Once this is done, click on `More > Document` in Rstudio to update the package when exporting for the first time. 

Example: 
```r
//' Return foobar 
//' @param  ...
//' @export
// [[Rcpp::export]]
void foo( int bar ){
  return bar; 
}
```

