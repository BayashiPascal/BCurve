# BCurve
BCurve is a C library to manipulate geometry based on Bezier curves of any dimension and order. 

It offers function to create, clone, load, save and modify a geometry, to print it, to scale, rotate (in 2D) or translate it, to get the weights (coefficients of each control point given the value of the parameter of the curve), and to get the bounding box. 

BCurve objects are Bezier curves from 1D to ND. For BCurve object, the library offers functions to get its approximate length (sum of distance between control points), and to create a BCurve connecting points of a point cloud.

SCurve objects are a set of BCurve (called segments) continuously connected and has the same interface as for a BCurve, plus function to add and remove segments.

BBody objects are extension of BCurve objects for the case MD to ND. If M equals 1 it is equivalent to a BCurve. If M equals 2 it is equivalent to a surface in ND. If M equals 3 it is equivalent ot a volume. Note that by using one dimension as the time dimension one can describes the movement of a curve, surface, etc... over time. The library offers the same functions for a BBody as for a BCurve.
