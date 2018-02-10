// ============ BCURVE-INLINE.C ================

// ================ Functions implementation ====================

// Set the value of the iCtrl-th control point to v
#if BUILDMODE != 0
inline
#endif 
void BCurveSetCtrl(BCurve* that, int iCtrl, VecFloat* v) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (v == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'v' is null");
    PBErrCatch(BCurveErr);
  }
  if (iCtrl < 0 || iCtrl > that->_order) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "'iCtrl' is invalid (0<=%d<%d)", 
      iCtrl, that->_order);
    PBErrCatch(BCurveErr);
  }
  if (VecDim(v) != BCurveGetDim(that)) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "'v' 's dimension is invalid (%d<%d)", 
      VecDim(v), BCurveGetDim(that));
    PBErrCatch(BCurveErr);
  }
#endif
  // Set the values
  VecCopy(that->_ctrl[iCtrl], v);
}

// Get a copy of the iCtrl-th control point
#if BUILDMODE != 0
inline
#endif 
VecFloat* BCurveGetCtrl(BCurve* that, int iCtrl) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (iCtrl < 0 || iCtrl > that->_order) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "'iCtrl' is invalid (0<=%d<%d)", 
      iCtrl, that->_order);
    PBErrCatch(BCurveErr);
  }
#endif
  // Return a copy of the control point
  return VecClone(that->_ctrl[iCtrl]);
}

// Get the iCtrl-th control point
#if BUILDMODE != 0
inline
#endif 
VecFloat* BCurveCtrl(BCurve* that, int iCtrl) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (iCtrl < 0 || iCtrl > that->_order) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "'iCtrl' is invalid (0<=%d<%d)", 
      iCtrl, that->_order);
    PBErrCatch(BCurveErr);
  }
#endif
  // Return the control point
  return that->_ctrl[iCtrl];
}

// Get the order of the BCurve
#if BUILDMODE != 0
inline
#endif 
int BCurveGetOrder(BCurve* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  return that->_order;
}

// Get the dimension of the BCurve
#if BUILDMODE != 0
inline
#endif 
int BCurveGetDim(BCurve* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  return that->_dim;
}

// Get the approximate length of the BCurve (sum of dist between
// control points)
#if BUILDMODE != 0
inline
#endif 
float BCurveGetApproxLen(BCurve* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // Declare a variable to calculate the length
  float res = 0.0;
  // Calculate the length
  for (int iCtrl = that->_order; iCtrl--;)
    res += VecDist(that->_ctrl[iCtrl], that->_ctrl[iCtrl + 1]);
  // Return the length
  return res;
}

// Return the center of the BCurve (average of control points)
#if BUILDMODE != 0
inline
#endif 
VecFloat* BCurveGetCenter(BCurve* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // Sum all the control points
  VecFloat* center = VecClone(that->_ctrl[that->_order]);
  for (int iCtrl = that->_order; iCtrl--;)
    VecOp(center, 1.0, that->_ctrl[iCtrl], 1.0);
  // Get the average
  VecScale(center, 1.0 / (float)(that->_order + 1));
  // Return the result
  return center;
}

// Rotate the curve CCW by 'theta' radians relatively to the origin
// of the coordinates system
#if BUILDMODE != 0
inline
#endif 
void BCurveRotOrigin(BCurve* that, float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (that->_dim != 2) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "'that' 's dimension is invalid (%d=2)",
      that->_dim);
    PBErrCatch(BCurveErr);
  }
#endif
  // For each control point
  for (int iCtrl = that->_order + 1; iCtrl--;)
    // Rotate the control point
    VecRot(that->_ctrl[iCtrl], theta);
}

// Rotate the curve CCW by 'theta' radians relatively to its 
// first control point
#if BUILDMODE != 0
inline
#endif 
void BCurveRotStart(BCurve* that, float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (that->_dim != 2) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "'that' 's dimension is invalid (%d=2)",
      that->_dim);
    PBErrCatch(BCurveErr);
  }
#endif
  // For each control point except the first one
  for (int iCtrl = that->_order + 1; iCtrl-- && iCtrl != 0;) {
    // Translate the control point
    VecOp(that->_ctrl[iCtrl], 1.0, that->_ctrl[0], -1.0);
    // Rotate the control point
    VecRot(that->_ctrl[iCtrl], theta);
    // Translate back the control point
    VecOp(that->_ctrl[iCtrl], 1.0, that->_ctrl[0], 1.0);
  }
}

// Rotate the curve CCW by 'theta' radians relatively to its 
// center
#if BUILDMODE != 0
inline
#endif 
void BCurveRotCenter(BCurve* that, float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (that->_dim != 2) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "'that' 's dimension is invalid (%d=2)",
      that->_dim);
    PBErrCatch(BCurveErr);
  }
#endif
  // Get the center
  VecFloat* center = BCurveGetCenter(that);
  // For each control point
  for (int iCtrl = that->_order + 1; iCtrl--;) {
    // Translate the control point
    VecOp(that->_ctrl[iCtrl], 1.0, center, -1.0);
    // Rotate the control point
    VecRot(that->_ctrl[iCtrl], theta);
    // Translate back the control point
    VecOp(that->_ctrl[iCtrl], 1.0, center, 1.0);
  }
  // Free memory
  VecFree(&center);
}

// Scale the curve by 'v' relatively to the origin
#if BUILDMODE != 0
inline
#endif 
void BCurveScaleOriginVector(BCurve* that, VecFloat* v) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (v == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'v' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // For each control point
  for (int iCtrl = that->_order + 1; iCtrl--;) {
    VecFloat* ctrl = that->_ctrl[iCtrl];
    // Scale the control point
    for (int dim = 0; dim < VecDim(ctrl); ++dim)
      VecSet(ctrl, dim, VecGet(ctrl, dim) * VecGet(v, dim));
  }
}

// Scale the curve by 'c' relatively to the origin
#if BUILDMODE != 0
inline
#endif 
void BCurveScaleOriginScalar(BCurve* that, float c) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // For each control point
  for (int iCtrl = that->_order + 1; iCtrl--;)
    // Scale the control point
    VecScale(that->_ctrl[iCtrl], c);
}

// Scale the curve by 'v' relatively to its origin
// (first control point)
#if BUILDMODE != 0
inline
#endif 
void BCurveScaleStartVector(BCurve* that, VecFloat* v) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (v == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'v' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // For each control point except the first one
  for (int iCtrl = that->_order + 1; iCtrl-- && iCtrl != 0;) {
    VecFloat* ctrl = that->_ctrl[iCtrl];
    // Translate the control point
    VecOp(ctrl, 1.0, that->_ctrl[0], -1.0);
    // Scale the control point
    for (int dim = 0; dim < VecDim(that->_ctrl[iCtrl]); ++dim)
      VecSet(ctrl, dim, VecGet(ctrl, dim) * VecGet(v, dim));
    // Translate back the control point
    VecOp(ctrl, 1.0, that->_ctrl[0], 1.0);
  }
}

// Scale the curve by 'c' relatively to its origin
// (first control point)
#if BUILDMODE != 0
inline
#endif 
void BCurveScaleStartScalar(BCurve* that, float c) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // For each control point except the first one
  for (int iCtrl = that->_order + 1; iCtrl-- && iCtrl != 0;) {
    VecFloat* ctrl = that->_ctrl[iCtrl];
    // Translate the control point
    VecOp(ctrl, 1.0, that->_ctrl[0], -1.0);
    // Scale the control point
    VecScale(ctrl, c);
    // Translate back the control point
    VecOp(ctrl, 1.0, that->_ctrl[0], 1.0);
  }
}

// Scale the curve by 'v' relatively to its center
// (average of control points)
#if BUILDMODE != 0
inline
#endif 
void BCurveScaleCenterVector(BCurve* that, VecFloat* v) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (v == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'v' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  VecFloat* center = BCurveGetCenter(that);
  // For each control point
  for (int iCtrl = that->_order + 1; iCtrl--;) {
    VecFloat* ctrl = that->_ctrl[iCtrl];
    // Translate the control point
    VecOp(ctrl, 1.0, center, -1.0);
    // Scale the control point
    for (int dim = 0; dim < VecDim(that->_ctrl[iCtrl]); ++dim)
      VecSet(ctrl, dim, VecGet(ctrl, dim) * VecGet(v, dim));
    // Translate back the control point
    VecOp(ctrl, 1.0, center, 1.0);
  }
  // Free memory
  VecFree(&center);
}

// Scale the curve by 'c' relatively to its center
// (average of control points)
#if BUILDMODE != 0
inline
#endif 
void BCurveScaleCenterScalar(BCurve* that, float c) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  VecFloat* center = BCurveGetCenter(that);
  // For each control point
  for (int iCtrl = that->_order + 1; iCtrl--;) {
    VecFloat* ctrl = that->_ctrl[iCtrl];
    // Translate the control point
    VecOp(ctrl, 1.0, center, -1.0);
    // Scale the control point
    VecScale(ctrl, c);
    // Translate back the control point
    VecOp(ctrl, 1.0, center, 1.0);
  }
  // Free memory
  VecFree(&center);
}

// Translate the curve by 'v'
#if BUILDMODE != 0
inline
#endif 
void BCurveTranslate(BCurve* that, VecFloat* v) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (v == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'v' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // For each control point
  for (int iCtrl = that->_order + 1; iCtrl--;)
    // Translate the control point
    VecOp(that->_ctrl[iCtrl], 1.0, v, 1.0);
}

// Get the number of BCurve in the SCurve
#if BUILDMODE != 0
inline
#endif 
int SCurveGetNbSeg(SCurve* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  return that->_nbSeg;
}


// Get the dimension of the SCurve
#if BUILDMODE != 0
inline
#endif 
int SCurveGetDim(SCurve* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  return that->_dim;
}

// Get the order of the SCurve
#if BUILDMODE != 0
inline
#endif 
int SCurveGetOrder(SCurve* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  return that->_order;
}

// Get a clone of the 'iCtrl'-th control point
#if BUILDMODE != 0
inline
#endif 
VecFloat* SCurveGetCtrl(SCurve* that, int iCtrl) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (iCtrl < 0 || iCtrl >= SCurveGetNbCtrl(that)) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "'iCtrl' is invalid (0<=%d<%d)", 
      iCtrl, SCurveGetNbCtrl(that));
    PBErrCatch(BCurveErr);
  }
#endif
  return VecClone((VecFloat*)GSetGet(&(that->_ctrl), iCtrl));
}

// Get the 'iCtrl'-th control point
#if BUILDMODE != 0
inline
#endif 
VecFloat* SCurveCtrl(SCurve* that, int iCtrl) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (iCtrl < 0 || iCtrl >= SCurveGetNbCtrl(that)) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "'iCtrl' is invalid (0<=%d<%d)", 
      iCtrl, SCurveGetNbCtrl(that));
    PBErrCatch(BCurveErr);
  }
#endif
  return (VecFloat*)GSetGet(&(that->_ctrl), iCtrl);
}

// Get a clone of the 'iSeg'-th segment
#if BUILDMODE != 0
inline
#endif 
BCurve* SCurveGetSeg(SCurve* that, int iSeg) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (iSeg < 0 || iSeg >= that->_nbSeg) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "'iSeg' is invalid (0<=%d<%d)", 
      iSeg, that->_nbSeg);
    PBErrCatch(BCurveErr);
  }
#endif
  return BCurveClone((BCurve*)GSetGet(&(that->_seg), iSeg));
}

// Get the 'iSeg'-th segment
#if BUILDMODE != 0
inline
#endif 
BCurve* SCurveSeg(SCurve* that, int iSeg) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (iSeg < 0 || iSeg >= that->_nbSeg) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "'iSeg' is invalid (0<=%d<%d)", 
      iSeg, that->_nbSeg);
    PBErrCatch(BCurveErr);
  }
#endif
  return (BCurve*)GSetGet(&(that->_seg), iSeg);
}

// Return the center of the SCurve (average of control points)
#if BUILDMODE != 0
inline
#endif 
VecFloat* SCurveGetCenter(SCurve* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // Sum all the control points
  VecFloat* center = VecFloatCreate(that->_dim);
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_ctrl));
  do {
    VecOp(center, 1.0, (VecFloat*)GSetIterGet(&iter), 1.0);
  } while (GSetIterStep(&iter));
  // Get the average
  VecScale(center, 1.0 / (float)GSetNbElem(&(that->_ctrl)));
  // Return the result
  return center;
}

// Return the max value for the parameter 'u' of SCurveGet
#if BUILDMODE != 0
inline
#endif 
float SCurveGetMaxU(SCurve* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  return (float)(that->_nbSeg);
}

// Get the number of control point in the SCurve
#if BUILDMODE != 0
inline
#endif 
int SCurveGetNbCtrl(SCurve* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  return that->_nbSeg * that->_order + 1;
}

// Rotate the curve CCW by 'theta' radians relatively to the origin
// of the coordinates system
#if BUILDMODE != 0
inline
#endif 
void SCurveRotOrigin(SCurve* that, float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // For each control point
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_ctrl));
  do {
    // Rotate the control point
    VecRot((VecFloat*)GSetIterGet(&iter), theta);
  } while (GSetIterStep(&iter));
}

// Rotate the curve CCW by 'theta' radians relatively to its 
// first control point
#if BUILDMODE != 0
inline
#endif 
void SCurveRotStart(SCurve* that, float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  VecFloat* origin = (VecFloat*)(that->_ctrl._head->_data);
  // For each control point except the first one
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_ctrl));
  if (GSetIterStep(&iter)) {
    do {
      VecFloat* ctrl = (VecFloat*)GSetIterGet(&iter);
      // Translate the control point
      VecOp(ctrl, 1.0, origin, -1.0);
      // Rotate the control point
      VecRot(ctrl, theta);
      // Translate back the control point
      VecOp(ctrl, 1.0, origin, 1.0);
    } while (GSetIterStep(&iter));
  }
}

// Rotate the curve CCW by 'theta' radians relatively to its 
// center
#if BUILDMODE != 0
inline
#endif 
void SCurveRotCenter(SCurve* that, float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // Get the center
  VecFloat* center = SCurveGetCenter(that);
  // For each control point
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_ctrl));
  do {
    VecFloat* ctrl = (VecFloat*)GSetIterGet(&iter);
    // Translate the control point
    VecOp(ctrl, 1.0, center, -1.0);
    // Rotate the control point
    VecRot(ctrl, theta);
    // Translate back the control point
    VecOp(ctrl, 1.0, center, 1.0);
  } while (GSetIterStep(&iter));
  // Free memory
  VecFree(&center);
}

// Scale the curve by 'v' relatively to the origin
// of the coordinates system
#if BUILDMODE != 0
inline
#endif 
void SCurveScaleOriginVector(SCurve* that, VecFloat* v) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // For each control point
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_ctrl));
  do {
    VecFloat* ctrl = (VecFloat*)GSetIterGet(&iter);
    // Scale the control point
    for (int iDim = SCurveGetDim(that); iDim--;)
      VecSet(ctrl, iDim, VecGet(ctrl, iDim) * VecGet(v, iDim));
  } while (GSetIterStep(&iter));
}

// Scale the curve by 'c' relatively to the origin
// of the coordinates system
#if BUILDMODE != 0
inline
#endif 
void SCurveScaleOriginScalar(SCurve* that, float c) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // For each control point
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_ctrl));
  do {
    // Scale the control point
    VecScale((VecFloat*)GSetIterGet(&iter), c);
  } while (GSetIterStep(&iter));
}

// Scale the curve by 'v' relatively to its origin
// (first control point)
#if BUILDMODE != 0
inline
#endif 
void SCurveScaleStartVector(SCurve* that, VecFloat* v) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (v == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'v' is null");
    PBErrCatch(BCurveErr);
  }
  if (VecDim(v) != SCurveGetDim(that)) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'v' 's dimension is invalid (%d=%d)",
      VecDim(v), SCurveGetDim(that));
    PBErrCatch(BCurveErr);
  }
#endif
  VecFloat* origin = (VecFloat*)(that->_ctrl._head->_data);
  // For each control point except the first one
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_ctrl));
  if (GSetIterStep(&iter)) {
    do {
      VecFloat* ctrl = (VecFloat*)GSetIterGet(&iter);
      // Translate the control point
      VecOp(ctrl, 1.0, origin, -1.0);
      // Scale the control point
      for (int iDim = SCurveGetDim(that); iDim--;)
        VecSet(ctrl, iDim, VecGet(ctrl, iDim) * VecGet(v, iDim));
      // Translate back the control point
      VecOp(ctrl, 1.0, origin, 1.0);
    } while (GSetIterStep(&iter));
  }
}

// Scale the curve by 'c' relatively to its origin
// (first control point)
#if BUILDMODE != 0
inline
#endif 
void SCurveScaleStartScalar(SCurve* that, float c) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  VecFloat* origin = (VecFloat*)(that->_ctrl._head->_data);
  // For each control point except teh first one
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_ctrl));
  if (GSetIterStep(&iter)) {
    do {
      VecFloat* ctrl = (VecFloat*)GSetIterGet(&iter);
      // Translate the control point
      VecOp(ctrl, 1.0, origin, -1.0);
      // Scale the control point
      VecScale(ctrl, c);
      // Translate back the control point
      VecOp(ctrl, 1.0, origin, 1.0);
    } while (GSetIterStep(&iter));
  }
}

// Scale the curve by 'v' relatively to its center
// (average of control points)
#if BUILDMODE != 0
inline
#endif 
void SCurveScaleCenterVector(SCurve* that, VecFloat* v) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (v == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'v' is null");
    PBErrCatch(BCurveErr);
  }
  if (VecDim(v) != SCurveGetDim(that)) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'v' 's dimension is invalid (%d=%d)",
      VecDim(v), SCurveGetDim(that));
    PBErrCatch(BCurveErr);
  }
#endif
  VecFloat* center = SCurveGetCenter(that);
  // For each control point
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_ctrl));
  do {
    VecFloat* ctrl = (VecFloat*)GSetIterGet(&iter);
    // Translate the control point
    VecOp(ctrl, 1.0, center, -1.0);
    // Scale the control point
    for (int iDim = SCurveGetDim(that); iDim--;)
      VecSet(ctrl, iDim, VecGet(ctrl, iDim) * VecGet(v, iDim));
    // Translate back the control point
    VecOp(ctrl, 1.0, center, 1.0);
  } while (GSetIterStep(&iter));
  // Free memory
  VecFree(&center);
}

// Scale the curve by 'c' relatively to its center
// (average of control points)
#if BUILDMODE != 0
inline
#endif 
void SCurveScaleCenterScalar(SCurve* that, float c) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  VecFloat* center = SCurveGetCenter(that);
  // For each control point
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_ctrl));
  do {
    VecFloat* ctrl = (VecFloat*)GSetIterGet(&iter);
    // Translate the control point
    VecOp(ctrl, 1.0, center, -1.0);
    // Scale the control point
    VecScale(ctrl, c);
    // Translate back the control point
    VecOp(ctrl, 1.0, center, 1.0);
  } while (GSetIterStep(&iter));
  // Free memory
  VecFree(&center);
}

// Translate the curve by 'v'
#if BUILDMODE != 0
inline
#endif 
void SCurveTranslate(SCurve* that, VecFloat* v) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (v == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'v' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // Translate all the control points
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_ctrl));
  do {
    VecOp((VecFloat*)GSetIterGet(&iter), 1.0, v, 1.0);
  } while (GSetIterStep(&iter));
}

// Get the value of the SCurve at paramater 'u' (in [0.0, _nbSeg])
// The value is equal to the value of the floor(u)-th segment at
// value (u - floor(u))
#if BUILDMODE != 0
inline
#endif 
VecFloat* SCurveGet(SCurve* that, float u) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (u < 0.0 - PBMATH_EPSILON || 
    u > (float)(that->_nbSeg) + PBMATH_EPSILON) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "'u' is invalid (0.0<=%f<=%d.0)",
      u, that->_nbSeg);
    PBErrCatch(BCurveErr);
  }
#endif
  // Get the segment the corresponding to 'u'
  int iSeg = (int)floor(u);
  // Ensure iSeg is correct for the case u == nbSeg
  if (iSeg == that->_nbSeg) {
    iSeg = that->_nbSeg - 1;
    u = 1.0;
  } else {
    // Get the value of 'u' in this segment
    u -= (float)iSeg;
  }
  // Get the value of the BCurve
  return BCurveGet(SCurveSeg(that, iSeg), u);
}

// Get the approximate length of the SCurve (sum of approxLen 
// of its BCurves)
#if BUILDMODE != 0
inline
#endif 
float SCurveGetApproxLen(SCurve* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // Declare a variable to memorize the length
  float length = 0.0;
  // For each segment
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_seg));
  do {
    // Add the length of this segment
    length += BCurveGetApproxLen((BCurve*)GSetIterGet(&iter));
  } while (GSetIterStep(&iter));
  // Return the result
  return length;
}

// Set the 'iCtrl'-th control point to 'v'
#if BUILDMODE != 0
inline
#endif 
void SCurveSetCtrl(SCurve* that, int iCtrl, VecFloat* v) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (v == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'v' is null");
    PBErrCatch(BCurveErr);
  }
  if (iCtrl < 0 || iCtrl >= SCurveGetNbCtrl(that)) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "'iCtrl' is invalid (0<=%d<%d)", 
      iCtrl, SCurveGetNbCtrl(that));
    PBErrCatch(BCurveErr);
  }
#endif
  VecCopy((VecFloat*)GSetGet(&(that->_ctrl), iCtrl), v);
}
