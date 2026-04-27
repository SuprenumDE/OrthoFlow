################################################################################
##    Training a deep learning model using an R-OrthoFlow-Dense_NN workflow
##                            (PoC)    
##
##
## Ad-Oculos-Projekt, https://www.faes.de/ad-oculos/
## GitHub: https://github.com/SuprenumDE/OrthoFlow
##
## Günter Faes, eigennet@faes.de
## Version 0.0.3, 25.04.2026
## R-Version: 4.5.3  
## OS Windows 11
##
################################################################################

### Clear all variables:
rm(list = ls())

### Required packages:
library(reticulate)  # Read *.npy files
np <- import("numpy")

library(grid)
library(gridExtra)
library(ggplot2)     # Head map



# The path to my R working directory (the bin directory containing Dense_NN.exe):
# setwd("F:/R-Projekte/Calling_C_NN/bin")
setwd("C:/Users/guent/Documents/R-Projekte/Calling_C_NN/bin")

############################ Prepare data ##########################

# The CIFAR10 dataset is used for this example. For more information,
# see also https://www.cs.toronto.edu/~kriz/cifar.html)

# Loading the datasets from the “Dense_NN, CIFAR-10, and SVD” example on
# YouTube or the resources at
# https://github.com/SuprenumDE/Dense_NN/tree/main/R%20scripts.
# Training:
df_train <- read.table("CIFAR_train.csv", sep=",")
# Test:
df_test <- read.table("CIFAR_test.csv", sep=",")

# Create and save the complete CIFAR10 dataset for SVD using OrthoFlow:
df_CIFAR10_completely <- rbind(df_train, df_test)
# For security reasons, the entire dataset is shuffled again:
set.seed(123)
df_CIFAR10_completely <- df_CIFAR10_completely[sample(nrow(df_CIFAR10_completely)), ]
# Save as CSV file:
write.table(na.omit(df_CIFAR10_completely), file = "CIFAR10_Dataset.csv", sep = ",", row.names = FALSE, col.names = FALSE)


################### Perform SVD with OrthoFlow ##########################

SVDInputFile <- "CIFAR10_Dataset.csv"
SVDScaler <- "minmax"
SVDVariant <- "mkl"
SVDThreshold <- "0.99"
SVDPlot <- "gnuplot"

system2("OrthoFlow.exe",
        args = c(
          paste0("--file=", SVDInputFile), 
          paste0("--scaler=", SVDScaler), 
          paste0("--svd-variant=", SVDVariant),
          paste0("--svd-threshold=", SVDThreshold),
          paste0("--svd-plot-mode=", SVDPlot)
        ),
        stdout = "",
        stderr = ""
)


#########################################################################
#      Split the CIFAR10 dataset into training and test data            #
#########################################################################

SVD_Output <- read.table("C:/Users/guent/Documents/R-Projekte/Calling_C_NN/bin/orthoflow_output/dense_input.csv", sep=",")

n <- nrow(SVD_Output)
train_idx <- sample(seq_len(n), size = floor(0.85 * n))

df_train <- SVD_Output[train_idx, ]
df_test  <- SVD_Output[-train_idx, ]

# Train:
write.table(na.omit(df_train), file = "CIFAR10_train.csv", sep = ",", row.names = FALSE, col.names = FALSE)
# Test:
write.table(na.omit(df_test), file = "CIFAR10_test.csv", sep = ",", row.names = FALSE, col.names = FALSE)

#########################################################################
#                         Perform training                              #
#########################################################################

### Setting the Dense_NN function parameters:
TrainingDataSet <- "C:/Users/guent/Documents/R-Projekte/Calling_C_NN/bin/CIFAR10_train.csv"
architecture <- "232,150,75,10"
ActivationFunctions <- "relu,relu,softmax"
Optimizer <- "RMSProp"        # or SGD/RMSProp/Adam
Opti_RMSProp_rho = "0.9"      # paste0("--rho=", Opti_RMSProp_rho),
Opti_Adam_b1 = "0.9"          # paste0("--beta1=", Opti_Adam_b1),
Opti_Adam_b2 = "0.999"        # paste0("--beta2=", Opti_Adam_b2),
Loss <- "CROSS_ENTROPY"
epochs <- "200"
n_TrainingSample <- "-1"
BatchSize <- "64"
LearningRate <- "0.001"
LR_dynamic <- "decay"
MinDelta <- "0.0001"
WeightInitialization <- "HE"

system2("Dense_NN.exe",
        args = c(
          "--mode=1",
          paste0("--architecture=", architecture), 
          paste0("--activations=", ActivationFunctions), 
          paste0("--optimizer=", Optimizer),
          paste0("--loss=", Loss), 
          paste0("--dataset=", TrainingDataSet), 
          paste0("--epochs=", epochs), 
          paste0("--samples=", n_TrainingSample),
          paste0("--batch=", BatchSize), 
          paste0("--learning_rate=", LearningRate),
          paste0("--lr_mode=", LR_dynamic),
          paste0("--min_delta=", MinDelta), 
          paste0("--init=", WeightInitialization)
        ),
        stdout = "",
        stderr = ""
)

 #########################################################################
#
# To graphically display the training parameters and weight matrices,
# run the entire R script “Display_Training_Progress.R.”
#
#########################################################################

#########################################################################
#                          Network Test                                 #
#########################################################################


TestDataSet <- "C:/Users/guent/Documents/R-Projekte/Calling_C_NN/bin/CIFAR10_test.csv"

system2("Dense_NN.exe",
        args = c(
          "--mode=2", 
          paste0("--dataset=",TestDataSet)
        ),
        stdout = "",
        stderr = ""
)

#
###################### Review of test results #########################

# CIFAR-10 Class labels:
cifar10_labels <- c(
  "airplane",   # 0
  "automobile", # 1
  "bird",       # 2
  "cat",        # 3
  "deer",       # 4
  "dog",        # 5
  "frog",       # 6
  "horse",      # 7
  "ship",       # 8
  "truck"       # 9
)


forecast <- read.csv("test_results.csv")
View(forecast)

# Overall prediction accuracy (VG):
average_forecast <- mean(forecast$true_label == forecast$predicted_label)
average_forecast


### Presentation as a tabular overview:
# Absolutely:
tabular_forecast <- table(forecast$true_label, forecast$predicted_label)
dimnames(tabular_forecast) <- list(
  True = cifar10_labels,
  Predicted = cifar10_labels
)
tabular_forecast

# Relative (%):
tabular_forecast_relative <- prop.table(tabular_forecast, margin = 1) * 100
round(tabular_forecast_relative, 1)

# Overall view based on 100:
tabular_forecast_total <- prop.table(tabular_forecast) * 100
round(tabular_forecast_total, 2)


### Confusion Matrix:

df_conf <- as.data.frame(as.table(tabular_forecast_relative))

# Plot
ggplot(df_conf, aes(x = Predicted, y = True, fill = Freq)) +
  geom_tile(color = "white") +
  scale_fill_gradient(low = "white", high = "steelblue") +
  geom_text(aes(label = round(Freq, 1)), size = 3) +
  theme_minimal() +
  labs(title = "CIFAR-10 Confusion Matrix (%)",
       x = "Predicted Label",
       y = "True Label")


#### EoS
