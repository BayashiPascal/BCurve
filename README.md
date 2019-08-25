# BCurve
BCurve is C library to manipulate geometry based on Bezier curves of any dimension and order.

It offers function to create, clone, load and save (JSON format), and modify a curve or surface, to print it, to scale, rotate (in 2D) or translate it, to get the weights (coefficients of each control point given the value of the parameter of the curve), and to get the bounding box.

BCurve objects are Bezier curves from 1D to ND. For BCurve object, the library offers functions to get its approximate length (sum of distance between control points), and to create a BCurve connecting points of a point cloud.

SCurve objects are a set of BCurve (called segments) continuously connected and has the same interface as a BCurve, plus function to add and remove segments, and apply the Chaikin subdivision algorithm on curve of order 1.

BBody objects are extension of BCurve objects for the case M dimensions to N dimensions. If M equals 1 it is equivalent to a BCurve. If M equals 2 it is equivalent to a surface in N dimension. If M equals 3 it is equivalent ot a volume. Note that by using one dimension as the time dimension one can describes the movement of a curve, surface, etc... over time. The library offers the same functions for a BBody as for a BCurve.

## How to install this repository
1) Create a directory which will contains this repository and all the repositories it is depending on. Lets call it "Repos"
2) Download the master branch of this repository into "Repos". Unzip it if necessary.
3) The folder's name for the repository should be followed by "-master", rename the folder by removing "-master".
4) cd to the repository's folder
5) If wget is not installed: ```sudo apt-get update && sudo apt-get install wget``` on Ubuntu 16.04, or ```brew install wget``` on Mac OSx
6) If gawk is not installed: ```sudo apt-get update && sudo apt-get install gawk```  on Ubuntu 16.04, ```brew install gawk``` on Mac OSx
7) If this repository is the first one you are installing in "Repos", run the command ```make -k pbmake_wget```
8) Run the command ```make``` to compile the repository. 
9) Eventually, run the command ```main``` to run the unit tests and check everything is ok.
10) Refer to the documentation to learn how to use this repository.

The dependancies to other repositories should be resolved automatically and needed repositories should be installed in the "Repos" folder. However this process is not completely functional and some repositories may need to be installed manually. In this case, you will see a message from the compiler saying it cannot find some headers. Then install the missing repository with the following command, e.g. if "pbmath.h" is missing: ```make pbmath_wget```. The repositories should compile fine on Ubuntu 16.04. On Mac OSx, there is currently a problem with the linker.
If you need assistance feel free to contact me with my gmail address: at bayashipascal.
