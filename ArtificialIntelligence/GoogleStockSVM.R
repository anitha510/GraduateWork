# SVM for Google Stocks

# Set the working directoty path
setwd("C:/Development/RProjects")

# Required Libraries
library('quantmod') # package for statistically based trading models
library(caret) # for some data handling functions like createDataPartition
library(e1071) # for svm
library(rgl) # for 3d graphics

# Get the stock data from the CSV file
googleData <- read.csv("GoogleStock.csv", header = TRUE)
googleData$Date <- as.Date(as.character(googleData$Date),format="%Y-%m-%d")
googleDataset <- read.csv("GoogleStock.csv", header = TRUE, row.names = 1)

# Calculates periodic returns for the given a set of prices
googleMonthlyReturn = periodReturn(googleDataset, period = "monthly")

gDate <- numeric(nrow(googleMonthlyReturn))
gYear <- numeric(nrow(googleMonthlyReturn))
gMonth <- numeric(nrow(googleMonthlyReturn))
gValue <- numeric(nrow(googleMonthlyReturn))
gVolume <- numeric(nrow(googleMonthlyReturn))
gLow <- numeric(nrow(googleMonthlyReturn))
gHigh <- numeric(nrow(googleMonthlyReturn))

for(i in 1:nrow(googleMonthlyReturn))
{
  gLow[i] = 99999
  gDate[i] = as.Date(as.character(index(googleMonthlyReturn)[i]),format="%Y-%m-%d")
  gYear[i] = as.numeric(substr(index(googleMonthlyReturn)[i],1,4))
  gMonth[i] = as.numeric(substr(index(googleMonthlyReturn)[i],6,7))
  if(googleMonthlyReturn$monthly.returns[i] > 0) 
    gValue[i] <- 1
  else
    gValue[i] <- 0
}

googleMain <- data.frame(gDate,gYear,gMonth,gLow,gHigh,gVolume,gValue)

for(i in 1:nrow(googleData))
{
  tmpYear = as.numeric(substr(googleData$Date[i],1,4))
  tmpMonth = as.numeric(substr(googleData$Date[i],6,7))
  tmpVol = googleMain[googleMain$gYear==tmpYear & googleMain$gMonth==tmpMonth, c('gVolume')]
  googleMain[googleMain$gYear==tmpYear & googleMain$gMonth==tmpMonth, c('gVolume')] = tmpVol + googleData$Volume[i]
  tmpLow = googleMain[googleMain$gYear==tmpYear & googleMain$gMonth==tmpMonth, c('gLow')]
  if(tmpLow > googleData$Low[i])
    googleMain[googleMain$gYear==tmpYear & googleMain$gMonth==tmpMonth, c('gLow')] = googleData$Low[i]
  tmpHigh = googleMain[googleMain$gYear==tmpYear & googleMain$gMonth==tmpMonth, c('gHigh')]
  if(tmpHigh < googleData$High[i])
    googleMain[googleMain$gYear==tmpYear & googleMain$gMonth==tmpMonth, c('gHigh')] = googleData$High[i]
}

# Divide the dataset to 2: training data and testing data
# testData with set having stocks of current year 2015
testData = subset(googleMain, googleMain$gYear == 2015)

# trainData with set having stocks from 2004 to 2014
trainData = subset(googleMain, googleMain$gYear < 2015)

names(testData)[names(testData)=="gValue"] <- "Actual"

###################################### SVM ######################################
# Prepare parameters for SVM learning algo
# The higher the cost produce less support vectors, increases accuracy. 
# But we may overfit

# Type
# . "C-svc"     C classification
# . "nu-svc"    nu classification
# . "C-bsvc"    bound-constraint svm classification
# . "spoc-svc"  Crammer, Singer native multi-class
# . "kbb-svc"   Weston, Watkins native multi-class
# . "one-svc"   novelty detection
# . "eps-svr"   epsilon regression
# . "nu-svr"    nu regression
# . "eps-bsvr"  bound-constraint svm regression

# Kernel
# . "rbfdot" Radial Basis kernel "Gaussian"
# . "polydot" Polynomial kernel
# . "vanilladot" Linear kernel
# . "tnhdot" Hyperbolic tangent kernel
# . "laplacedot" Laplacian kernel
# . "besseldot" Bessel kernel
# . "anovadot" ANOVA RBF kernel
# . "splinedot" Spline kernel
# . "stringdot" String kerne

# Parameters for kernels are :
# . sigma inverse kernel width for the Radial Basis kernel function "rbfdot"
# and the Laplacian kernel "laplacedot".
# . degree, scale, offset for the Polynomial kernel "polydot"
# . scale, offset for the Hyperbolic tangent kernel function "tanhdot"
# . sigma, order, degree for the Bessel kernel "besseldot".
# . sigma, degree for the ANOVA kernel "anovadot".
# . length, lambda, normalized for the "stringdot" kernel

# Example for regression SVM
# svmFit = svm(Predictions ~ .,
#              data=trainingDS,
#              type="eps-regression",      
#              kernel="polynomial", # "linear",
#              degree=2
# )

# Example for Classifier SVM
#svmFit = svm (Predictions ~ .,
#              data=trainData,
#              type="C",
#              kernel= "radial",
#              gamma=10^{-2},  # -3,-2,-1,0,1,2,3
#              cost=10^{4.5}   # 1.5,-1,0.5,2,3,4
#)

svmFit = svm (gValue ~ .,
              data=trainData,
              type="C",
              kernel= "linear",
              scale=FALSE
)

summary(svmFit)
fitted(svmFit)

# plot with training data
plot(svmFit, trainData, gDate ~ gVolume)

# predict for testing dataset
Predicted = predict(svmFit, testData, type="class") # type="probabilities"

newTestData <- data.frame(testData,Predicted)

# plot with testing data
plot(testData, Predicted)

# Calculate the accuracy
correct=0
total=nrow(newTestData)
for(i in 1:total)
{
  if(newTestData$Actual[i] == newTestData$Predicted[i])
    correct = correct + 1
}
Accuracy = (correct/total) * 100
Accuracy
