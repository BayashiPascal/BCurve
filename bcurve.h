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

#define BCurveTranslate(Curve, Vec) _Generic(Vec, \
  VecFloat*: _BCurveTranslate, \
  VecFloat2D*: _BCurveTranslate, \
  VecFloat3D*: _BCurveTranslate, \
  default: PBErrInvalidPolymorphism)(Curve, (VecFloat*)(Vec))

#define SCurveTranslate(Curve, Vec) _Generic(Vec, \
  VecFloat*: _SCurveTranslate, \
  VecFloat2D*: _SCurveTranslate, \
  VecFloat3D*: _SCurveTranslate, \
  default: PBErrInvalidPolymorphism)(Curve, (VecFloat*)(Vec))

#define BBodyTranslate(Body, Vec) _Generic(Vec, \
  VecFloat*: _BBodyTranslate, \
  VecFloat2D*: _BBodyTranslate, \
  VecFloat3D*: _BBodyTranslate, \
  default: PBErrInvalidPolymorphism)(Body, (VecFloat*)(Vec))

#define BCurveScaleOrigin(Curve, Scale) _Generic(Scale, \
  VecFloat*: _BCurveScaleOriginVector, \
  float: _BCurveScaleOriginScalar, \
  default: PBErrInvalidPolymorphism)(Curve, Scale)

#define BCurveScaleStart(Curve, Scale) _Generic(Scale, \
  VecFloat*: _BCurveScaleStartVector, \
  float: _BCurveScaleStartScalar, \
  default: PBErrInvalidPolymorphism)(Curve, Scale)

#define BCurveScaleCenter(Curve, Scale) _Generic(Scale, \
  VecFloat*: _BCurveScaleCenterVector, \
  float: _BCurveScaleCenterScalar, \
  default: PBErrInvalidPolymorphism)(Curve, Scale)

#define BBodyScaleOrigin(Body, Scale) _Generic(Scale, \
  VecFloat*: _BBodyScaleOriginVector, \
  float: _BBodyScaleOriginScalar, \
  default: PBErrInvalidPolymorphism)(Body, Scale)

#define BBodyScaleStart(Body, Scale) _Generic(Scale, \
  VecFloat*: _BBodyScaleStartVector, \
  float: _BBodyScaleStartScalar, \
  default: PBErrInvalidPolymorphism)(Body, Scale)

#define BBodyScaleCenter(Body, Scale) _Generic(Scale, \
  VecFloat*: _BBodyScaleCenterVector, \
  float: _BBodyScaleCenterScalar, \
  default: PBErrInvalidPolymorphism)(Body, Scale)

#define SCurveScaleOrigin(Curve, Scale) _Generic(Scale, \
  VecFloat*: _SCurveScaleOriginVector, \
  float: _SCurveScaleOriginScalar, \
  default: PBErrInvalidPolymorphism)(Curve, Scale)

#define SCurveScaleStart(Curve, Scale) _Generic(Scale, \
  VecFloat*: _SCurveScaleStartVector, \
  float: _SCurveScaleStartScalar, \
  default: PBErrInvalidPolymorphism)(Curve, Scale)

#define SCurveScaleCenter(Curve, Scale) _Generic(Scale, \
  VecFloat*: _SCurveScaleCenterVector, \
  float: _SCurveScaleCenterScalar, \
  default: PBErrInvalidPolymorphism)(Curve, Scale)

#define BBodyGetIndexCtrl(Body, ICtrl) _Generic(ICtrl, \
  VecShort*: _BBodyGetIndexCtrl, \
  VecShort2D*: _BBodyGetIndexCtrl, \
  VecShort3D*: _BBodyGetIndexCtrl, \
  VecShort4D*: _BBodyGetIndexCtrl, \
  default: PBErrInvalidPolymorphism)(Body, (VecShort*)(ICtrl))

#define BBodyGet(Body, U) _Generic(U, \
  VecFloat*: _BBodyGet, \
  VecFloat2D*: _BBodyGet, \
  VecFloat3D*: _BBodyGet, \
  default: PBErrInvalidPolymorphism)(Body, (VecFloat*)(U))

#define BBodyCtrl(Body, ICtrl) _Generic(ICtrl, \
  VecShort*: _BBodyCtrl, \
  VecShort2D*: _BBodyCtrl, \
  VecShort3D*: _BBodyCtrl, \
  VecShort4D*: _BBodyCtrl, \
  default: PBErrInvalidPolymorphism)(Body, (VecShort*)(ICtrl))

#define BBodySetCtrl(Body, ICtrl, Vec) _Generic(ICtrl, \
  VecShort*: _Generic(Vec, \
    VecFloat*: _BBodySetCtrl, \
    VecFloat2D*: _BBodySetCtrl, \
    VecFloat3D*: _BBodySetCtrl, \
    default: PBErrInvalidPolymorphism), \
  VecShort2D*: _Generic(Vec, \
    VecFloat*: _BBodySetCtrl, \
    VecFloat2D*: _BBodySetCtrl, \
    VecFloat3D*: _BBodySetCtrl, \
    default: PBErrInvalidPolymorphism), \
  VecShort3D*: _Generic(Vec, \
    VecFloat*: _BBodySetCtrl, \
    VecFloat2D*: _BBodySetCtrl, \
    VecFloat3D*: _BBodySetCtrl, \
    default: PBErrInvalidPolymorphism), \
  VecShort4D*: _Generic(Vec, \
    VecFloat*: _BBodySetCtrl, \
    VecFloat2D*: _BBodySetCtrl, \
    VecFloat3D*: _BBodySetCtrl, \
    default: PBErrInvalidPolymorphism), \
  default: PBErrInvalidPolymorphism)(Body, (VecShort*)(ICtrl), \
    (VecFloat*)(Vec))

// ================= Data structure ===================

typedef struct BCurve {
  // Order
  int _order;
  // Dimension
  int _dim;
  // array of (_order + 1) control points (vectors of dimension _dim)
  // defining the curve
  VecFloat** _ctrl;
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

typedef struct BBody {
  // Order
  int _order;
  // Dimensions (input/output) (for example (2,3) gives a surface in 3D)
  VecShort2D _dim;
  // ((_order + 1) ^ _dim[0]) control points of the surface
  // they are ordered as follow: 
  // (0,0,0),(0,0,1),...,(0,0,order+1),(0,1,0),(0,1,1),...
  VecFloat** _ctrl;
} BBody;

// ================ Functions declaration ====================

// Create a new BCurve of order 'order' and dimension 'dim'
BCurve* BCurveCreate(int order, int dim);

// Clone the BCurve
BCurve* BCurveClone(BCurve* that);

// Load the BCurve from the stream
// If the BCurve is already allocated, it is freed before loading
// Return true upon success, false else
bool BCurveLoad(BCurve** that, FILE* stream);

// Save the BCurve to the stream
// Return true upon success, false else
bool BCurveSave(BCurve* that, FILE* stream);

// Free the memory used by a BCurve
void BCurveFree(BCurve** that);

// Print the BCurve on 'stream'
void BCurvePrint(BCurve* that, FILE* stream);

// Set the value of the iCtrl-th control point to v
#if BUILDMODE != 0
inline
#endif 
void BCurveSetCtrl(BCurve* that, int iCtrl, VecFloat* v);

// Get a copy of the iCtrl-th control point
#if BUILDMODE != 0
inline
#endif 
VecFloat* BCurveGetCtrl(BCurve* that, int iCtrl);

// Get the iCtrl-th control point
#if BUILDMODE != 0
inline
#endif 
VecFloat* BCurveCtrl(BCurve* that, int iCtrl);

// Get the value of the BCurve at paramater 'u'
// u can extend beyond [0.0, 1.0]
VecFloat* BCurveGet(BCurve* that, float u);

// Get the order of the BCurve
#if BUILDMODE != 0
inline
#endif 
int BCurveGetOrder(BCurve* that);

// Get the dimension of the BCurve
#if BUILDMODE != 0
inline
#endif 
int BCurveGetDim(BCurve* that);

// Get the approximate length of the BCurve (sum of dist between
// control points)
#if BUILDMODE != 0
inline
#endif 
float BCurveGetApproxLen(BCurve* that);

// Return the center of the BCurve (average of control points)
#if BUILDMODE != 0
inline
#endif 
VecFloat* BCurveGetCenter(BCurve* that);

// Rotate the curve CCW by 'theta' radians relatively to the origin
// of the coordinates system
#if BUILDMODE != 0
inline
#endif 
void BCurveRotOrigin(BCurve* that, float theta);

// Rotate the curve CCW by 'theta' radians relatively to its 
// first control point
#if BUILDMODE != 0
inline
#endif 
void BCurveRotStart(BCurve* that, float theta);

// Rotate the curve CCW by 'theta' radians relatively to its 
// center
#if BUILDMODE != 0
inline
#endif 
void BCurveRotCenter(BCurve* that, float theta);

// Scale the curve by 'v' relatively to the origin
// of the coordinates system
#if BUILDMODE != 0
inline
#endif 
void _BCurveScaleOriginVector(BCurve* that, VecFloat* v);

// Scale the curve by 'c' relatively to the origin
// of the coordinates system
#if BUILDMODE != 0
inline
#endif 
void _BCurveScaleOriginScalar(BCurve* that, float c);

// Scale the curve by 'v' relatively to its origin
// (first control point)
#if BUILDMODE != 0
inline
#endif 
void _BCurveScaleStartVector(BCurve* that, VecFloat* v);

// Scale the curve by 'c' relatively to its origin
// (first control point)
#if BUILDMODE != 0
inline
#endif 
void _BCurveScaleStartScalar(BCurve* that, float c);

// Scale the curve by 'v' relatively to its center
// (average of control points)
#if BUILDMODE != 0
inline
#endif 
void _BCurveScaleCenterVector(BCurve* that, VecFloat* v);

// Scale the curve by 'c' relatively to its center
// (average of control points)
#if BUILDMODE != 0
inline
#endif 
void _BCurveScaleCenterScalar(BCurve* that, float c);

// Translate the curve by 'v'
#if BUILDMODE != 0
inline
#endif 
void _BCurveTranslate(BCurve* that, VecFloat* v);

// Create a BCurve which pass through the points given in the GSet 'set'
// The GSet must contains VecFloat of same dimensions
// The BCurve pass through the points in the order they are given
// in the GSet. The points don't need to be uniformly distributed
// The created BCurve is of same dimension as the VecFloat and of order 
// equal to the number of VecFloat in 'set' minus one
// Return NULL if it couldn't create the BCurve
BCurve* BCurveFromCloudPoint(GSet* set);

// Get a VecFloat of dimension equal to the number of control points
// Values of the VecFloat are the weight of each control point in the 
// BCurve given the curve's order and the value of 't' (in [0.0,1.0])
VecFloat* BCurveGetWeightCtrlPt(BCurve* that, float t);

// Get the bounding box of the BCurve.
// Return a Facoid whose axis are aligned on the standard coordinate 
// system.
Facoid* BCurveGetBoundingBox(BCurve* that);

// Create a new SCurve of dimension 'dim', order 'order' and 
// 'nbSeg' segments
SCurve* SCurveCreate(int order, int dim, int nbSeg);

// Clone the SCurve
SCurve* SCurveClone(SCurve* that);

// Load the SCurve from the stream
// If the SCurve is already allocated, it is freed before loading
// Return true in case of success, false else
bool SCurveLoad(SCurve** that, FILE* stream);

// Save the SCurve to the stream
// Return true upon success, false else
bool SCurveSave(SCurve* that, FILE* stream);

// Free the memory used by a SCurve
void SCurveFree(SCurve** that);

// Print the SCurve on 'stream'
void SCurvePrint(SCurve* that, FILE* stream);

// Get the number of BCurve in the SCurve
#if BUILDMODE != 0
inline
#endif 
int SCurveGetNbSeg(SCurve* that);

// Get the dimension of the SCurve
#if BUILDMODE != 0
inline
#endif 
int SCurveGetDim(SCurve* that);

// Get the order of the SCurve
#if BUILDMODE != 0
inline
#endif 
int SCurveGetOrder(SCurve* that);

// Get the number of control point in the SCurve
#if BUILDMODE != 0
inline
#endif 
int SCurveGetNbCtrl(SCurve* that);

// Get a clone of the 'iCtrl'-th control point
#if BUILDMODE != 0
inline
#endif 
VecFloat* SCurveGetCtrl(SCurve* that, int iCtrl);

// Set the 'iCtrl'-th control point to 'v'
#if BUILDMODE != 0
inline
#endif 
void SCurveSetCtrl(SCurve* that, int iCtrl, VecFloat* v);

// Get the 'iCtrl'-th control point
#if BUILDMODE != 0
inline
#endif 
VecFloat* SCurveCtrl(SCurve* that, int iCtrl);

// Get a clone of the 'iSeg'-th segment
#if BUILDMODE != 0
inline
#endif 
BCurve* SCurveGetSeg(SCurve* that, int iSeg);

// Get the 'iSeg'-th segment
#if BUILDMODE != 0
inline
#endif 
BCurve* SCurveSeg(SCurve* that, int iSeg);

// Add one segment at the end of the curve (controls are set to 
// vectors null, except the first one which the last one of the current
// last segment)
void SCurveAddSegTail(SCurve* that);

// Add one segment at the head of the curve (controls are set to 
// vectors null, except the last one which the first one of the current
// first segment)
void SCurveAddSegHead(SCurve* that);

// Remove the fist segment of the curve (which must have more than one
// segment)
void SCurveRemoveHeadSeg(SCurve* that);

// Remove the last segment of the curve (which must have more than one
// segment)
void SCurveRemoveTailSeg(SCurve* that);

// Get the approximate length of the SCurve (sum of approxLen 
// of its BCurves)
#if BUILDMODE != 0
inline
#endif 
float SCurveGetApproxLen(SCurve* that);

// Return the center of the SCurve (average of control points)
#if BUILDMODE != 0
inline
#endif 
VecFloat* SCurveGetCenter(SCurve* that);

// Get the value of the SCurve at paramater 'u' 
// The value is equal to the value of the floor(u)-th segment at
// value (u - floor(u))
// u can extend beyond [0.0, _nbSeg]
#if BUILDMODE != 0
inline
#endif 
VecFloat* SCurveGet(SCurve* that, float u);

// Return the max value for the parameter 'u' of SCurveGet
#if BUILDMODE != 0
inline
#endif 
float SCurveGetMaxU(SCurve* that);

// Get the bounding box of the SCurve.
// Return a Facoid whose axis are aligned on the standard coordinate 
// system.
Facoid* SCurveGetBoundingBox(SCurve* that);

// Rotate the curve CCW by 'theta' radians relatively to the origin
// of the coordinates system
#if BUILDMODE != 0
inline
#endif 
void SCurveRotOrigin(SCurve* that, float theta);

// Rotate the curve CCW by 'theta' radians relatively to its 
// first control point
#if BUILDMODE != 0
inline
#endif 
void SCurveRotStart(SCurve* that, float theta);

// Rotate the curve CCW by 'theta' radians relatively to its 
// center
#if BUILDMODE != 0
inline
#endif 
void SCurveRotCenter(SCurve* that, float theta);

// Scale the curve by 'v' relatively to the origin
// of the coordinates system
#if BUILDMODE != 0
inline
#endif 
void _SCurveScaleOriginVector(SCurve* that, VecFloat* v);

// Scale the curve by 'c' relatively to the origin
// of the coordinates system
#if BUILDMODE != 0
inline
#endif 
void _SCurveScaleOriginScalar(SCurve* that, float c);

// Scale the curve by 'v' relatively to its origin
// (first control point)
#if BUILDMODE != 0
inline
#endif 
void _SCurveScaleStartVector(SCurve* that, VecFloat* v);

// Scale the curve by 'c' relatively to its origin
// (first control point)
#if BUILDMODE != 0
inline
#endif 
void _SCurveScaleStartScalar(SCurve* that, float c);

// Scale the curve by 'v' relatively to its center
// (average of control points)
#if BUILDMODE != 0
inline
#endif 
void _SCurveScaleCenterVector(SCurve* that, VecFloat* v);

// Scale the curve by 'c' relatively to its center
// (average of control points)
#if BUILDMODE != 0
inline
#endif 
void _SCurveScaleCenterScalar(SCurve* that, float c);

// Translate the curve by 'v'
#if BUILDMODE != 0
inline
#endif 
void _SCurveTranslate(SCurve* that, VecFloat* v);

// Create a new BBody of order 'order' and dimension 'dim'
// Controls are initialized with null vectors
BBody* BBodyCreate(int order, VecShort2D* dim);

// Free the memory used by a BBody
void BBodyFree(BBody** that);

// Set the value of the iCtrl-th control point to v
#if BUILDMODE != 0
inline
#endif 
void _BBodySetCtrl(BBody* that, VecShort* iCtrl, VecFloat* v);

// Get the value of the BBody at paramater 'u'
// u can extend beyond [0.0, 1.0]
VecFloat* _BBodyGet(BBody* that, VecFloat* u);

// Get the number of control points of the BBody 'that'
#if BUILDMODE != 0
inline
#endif 
int BBodyGetNbCtrl(BBody* that);

// Get the the 'iCtrl'-th control point of 'that'
#if BUILDMODE != 0
inline
#endif 
VecFloat* _BBodyCtrl(BBody* that, VecShort* iCtrl);

// Get the index in _ctrl of the 'iCtrl' control point of 'that'
#if BUILDMODE != 0
inline
#endif 
int _BBodyGetIndexCtrl(BBody* that, VecShort* iCtrl);

// Get the order of the BBody 'that'
#if BUILDMODE != 0
inline
#endif 
int BBodyGetOrder(BBody* that);

// Get the dimensions of the BBody 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D* BBodyDim(BBody* that);

// Get a copy of the dimensions of the BBody 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D BBodyGetDim(BBody* that);

// Return a clone of the BBody 'that'
BBody* BBodyClone(BBody* that);

// Print the BBody 'that' on the stream 'stream'
void BBodyPrint(BBody* that, FILE* stream);

// Load the BBody from the stream
// If the BBody is already allocated, it is freed before loading
// Return true upon success, false else
bool BBodyLoad(BBody** that, FILE* stream);

// Save the BBody to the stream
// Return true upon success, false else
bool BBodySave(BBody* that, FILE* stream);

// Return the center of the BBody (average of control points)
#if BUILDMODE != 0
inline
#endif 
VecFloat* BBodyGetCenter(BBody* that);

// Translate the BBody by 'v'
#if BUILDMODE != 0
inline
#endif 
void _BBodyTranslate(BBody* that, VecFloat* v);

// Scale the curve by 'v' relatively to the origin
// of the coordinates system
#if BUILDMODE != 0
inline
#endif 
void _BBodyScaleOriginVector(BBody* that, VecFloat* v);

// Scale the curve by 'c' relatively to the origin
// of the coordinates system
#if BUILDMODE != 0
inline
#endif 
void _BBodyScaleOriginScalar(BBody* that, float c);

// Scale the curve by 'v' relatively to its origin
// (first control point)
#if BUILDMODE != 0
inline
#endif 
void _BBodyScaleStartVector(BBody* that, VecFloat* v);

// Scale the curve by 'c' relatively to its origin
// (first control point)
#if BUILDMODE != 0
inline
#endif 
void _BBodyScaleStartScalar(BBody* that, float c);

// Scale the curve by 'v' relatively to its center
// (average of control points)
#if BUILDMODE != 0
inline
#endif 
void _BBodyScaleCenterVector(BBody* that, VecFloat* v);

// Scale the curve by 'c' relatively to its center
// (average of control points)
#if BUILDMODE != 0
inline
#endif 
void _BBodyScaleCenterScalar(BBody* that, float c);

// Get the bounding box of the BBody.
// Return a Facoid whose axis are aligned on the standard coordinate 
// system.
Facoid* BBodyGetBoundingBox(BBody* that);

// Rotate the BBody by 'theta' relatively to the origin
// of the coordinates system around 'axis'
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotateOrigin(BBody* that, VecFloat3D* axis, float theta);

// Rotate the BBody by 'theta' relatively to the center
// of the body around 'axis'
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotateCenter(BBody* that, VecFloat3D* axis, float theta);

// Rotate the BBody by 'theta' relatively to the first control point
// of the body around 'axis'
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotateStart(BBody* that, VecFloat3D* axis, float theta);

// Rotate the BBody by 'theta' relatively to the origin
// of the coordinates system around X
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotateXOrigin(BBody* that, float theta);

// Rotate the BBody by 'theta' relatively to the center
// of the body around X
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotateXCenter(BBody* that, float theta);

// Rotate the BBody by 'theta' relatively to the first control point
// of the body around X
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotateXStart(BBody* that, float theta);

// Rotate the BBody by 'theta' relatively to the origin
// of the coordinates system around Y
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotateYOrigin(BBody* that, float theta);

// Rotate the BBody by 'theta' relatively to the center
// of the body around Y
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotateYCenter(BBody* that, float theta);

// Rotate the BBody by 'theta' relatively to the first control point
// of the body around Y
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotateYStart(BBody* that, float theta);

// Rotate the BBody by 'theta' relatively to the origin
// of the coordinates system around Z
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotateZOrigin(BBody* that, float theta);

// Rotate the BBody by 'theta' relatively to the center
// of the body around Z
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotateZCenter(BBody* that, float theta);

// Rotate the BBody by 'theta' relatively to the first control point
// of the body around Z
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotateZStart(BBody* that, float theta);

// ================ Inliner ====================

#if BUILDMODE != 0
#include "bcurve-inline.c"
#endif


#endif
