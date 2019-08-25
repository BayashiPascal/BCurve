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

// -------------- BCurve

// ================= Data structure ===================

typedef struct BCurve {
  // Order
  const int _order;
  // Dimension
  const int _dim;
  // array of (_order + 1) control points (vectors of dimension _dim)
  // defining the curve
  VecFloat** _ctrl;
} BCurve;

// ================ Functions declaration ====================

// Create a new BCurve of order 'order' and dimension 'dim'
BCurve* BCurveCreate(const int order, const int dim);

// Clone the BCurve
BCurve* BCurveClone(const BCurve* const that);

// Function which return the JSON encoding of 'that' 
JSONNode* BCurveEncodeAsJSON(const BCurve* const that);

// Function which decode from JSON encoding 'json' to 'that'
bool BCurveDecodeAsJSON(BCurve** that, const JSONNode* const json);

// Load the BCurve from the stream
// If the BCurve is already allocated, it is freed before loading
// Return true upon success, false else
bool BCurveLoad(BCurve** that, FILE* const stream);

// Save the BCurve to the stream
// If 'compact' equals true it saves in compact form, else it saves in 
// readable form
// Return true upon success, false else
bool BCurveSave(const BCurve* const that, FILE* const stream, 
  const bool compact);

// Free the memory used by a BCurve
void BCurveFree(BCurve** that);

// Print the BCurve on 'stream'
void BCurvePrint(const BCurve* const that, FILE* const stream);

// Set the value of the iCtrl-th control point to v
#if BUILDMODE != 0
inline
#endif 
void BCurveSetCtrl(BCurve* const that, const int iCtrl, 
  const VecFloat* const v);

// Get a copy of the iCtrl-th control point
#if BUILDMODE != 0
inline
#endif 
VecFloat* BCurveGetCtrl(const BCurve* const that, const int iCtrl);

// Get the iCtrl-th control point
#if BUILDMODE != 0
inline
#endif 
const VecFloat* BCurveCtrl(const BCurve* const that, const int iCtrl);

// Get the 'iDim'-th value of the 'iCtrl'-th control point
#if BUILDMODE != 0
inline
#endif 
float BCurveCtrlGet(const BCurve* const that, const int iCtrl, 
  const int iDim);

// Get the value of the BCurve at paramater 'u'
// u can extend beyond [0.0, 1.0]
VecFloat* BCurveGet(const BCurve* const that, const float u);

// Get the order of the BCurve
#if BUILDMODE != 0
inline
#endif 
int BCurveGetOrder(const BCurve* const that);

// Get the dimension of the BCurve
#if BUILDMODE != 0
inline
#endif 
int BCurveGetDim(const BCurve* const that);

// Get the approximate length of the BCurve (sum of dist between
// control points)
#if BUILDMODE != 0
inline
#endif 
float BCurveGetApproxLen(const BCurve* const that);

// Return the center of the BCurve (average of control points)
#if BUILDMODE != 0
inline
#endif 
VecFloat* BCurveGetCenter(const BCurve* const that);

// Rotate the curve CCW by 'theta' radians relatively to the origin
// of the coordinates system
#if BUILDMODE != 0
inline
#endif 
void BCurveRotOrigin(BCurve* const that, const float theta);

// Rotate the curve CCW by 'theta' radians relatively to its 
// first control point
#if BUILDMODE != 0
inline
#endif 
void BCurveRotStart(BCurve* const that, const float theta);

// Rotate the curve CCW by 'theta' radians relatively to its 
// center
#if BUILDMODE != 0
inline
#endif 
void BCurveRotCenter(BCurve* const that, const float theta);

// Scale the curve by 'v' relatively to the origin
// of the coordinates system
#if BUILDMODE != 0
inline
#endif 
void _BCurveScaleOriginVector(BCurve* const that, 
  const VecFloat* const v);

// Scale the curve by 'c' relatively to the origin
// of the coordinates system
#if BUILDMODE != 0
inline
#endif 
void _BCurveScaleOriginScalar(BCurve* const that, const float c);

// Scale the curve by 'v' relatively to its origin
// (first control point)
#if BUILDMODE != 0
inline
#endif 
void _BCurveScaleStartVector(BCurve* const that, const VecFloat* const v);

// Scale the curve by 'c' relatively to its origin
// (first control point)
#if BUILDMODE != 0
inline
#endif 
void _BCurveScaleStartScalar(BCurve* const that, const float c);

// Scale the curve by 'v' relatively to its center
// (average of control points)
#if BUILDMODE != 0
inline
#endif 
void _BCurveScaleCenterVector(BCurve* const that, 
  const VecFloat* const v);

// Scale the curve by 'c' relatively to its center
// (average of control points)
#if BUILDMODE != 0
inline
#endif 
void _BCurveScaleCenterScalar(BCurve* const that, const float c);

// Translate the curve by 'v'
#if BUILDMODE != 0
inline
#endif 
void _BCurveTranslate(BCurve* const that, const VecFloat* const v);

// Create a BCurve which pass through the points given in the GSet 'set'
// The GSet must contains VecFloat of same dimensions
// The BCurve pass through the points in the order they are given
// in the GSet. The points don't need to be uniformly distributed
// The created BCurve is of same dimension as the VecFloat and of order 
// equal to the number of VecFloat in 'set' minus one
// Return NULL if it couldn't create the BCurve
BCurve* BCurveFromCloudPoint(const GSetVecFloat* const set);

// Get a VecFloat of dimension equal to the number of control points
// Values of the VecFloat are the weight of each control point in the 
// BCurve given the curve's order and the value of 't' (in [0.0,1.0])
VecFloat* BCurveGetWeightCtrlPt(const BCurve* const that, const float t);

// Get the bounding box of the BCurve.
// Return a Facoid whose axis are aligned on the standard coordinate 
// system.
Facoid* BCurveGetBoundingBox(const BCurve* const that);

// -------------- SCurve

// ================= Data structure ===================

typedef struct SCurve {
  // Order
  const int _order;
  // Dimension
  const int _dim;
  // Number of segments (one segment equals one BCurve)
  int _nbSeg;
  // Set of BCurve
  GSetBCurve _seg;
  // Set of control points
  GSetVecFloat _ctrl;
} SCurve;

// ================ Functions declaration ====================

// Create a new SCurve of dimension 'dim', order 'order' and 
// 'nbSeg' segments
SCurve* SCurveCreate(const int order, const int dim, const int nbSeg);

// Clone the SCurve
SCurve* SCurveClone(const SCurve* const that);

// Return a new SCurve as a copy of the SCurve 'that' with 
// dimension changed to 'dim'
// if it is extended, the values of new components are 0.0
// If it is shrinked, values are discarded from the end of the vectors
SCurve* SCurveGetNewDim(const SCurve* const that, const int dim);

// Function which return the JSON encoding of 'that' 
JSONNode* SCurveEncodeAsJSON(const SCurve* const that);

// Function which decode from JSON encoding 'json' to 'that'
bool SCurveDecodeAsJSON(SCurve** that, const JSONNode* const json);

// Load the SCurve from the stream
// If the SCurve is already allocated, it is freed before loading
// Return true in case of success, false else
bool SCurveLoad(SCurve** that, FILE* const stream);

// Save the SCurve to the stream
// If 'compact' equals true it saves in compact form, else it saves in 
// readable form
// Return true upon success, false else
bool SCurveSave(const SCurve* const that, FILE* const stream, 
  const bool compact);

// Free the memory used by a SCurve
void SCurveFree(SCurve** that);

// Print the SCurve on 'stream'
void SCurvePrint(const SCurve* const that, FILE* const stream);

// Get the number of BCurve in the SCurve
#if BUILDMODE != 0
inline
#endif 
int SCurveGetNbSeg(const SCurve* const that);

// Get the dimension of the SCurve
#if BUILDMODE != 0
inline
#endif 
int SCurveGetDim(const SCurve* const that);

// Get the order of the SCurve
#if BUILDMODE != 0
inline
#endif 
int SCurveGetOrder(const SCurve* const that);

// Get the number of control point in the SCurve
#if BUILDMODE != 0
inline
#endif 
int SCurveGetNbCtrl(const SCurve* const that);

// Get a clone of the 'iCtrl'-th control point
#if BUILDMODE != 0
inline
#endif 
VecFloat* SCurveGetCtrl(const SCurve* const that, const int iCtrl);

// Set the 'iCtrl'-th control point to a copy 'v'
#if BUILDMODE != 0
inline
#endif 
void SCurveSetCtrl(SCurve* const that, const int iCtrl, 
  const VecFloat* const v);

// Set the 'iDim'-th value of the 'iCtrl'-th control point to 'v'
#if BUILDMODE != 0
inline
#endif 
void SCurveCtrlSet(SCurve* const that, const int iCtrl, const int iDim, 
  float v);

// Get the 'iCtrl'-th control point
#if BUILDMODE != 0
inline
#endif 
const VecFloat* SCurveCtrl(const SCurve* const that, const int iCtrl);

// Get the 'iDim'-th value of the 'iCtrl'-th control point
#if BUILDMODE != 0
inline
#endif 
float SCurveCtrlGet(const SCurve* const that, const int iCtrl, 
  const int iDim);

// Get the GSet of control points of the SCurve 'that'
#if BUILDMODE != 0
inline
#endif 
const GSetVecFloat* SCurveCtrls(const SCurve* const that);

// Get a clone of the 'iSeg'-th segment
#if BUILDMODE != 0
inline
#endif 
BCurve* SCurveGetSeg(const SCurve* const that, const int iSeg);

// Get the 'iSeg'-th segment
#if BUILDMODE != 0
inline
#endif 
const BCurve* SCurveSeg(const SCurve* const that, const int iSeg);

// Get the GSet of segments of the SCurve 'that'
#if BUILDMODE != 0
inline
#endif 
const GSetBCurve* SCurveSegs(const SCurve* const that);

// Add one segment at the end of the curve (controls are set to 
// vectors null, except the first one which the last one of the current
// last segment)
void SCurveAddSegTail(SCurve* const that);

// Add one segment at the head of the curve (controls are set to 
// vectors null, except the last one which the first one of the current
// first segment)
void SCurveAddSegHead(SCurve* const that);

// Remove the fist segment of the curve (which must have more than one
// segment)
void SCurveRemoveHeadSeg(SCurve* const that);

// Remove the last segment of the curve (which must have more than one
// segment)
void SCurveRemoveTailSeg(SCurve* const that);

// Get the approximate length of the SCurve (sum of approxLen 
// of its BCurves)
#if BUILDMODE != 0
inline
#endif 
float SCurveGetApproxLen(const SCurve* const that);

// Return the center of the SCurve (average of control points)
#if BUILDMODE != 0
inline
#endif 
VecFloat* SCurveGetCenter(const SCurve* const that);

// Get the value of the SCurve at paramater 'u' 
// The value is equal to the value of the floor(u)-th segment at
// value (u - floor(u))
// u can extend beyond [0.0, _nbSeg]
#if BUILDMODE != 0
inline
#endif 
VecFloat* SCurveGet(const SCurve* const that, const float u);

// Return the max value for the parameter 'u' of SCurveGet
#if BUILDMODE != 0
inline
#endif 
float SCurveGetMaxU(const SCurve* const that);

// Get the bounding box of the SCurve.
// Return a Facoid whose axis are aligned on the standard coordinate 
// system.
Facoid* SCurveGetBoundingBox(const SCurve* const that);

// Rotate the curve CCW by 'theta' radians relatively to the origin
// of the coordinates system
#if BUILDMODE != 0
inline
#endif 
void SCurveRotOrigin(SCurve* const that, const float theta);

// Rotate the curve CCW by 'theta' radians relatively to its 
// first control point
#if BUILDMODE != 0
inline
#endif 
void SCurveRotStart(SCurve* const that, const float theta);

// Rotate the curve CCW by 'theta' radians relatively to its 
// center
#if BUILDMODE != 0
inline
#endif 
void SCurveRotCenter(SCurve* const that, const float theta);

// Scale the curve by 'v' relatively to the origin
// of the coordinates system
#if BUILDMODE != 0
inline
#endif 
void _SCurveScaleOriginVector(SCurve* const that, 
  const VecFloat* const v);

// Scale the curve by 'c' relatively to the origin
// of the coordinates system
#if BUILDMODE != 0
inline
#endif 
void _SCurveScaleOriginScalar(SCurve* const that, const float c);

// Scale the curve by 'v' relatively to its origin
// (first control point)
#if BUILDMODE != 0
inline
#endif 
void _SCurveScaleStartVector(SCurve* const that, const VecFloat* const v);

// Scale the curve by 'c' relatively to its origin
// (first control point)
#if BUILDMODE != 0
inline
#endif 
void _SCurveScaleStartScalar(SCurve* const that, const float c);

// Scale the curve by 'v' relatively to its center
// (average of control points)
#if BUILDMODE != 0
inline
#endif 
void _SCurveScaleCenterVector(SCurve* const that, 
  const VecFloat* const v);

// Scale the curve by 'c' relatively to its center
// (average of control points)
#if BUILDMODE != 0
inline
#endif 
void _SCurveScaleCenterScalar(SCurve* const that, const float c);

// Translate the curve by 'v'
#if BUILDMODE != 0
inline
#endif 
void _SCurveTranslate(SCurve* const that, const VecFloat* const v);

// Create a new SCurve from the outline of the Shapoid 'shap'
// The Shapoid must be of dimension 2
// Control points are ordered CCW of the Shapoid
#if BUILDMODE != 0
inline
#endif 
SCurve* SCurveCreateFromShapoid(const Shapoid* const shap);

// Create a new SCurve from the outline of the Facoid 'shap'
// The Facoid must be of dimension 2
// Control points are ordered CCW of the Shapoid
SCurve* SCurveCreateFromFacoid(const Facoid* const shap);

// Create a new SCurve from the outline of the Pyramidoid 'shap'
// The Pyramidoid must be of dimension 2
// Control points are ordered CCW of the Shapoid
SCurve* SCurveCreateFromPyramidoid(const Pyramidoid* const shap);

// Create a new SCurve from the outline of the Spheroid 'shap'
// The Spheroid must be of dimension 2
// Control points are ordered CCW of the Shapoid
// Calculate an approximation as there is no exact solution
SCurve* SCurveCreateFromSpheroid(const Spheroid* const shap);

// Get the distance between the SCurve 'that' and the SCurve 'curve'
// The distance is defined as the integral of 
// ||'that'(u(t))-'curve'(v(t))|| where u and v are the relative 
// positions on the curve over t varying from 0.0 to 1.0
float SCurveGetDistToCurve(const SCurve* const that, 
  const SCurve* const curve);

// Apply the chaikin curve subdivision algorithm to the SCurve 'that'
// with 'depth' times recursion and 'strength' is the parametric
// distance from each corner where the curve is cut at each recursion
// 'strength' in [0.0, 1.0]
// The SCurve must be of order 1, if it is not nothing happens
// cf http://graphics.cs.ucdavis.edu/education/CAGDNotes/Chaikins-Algorithm.pdf
SCurve* SCurveChaikinSubdivision(SCurve* const that, 
  const float strength, const unsigned int depth);

// -------------- SCurveIter

// ================= Data structure ===================

typedef struct SCurveIter {
  // Attached SCurve
  const SCurve* _curve;
  // Current position
  float _curPos;
  // Step delta
  float _delta;
} SCurveIter;

// ================ Functions declaration ====================

// Create a new SCurveIter attached to the SCurve 'curve' with a step 
// of 'delta'
SCurveIter SCurveIterCreateStatic(const SCurve* const curve, 
  const float delta);

// Set the attached SCurve of the SCurveIter 'that' to 'curve'
#if BUILDMODE != 0
inline
#endif 
void SCurveIterSetCurve(SCurveIter* const that, 
  const SCurve* const curve);

// Set the delta of the SCurveIter 'that' to 'delta'
#if BUILDMODE != 0
inline
#endif 
void SCurveIterSetDelta(SCurveIter* const that, const float delta);

// Get the attached curve of the SCurveIter 'that'
#if BUILDMODE != 0
inline
#endif 
const SCurve* SCurveIterCurve(const SCurveIter* const that);

// Get the delta of the SCurveIter 'that'
#if BUILDMODE != 0
inline
#endif 
float SCurveIterGetDelta(const SCurveIter* const that);

// Init the SCurveIter 'that'
#if BUILDMODE != 0
inline
#endif 
void SCurveIterInit(SCurveIter* const that);

// Step the SCurveIter 'that'
// Return false if it couldn't step, true else
#if BUILDMODE != 0
inline
#endif 
bool SCurveIterStep(SCurveIter* const that);

// Step back the SCurveIter 'that'
// Return false if it couldn't step, true else
#if BUILDMODE != 0
inline
#endif 
bool SCurveIterStepP(SCurveIter* const that);

// Get the current value of the internal parameter of the 
// SCurveIter 'that'
#if BUILDMODE != 0
inline
#endif 
float SCurveIterGetPos(const SCurveIter* const that);

// Get the current value of the attached SCurve at the current 
// internal position of the SCurveIter 'that'
#if BUILDMODE != 0
inline
#endif 
VecFloat* SCurveIterGet(const SCurveIter* const that);

// -------------- BBody

// ================= Data structure ===================

typedef struct BBody {
  // Order
  const int _order;
  // Dimensions (input/output) (for example (2,3) gives a surface in 3D)
  const VecShort2D _dim;
  // ((_order + 1) ^ _dim[0]) control points of the surface
  // they are ordered as follow: 
  // (0,0,0),(0,0,1),...,(0,0,order+1),(0,1,0),(0,1,1),...
  VecFloat** _ctrl;
} BBody;

// ================ Functions declaration ====================

// Create a new BBody of order 'order' and dimension 'dim'
// Controls are initialized with null vectors
BBody* BBodyCreate(const int order, const VecShort2D* const dim);

// Free the memory used by a BBody
void BBodyFree(BBody** that);

// Set the value of the iCtrl-th control point to v
#if BUILDMODE != 0
inline
#endif 
void _BBodySetCtrl(BBody* const that, const VecShort* const iCtrl, 
  const VecFloat* const v);

// Get the value of the BBody at paramater 'u'
// u can extend beyond [0.0, 1.0]
VecFloat* _BBodyGet(const BBody* const that, const VecFloat* const u);

// Get the number of control points of the BBody 'that'
#if BUILDMODE != 0
inline
#endif 
int BBodyGetNbCtrl(const BBody* const that);

// Get the the 'iCtrl'-th control point of 'that'
#if BUILDMODE != 0
inline
#endif 
const VecFloat* _BBodyCtrl(const BBody* const that, 
  const VecShort* const iCtrl);

// Get the index in _ctrl of the 'iCtrl' control point of 'that'
#if BUILDMODE != 0
inline
#endif 
int _BBodyGetIndexCtrl(const BBody* const that, 
  const VecShort* const iCtrl);

// Get the order of the BBody 'that'
#if BUILDMODE != 0
inline
#endif 
int BBodyGetOrder(const BBody* const that);

// Get the dimensions of the BBody 'that'
#if BUILDMODE != 0
inline
#endif 
const VecShort2D* BBodyDim(const BBody* const that);

// Get a copy of the dimensions of the BBody 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D BBodyGetDim(const BBody* const that);

// Return a clone of the BBody 'that'
BBody* BBodyClone(const BBody* const that);

// Print the BBody 'that' on the stream 'stream'
void BBodyPrint(const BBody* const that, FILE* const stream);

// Function which return the JSON encoding of 'that' 
JSONNode* BBodyEncodeAsJSON(const BBody* const that);

// Function which decode from JSON encoding 'json' to 'that'
bool BBodyDecodeAsJSON(BBody** that, const JSONNode* const json);

// Load the BBody from the stream
// If the BBody is already allocated, it is freed before loading
// Return true upon success, false else
bool BBodyLoad(BBody** that, FILE* const stream);

// Save the BBody to the stream
// If 'compact' equals true it saves in compact form, else it saves in 
// readable form
// Return true upon success, false else
bool BBodySave(const BBody* const that, FILE* const stream, 
  const bool compact);

// Return the center of the BBody (average of control points)
#if BUILDMODE != 0
inline
#endif 
VecFloat* BBodyGetCenter(const BBody* const that);

// Translate the BBody by 'v'
#if BUILDMODE != 0
inline
#endif 
void _BBodyTranslate(BBody* const that, const VecFloat* const v);

// Scale the curve by 'v' relatively to the origin
// of the coordinates system
#if BUILDMODE != 0
inline
#endif 
void _BBodyScaleOriginVector(BBody* const that, const VecFloat* const v);

// Scale the curve by 'c' relatively to the origin
// of the coordinates system
#if BUILDMODE != 0
inline
#endif 
void _BBodyScaleOriginScalar(BBody* const that, const float c);

// Scale the curve by 'v' relatively to its origin
// (first control point)
#if BUILDMODE != 0
inline
#endif 
void _BBodyScaleStartVector(BBody* const that, const VecFloat* const v);

// Scale the curve by 'c' relatively to its origin
// (first control point)
#if BUILDMODE != 0
inline
#endif 
void _BBodyScaleStartScalar(BBody* const that, const float c);

// Scale the curve by 'v' relatively to its center
// (average of control points)
#if BUILDMODE != 0
inline
#endif 
void _BBodyScaleCenterVector(BBody* const that, const VecFloat* const v);

// Scale the curve by 'c' relatively to its center
// (average of control points)
#if BUILDMODE != 0
inline
#endif 
void _BBodyScaleCenterScalar(BBody* const that, const float c);

// Get the bounding box of the BBody.
// Return a Facoid whose axis are aligned on the standard coordinate 
// system.
Facoid* BBodyGetBoundingBox(const BBody* const that);

// Rotate the BBody by 'theta' relatively to the origin
// of the coordinates system around 'axis'
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotAxisOrigin(BBody* const that, const VecFloat3D* const axis, 
  const float theta);

// Rotate the BBody by 'theta' relatively to the center
// of the body around 'axis'
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotAxisCenter(BBody* const that, const VecFloat3D* const axis, 
  const float theta);

// Rotate the BBody by 'theta' relatively to the first control point
// of the body around 'axis'
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotAxisStart(BBody* const that, const VecFloat3D* const axis, 
  const float theta);

// Rotate the BBody by 'theta' relatively to the origin
// of the coordinates system around X
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotXOrigin(BBody* const that, const float theta);

// Rotate the BBody by 'theta' relatively to the center
// of the body around X
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotXCenter(BBody* const that, const float theta);

// Rotate the BBody by 'theta' relatively to the first control point
// of the body around X
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotXStart(BBody* const that, const float theta);

// Rotate the BBody by 'theta' relatively to the origin
// of the coordinates system around Y
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotYOrigin(BBody* const that, const float theta);

// Rotate the BBody by 'theta' relatively to the center
// of the body around Y
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotYCenter(BBody* const that, const float theta);

// Rotate the BBody by 'theta' relatively to the first control point
// of the body around Y
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotYStart(BBody* const that, const float theta);

// Rotate the BBody by 'theta' relatively to the origin
// of the coordinates system around Z
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotZOrigin(BBody* const that, const float theta);

// Rotate the BBody by 'theta' relatively to the center
// of the body around Z
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotZCenter(BBody* const that, const float theta);

// Rotate the BBody by 'theta' relatively to the first control point
// of the body around Z
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotZStart(BBody* const that, const float theta);

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
  const VecShort*: _BBodyGetIndexCtrl, \
  const VecShort2D*: _BBodyGetIndexCtrl, \
  const VecShort3D*: _BBodyGetIndexCtrl, \
  const VecShort4D*: _BBodyGetIndexCtrl, \
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

// ================ Inliner ====================

#if BUILDMODE != 0
#include "bcurve-inline.c"
#endif


#endif
