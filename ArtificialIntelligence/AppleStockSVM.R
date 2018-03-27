# SVM for Apple Stocks

# Set the working directoty path
setwd("C:/Development/RProjects")

# Required Libraries
library('quantmod') # package for statistically based trading models
library(caret) # for some data handling functions like createDataPartition
library(e1071) # for svm
library(rgl) # for 3d graphics

# Get the stock data from the CSV file
appleData <- read.csv("AppleStock.csv", header = TRUE)

# convert column Date from string to date type
appleData$Date <- as.Date(as.character(appleData$Date),format="%Y-%m-%d")

# Create an empty values column
values <- numeric(nrow(appleData))

# Populate values 
yday = appleData$Open[1]
for(i in 1:nrow(appleData))
{
  today = appleData$Close[i]
  tmp = today - yday
  if(tmp > 0) 
    values[i] <- 1 # if today's value is greater than y'day
  else
    values[i] <- 0 # stock close price decreased/not changed from y'day
  yday = today
}

# add values column to appleData 
appleData["Values"] <- values

# Divide the dataset to 2: training data and testing data
# testData with set having stocks of current year 2015
testData = subset(appleData, as.numeric(substr(appleData$Date,1,4)) == 2015)

# trainData with set having stocks from 2004 to 2014
trainData = subset(appleData, as.numeric(substr(appleData$Date,1,4)) < 2015)

names(testData)[names(testData)=="Values"] <- "Actual"

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
# . "stringdot" String kernel

# Parameters for kernels are :
# . sigma inverse kernel width for the Radial Basis kernel function "rbfdot"
# and the Laplacian kernel "laplacedot".
# . degree, scale, offset for the Polynomial kernel "polydot"
# . scale, offset for the Hyperbolic tangent kernel function "tanhdot"
# . sigma, order, degree for the Bessel kernel "besseldot".
# . sigma, degree for the ANOVA kernel "anovadot".
# . length, lambda, normalized for the "stringdot" kernel

# Example for regression SVM
# svmFit = svm(Predictions ~ Volume + Close,
#              data=trainingDS,
#              type="eps-regression",      
#              kernel="polynomial", # "linear",
#              degree=2
# )

# Example for Classifier SVM
# svmFit = svm (Predictions ~ .,
#              data=trainData,
#              type="C",
#              kernel= "radial",
#              gamma=10^{-2},  # -3,-2,-1,0,1,2,3
#              cost=10^{4.5}   # 1.5,-1,0.5,2,3,4
#)

svmFit = svm (Values ~ .,
              data=trainData,
              type="C",
              kernel= "linear",
              scale=FALSE
)

summary(svmFit)
fitted(svmFit)

# plot with training data
plot(svmFit, trainData, Close ~ Volume)

# predict for testing dataset
Predicted = predict(svmFit, testData) # type="probabilities" type="class"

newTestData <- data.frame(testData,Predicted)

# plot with testing data
plot(testData, Predicted)

plot(svmFit, testData, Close ~ Volume)

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
