
![OrthoFlow](https://github.com/SuprenumDE/EigenNET/blob/main/images/OrthoFlow.ico) <h3>Resources</h3>


Here you will find examples of how to **integrate OrthoFlow into R-Dense_NN workflows**:

![R-Workflow](https://github.com/SuprenumDE/EigenNET/blob/main/images/R_Workflow.png)

The OrthoFlow application is useful when the **matrix dimension needs to be reduced**. This reduction is reflected in the **number of parameters in a deep learning architecture** and thus impacts performance. 

The R scripts *shown here* refer to the [**Dense_NN R scripts**](https://github.com/SuprenumDE/Dense_NN/tree/main/R%20scripts).

<h3>R scripts</h3>

1. [OrthoFlow and the CIFAR10 dataset](https://github.com/SuprenumDE/OrthoFlow/blob/main/R%20scripts/CIFAR_10_OrthoFlow.R).
<p>Training a deep learning model using the CIFAR10 dataset requires a large number of model parameters to achieve sufficient prediction quality. This has a significant impact on training performance. Using an SVD, which is implemented as a high-performance feature in OrthoFlow, the dimension of the input (image) matrix can be significantly reduced without any significant loss of information. This significantly reduces the number of network parameters and substantially improves training performance. The use of OrthoFlow is described in this R script as an example.</p>



