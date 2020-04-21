#####################################################
# This is an R script I use when doing development work on the package. The end user should not have a need to use this script.
#
# Used to write and test code for looking at different voting Rules
#
#####################################################
rm(list = ls(all = TRUE))

##############################################
## LOAD THE PACKAGES (Install if not loaded):
##############################################

library(MinimumRcpp)

####################################################################
#
####################################################################

set.seed(123) # for development purposes

# 1) Create three voter data frames to test with:

staticVoterDataFrame1 <- data.frame(pointType = rep(x = "voter", 3), ID = c("V-1", "V-2", "V-3"), xLocation=c(-1/8, 7/8, 4/8), yLocation=c(3/8, 4/8, -3/8), minkoOrder=c(2, 2, 2), xSalience = c(1, 1, 1), ySalience = c(1, 1, 1), lossOrder = c(1, 1, 1) )

# 2) Find the Pareto Set of the Group of Voters using undelying Rcpp function

paretoSetS1 <- findParetoSet( cbind(staticVoterDataFrame1$xLocation, staticVoterDataFrame1$yLocation) )

paretoSetS1

p1 <- data.frame(x = c(0,0,10,10,0), y = c(0,10,10,0,0))
p2 <- data.frame(x = c(2,2,8,8,2), y = c(0,15,15,0,0))
p3 <- data.frame(x = c(0,0,20,20,0), y = c(0,5,5,0,0))
p4 <- data.frame(x = c(0,0,10,10,0), y = c(15,25,25,15,15))
p5 <- data.frame(x = c(10,5,10,15,10), y = c(0,10,20,10,0))
p6 <- data.frame(x = c(0,-7.5,0,7.5,0), y = c(0,7.5,15,7.5,0))
p.list <- list(p1,p2,p3,p4,p5,p6)
p.list

