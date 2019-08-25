// ============ BCURVE.C ================

// ================= Include =================

#include "bcurve.h"
#if BUILDMODE == 0
#include "bcurve-inline.c"
#endif

// -------------- BCurve

// ================ Functions implementation ====================

// Create a new BCurve of order 'order' and dimension 'dim'
BCurve* BCurveCreate(const int order, const int dim) {
#if BUILDMODE == 0
  if (order < 0) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "Invalid order (%d>=0)", order);
    PBErrCatch(BCurveErr);
  }
  if (dim < 1) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "Invalid dimension (%d>=1)", dim);
    PBErrCatch(BCurveErr);
  }
#endif
  // Allocate memory
  BCurve* that = PBErrMalloc(BCurveErr, sizeof(BCurve));
  // Set the values
  *((int*)&(that->_dim)) = dim;
  *((int*)&(that->_order)) = order;
  // Allocate memory for the array of control points
  that->_ctrl = PBErrMalloc(BCurveErr, sizeof(VecFloat*) * (order + 1));
  // For each control point
  for (int iCtrl = order + 1; iCtrl--;)
    // Allocate memory
    that->_ctrl[iCtrl] = VecFloatCreate(dim);
  // Return the new BCurve
  return that;
}

// Clone the BCurve
BCurve* BCurveClone(const BCurve* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // Allocate memory for the clone
  BCurve* clone = PBErrMalloc(BCurveErr, sizeof(BCurve));
  // Clone the properties
  *((int*)&(clone->_dim)) = that->_dim;
  *((int*)&(clone->_order)) = that->_order;
  // Allocate memory for the array of control points
  clone->_ctrl = PBErrMalloc(BCurveErr, sizeof(VecFloat*) * 
    (clone->_order + 1));
  // For each control point
  for (int iCtrl = clone->_order + 1; iCtrl--;)
    // Clone the control point
    clone->_ctrl[iCtrl] = VecClone(that->_ctrl[iCtrl]);
  // Return the clone
  return clone;
}

// Function which return the JSON encoding of 'that' 
JSONNode* BCurveEncodeAsJSON(const BCurve* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    PBMathErr->_type = PBErrTypeNullPointer;
    sprintf(PBMathErr->_msg, "'that' is null");
    PBErrCatch(PBMathErr);
  }
#endif
  // Create the JSON structure
  JSONNode* json = JSONCreate();
  // Declare a buffer to convert value into string
  char val[100];
  // Encode the order
  sprintf(val, "%d", BCurveGetOrder(that));
  JSONAddProp(json, "_order", val);
  // Encode the dimension
  sprintf(val, "%d", BCurveGetDim(that));
  JSONAddProp(json, "_dim", val);
  // Encode the control points
  JSONArrayStruct setCtrl = JSONArrayStructCreateStatic();
  for (int iCtrl = 0; iCtrl < BCurveGetOrder(that) + 1; ++iCtrl)
    JSONArrayStructAdd(&setCtrl, 
      VecEncodeAsJSON(BCurveCtrl(that, iCtrl)));
  JSONAddProp(json, "_ctrl", &setCtrl);
  // Free memory
  JSONArrayStructFlush(&setCtrl);
  // Return the created JSON 
  return json;
}

// Function which decode from JSON encoding 'json' to 'that'
bool BCurveDecodeAsJSON(BCurve** that, const JSONNode* const json) {
#if BUILDMODE == 0
  if (that == NULL) {
    PBMathErr->_type = PBErrTypeNullPointer;
    sprintf(PBMathErr->_msg, "'that' is null");
    PBErrCatch(PBMathErr);
  }
  if (json == NULL) {
    PBMathErr->_type = PBErrTypeNullPointer;
    sprintf(PBMathErr->_msg, "'json' is null");
    PBErrCatch(PBMathErr);
  }
#endif
  // If 'that' is already allocated
  if (*that != NULL)
    // Free memory
    BCurveFree(that);
  // Get the order from the JSON
  JSONNode* prop = JSONProperty(json, "_order");
  if (prop == NULL) {
    return false;
  }
  int order = atoi(JSONLblVal(prop));
  // Get the dimension from the JSON
  prop = JSONProperty(json, "_dim");
  if (prop == NULL) {
    return false;
  }
  int dim = atoi(JSONLblVal(prop));
  // If data are invalid
  if (order < 0 || dim < 1)
    return false;
  // Allocate memory
  *that = BCurveCreate(order, dim);
  // Decode the control points
  prop = JSONProperty(json, "_ctrl");
  if (prop == NULL) {
    return false;
  }
  if (JSONGetNbValue(prop) != order + 1) {
    return false;
  }
  for (int iCtrl = 0; iCtrl < order + 1; ++iCtrl) {
    JSONNode* ctrl = JSONValue(prop, iCtrl);
    if (!VecDecodeAsJSON((*that)->_ctrl + iCtrl, ctrl) ||
      VecGetDim((*that)->_ctrl[iCtrl]) != BCurveGetDim(*that)) {
      return false;
    }
  }
  // Return the success code
  return true;
}

// Load the BCurve from the stream
// If the BCurve is already allocated, it is freed before loading
// Return true upon success, false else
bool BCurveLoad(BCurve** that, FILE* const stream) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (stream == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'stream' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // Declare a json to load the encoded data
  JSONNode* json = JSONCreate();
  // Load the whole encoded data
  if (!JSONLoad(json, stream)) {
    return false;
  }
  // Decode the data from the JSON
  if (!BCurveDecodeAsJSON(that, json)) {
    return false;
  }
  // Free the memory used by the JSON
  JSONFree(&json);
  // Return the success code
  return true;
}

// Save the BCurve to the stream
// If 'compact' equals true it saves in compact form, else it saves in 
// readable form
// Return true upon success, false else
bool BCurveSave(const BCurve* const that, FILE* const stream, 
  const bool compact) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (stream == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'stream' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // Get the JSON encoding
  JSONNode* json = BCurveEncodeAsJSON(that);
  // Save the JSON
  if (!JSONSave(json, stream, compact)) {
    return false;
  }
  // Free memory
  JSONFree(&json);
  // Return success code
  return true;
}

// Free the memory used by a BCurve
void BCurveFree(BCurve** that) {
  // Check argument
  if (that == NULL || *that == NULL)
    return;
  // If there are control points
  if ((*that)->_ctrl != NULL)
    // For each control point
    for (int iCtrl = (*that)->_order + 1; iCtrl--;)
      // Free the control point
      VecFree((*that)->_ctrl + iCtrl);
  // Free the array of control points
  free((*that)->_ctrl);
  // Free memory
  free(*that);
  *that = NULL;
}

// Print the BCurve on 'stream'
void BCurvePrint(const BCurve* const that, FILE* const stream) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (stream == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'stream' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // Print the order and dim
  fprintf(stream, "order(%d) dim(%d) ", that->_order, that->_dim);
  // For each control point
  for (int iCtrl = 0; iCtrl < that->_order + 1; ++iCtrl) {
    VecPrint(that->_ctrl[iCtrl], stream);
    if (iCtrl < that->_order) 
      fprintf(stream, " ");
  }
}

// Get the value of the BCurve at paramater 'u'
// u can extend beyond [0.0, 1.0]
VecFloat* BCurveGet(const BCurve* const that, const float u) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // Allocate memory for the result
  VecFloat* v = VecFloatCreate(that->_dim);
  // Declare a variable for calcul
  VecFloat* val = VecFloatCreate(that->_order + 1);
  // Loop on dimension
  for (int dim = that->_dim; dim--;) {
    // Initialise the temporary variable with the value in current
    // dimension of the control points
    for (int iCtrl = 0; iCtrl < that->_order + 1; ++iCtrl)
      VecSet(val, iCtrl, VecGet(that->_ctrl[iCtrl], dim));
    // Loop on order
    int subOrder = that->_order;
    while (subOrder != 0) {
      // Loop on sub order
      for (int order = 0; order < subOrder; ++order)
        VecSet(val, order, 
          (1.0 - u) * VecGet(val, order) + u * VecGet(val, order + 1));
      --subOrder;
    }
    // Set the value for the current dim
    VecSet(v, dim, VecGet(val, 0));
  }
  // Free memory
  VecFree(&val);
  // Return the result
  return v;
}

// Create a BCurve which pass through the points given in the GSet 'set'
// The GSet must contains VecFloat of same dimensions
// The BCurve pass through the points in the order they are given
// in the GSet. The points don't need to be uniformly distributed
// The created BCurve is of same dimension as the VecFloat and of order 
// equal to the number of VecFloat in 'set' minus one
// Return NULL if it couldn't create the BCurve
BCurve* BCurveFromCloudPoint(const GSetVecFloat* const set) {
#if BUILDMODE == 0
  if (set == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'set' is null");
    PBErrCatch(BCurveErr);
  }
  if (GSetNbElem(set) < 1) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "'set' is empty");
    PBErrCatch(BCurveErr);
  }
#endif
  // Declare a variable to memorize the result
  int order = GSetNbElem(set) - 1;
  long dim = VecGetDim(GSetHead(set));
  BCurve* curve = BCurveCreate(order, dim);
  // Set the first control point to the first point in the point cloud
  BCurveSetCtrl(curve, 0, GSetHead(set));
  // If the order is greater than 0
  if (order > 0) {
    // Set the last control point to the last point in the point cloud
    BCurveSetCtrl(curve, order, GSetTail(set));
    // If the order is greater than 1
    if (order > 1) {
      // Calculate the t values for intermediate control points
      // They are equal to the relative distance on the polyline
      // linking the point in the point cloud
      // Declare a variable to memorize the dimension of the matrix
      // in the linear system to solve
      VecShort2D dimMat = VecShortCreateStatic2D();
      // Declare a variable to memorize the t values
      VecFloat* t = VecFloatCreate(GSetNbElem(set));
      // Set the dimensions of the matrix of the linear system
      VecSet(&dimMat, 0, order - 1);
      VecSet(&dimMat, 1, order - 1);
      // For each point 
      GSetIterForward iter = GSetIterForwardCreateStatic(set);
      (void)GSetIterStep(&iter);
      int iPoint = 1;
      do {
        // Get the distance from the previous point
        VecFloat* curPoint = GSetIterGet(&iter);
        VecFloat* prevPoint = 
          GSetElemData(GSetElemPrev(GSetIterGetElem(&iter)));
        float d = VecDist(prevPoint, curPoint);
        VecSet(t, iPoint, d + VecGet(t, iPoint - 1));
        ++iPoint;
      } while(GSetIterStep(&iter));
      // Normalize t
      for (iPoint = 1; iPoint <= order; ++iPoint)
        VecSet(t, iPoint, VecGet(t, iPoint) / VecGet(t, order));
      // For each dimension
      for (long iDim = dim; iDim--;) {
        // Declare a variable to memorize the matrix and vector 
        // of the linear system
        MatFloat* m = MatFloatCreate(&dimMat);
        VecFloat* v = VecFloatCreate(order - 1);
        // Set the values of the linear system
        // For each line (equivalent to each intermediate point 
        // in point cloud)
        for (VecSet(&dimMat, 1, 0); 
          VecGet(&dimMat, 1) < order - 1;
          VecSetAdd(&dimMat, 1, 1)) {
          // Get the weight of the control point at the value 
          // of t for this point
          VecFloat* weight = 
            BCurveGetWeightCtrlPt(curve, VecGet(t, 
            VecGet(&dimMat, 1) + 1));
          // For each intermediate control point
          for (VecSet(&dimMat, 0, 0); 
            VecGet(&dimMat, 0) < order - 1;
            VecSetAdd(&dimMat, 0, 1))
            // Set the matrix value with the corresponding
            // weight
            MatSet(m, &dimMat, VecGet(weight, 
              VecGet(&dimMat, 0) + 1));
          // Set the vector value with the corresponding point
          // coordinate
          float x = VecGet((VecFloat*)(GSetGet(set, 
            VecGet(&dimMat, 1) + 1)), iDim);
          x -= VecGet(weight, 0) * VecGet(GSetHead(set), iDim);
          x -= VecGet(weight, order) * 
            VecGet(GSetTail(set), iDim);
          VecSet(v, VecGet(&dimMat, 1), x);
          // Free memory
          VecFree(&weight);
        }
        // Declare a variable to memorize the linear system
        SysLinEq* sys = SysLinEqCreate(m, v);
        // Solve the system
        VecFloat* solSys = SysLinEqSolve(sys);
        // If we could solve the linear system
        if (solSys != NULL) {
          // Memorize the values of control points for the 
          // current dimension
          for (int iCtrl = 1; iCtrl < order; ++iCtrl)
            VecSet(curve->_ctrl[iCtrl], iDim, 
              VecGet(solSys, iCtrl - 1));
          // Free memory
          VecFree(&solSys);
        } else {
          // Free memory
          SysLinEqFree(&sys);
          VecFree(&v);
          MatFree(&m);
          VecFree(&t);
          BCurveFree(&curve);
          // Return NULL
          return NULL;
        }
        // Free memory
        SysLinEqFree(&sys);
        VecFree(&v);
        MatFree(&m);
      }
      // Free memory
      VecFree(&t);
    }
  }
  // Return the result
  return curve;
}

// Get a VecFloat of dimension equal to the number of control points
// Values of the VecFloat are the weight of each control point in the 
// BCurve given the curve's order and the value of 't' (in [0.0,1.0])
VecFloat* BCurveGetWeightCtrlPt(const BCurve* const that, 
  const float t) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (t < 0.0 - PBMATH_EPSILON || t > 1.0 + PBMATH_EPSILON) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "'t' is invalid (0.0<=%f<=1.0)", t);
    PBErrCatch(BCurveErr);
  }
#endif
  // Declare a variable to memorize the result
  VecFloat* res = VecFloatCreate(that->_order + 1);
  // Initilize the two first weights
  VecSet(res, 0, 1.0 - t);
  VecSet(res, 1, t);
  // For each higher order
  for (int order = 1; order < that->_order; ++order) {
    // For each control point at this order, starting by the last one
    // to avoid using a temporary buffer
    for (int iCtrl = order + 2; iCtrl-- && iCtrl != 0;)
      // Calculate the weight of this control point
      VecSet(res, iCtrl, 
        (1.0 - t) * VecGet(res, iCtrl) + t * VecGet(res, iCtrl - 1));
    // Calculate the weight of the first control point
    VecSet(res, 0, (1.0 - t) * VecGet(res, 0));
  }
  // Return the result
  return res;
}

// Get the bounding box of the BCurve.
// Return a Facoid whose axis are aligned on the standard coordinate 
// system.
Facoid* BCurveGetBoundingBox(const BCurve* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // Declare a variable to memorize the result
  Facoid* res = FacoidCreate(BCurveGetDim(that));
  // Create 2 vectors to memorize min and max coordinates in each 
  // dimensions
  VecFloat* min = VecClone(BCurveCtrl(that, 0));
  VecFloat* max = VecClone(BCurveCtrl(that, 0));
  // For each dimension
  for (int iDim = that->_dim; iDim--;) {
    // For each control point except the first one
    for (int iCtrl = that->_order + 1; iCtrl-- && iCtrl != 0;) {
      // Update the bounding box
      if (BCurveCtrlGet(that, iCtrl, iDim) < VecGet(min, iDim))
        VecSet(min, iDim, BCurveCtrlGet(that, iCtrl, iDim));
      if (BCurveCtrlGet(that, iCtrl, iDim) > VecGet(max, iDim))
        VecSet(max, iDim, BCurveCtrlGet(that, iCtrl, iDim));
    }
  }
  // Update the result Facoid
  ShapoidSetPos(res, min);
  for (int iDim = that->_dim; iDim--;) {
    float d = VecGet(max, iDim) - VecGet(min, iDim);
    if (d < PBMATH_EPSILON)
      d = 2.0 * PBMATH_EPSILON;
    ShapoidAxisSet(res, iDim, iDim, d);
  }
  // Free memory
  VecFree(&min);
  VecFree(&max);
  // Return the result
  return res;
}

// -------------- SCurve

// ================ Functions implementation ====================

// Create a new SCurve of dimension 'dim', order 'order' and 
// 'nbSeg' segments
SCurve* SCurveCreate(const int order, const int dim, const int nbSeg) {
#if BUILDMODE == 0
  if (order < 0) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "Invalid order (%d>=0)", order);
    PBErrCatch(BCurveErr);
  }
  if (dim < 1) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "Invalid dimension (%d>=1)", dim);
    PBErrCatch(BCurveErr);
  }
  if (nbSeg < 1) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "Invalid number of segment (%d>=1)", nbSeg);
    PBErrCatch(BCurveErr);
  }
#endif
  // Allocate memory
  SCurve* that = PBErrMalloc(BCurveErr, sizeof(SCurve));
  // Set the values
  int* d = (int*)&(that->_dim);
  *d = dim;
  int* o = (int*)&(that->_order);
  *o = order;
  that->_nbSeg = nbSeg;
  // Create the GSet
  that->_ctrl = GSetVecFloatCreateStatic();
  that->_seg = GSetBCurveCreateStatic();
  // For each segment
  for (int iSeg = nbSeg; iSeg--;) {
    // Create a segment
    BCurve* seg = BCurveCreate(order, dim);
    // If it's not the first added segment
    if (iSeg != nbSeg - 1) {
      // Replace the last control points by the current first
      VecFree(seg->_ctrl + order);
      seg->_ctrl[order] = GSetHead(&(that->_ctrl));
      // Add the control points
      for (int iCtrl = order; iCtrl--;)
        GSetPush(&(that->_ctrl), (VecFloat*)BCurveCtrl(seg, iCtrl));
    // Else, it's the first segment
    } else {
      // Add the control points
      for (int iCtrl = order + 1; iCtrl--;)
        GSetPush(&(that->_ctrl), (VecFloat*)BCurveCtrl(seg, iCtrl));
    }
    // Add the segment
    GSetPush(&(that->_seg), seg);
  }
  // Return the new SCurve
  return that;
}

// Clone the SCurve
SCurve* SCurveClone(const SCurve* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  SCurve* clone = SCurveCreate(SCurveGetOrder(that), SCurveGetDim(that),
    SCurveGetNbSeg(that));
  // For each control point
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_ctrl));
  GSetIterForward iterClone = 
    GSetIterForwardCreateStatic(&(clone->_ctrl));
  do {
    VecFloat* ctrl = GSetIterGet(&iter);
    VecFloat* ctrlClone = GSetIterGet(&iterClone);
    VecCopy(ctrlClone, ctrl);
  } while (GSetIterStep(&iter) && GSetIterStep(&iterClone));
  return clone;
}

// Return a new SCurve as a copy of the SCurve 'that' with 
// dimension changed to 'dim'
// if it is extended, the values of new components are 0.0
// If it is shrinked, values are discarded from the end of the vectors
SCurve* SCurveGetNewDim(const SCurve* const that, const int dim) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (dim <= 0) {
    PBMathErr->_type = PBErrTypeInvalidArg;
    sprintf(PBMathErr->_msg, "'dim' is invalid match (%d>0)", dim);
    PBErrCatch(PBMathErr);
  }
#endif
  // If the new dimension is equals to the current one
  if (SCurveGetDim(that) == dim) {
    // Return the clone of the initial curve
    return SCurveClone(that);
  } else {
    // Clone the initial curve
    SCurve* ret = 
      SCurveCreate(SCurveGetOrder(that), dim, SCurveGetNbSeg(that));
    // Convert the dimension of each control point
    GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_ctrl));
    GSetIterForward iterNew = 
      GSetIterForwardCreateStatic(&(ret->_ctrl));
    do {
      VecFloat* newCtrl = 
        VecGetNewDim((VecFloat*)GSetIterGet(&iter), dim);
      VecCopy((VecFloat*)(GSetIterGetElem(&iterNew)->_data), newCtrl);
      VecFree(&newCtrl);
    } while (GSetIterStep(&iter) && GSetIterStep(&iterNew));
    // Return the new curve
    return ret;
  }
}

// Function which return the JSON encoding of 'that' 
JSONNode* SCurveEncodeAsJSON(const SCurve* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    PBMathErr->_type = PBErrTypeNullPointer;
    sprintf(PBMathErr->_msg, "'that' is null");
    PBErrCatch(PBMathErr);
  }
#endif
  // Create the JSON structure
  JSONNode* json = JSONCreate();
  // Declare a buffer to convert value into string
  char val[100];
  // Encode the order
  sprintf(val, "%d", SCurveGetOrder(that));
  JSONAddProp(json, "_order", val);
  // Encode the dimension
  sprintf(val, "%d", SCurveGetDim(that));
  JSONAddProp(json, "_dim", val);
  // Encode the nb of segment
  sprintf(val, "%d", SCurveGetNbSeg(that));
  JSONAddProp(json, "_nbSeg", val);
  // Encode the control points
  JSONArrayStruct setCtrl = JSONArrayStructCreateStatic();
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_ctrl));
  do {
    VecFloat* ctrl = (VecFloat*)GSetIterGet(&iter);
    JSONArrayStructAdd(&setCtrl, VecEncodeAsJSON(ctrl));
  } while (GSetIterStep(&iter));
  JSONAddProp(json, "_ctrl", &setCtrl);
  // Free memory
  JSONArrayStructFlush(&setCtrl);
  // Return the created JSON 
  return json;
}

// Function which decode from JSON encoding 'json' to 'that'
bool SCurveDecodeAsJSON(SCurve** that, const JSONNode* const json) {
#if BUILDMODE == 0
  if (that == NULL) {
    PBMathErr->_type = PBErrTypeNullPointer;
    sprintf(PBMathErr->_msg, "'that' is null");
    PBErrCatch(PBMathErr);
  }
  if (json == NULL) {
    PBMathErr->_type = PBErrTypeNullPointer;
    sprintf(PBMathErr->_msg, "'json' is null");
    PBErrCatch(PBMathErr);
  }
#endif
  // If 'that' is already allocated
  if (*that != NULL)
    // Free memory
    SCurveFree(that);
  // Get the order from the JSON
  JSONNode* prop = JSONProperty(json, "_order");
  if (prop == NULL) {
    return false;
  }
  int order = atoi(JSONLblVal(prop));
  // Get the dimension from the JSON
  prop = JSONProperty(json, "_dim");
  if (prop == NULL) {
    return false;
  }
  int dim = atoi(JSONLblVal(prop));
  // Get the nb of segment from the JSON
  prop = JSONProperty(json, "_nbSeg");
  if (prop == NULL) {
    return false;
  }
  int nbSeg = atoi(JSONLblVal(prop));
  // If data are invalid
  if (nbSeg < 1 || order < 0 || dim < 1)
    return false;
  // Allocate memory
  *that = SCurveCreate(order, dim, nbSeg);
  // Decode the control points
  prop = JSONProperty(json, "_ctrl");
  if (prop == NULL) {
    return false;
  }
  if (JSONGetNbValue(prop) != SCurveGetNbCtrl(*that)) {
    return false;
  }
  GSetIterForward iter = GSetIterForwardCreateStatic(&((*that)->_ctrl));
  int iCtrl = 0;
  do {
    VecFloat* loadCtrl = NULL;
    JSONNode* ctrl = JSONValue(prop, iCtrl);
    if (!VecDecodeAsJSON(&loadCtrl, ctrl) || 
      VecGetDim(loadCtrl) != dim) {
      return false;
    }
    // Set the loaded control point into the set of control point
    // and segment
    VecCopy((VecFloat*)GSetIterGet(&iter), loadCtrl);
    // Free memory used by the loaded control
    VecFree(&loadCtrl);
    ++iCtrl;
  } while (GSetIterStep(&iter));
  // Return the success code
  return true;
}

// Load the SCurve from the stream
// If the SCurve is already allocated, it is freed before loading
// Return true in case of success, false else
bool SCurveLoad(SCurve** that, FILE* const stream) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (stream == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'stream' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // Declare a json to load the encoded data
  JSONNode* json = JSONCreate();
  // Load the whole encoded data
  if (!JSONLoad(json, stream)) {
    return false;
  }
  // Decode the data from the JSON
  if (!SCurveDecodeAsJSON(that, json)) {
    return false;
  }
  // Free the memory used by the JSON
  JSONFree(&json);
  // Return the success code
  return true;
}

// Save the SCurve to the stream
// If 'compact' equals true it saves in compact form, else it saves in 
// readable form
// Return true upon success, false else
bool SCurveSave(const SCurve* const that, FILE* const stream, 
  const bool compact) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (stream == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'stream' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // Get the JSON encoding
  JSONNode* json = SCurveEncodeAsJSON(that);
  // Save the JSON
  if (!JSONSave(json, stream, compact)) {
    return false;
  }
  // Free memory
  JSONFree(&json);
  // Return success code
  return true;
}

// Free the memory used by a SCurve
void SCurveFree(SCurve** that) {
  // Check argument
  if (that == NULL || *that == NULL)
    return;
  // For each control point
  GSetIterForward iter = GSetIterForwardCreateStatic(&((*that)->_ctrl));
  do {
    VecFloat* ctrl = (VecFloat*)GSetIterGet(&iter);
    // Free the memory used by the control point
    VecFree(&ctrl);
  } while (GSetIterStep(&iter));
  // Free the memory used by the set of control point
  GSetFlush(&((*that)->_ctrl));
  // For each segment
  iter = GSetIterForwardCreateStatic(&((*that)->_seg));
  do {
    BCurve* seg = (BCurve*)GSetIterGet(&iter);
    // Disconnect the control points which have been already freed
    // or doesn't need to be freed (the last one)
    for (int iCtrl = 0; iCtrl <= (*that)->_order; ++iCtrl)
      seg->_ctrl[iCtrl] = NULL;
    // Free the meory used by the segment
    BCurveFree(&seg);
  } while (GSetIterStep(&iter));
  // Free the memory used by the set of segment
  GSetFlush(&((*that)->_seg));
  // Free memory
  free(*that);
  *that = NULL;
}

// Print the SCurve on 'stream'
void SCurvePrint(const SCurve* const that, FILE* const stream) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (stream == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'stream' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // Print the order and dim
  fprintf(stream, "order(%d) dim(%d) nbSeg(%d) ", 
    that->_order, that->_dim, that->_nbSeg);
  // For each control point
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_ctrl));
  int iMark = 0;
  do {
    VecFloat* ctrl = (VecFloat*)GSetIterGet(&iter);
    if (iMark == 0)
      fprintf(stream, "<");
    //VecPrint(ctrl, stream);
    VecFloatPrint(ctrl, stream, 6);
    if (iMark == 0)
      fprintf(stream, ">");
    if (GSetIterIsLast(&iter) == false) 
      fprintf(stream, " ");
    ++iMark;
    if (iMark == that->_order)
      iMark = 0;
  } while (GSetIterStep(&iter));
}

// Add one segment at the end of the curve (controls are set to 
// vectors null, except the first one which the last one of the current
// last segment)
void SCurveAddSegTail(SCurve* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // Create the new segment
  BCurve* seg = BCurveCreate(that->_order, that->_dim);
  // Free memory used by the first control point
  VecFree(seg->_ctrl);
  // Replace it with the current last control
  seg->_ctrl[0] = GSetTail(&(that->_ctrl));
  // Add the segment to the set of segment
  GSetAppend(&(that->_seg), seg);
  // Add the new control points to the set of control points
  for (int iCtrl = 1; iCtrl <= that->_order; ++iCtrl)
    GSetAppend(&(that->_ctrl), seg->_ctrl[iCtrl]);
  // Update the number of segment
  ++(that->_nbSeg);
}

// Add one segment at the head of the curve (controls are set to 
// vectors null, except the last one which the first one of the current
// first segment)
void SCurveAddSegHead(SCurve* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // Create the new segment
  BCurve* seg = BCurveCreate(that->_order, that->_dim);
  // Free memory used by the last control point
  VecFree(seg->_ctrl + that->_order);
  // Replace it with the current first control
  seg->_ctrl[that->_order] = GSetHead(&(that->_ctrl));
  // Add the segment to the set of segment
  GSetPush(&(that->_seg), seg);
  // Add the new control points to the set of control points
  for (int iCtrl = that->_order; iCtrl--;)
    GSetPush(&(that->_ctrl), seg->_ctrl[iCtrl]);
  // Update the number of segment
  ++(that->_nbSeg);
}

// Remove the first segment of the curve (which must have more than one
// segment)
void SCurveRemoveHeadSeg(SCurve* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (that->_nbSeg < 2) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "'that' has only one segment");
    PBErrCatch(BCurveErr);
  }
#endif
  // Remove the control points from the set of control points
  for (int iCtrl = 0; iCtrl < that->_order; ++iCtrl) {
    VecFloat* ctrl = (VecFloat*)GSetPop(&(that->_ctrl));
    VecFree(&ctrl);
  }
  // Remove the first segment
  BCurve* seg = (BCurve*)GSetPop(&(that->_seg));
  // Disconnect the control points which have been already freed
  // or doesn't need to be freed (the last one)
  for (int iCtrl = 0; iCtrl <= that->_order; ++iCtrl)
    seg->_ctrl[iCtrl] = NULL;
  // Free the memory used by the segment
  BCurveFree(&seg);
  // Update the number of segment
  --(that->_nbSeg);
}

// Remove the last segment of the curve (which must have more than one
// segment)
void SCurveRemoveTailSeg(SCurve* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (that->_nbSeg < 2) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "'that' has only one segment");
    PBErrCatch(BCurveErr);
  }
#endif
  // Remove the control points from the set of control points
  for (int iCtrl = 0; iCtrl < that->_order; ++iCtrl) {
    VecFloat* ctrl = (VecFloat*)GSetDrop(&(that->_ctrl));
    VecFree(&ctrl);
  }
  // Remove the last segment
  BCurve* seg = (BCurve*)GSetDrop(&(that->_seg));
  // Disconnect the control points which have been already freed
  // or doesn't need to be freed (the first one)
  for (int iCtrl = 0; iCtrl <= that->_order; ++iCtrl)
    seg->_ctrl[iCtrl] = NULL;
  // Free the memory used by the segment
  BCurveFree(&seg);
  // Update the number of segment
  --(that->_nbSeg);
}

// Get the bounding box of the SCurve.
// Return a Facoid whose axis are aligned on the standard coordinate 
// system.
// TODO : better solution possible, refer to 
// https://pomax.github.io/bezierinfo/#circles_cubic
Facoid* SCurveGetBoundingBox(const SCurve* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // Declare a set to memorize the bounding box of each segment
  GSetShapoid set = GSetShapoidCreateStatic();
  // For each segment
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_seg));
  do {
    // Add the bounding box of this segment to the set
    GSetPush(&set, 
      BCurveGetBoundingBox((BCurve*)GSetIterGet(&iter)));
  } while (GSetIterStep(&iter));
  // Get the bounding box of all the segment's bounding box
  Facoid* bound = ShapoidGetBoundingBoxSet(&set);
  // Free the memory used by the bounding box of each segment
  iter = GSetIterForwardCreateStatic(&set);
  do {
    Facoid* facoid = (Facoid*)GSetIterGet(&iter);
    ShapoidFree(&facoid);
  } while (GSetIterStep(&iter));
  GSetFlush(&set);
  // Return the bounding box
  return bound;
}

// Create a new SCurve from the outline of the Facoid 'shap'
// The Facoid must be of dimension 2
// Control points are ordered CCW of the Shapoid
SCurve* SCurveCreateFromFacoid(const Facoid* const shap) {
#if BUILDMODE == 0
  if (shap == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'shap' is null");
    PBErrCatch(BCurveErr);
  }
  if (ShapoidGetDim(shap) != 2) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, 
      "'shap' 's dimension is invalid (%d==2)", 
      ShapoidGetDim(shap));
    PBErrCatch(BCurveErr);
  }
#endif
  // Create the curve
  int order = 1;
  int dim = 2;
  int nbSeg = 4;
  SCurve* ret = SCurveCreate(order, dim, nbSeg);
  // Set the coordinates of the control points according to the 
  // Facoid
  VecFloat* v = VecClone(ShapoidPos(shap));
  SCurveSetCtrl(ret, 0, v);
  SCurveSetCtrl(ret, 4, v);
  VecOp(v, 1.0, ShapoidAxis(shap, 0), 1.0);
  SCurveSetCtrl(ret, 1, v);
  VecOp(v, 1.0, ShapoidAxis(shap, 1), 1.0);
  SCurveSetCtrl(ret, 2, v);
  VecOp(v, 1.0, ShapoidAxis(shap, 0), -1.0);
  SCurveSetCtrl(ret, 3, v);
  VecFree(&v);
  // Return the curve
  return ret;
}

// Create a new SCurve from the outline of the Pyramidoid 'shap'
// The Pyramidoid must be of dimension 2
// Control points are ordered CCW of the Shapoid
SCurve* SCurveCreateFromPyramidoid(const Pyramidoid* const shap) {
#if BUILDMODE == 0
  if (shap == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'shap' is null");
    PBErrCatch(BCurveErr);
  }
  if (ShapoidGetDim(shap) != 2) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, 
      "'shap' 's dimension is invalid (%d==2)", 
      ShapoidGetDim(shap));
    PBErrCatch(BCurveErr);
  }
#endif
  // Create the curve
  SCurve* ret = SCurveCreate(1, 2, 3);
  // Set the coordinates of the control points according to the 
  // Facoid
  VecFloat* ctrl[2] = {NULL};
  for (int i = 2; i--;) {
    ctrl[i] = VecClone(ShapoidPos(shap));
  }
  VecOp(ctrl[0], 1.0, ShapoidAxis(shap, 0), 1.0);
  VecOp(ctrl[1], 1.0, ShapoidAxis(shap, 1), 1.0);
  SCurveSetCtrl(ret, 0, ShapoidPos(shap));
  SCurveSetCtrl(ret, 1, ctrl[0]);
  SCurveSetCtrl(ret, 2, ctrl[1]);
  SCurveSetCtrl(ret, 3, ShapoidPos(shap));
  for (int i = 2; i--;)
    VecFree(ctrl + i);
  // Return the curve
  return ret;
}

// Create a new SCurve from the outline of the Spheroid 'shap'
// The Spheroid must be of dimension 2
// Control points are ordered CCW of the Shapoid
// Calculate an approximation as there is no exact solution
SCurve* SCurveCreateFromSpheroid(const Spheroid* const shap) {
#if BUILDMODE == 0
  if (shap == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'shap' is null");
    PBErrCatch(BCurveErr);
  }
  if (ShapoidGetDim(shap) != 2) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, 
      "'shap' 's dimension is invalid (%d==2)", 
      ShapoidGetDim(shap));
    PBErrCatch(BCurveErr);
  }
#endif
  // Create the curve
  SCurve* ret = SCurveCreate(3, 2, 4);
  // Set the control points
  // The anchors of the curve can be easily calculated from the 
  // position and axis of the Spheroid
  int iAxis = 0;
  float coeff = 0.5;
  VecFloat* ctrl[13] = {NULL};
  for (int i = 13; i--;)
    ctrl[i] = VecFloatCreate(2);
  for (int i = 0; i < 12; i += 3) {
    VecCopy(ctrl[i], ShapoidPos(shap));
    if (i == 6)
      coeff *= -1.0;
    VecOp(ctrl[i], 1.0, ShapoidAxis(shap, iAxis), coeff);
    if (i > 0)
      VecCopy(ctrl[i - 1], ctrl[i]);
    if (i < 11)
      VecCopy(ctrl[i + 1], ctrl[i]);
    iAxis = (iAxis == 0 ? 1 : 0);
  }
  VecCopy(ctrl[12], ctrl[0]);
  VecCopy(ctrl[11], ctrl[0]);
  // Calculate the others control points by transforming the 
  // quadratic approximation of a quarter of the unit circle :
  // A(1,0), B(1,4(sqrt(2)-1)/3), C(4(sqrt(2)-1)/3,1), D(0,1)
  // toward the Spheroid
  float c = 0.276142;
  VecOp(ctrl[1], 1.0, ShapoidAxis(shap, 1), c);
  VecOp(ctrl[2], 1.0, ShapoidAxis(shap, 0), c);
  VecOp(ctrl[4], 1.0, ShapoidAxis(shap, 0), -1.0 * c);
  VecOp(ctrl[5], 1.0, ShapoidAxis(shap, 1), c);
  VecOp(ctrl[7], 1.0, ShapoidAxis(shap, 1), -1.0 * c);
  VecOp(ctrl[8], 1.0, ShapoidAxis(shap, 0), -1.0 * c);
  VecOp(ctrl[10], 1.0, ShapoidAxis(shap, 0), c);
  VecOp(ctrl[11], 1.0, ShapoidAxis(shap, 1), -1.0 * c);
  for (int i = 13; i--;)
    SCurveSetCtrl(ret, i, ctrl[i]);
  for (int i = 13; i--;)
    VecFree(ctrl + i);
  // Return the curve
  return ret;
}

// Get the distance between the SCurve 'that' and the SCurve 'curve'
// The distance is defined as the integral of 
// ||'that'(u(t))-'curve'(v(t))|| where u and v are the relative 
// positions on the curve over t varying from 0.0 to 1.0
float SCurveGetDistToCurve(const SCurve* const that, 
  const SCurve* const curve) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'shap' is null");
    PBErrCatch(BCurveErr);
  }
  if (curve == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'shap' is null");
    PBErrCatch(BCurveErr);
  }
  if (SCurveGetDim(that) != SCurveGetDim(curve)) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, 
      "dimensions of 'that' and 'curve' differ (%d==%d)",
      SCurveGetDim(that), SCurveGetDim(curve));
    PBErrCatch(BCurveErr);
  }
#endif
  // Declare a variable to memorize the result
  float res = 0.0;
  // Declare a variable to memorize the step over parameter
  float dt = 0.01;
  int nb = (int)floor(1.0 / dt); 
  float t = 0.0;
  // Loop over the parameter
  for (int i = nb; i--;) {
    // Calculate the relative parameter for both curves
    float u = t * SCurveGetMaxU(that);
    float v = t * SCurveGetMaxU(curve);
    // Get the value of both curve at these relative parameters
    VecFloat* valA = SCurveGet(that, u);
    VecFloat* valB = SCurveGet(curve, v);
    // Get the distance between value
    float dist = VecDist(valA, valB);
    // Add to result
    res += dist * dt;
    // Step the parameter
    t += dt;
    // Free memory
    VecFree(&valA);
    VecFree(&valB);
  }
  // Return the result
  return res;
}

// Apply the chaikin curve subdivision algorithm to the SCurve 'that'
// with 'depth' times recursion and 'strength' is the parametric
// distance from each corner where the curve is cut at each recursion
// 'strength' in [0.0, 1.0]
// The SCurve must be of order 1, if it is not nothing happens
// cf http://graphics.cs.ucdavis.edu/education/CAGDNotes/Chaikins-Algorithm.pdf
SCurve* SCurveChaikinSubdivision(SCurve* const that, 
  const float strength, const unsigned int depth) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (strength < 0.0 || strength > 1.0) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "'strength' is invalid (0<=%f<=1)", 
      strength);
    PBErrCatch(BCurveErr);
  }
#endif

  // Init the result with a clone of the original curve
  SCurve* res = SCurveClone(that);

  // If the SCurve order is 0
  if (SCurveGetOrder(that) == 1) {

    // Loop on the depth
    for (unsigned int iDepth = depth; iDepth--;) {

      // Declare the new set of BCurve
      GSetBCurve segs = GSetBCurveCreateStatic();

      // Declare the new set of control points
      GSetVecFloat ctrls = GSetVecFloatCreateStatic();

      // Declare a variable to memorize the new segment
      BCurve* newSeg = NULL;
      
      // Loop on the segments of the curve
      GSetIterForward iter = GSetIterForwardCreateStatic(
        SCurveSegs(res));
      do {

        // Get the current segment
        BCurve* curSeg = GSetIterGet(&iter);

        // If it's the first segment
        if (GSetIterIsFirst(&iter)) {
          
          // Get the cut position
          VecFloat* pos = BCurveGet(curSeg, 1.0 - strength);

          // Create the new segment for the head of the current segment
          newSeg = BCurveCreate(SCurveGetOrder(that), 
            SCurveGetDim(that));
          VecFree(newSeg->_ctrl);
          VecFree(newSeg->_ctrl + 1);
          newSeg->_ctrl[0] = VecClone(curSeg->_ctrl[0]);
          newSeg->_ctrl[1] = pos;
          
          // Add the new segment
          GSetAppend(&segs, newSeg);
          GSetAppend(&ctrls, newSeg->_ctrl[0]);
          GSetAppend(&ctrls, newSeg->_ctrl[1]);
          
          // Create the new segment for the tail of the current segment
          // The second control of this segment will be set later
          newSeg = BCurveCreate(SCurveGetOrder(that), 
            SCurveGetDim(that));
          VecFree(newSeg->_ctrl);
          VecFree(newSeg->_ctrl + 1);
          newSeg->_ctrl[0] = pos;
          
          // Add the new segment
          GSetAppend(&segs, newSeg);
          
        // Else, if it's the last segment
        } else if (GSetIterIsLast(&iter)) {
          
          // Get the cut position
          VecFloat* pos = BCurveGet(curSeg, strength);

          // Set the control of the last new segment
          newSeg->_ctrl[1] = pos;
          GSetAppend(&ctrls, newSeg->_ctrl[1]);
          
          // Create the new segment for the tail of the current segment
          // The second control of this segment will be set later
          newSeg = BCurveCreate(SCurveGetOrder(that), 
            SCurveGetDim(that));
          VecFree(newSeg->_ctrl);
          VecFree(newSeg->_ctrl + 1);
          newSeg->_ctrl[0] = pos;
          newSeg->_ctrl[1] = VecClone(curSeg->_ctrl[1]);
          
          // Add the new segment
          GSetAppend(&segs, newSeg);
          GSetAppend(&ctrls, newSeg->_ctrl[1]);
          
        // Else, it's a segment inside the curve
        } else {
          
          // Get the cut positions
          VecFloat* posA = BCurveGet(curSeg, strength);
          VecFloat* posB = BCurveGet(curSeg, 1.0 - strength);

          // Set the second control of the last new segment
          newSeg->_ctrl[1] = posA;
          GSetAppend(&ctrls, newSeg->_ctrl[1]);
          
          // Create the new segment for the center of the current segment
          newSeg = BCurveCreate(SCurveGetOrder(that), 
            SCurveGetDim(that));
          VecFree(newSeg->_ctrl);
          VecFree(newSeg->_ctrl + 1);
          newSeg->_ctrl[0] = posA;
          newSeg->_ctrl[1] = posB;
          
          // Add the new segment
          GSetAppend(&segs, newSeg);
          GSetAppend(&ctrls, newSeg->_ctrl[1]);
          
          // Create the new segment for the tail of the current segment
          // The second control of this segment will be set later
          newSeg = BCurveCreate(SCurveGetOrder(that), 
            SCurveGetDim(that));
          VecFree(newSeg->_ctrl);
          VecFree(newSeg->_ctrl + 1);
          newSeg->_ctrl[0] = posB;
          
          // Add the new segment
          GSetAppend(&segs, newSeg);
          
        }
      } while (GSetIterStep(&iter));

      // Free the current result
      SCurveFree(&res);

      // Create the new result SCurve
      res = SCurveCreate(
        SCurveGetOrder(that), SCurveGetDim(that), 1);
      GSetFlush(&(res->_ctrl));
      BCurve* curve = GSetPop((GSet*)SCurveSegs(res));
      BCurveFree(&curve);
      res->_seg = segs;
      res->_ctrl = ctrls;
      res->_nbSeg = GSetNbElem(SCurveSegs(res));

    }
  }

  // Return the result
  return res;
}

void SCurveChaikinSubdivisionOld(SCurve* const that, 
  const float strength, const unsigned int depth) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (strength < 0.0 || strength > 1.0) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "'strength' is invalid (0<=%f<=1)", 
      strength);
    PBErrCatch(BCurveErr);
  }
#endif  
  // If the SCurve order is 0
  if (SCurveGetOrder(that) == 1) {

    // Loop on the depth
    for (unsigned int iDepth = depth; iDepth--;) {

      // Declare the new set of BCurve
      GSetBCurve segs = GSetBCurveCreateStatic();

      // Declare a variable to memorize the current segment and
      // previous segment
      BCurve* curSeg = NULL;
      BCurve* prevSeg = NULL;

      // Pop the first segment
      prevSeg = GSetPop(&(that->_seg));

      // Get the cut position on the first segment
      VecFloat* nextSegCutPos = BCurveGet(prevSeg, 1.0 - strength);

      // Flush the set of control points
      GSetFlush(&(that->_ctrl));
      
      // Add the first segment to the new set of BCurve
      GSetAppend(&segs, prevSeg);

      // Add the first control point
      GSetAppend(&(that->_ctrl), prevSeg->_ctrl[0]);
      
      // Loop until we have popped all the segments of the SCurve
      while (GSetNbElem(SCurveSegs(that)) > 0) {
        
        // Pop one segment
        curSeg = GSetPop(&(that->_seg));

        // Get the cut position on the previous segment
        VecFloat* prevSegCutPos = nextSegCutPos;
        
        // Get the cut position on the current segment
        VecFloat* curSegCutPos = BCurveGet(curSeg, strength);
        nextSegCutPos = BCurveGet(curSeg, 1.0 - strength);

        // Create a new BCurve with the two cut positions
        BCurve* seg = BCurveCreate(SCurveGetOrder(that), 
          SCurveGetDim(that));
        
        // Replace the anchors of the new segment with the cut
        // positions
        VecFree(seg->_ctrl);
        VecFree(seg->_ctrl + 1);
        seg->_ctrl[0] = prevSegCutPos;
        seg->_ctrl[1] = curSegCutPos;

        // Replace the last anchor of the prev segment with the
        // first cut position
        VecFree(prevSeg->_ctrl + 1);
        prevSeg->_ctrl[1] = prevSegCutPos;

        // Replace the first anchor of the current segment with the
        // second cut position
        curSeg->_ctrl[0] = curSegCutPos;

        // Add the new segment to the new set of BCurve
        GSetAppend(&segs, seg);

        // Add the control point
        GSetAppend(&(that->_ctrl), seg->_ctrl[0]);
       
        // Set the current segment as the next prev segment
        prevSeg = curSeg;
        
      }
      
      // Free memory
      VecFree(&nextSegCutPos);
      
      // Add the last control points
      GSetAppend(&(that->_ctrl), prevSeg->_ctrl[0]);
      GSetAppend(&(that->_ctrl), prevSeg->_ctrl[1]);

      // Add the last segment to the new set of BCurve
      GSetAppend(&segs, prevSeg);

      // Replace the old set of BCurve (which is now empty) with 
      // the new set of BCurve
      that->_seg = segs;
      
      // Update the number of segments
      that->_nbSeg = GSetNbElem(SCurveSegs(that));
      
    }

  }
}

// -------------- SCurveIter

// ================ Functions implementation ====================

// Create a new SCurveIter attached to the SCurve 'curve' with a step 
// of 'delta'
SCurveIter SCurveIterCreateStatic(const SCurve* const curve, 
  const float delta) {
#if BUILDMODE == 0
  if (curve == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'curve' is null");
    PBErrCatch(BCurveErr);
  }
  if (delta <= 0.0) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "'delta' is invalid (%f>0)", delta);
    PBErrCatch(BCurveErr);
  }
#endif  
  // Declare the new SCurveIter
  SCurveIter iter;
  // Set the properties
  iter._curve = curve;
  iter._curPos = 0.0;
  iter._delta = delta;
  // Return the new iterator
  return iter;
}

// -------------- BBody

// ================ Functions declaration ====================

// Recursive function to calculate the value of a BBody
VecFloat* BBodyGetRec(const BBody* const that, BCurve* curve, 
  VecShort* iCtrl, VecFloat* u, int iDimIn);

// ================ Functions implementation ====================

// Create a new BBody of order 'order' and dimension 'dim'
// Controls are initialized with null vectors
BBody* BBodyCreate(const int order, const VecShort2D* const dim) {
#if BUILDMODE == 0
  if (order < 0) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "Invalid order (%d>=0)", order);
    PBErrCatch(BCurveErr);
  }
  if (dim == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'dim' is null");
    PBErrCatch(BCurveErr);
  }
  for (int iDim = 2; iDim--;) {
    if (VecGet(dim, iDim) <= 0) {
      BCurveErr->_type = PBErrTypeInvalidArg;
      sprintf(BCurveErr->_msg, "Dimension is invalid (dim[%d]:%d>0)", 
        iDim, VecGet(dim, iDim));
      PBErrCatch(BCurveErr);
    }
  }
#endif
  // Allocate memory for the new BBody
  BBody* that = PBErrMalloc(BCurveErr, sizeof(BBody));
  // Init pointers
  *((VecShort2D*)&(that->_dim)) = VecShortCreateStatic2D();
  that->_ctrl = NULL;
  // Init properties
  *((int*)&(that->_order)) = order;
  *((VecShort2D*)&(that->_dim)) = *dim;
  // Init the control
  int nbCtrl = BBodyGetNbCtrl(that);
  that->_ctrl = PBErrMalloc(BCurveErr, sizeof(VecFloat*) * nbCtrl);
  for (int iCtrl = nbCtrl; iCtrl--;)
    that->_ctrl[iCtrl] = VecFloatCreate(VecGet(dim, 1));
  // Return the new BBody
  return that;
}

// Free the memory used by a BBody
void BBodyFree(BBody** that) {
  // Check arguments
  if (that == NULL || *that == NULL)
    return;
  // Get the number of ctrl
  int nbCtrl = BBodyGetNbCtrl(*that);
  // Free memory
  for (int iCtrl = nbCtrl; iCtrl--;)
    VecFree((*that)->_ctrl + iCtrl);
  free((*that)->_ctrl);
  free(*that);
  *that = NULL;
}

// Get the value of the BBody at paramater 'u'
// u can extend beyond [0.0, 1.0]
VecFloat* _BBodyGet(const BBody* const that, const VecFloat* const u) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (u == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'u' is null");
    PBErrCatch(BCurveErr);
  }
  if (VecGetDim(u) != VecGet(&(that->_dim), 0)) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "Dimension of 'u' is invalid (%ld=%d)", 
      VecGetDim(u), VecGet(&(that->_dim), 0));
    PBErrCatch(BCurveErr);
  }
#endif
  // Declare variables to memorize the nb of dimension
  int nbDimIn = VecGet(&(that->_dim), 0);
  int nbDimOut = VecGet(&(that->_dim), 1);
  // Create a clone of u to be checked for components interval
  VecFloat* uSafe = VecClone(u);
  // Declare a vector to memorize the index of the ctrl
  VecShort* iCtrl = VecShortCreate(nbDimIn);
  // Declare a BCurve used for calculation
  BCurve* curve = BCurveCreate(that->_order, nbDimOut);
  // Calculate recursively the result value
  VecFloat* res = BBodyGetRec(that, curve, iCtrl, uSafe, 0);
  // Free memory
  VecFree(&uSafe);
  VecFree(&iCtrl);
  BCurveFree(&curve);
  // Return the result
  return res;
}

// Recursive function to calculate the value of SCurve
VecFloat* BBodyGetRec(const BBody* const that, BCurve* curve, 
  VecShort* iCtrl, VecFloat* u, int iDimIn) {
  // Declare a variable for the result
  VecFloat* res = NULL;
  // If we are at the last dimension in the recursion, 
  // the curve controls are the controls of the surface at current
  // position in control's space
  if (iDimIn == VecGet(&(that->_dim), 0) - 1) {
    for (int i = that->_order + 1; i--;) {
      VecSet(iCtrl, iDimIn, i); 
      BCurveSetCtrl(curve, i, BBodyCtrl(that, iCtrl));
    }
  // Else, we are not at the last dimension in control's space
  } else {
    // Clone the position (to edit the lower dimension at lower 
    // level of the recursion)
    VecShort* jCtrl = VecClone(iCtrl);
    // Declare an array of VecFloat to memorize the control at 
    // the current level
    VecFloat** tmpCtrl = 
      PBErrMalloc(BCurveErr, sizeof(VecFloat*) * (that->_order + 1));
    // For each control
    for (int i = that->_order + 1; i--;) {
      // Update the control position
      VecSet(jCtrl, iDimIn, i);
      // Get recursively the control (equal to the BCurve value at 
      // lower level)
      tmpCtrl[i] = 
        BBodyGetRec(that, curve, jCtrl, u, iDimIn + 1);
    }
    // Set the control of the curve at current level
    // Use a temporary instead of affecting directly into curve
    // because it is shared between recursion level and affecting
    // directly would lead to overwritting during the process
    for (int i = that->_order + 1; i--;)
      BCurveSetCtrl(curve, i, tmpCtrl[i]);
    // Free the temporary Vecfloat for the controls
    for (int i = that->_order + 1; i--;)
      VecFree(tmpCtrl + i);
    free(tmpCtrl);
    // Free the temporary position in control space
    VecFree(&jCtrl);
  }
  // Here we have the curve set up with the appropriate control at the 
  // current recursion level
  // Calculate its value at the parameters value for the current 
  // dimension
  res = BCurveGet(curve, VecGet(u, iDimIn));
  // Return the result
  return res;
}

// Return a clone of the BBody 'that'
BBody* BBodyClone(const BBody* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // Declare the clone
  BBody* clone = BBodyCreate(BBodyGetOrder(that), BBodyDim(that));
  // For each control
  for (int iCtrl = BBodyGetNbCtrl(clone); iCtrl--;)
    // Copy the control values
    VecCopy(clone->_ctrl[iCtrl], that->_ctrl[iCtrl]);
  // Return the clone
  return clone;
}

// Print the BBody 'that' on the stream 'stream'
void BBodyPrint(const BBody* const that, FILE* const stream) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (stream == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'stream' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // Print the order and dim
  fprintf(stream, "order(%d) dim(", that->_order);
  VecPrint(&(that->_dim), stream);
  fprintf(stream, ") ");
  // For each control point
  for (int iCtrl = 0; iCtrl < BBodyGetNbCtrl(that); ++iCtrl) {
    VecPrint(that->_ctrl[iCtrl], stream);
    if (iCtrl < that->_order) 
      fprintf(stream, " ");
  }
}

// Function which return the JSON encoding of 'that' 
JSONNode* BBodyEncodeAsJSON(const BBody* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    PBMathErr->_type = PBErrTypeNullPointer;
    sprintf(PBMathErr->_msg, "'that' is null");
    PBErrCatch(PBMathErr);
  }
#endif
  // Create the JSON structure
  JSONNode* json = JSONCreate();
  // Declare a buffer to convert value into string
  char val[100];
  // Encode the order
  sprintf(val, "%d", BBodyGetOrder(that));
  JSONAddProp(json, "_order", val);
  // Encode the dimension
  JSONAddProp(json, "_dim", VecEncodeAsJSON((VecShort*)BBodyDim(that)));
  // Encode the control points
  JSONArrayStruct setCtrl = JSONArrayStructCreateStatic();
  // For each control point
  for (int iCtrl = 0; iCtrl < BBodyGetNbCtrl(that); ++iCtrl)
    JSONArrayStructAdd(&setCtrl, VecEncodeAsJSON(that->_ctrl[iCtrl]));
  JSONAddProp(json, "_ctrl", &setCtrl);
  // Free memory
  JSONArrayStructFlush(&setCtrl);
  // Return the created JSON 
  return json;
}

// Function which decode from JSON encoding 'json' to 'that'
bool BBodyDecodeAsJSON(BBody** that, const JSONNode* const json) {
#if BUILDMODE == 0
  if (that == NULL) {
    PBMathErr->_type = PBErrTypeNullPointer;
    sprintf(PBMathErr->_msg, "'that' is null");
    PBErrCatch(PBMathErr);
  }
  if (json == NULL) {
    PBMathErr->_type = PBErrTypeNullPointer;
    sprintf(PBMathErr->_msg, "'json' is null");
    PBErrCatch(PBMathErr);
  }
#endif
  // If 'that' is already allocated
  if (*that != NULL)
    // Free memory
    BBodyFree(that);
  // Get the order from the JSON
  JSONNode* prop = JSONProperty(json, "_order");
  if (prop == NULL) {
    return false;
  }
  int order = atoi(JSONLblVal(prop));
  // Get the dimension from the JSON
  prop = JSONProperty(json, "_dim");
  if (prop == NULL) {
    return false;
  }
  VecShort* dim = NULL;
  if (!VecDecodeAsJSON(&dim, prop)) {
    return false;
  }
  // If data are invalid
  if (order < 0 || VecGetDim(dim) != 2 || 
    VecGet(dim, 0) < 1 || VecGet(dim, 1) < 1) {
    return false;
  }
  // Allocate memory
  *that = BBodyCreate(order, (VecShort2D*)dim);
  // Decode the control points
  prop = JSONProperty(json, "_ctrl");
  if (prop == NULL) {
    return false;
  }
  if (JSONGetNbValue(prop) != BBodyGetNbCtrl(*that)) {
    return false;
  }
  for (int iCtrl = 0; iCtrl < BBodyGetNbCtrl(*that); ++iCtrl) {
    JSONNode* ctrl = JSONValue(prop, iCtrl);
    if (!VecDecodeAsJSON((*that)->_ctrl + iCtrl, ctrl))
      return false;
    // If the control point is not of the correct dimension
    if (VecGetDim((*that)->_ctrl[iCtrl]) != VecGet(&((*that)->_dim), 1))
      return false;
  }
  // Free memory
  VecFree(&dim);
  // Return the success code
  return true;
}

// Load the BBody from the stream
// If the BBody is already allocated, it is freed before loading
// Return true upon success, false else
bool BBodyLoad(BBody** that, FILE* const stream) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (stream == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'stream' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // Declare a json to load the encoded data
  JSONNode* json = JSONCreate();
  // Load the whole encoded data
  if (!JSONLoad(json, stream)) {
    return false;
  }
  // Decode the data from the JSON
  if (!BBodyDecodeAsJSON(that, json)) {
    return false;
  }
  // Free the memory used by the JSON
  JSONFree(&json);
  // Return the success code
  return true;
}

// Save the BBody to the stream
// If 'compact' equals true it saves in compact form, else it saves in 
// readable form
// Return true upon success, false else
bool BBodySave(const BBody* const that, FILE* const stream, 
  const bool compact) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (stream == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'stream' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // Get the JSON encoding
  JSONNode* json = BBodyEncodeAsJSON(that);
  // Save the JSON
  if (!JSONSave(json, stream, compact)) {
    return false;
  }
  // Free memory
  JSONFree(&json);
  // Return success code
  return true;
}

// Get the bounding box of the BBody.
// Return a Facoid whose axis are aligned on the standard coordinate 
// system.
Facoid* BBodyGetBoundingBox(const BBody* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // Declare a variable to memorize the result
  Facoid* res = FacoidCreate(VecGet(BBodyDim(that), 1));
  // For each dimension
  for (int iDim = VecGet(BBodyDim(that), 1); iDim--;) {
    // Initialise the bounding box in this dimension
    VecSet(res->_s._pos, iDim, VecGet(that->_ctrl[0], iDim));
    VecSet(res->_s._axis[iDim], iDim, VecGet(that->_ctrl[0], iDim));
    // For each control point except the first one
    for (int iCtrl = BBodyGetNbCtrl(that); iCtrl-- && iCtrl != 0;) {
      // Update the bounding box
      if (VecGet(that->_ctrl[iCtrl], iDim) < VecGet(res->_s._pos, iDim))
        VecSet(res->_s._pos, iDim, VecGet(that->_ctrl[iCtrl], iDim));
      if (VecGet(that->_ctrl[iCtrl], iDim) > 
        ShapoidAxisGet(res, iDim, iDim))
        ShapoidAxisSet(res, iDim, iDim,
        VecGet(that->_ctrl[iCtrl], iDim));
    }
    ShapoidAxisSetAdd(res, iDim, iDim, -1.0 * ShapoidPosGet(res, iDim));
  }
  // Return the result
  return res;
}
