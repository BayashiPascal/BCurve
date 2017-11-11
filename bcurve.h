// ============ BCURVE.H ================

#ifndef BCURVE_H
#define BCURVE_H

// ================= Include =================

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "pbmath.h"
#include "gset.h"

// ================= Define ==================

// ================= Data structure ===================

typedef struct BCurve {
  // Order
  int _order;
  // Dimension
  int _dim;
  // array of (_order + 1) control points defining the curve
  VecFloat **_ctrl;
} BCurve;

typedef struct SCurve {
  // Dimension
  int _dim;
  // Set of BCurve
  GSet *_curves;
} SCurve;

typedef struct BSurf {
  // Order
  int _order;
  // Dimensions (input/output)
  VecShort *_dim;
  // ((_order + 1) ^ _dim[0]) control points of the surface
  VecFloat **_ctrl;
} BSurf;

// ================ Functions declaration ====================

// Create a new BCurve of order 'order' and dimension 'dim'
// Return NULL if we couldn't create the BCurve
BCurve* BCurveCreate(int order, int dim);

// Clone the BCurve
// Return NULL if we couldn't clone the BCurve
BCurve* BCurveClone(BCurve *that);

// Load the BCurve from the stream
// If the BCurve is already allocated, it is freed before loading
// Return 0 in case of success, or:
// 1: invalid arguments
// 2: can't allocate memory
// 3: invalid data
// 4: fscanf error
// 5: VecLoad error
int BCurveLoad(BCurve **that, FILE *stream);

// Save the BCurve to the stream
// Return 0 upon success, else
// 1: invalid arguments
// 2: fprintf error
// 3: VecSave error
int BCurveSave(BCurve *that, FILE *stream);

// Free the memory used by a BCurve
// Do nothing if arguments are invalid
void BCurveFree(BCurve **that);

// Print the BCurve on 'stream'
// Do nothing if arguments are invalid
void BCurvePrint(BCurve *that, FILE *stream);

// Set the value of the iCtrl-th control point to v
// Do nothing if arguments are invalid
void BCurveSet(BCurve *that, int iCtrl, VecFloat *v);

// Get the value of the BCurve at paramater 'u' (in [0.0, 1.0])
// Return NULL if arguments are invalid or malloc failed
// if 'u' < 0.0 it is replaced by 0.0
// if 'u' > 1.0 it is replaced by 1.0
VecFloat* BCurveGet(BCurve *that, float u);

// Get the order of the BCurve
// Return -1 if argument is invalid
int BCurveOrder(BCurve *that);

// Get the dimension of the BCurve
// Return 0 if argument is invalid
int BCurveDim(BCurve *that);

// Get the approximate length of the BCurve (sum of dist between
// control points)
// Return 0.0 if argument is invalid
float BCurveApproxLen(BCurve *that);

// Rotate the curve CCW by 'theta' radians relatively to the origin
// Do nothing if arguments are invalid
void BCurveRot2D(BCurve *that, float theta);

// Scale the curve by 'v' relatively to the origin
// Do nothing if arguments are invalid
void BCurveScale(BCurve *that, VecFloat *v);

// Translate the curve by 'v'
// Do nothing if arguments are invalid
void BCurveTranslate(BCurve *that, VecFloat *v);

// Create a BCurve which pass through the points given in the GSet 'set'
// The GSet must contains VecFloat of same dimensions
// The BCurve pass through the points in the order they are given
// in the GSet. The points don't need to be uniformly distributed
// The created BCurve is of same dimension as the VecFloat and of order 
// equal to the number of VecFloat in 'set' minus one
// Return NULL if it couldn't create the BCurve or the arguments are
// invalid
BCurve* BCurveFromCloudPoint(GSet *set);

// Get a VecFloat of dimension equal to the number of control points
// Values of the VecFloat are the weight of each control point in the 
// BCurve given the curve's order and the value of 't' (in [0.0,1.0])
// Return null if the arguments are invalid or memory allocation failed
VecFloat* BCurveGetWeightCtrlPt(BCurve *that, float t);

// Get the bounding box of the BCurve.
// Return a Facoid whose axis are aligned on the standard coordinate 
// system.
// Return NULL if arguments are invalid.
Shapoid* BCurveGetBoundingBox(BCurve *that);

// Create a new SCurve of dimension 'dim'
// Return NULL if we couldn't create the SCurve
SCurve* SCurveCreate(int dim);

// Clone the SCurve
// Return NULL if we couldn't clone the SCurve
SCurve* SCurveClone(SCurve *that);

// Load the SCurve from the stream
// If the SCurve is already allocated, it is freed before loading
// Return 0 in case of success, or:
// 1: invalid arguments
// 2: can't allocate memory
// 3: invalid data
// 4: fscanf error
// 5: BCurveLoad error
int SCurveLoad(SCurve **that, FILE *stream);

// Save the SCurve to the stream
// Return 0 upon success, else
// 1: invalid arguments
// 2: fprintf error
// 3: BCurveSave error
int SCurveSave(SCurve *that, FILE *stream);

// Free the memory used by a SCurve
// Do nothing if arguments are invalid
void SCurveFree(SCurve **that);

// Print the SCurve on 'stream'
// Do nothing if arguments are invalid
void SCurvePrint(SCurve *that, FILE *stream);

// Set the 'iCurve'-th BCurve to a clone of 'curve'
// 'iCurve' must be in [0, current number of BCurve]
// 'curve' 's dimension must be equal to SCurve's dimension
// Do nothing if arguments are invalid
void SCurveSet(SCurve *that, int iCurve, BCurve *curve);

// Append a clone of 'curve'
// 'curve' 's dimension must be equal to SCurve's dimension
// Do nothing if arguments are invalid
void SCurveAdd(SCurve *that, BCurve *curve);

// Remove the 'iCurve'-th BCurve from the SCurve
// Return NULL if arguments are invalid
BCurve* SCurveRemove(SCurve *that, int iCurve);

// Get the 'iCurve'-th BCurve of the SCurve without removing it
// Return NULL if arguments are invalid
BCurve* SCurveGet(SCurve *that, int iCurve);

// Get the number of BCurve in the SCurve
// Return 0 if arguments are invalid
int SCurveGetNbCurve(SCurve *that);

// Get the dimension of the SCurve
// Return 0 if argument is invalid
int SCurveDim(SCurve *that);

// Get the approximate length of the SCurve (sum of approxLen 
// of its BCurves)
// Return 0.0 if argument is invalid
float SCurveApproxLen(SCurve *that);

// Rotate the SCurve CCW by 'theta' radians relatively to the origin
// Do nothing if arguments are invalid
void SCurveRot2D(SCurve *that, float theta);

// Scale the SCurve by 'v' relatively to the origin
// Do nothing if arguments are invalid
void SCurveScale(SCurve *that, VecFloat *v);

// Translate the SCurve by 'v'
// Do nothing if arguments are invalid
void SCurveTranslate(SCurve *that, VecFloat *v);

// Get the bounding box of the SCurve.
// Return a Facoid whose axis are aligned on the standard coordinate 
// system.
// Return NULL if arguments are invalid.
Shapoid* SCurveGetBoundingBox(SCurve *that);

// Create a new BSurf of order 'order' and dimension 'dim'
// Controls are initialized with null vectors
// Return NULL if we couldn't create the BSurf
BSurf* BSurfCreate(int order, VecShort *dim);

// Free the memory used by a BSurf
// Do nothing if arguments are invalid
void BSurfFree(BSurf **that);

// Set the value of the iCtrl-th control point to v
// Do nothing if arguments are invalid
void BSurfSet(BSurf *that, VecShort *iCtrl, VecFloat *v);

// Get the value of the BSurf at paramater 'u' (in [0.0, 1.0])
// Return NULL if arguments are invalid or malloc failed
// Components of 'u' < 0.0 are replaced by 0.0
// Components of 'u' > 1.0 are replaced by 1.0
VecFloat* BSurfGet(BSurf *that, VecFloat *u);

// Get the number of control point of the BSurf 'that'
// Return 0 if arguments are invalid
int BSurfGetNbCtrl(BSurf *that);

// Get the the 'iCtrl'-th control point of 'that'
// ctrl are ordered as follow: 
// (0,0,0),(0,0,1),...,(0,0,order+1),(0,1,0),(0,1,1),...
// Return NULL if arguments are invalid
VecFloat* BSurfGetCtrl(BSurf *that, VecShort *iCtrl);

#endif
