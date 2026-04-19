# OrthoFlow
Efficient Singular Value Decomposition (SVD) with *OrthoFlow*: A High-Performance Approach Based on Eigen and MKL and part of the *EigenNET project*

What is OrthoFlow? OrthoFlow is a toolkit for matrix dimensionality reduction based on a high-performance parallelized SVD (singular value decomposition).
But OrthoFlow is more than just an SVD!

OrthoFlow integrates a workflow that includes reading in the
<ul>
<li>data matrix and validation, </li>
<li>data scaling methods,</li>
<li>SVD using the Jacobi and BDC methods,</li>
<li>MKL for performance enhancement,</li>
<li>and output of results in CSV or binary format. </li>
</ul>

### Visual overview of OrthoFlow...

![Workflow](https://github.com/SuprenumDE/EigenNET/blob/main/images/Workflow.png).

### ... and the potential integration into an R-Dense_NN workflow when dimensionality reduction is required:

![Workflow](https://github.com/SuprenumDE/EigenNET/blob/main/images/R_Workflow.png).

