// ============ BCURVE.C ================

// ================= Include =================

#include "bcurve.h"
#if BUILDMODE == 0
#include "bcurve-inline.c"
#endif

// ================ Functions implementation ====================

// Create a new BCurve of order 'order' and dimension 'dim'
BCurve* BCurveCreate(int order, int dim) {
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
  that->_dim = dim;
  that->_order = order;
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
BCurve* BCurveClone(BCurve* that) {
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
  clone->_dim = that->_dim;
  clone->_order = that->_order;
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

// Load the BCurve from the stream
// If the BCurve is already allocated, it is freed before loading
// Return true upon success, false else
bool BCurveLoad(BCurve** that, FILE* stream) {
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
  // If 'that' is already allocated
  if (*that != NULL)
    // Free memory
    BCurveFree(that);
  // Read the order and dimension
  int order;
  int dim;
  int ret = fscanf(stream, "%d %d", &order, &dim);
  // If we couldn't read
  if (ret == EOF)
    return false;
  // Allocate memory
  *that = BCurveCreate(order, dim);
  // For each control point
  for (int iCtrl = 0; iCtrl < (order + 1); ++iCtrl) {
    // Load the control point
    ret = VecLoad((*that)->_ctrl + iCtrl, stream);
    // If we couldn't read the control point or the control point
    // is not of the correct dimension
    if (ret == false || VecDim((*that)->_ctrl[iCtrl]) != (*that)->_dim)
      return false;
  }
  // Return success code
  return true;
}

// Save the BCurve to the stream
// Return true upon success, false else
bool BCurveSave(BCurve* that, FILE* stream) {
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
  // Save the order and dimension
  int ret = fprintf(stream, "%d %d\n", that->_order, that->_dim);
  // If the fprintf failed
  if (ret < 0)
    // Stop here
    return false;
  // For each control point
  for (int iCtrl = 0; iCtrl < that->_order + 1; ++iCtrl) {
    // Save the control point
    ret = VecSave(that->_ctrl[iCtrl], stream);
    // If we couldn't save the control point
    if (ret == false)
      // Stop here
      return false;
  }
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
void BCurvePrint(BCurve* that, FILE* stream) {
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

// Get the value of the BCurve at paramater 'u' (in [0.0, 1.0])
VecFloat* BCurveGet(BCurve* that, float u) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (u < 0.0 - PBMATH_EPSILON || u > 1.0 + PBMATH_EPSILON) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "'u' is invalid (0.0<=%f<=1.0)", u);
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
BCurve* BCurveFromCloudPoint(GSet* set) {
#if BUILDMODE == 0
  if (set == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'set' is null");
    PBErrCatch(BCurveErr);
  }
  if (set->_nbElem < 1) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "'set' is empty");
    PBErrCatch(BCurveErr);
  }
#endif
  // Declare a variable to memorize the result
  int order = set->_nbElem - 1;
  int dim = VecDim((VecFloat*)(set->_head->_data));
  BCurve* curve = BCurveCreate(order, dim);
  // Set the first control point to the first point in the point cloud
  BCurveSetCtrl(curve, 0, (VecFloat*)(set->_head->_data));
  // If the order is greater than 0
  if (order > 0) {
    // Set the last control point to the last point in the point cloud
    BCurveSetCtrl(curve, order, (VecFloat*)(set->_tail->_data));
    // If the order is greater than 1
    if (order > 1) {
      // Calculate the t values for intermediate control points
      // They are equal to the relative distance on the polyline
      // linking the point in the point cloud
      // Declare a variable to memorize the dimension of the matrix
      // in the linear system to solve
      VecShort2D dimMat = VecShortCreateStatic2D();
      // Declare a variable to memorize the t values
      VecFloat* t = VecFloatCreate(set->_nbElem);
      // Set the dimensions of the matrix of the linear system
      VecSet(&dimMat, 0, order - 1);
      VecSet(&dimMat, 1, order - 1);
      // For each point 
      GSetElem* elem = set->_head->_next;
      int iPoint = 1;
      while (elem != NULL) {
        // Get the distance from the previous point
        float d = VecDist((VecFloat*)(elem->_prev->_data),
          (VecFloat*)(elem->_data));
        VecSet(t, iPoint, d + VecGet(t, iPoint - 1));
        ++iPoint;
        elem = elem->_next;
      }
      // Normalize t
      for (iPoint = 1; iPoint <= order; ++iPoint)
        VecSet(t, iPoint, VecGet(t, iPoint) / VecGet(t, order));
      // For each dimension
      for (int iDim = dim; iDim--;) {
        // Declare a variable to memorize the matrix and vector 
        // of the linear system
        MatFloat* m = MatFloatCreate(&dimMat);
        VecFloat* v = VecFloatCreate(order - 1);
        // Set the values of the linear system
        // For each line (equivalent to each intermediate point 
        // in point cloud)
        for (VecSet(&dimMat, 1, 0); 
          VecGet(&dimMat, 1) < order - 1;
          VecSet(&dimMat, 1, VecGet(&dimMat, 1) + 1)) {
          // Get the weight of the control point at the value 
          // of t for this point
          VecFloat* weight = 
            BCurveGetWeightCtrlPt(curve, VecGet(t, 
            VecGet(&dimMat, 1) + 1));
          // For each intermediate control point
          for (VecSet(&dimMat, 0, 0); 
            VecGet(&dimMat, 0) < order - 1;
            VecSet(&dimMat, 0, VecGet(&dimMat, 0) + 1))
            // Set the matrix value with the corresponding
            // weight
            MatSet(m, &dimMat, VecGet(weight, 
              VecGet(&dimMat, 0) + 1));
          // Set the vector value with the corresponding point
          // coordinate
          float x = VecGet((VecFloat*)(GSetGet(set, 
            VecGet(&dimMat, 1) + 1)), iDim);
          x -= VecGet(weight, 0) * 
            VecGet((VecFloat*)(set->_head->_data), iDim);
          x -= VecGet(weight, order) * 
            VecGet((VecFloat*)(set->_tail->_data), iDim);
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
VecFloat* BCurveGetWeightCtrlPt(BCurve* that, float t) {
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
Facoid* BCurveGetBoundingBox(BCurve* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // Declare a variable to memorize the result
  Facoid* res = FacoidCreate(that->_dim);
  // For each dimension
  for (int iDim = that->_dim; iDim--;) {
    // For each control point
    for (int iCtrl = that->_order + 1; iCtrl--;) {
      // If it's the first control point in this dimension
      if (iCtrl == that->_order) {
        // Initialise the bounding box
        VecSet(res->_s._pos, iDim, VecGet(that->_ctrl[iCtrl], iDim));
        VecSet(res->_s._axis[iDim], iDim, 
          VecGet(that->_ctrl[iCtrl], iDim));
      // Else, it's not the first control point in this dimension
      } else {
        // Update the bounding box
        if (VecGet(that->_ctrl[iCtrl], iDim) < 
          VecGet(res->_s._pos, iDim))
          VecSet(res->_s._pos, iDim, VecGet(that->_ctrl[iCtrl], iDim));
        if (VecGet(that->_ctrl[iCtrl], iDim) > 
          VecGet(ShapoidAxis(res, iDim), iDim))
          VecSet(ShapoidAxis(res, iDim), iDim,
            VecGet(that->_ctrl[iCtrl], iDim));
      }
    }
    VecSet(ShapoidAxis(res, iDim), iDim,
      VecGet(ShapoidAxis(res, iDim), iDim) - 
      VecGet(ShapoidPos(res), iDim));
  }
  // Return the result
  return res;
}

// Create a new SCurve of dimension 'dim', order 'order' and 
// 'nbSeg' segments
SCurve* SCurveCreate(int order, int dim, int nbSeg) {
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
  that->_dim = dim;
  that->_order = order;
  that->_nbSeg = nbSeg;
  // Create the GSet
  that->_ctrl = GSetCreateStatic();
  that->_seg = GSetCreateStatic();
  // For each segment
  for (int iSeg = nbSeg; iSeg--;) {
    // Create a segment
    BCurve* seg = BCurveCreate(order, dim);
    // If it's not the first added segment
    if (iSeg != nbSeg - 1) {
      // Replace the last control points by the current first
      VecFree(seg->_ctrl + order);
      seg->_ctrl[order] = (VecFloat*)(that->_ctrl._head->_data);
      // Add the control points
      for (int iCtrl = order; iCtrl--;)
        GSetPush(&(that->_ctrl), BCurveCtrl(seg, iCtrl));
    // Else, it's the first segment
    } else {
      // Add the control points
      for (int iCtrl = order + 1; iCtrl--;)
        GSetPush(&(that->_ctrl), BCurveCtrl(seg, iCtrl));
    }
    // Add the segment
    GSetPush(&(that->_seg), seg);
  }
  // Return the new SCurve
  return that;
}

// Clone the SCurve
SCurve* SCurveClone(SCurve* that) {
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
    VecFloat* ctrl = (VecFloat*)GSetIterGet(&iter);
    VecFloat* ctrlClone = (VecFloat*)GSetIterGet(&iterClone);
    VecCopy(ctrlClone, ctrl);
  } while (GSetIterStep(&iter) && GSetIterStep(&iterClone));
  return clone;
}

// Load the SCurve from the stream
// If the SCurve is already allocated, it is freed before loading
// Return true in case of success, false else
bool SCurveLoad(SCurve** that, FILE* stream) {
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
  // If 'that' is already allocated
  if (*that != NULL)
    // Free memory
    SCurveFree(that);
  // Read the number of segment, order and dimension
  int nbSeg;
  int order;
  int dim;
  int ret = fscanf(stream, "%d %d %d", &order, &dim, &nbSeg);
  // If we couldn't read
  if (ret == EOF)
    return false;
  // If data are invalid
  if (nbSeg < 1 || order < 0 || dim < 1)
    return false;
  // Allocate memory
  *that = SCurveCreate(order, dim, nbSeg);
  // For each control point
  GSetIterForward iter = GSetIterForwardCreateStatic(&((*that)->_ctrl));
  do {
    // Load the control point
    VecFloat* loadCtrl = NULL;
    ret = VecLoad(&loadCtrl, stream);
    // If we couldn't read the control point or the control point
    // is not of the correct dimension
    if (ret == false || VecDim(loadCtrl) != (*that)->_dim)
      return false;
    // Set the loaded control point into the set of control point
    // and segment
    VecCopy((VecFloat*)GSetIterGet(&iter), loadCtrl);
    // Free memory used by the loaded control
    VecFree(&loadCtrl);
  } while (GSetIterStep(&iter));
  // Return success code
  return true;
}

// Save the SCurve to the stream
// Return true upon success, false else
bool SCurveSave(SCurve* that, FILE* stream) {
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
  // Save the nb of segment, order and dimension
  int ret = fprintf(stream, "%d %d %d\n", 
    that->_order, that->_dim, that->_nbSeg);
  // If the fprintf failed
  if (ret < 0)
    // Stop here
    return false;
  // For each control point
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_ctrl));
  do {
    VecFloat* ctrl = (VecFloat*)GSetIterGet(&iter);
    // Save the control point
    ret = VecSave(ctrl, stream);
    // If we couldn't save the control point
    if (ret == false)
      // Stop here
      return false;
  } while (GSetIterStep(&iter));
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
void SCurvePrint(SCurve* that, FILE* stream) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
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
    VecPrint(ctrl, stream);
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
void SCurveAddSegTail(SCurve* that) {
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
  seg->_ctrl[0] = that->_ctrl._tail->_data;
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
void SCurveAddSegHead(SCurve* that) {
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
  seg->_ctrl[that->_order] = that->_ctrl._head->_data;
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
void SCurveRemoveHeadSeg(SCurve* that) {
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
void SCurveRemoveTailSeg(SCurve* that) {
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
Facoid* SCurveGetBoundingBox(SCurve* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // Declare a set to memorize the bounding box of each segment
  GSet set = GSetCreateStatic();
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
