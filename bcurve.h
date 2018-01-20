// ============ BCURVE.H ================

#ifndef BCURVE_H
#define BCURVE_H

// ================= Include =================

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "pberr.h"
#include "pbmath.h"
#include "gset.h"
#include "shapoid.h"

// ================= Define ==================

// ================= Polymorphism ==================

#define BCurveScaleOrigin(T, C) _Generic((C), \
  VecFloat*: BCurveScaleOriginVector, \
  float: BCurveScaleOriginScalar, \
  default: PBErrInvalidPolymorphism)(T, C)

#define BCurveScaleStart(T, C) _Generic((C), \
  VecFloat*: BCurveScaleStartVector, \
  float: BCurveScaleStartScalar, \
  default: PBErrInvalidPolymorphism)(T, C)

#define BCurveScaleCenter(T, C) _Generic((C), \
  VecFloat*: BCurveScaleCenterVector, \
  float: BCurveScaleCenterScalar, \
  default: PBErrInvalidPolymorphism)(T, C)

#define SCurveScaleOrigin(T, C) _Generic((C), \
  VecFloat*: SCurveScaleOriginVector, \
  float: SCurveScaleOriginScalar, \
  default: PBErrInvalidPolymorphism)(T, C)

#define SCurveScaleStart(T, C) _Generic((C), \
  VecFloat*: SCurveScaleStartVector, \
  float: SCurveScaleStartScalar, \
  default: PBErrInvalidPolymorphism)(T, C)

#define SCurveScaleCenter(T, C) _Generic((C), \
  VecFloat*: SCurveScaleCenterVector, \
  float: SCurveScaleCenterScalar, \
  default: PBErrInvalidPolymorphism)(T, C)

// ================= Data structure ===================

typedef struct BCurve {
  // Order
  int _order;
  // Dimension
  int _dim;
  // array of (_order + 1) control points (vectors of dimension _dim)
  // defining the curve
  VecFloat **_ctrl;
} BCurve;

typedef struct SCurve {
  // Order
  int _order;
  // Dimension
  int _dim;
  // Number of segments (one segment equals one BCurve)
  int _nbSeg;
  // Set of BCurve
  GSet _seg;
  // Set of control points
  GSet _ctrl;
} SCurve;

/*typedef struct BSurf {
  // Order
  int _order;
  // Dimensions (input/output)
  VecShort *_dim;
  // ((_order + 1) ^ _dim[0]) control points of the surface
  VecFloat **_ctrl;
} BSurf;*/

// ================ Functions declaration ====================

// Create a new BCurve of order 'order' and dimension 'dim'
BCurve* BCurveCreate(int order, int dim);

// Clone the BCurve
BCurve* BCurveClone(BCurve *that);

// Load the BCurve from the stream
// If the BCurve is already allocated, it is freed before loading
// Return true upon success, false else
bool BCurveLoad(BCurve **that, FILE *stream);

// Save the BCurve to the stream
// Return true upon success, false else
bool BCurveSave(BCurve *that, FILE *stream);

// Free the memory used by a BCurve
void BCurveFree(BCurve **that);

// Print the BCurve on 'stream'
void BCurvePrint(BCurve *that, FILE *stream);

// Set the value of the iCtrl-th control point to v
#if BUILDMODE != 0
inline
#endif 
void BCurveSetCtrl(BCurve *that, int iCtrl, VecFloat *v);

// Get a copy of the iCtrl-th control point
#if BUILDMODE != 0
inline
#endif 
VecFloat* BCurveGetCtrl(BCurve *that, int iCtrl);

// Get the iCtrl-th control point
#if BUILDMODE != 0
inline
#endif 
VecFloat* BCurveCtrl(BCurve *that, int iCtrl);

// Get the value of the BCurve at paramater 'u' (in [0.0, 1.0])
VecFloat* BCurveGet(BCurve *that, float u);

// Get the order of the BCurve
#if BUILDMODE != 0
inline
#endif 
int BCurveGetOrder(BCurve *that);

// Get the dimension of the BCurve
#if BUILDMODE != 0
inline
#endif 
int BCurveGetDim(BCurve *that);

// Get the approximate length of the BCurve (sum of dist between
// control points)
#if BUILDMODE != 0
inline
#endif 
float BCurveGetApproxLen(BCurve *that);

// Return the center of the BCurve (average of control points)
#if BUILDMODE != 0
inline
#endif 
VecFloat* BCurveGetCenter(BCurve *that);

// Rotate the curve CCW by 'theta' radians relatively to the origin
// of the coordinates system
#if BUILDMODE != 0
inline
#endif 
void BCurveRotOrigin(BCurve *that, float theta);

// Rotate the curve CCW by 'theta' radians relatively to its 
// first control point
#if BUILDMODE != 0
inline
#endif 
void BCurveRotStart(BCurve *that, float theta);

// Rotate the curve CCW by 'theta' radians relatively to its 
// center
#if BUILDMODE != 0
inline
#endif 
void BCurveRotCenter(BCurve *that, float theta);

// Scale the curve by 'v' relatively to the origin
// of the coordinates system
#if BUILDMODE != 0
inline
#endif 
void BCurveScaleOriginVector(BCurve *that, VecFloat *v);

// Scale the curve by 'c' relatively to the origin
// of the coordinates system
#if BUILDMODE != 0
inline
#endif 
void BCurveScaleOriginScalar(BCurve *that, float c);

// Scale the curve by 'v' relatively to its origin
// (first control point)
#if BUILDMODE != 0
inline
#endif 
void BCurveScaleStartVector(BCurve *that, VecFloat *v);

// Scale the curve by 'c' relatively to its origin
// (first control point)
#if BUILDMODE != 0
inline
#endif 
void BCurveScaleStartScalar(BCurve *that, float c);

// Scale the curve by 'v' relatively to its center
// (average of control points)
#if BUILDMODE != 0
inline
#endif 
void BCurveScaleCenterVector(BCurve *that, VecFloat *v);

// Scale the curve by 'c' relatively to its center
// (average of control points)
#if BUILDMODE != 0
inline
#endif 
void BCurveScaleCenterScalar(BCurve *that, float c);

// Translate the curve by 'v'
#if BUILDMODE != 0
inline
#endif 
void BCurveTranslate(BCurve *that, VecFloat *v);

// Create a BCurve which pass through the points given in the GSet 'set'
// The GSet must contains VecFloat of same dimensions
// The BCurve pass through the points in the order they are given
// in the GSet. The points don't need to be uniformly distributed
// The created BCurve is of same dimension as the VecFloat and of order 
// equal to the number of VecFloat in 'set' minus one
// Return NULL if it couldn't create the BCurve
BCurve* BCurveFromCloudPoint(GSet *set);

// Get a VecFloat of dimension equal to the number of control points
// Values of the VecFloat are the weight of each control point in the 
// BCurve given the curve's order and the value of 't' (in [0.0,1.0])
VecFloat* BCurveGetWeightCtrlPt(BCurve *that, float t);

// Get the bounding box of the BCurve.
// Return a Facoid whose axis are aligned on the standard coordinate 
// system.
Facoid* BCurveGetBoundingBox(BCurve *that);

// Create a new SCurve of dimension 'dim', order 'order' and 
// 'nbSeg' segments
SCurve* SCurveCreate(int order, int dim, int nbSeg);

// Clone the SCurve
SCurve* SCurveClone(SCurve *that);

// Load the SCurve from the stream
// If the SCurve is already allocated, it is freed before loading
// Return true in case of success, false else
bool SCurveLoad(SCurve **that, FILE *stream);

// Save the SCurve to the stream
// Return true upon success, false else
bool SCurveSave(SCurve *that, FILE *stream);

// Free the memory used by a SCurve
void SCurveFree(SCurve **that);

// Print the SCurve on 'stream'
void SCurvePrint(SCurve *that, FILE *stream);

// Get the number of BCurve in the SCurve
#if BUILDMODE != 0
inline
#endif 
int SCurveGetNbSeg(SCurve *that);

// Get the dimension of the SCurve
#if BUILDMODE != 0
inline
#endif 
int SCurveGetDim(SCurve *that);

// Get the order of the SCurve
#if BUILDMODE != 0
inline
#endif 
int SCurveGetOrder(SCurve *that);

// Get the number of control point in the SCurve
#if BUILDMODE != 0
inline
#endif 
int SCurveGetNbCtrl(SCurve *that);

// Get a clone of the 'iCtrl'-th control point
#if BUILDMODE != 0
inline
#endif 
VecFloat* SCurveGetCtrl(SCurve *that, int iCtrl);

// Set the 'iCtrl'-th control point to 'v'
#if BUILDMODE != 0
inline
#endif 
void SCurveSetCtrl(SCurve *that, int iCtrl, VecFloat *v);

// Get the 'iCtrl'-th control point
#if BUILDMODE != 0
inline
#endif 
VecFloat* SCurveCtrl(SCurve *that, int iCtrl);

// Get a clone of the 'iSeg'-th segment
#if BUILDMODE != 0
inline
#endif 
BCurve* SCurveGetSeg(SCurve *that, int iSeg);

// Get the 'iSeg'-th segment
#if BUILDMODE != 0
inline
#endif 
BCurve* SCurveSeg(SCurve *that, int iSeg);

// Add one segment at the end of the curve (controls are set to 
// vectors null, except the first one which the last one of the current
// last segment)
void SCurveAddSegTail(SCurve *that);

// Add one segment at the head of the curve (controls are set to 
// vectors null, except the last one which the first one of the current
// first segment)
void SCurveAddSegHead(SCurve *that);

// Remove the fist segment of the curve (which must have more than one
// segment)
void SCurveRemoveHeadSeg(SCurve *that);

// Remove the last segment of the curve (which must have more than one
// segment)
void SCurveRemoveTailSeg(SCurve *that);

// Get the approximate length of the SCurve (sum of approxLen 
// of its BCurves)
#if BUILDMODE != 0
inline
#endif 
float SCurveGetApproxLen(SCurve *that);

// Return the center of the SCurve (average of control points)
#if BUILDMODE != 0
inline
#endif 
VecFloat* SCurveGetCenter(SCurve *that);

// Get the value of the SCurve at paramater 'u' (in [0.0, _nbSeg])
// The value is equal to the value of the floor(u)-th segment at
// value (u - floor(u))
#if BUILDMODE != 0
inline
#endif 
VecFloat* SCurveGet(SCurve *that, float u);

// Return the max value for the parameter 'u' of SCurveGet
#if BUILDMODE != 0
inline
#endif 
float SCurveGetMaxU(SCurve *that);

// Get the bounding box of the SCurve.
// Return a Facoid whose axis are aligned on the standard coordinate 
// system.
Facoid* SCurveGetBoundingBox(SCurve *that);

// Rotate the curve CCW by 'theta' radians relatively to the origin
// of the coordinates system
#if BUILDMODE != 0
inline
#endif 
void SCurveRotOrigin(SCurve *that, float theta);

// Rotate the curve CCW by 'theta' radians relatively to its 
// first control point
#if BUILDMODE != 0
inline
#endif 
void SCurveRotStart(SCurve *that, float theta);

// Rotate the curve CCW by 'theta' radians relatively to its 
// center
#if BUILDMODE != 0
inline
#endif 
void SCurveRotCenter(SCurve *that, float theta);

// Scale the curve by 'v' relatively to the origin
// of the coordinates system
#if BUILDMODE != 0
inline
#endif 
void SCurveScaleOriginVector(SCurve *that, VecFloat *v);

// Scale the curve by 'c' relatively to the origin
// of the coordinates system
#if BUILDMODE != 0
inline
#endif 
void SCurveScaleOriginScalar(SCurve *that, float c);

// Scale the curve by 'v' relatively to its origin
// (first control point)
#if BUILDMODE != 0
inline
#endif 
void SCurveScaleStartVector(SCurve *that, VecFloat *v);

// Scale the curve by 'c' relatively to its origin
// (first control point)
#if BUILDMODE != 0
inline
#endif 
void SCurveScaleStartScalar(SCurve *that, float c);

// Scale the curve by 'v' relatively to its center
// (average of control points)
#if BUILDMODE != 0
inline
#endif 
void SCurveScaleCenterVector(SCurve *that, VecFloat *v);

// Scale the curve by 'c' relatively to its center
// (average of control points)
#if BUILDMODE != 0
inline
#endif 
void SCurveScaleCenterScalar(SCurve *that, float c);

// Translate the curve by 'v'
#if BUILDMODE != 0
inline
#endif 
void SCurveTranslate(SCurve *that, VecFloat *v);

// ================ Inliner ====================

#if BUILDMODE != 0
#include "bcurve-inline.c"
#endif


#endif
