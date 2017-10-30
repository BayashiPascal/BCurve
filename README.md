# BCurve
BCurve is a C library to manipulate Bezier curves of any dimension and order.

It offers function to create, clone, load, save and modify a curve, to print
it, to scale, rotate (in 2D) or translate it, to get its approximate length (sum
of distance between control points), to create a BCurve connecting points of
a point cloud, to get the weights (coefficients of each control point given the
value of the parameter of the curve), and to get the bounding box.

The library also includes a SCurve structure which is simply a GSet<BCurve> to manipulate a set of curves.
