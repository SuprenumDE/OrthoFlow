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

... and the **potential integration into an R-Dense_NN workflow** when dimensionality reduction is required:

![Workflow](https://github.com/SuprenumDE/EigenNET/blob/main/images/R_Workflow.png).

The following illustration provides a more detailed overview of the **R-OrthoFlow integration**:

![Workflow_2](https://github.com/SuprenumDE/EigenNET/blob/main/images/R_Workflow_2.png).

OrthoFlow is run via an **R script** with the required and optional parameters, such as
<ul>
<li>input matrix in CSV format</li>
<li>SVD method (Jacobi, BDC, or MKL)</li>
<li>scaling method</li>
<li>threshold k for dimensionality reduction</li>
<li>and its visualization via the cumulative variance using GNUPlot</li>
<li>and, if possible, the number of threads to use.</li>
</ul>

OrthoFlow performs parameter-based SVD with the *maximum possible system performance* and saves the results in CSV and binary NPY formats.
In addition, information about the SVD operation is saved as an `svd.json` file, and the scaling method used is saved as a `scaler.json` file.
If GNUPlot is installed, information regarding the *reduction factor k* is saved as a graphic file named `scree.png`.

Of course, a log of the entire internal OrthFlow pipeline is also stored in the *log directory*. This makes it easy to determine whether, for example, loading the data or the SVD was the main factor affecting runtime. 
