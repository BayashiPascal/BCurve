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
  if (VecGetDim(v) != BCurveGetDim(that)) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "'v' 's dimension is invalid (%d<%d)", 
      VecGetDim(v), BCurveGetDim(that));
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
void _BCurveScaleOriginVector(BCurve* that, VecFloat* v) {
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
  if (VecGetDim(v) != BCurveGetDim(that)) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "Dimension of 'v' is invalid (%d=%d)",
      VecGetDim(v), BCurveGetDim(that));
    PBErrCatch(BCurveErr);
  }
#endif
  // For each control point
  for (int iCtrl = that->_order + 1; iCtrl--;) {
    VecFloat* ctrl = that->_ctrl[iCtrl];
    // Scale the control point
    for (int dim = 0; dim < VecGetDim(ctrl); ++dim)
      VecSet(ctrl, dim, VecGet(ctrl, dim) * VecGet(v, dim));
  }
}

// Scale the curve by 'c' relatively to the origin
#if BUILDMODE != 0
inline
#endif 
void _BCurveScaleOriginScalar(BCurve* that, float c) {
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
void _BCurveScaleStartVector(BCurve* that, VecFloat* v) {
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
  if (VecGetDim(v) != BCurveGetDim(that)) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "Dimension of 'v' is invalid (%d=%d)",
      VecGetDim(v), BCurveGetDim(that));
    PBErrCatch(BCurveErr);
  }
#endif
  // For each control point except the first one
  for (int iCtrl = that->_order + 1; iCtrl-- && iCtrl != 0;) {
    VecFloat* ctrl = that->_ctrl[iCtrl];
    // Translate the control point
    VecOp(ctrl, 1.0, that->_ctrl[0], -1.0);
    // Scale the control point
    for (int dim = 0; dim < VecGetDim(that->_ctrl[iCtrl]); ++dim)
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
void _BCurveScaleStartScalar(BCurve* that, float c) {
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
void _BCurveScaleCenterVector(BCurve* that, VecFloat* v) {
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
  if (VecGetDim(v) != BCurveGetDim(that)) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "Dimension of 'v' is invalid (%d=%d)",
      VecGetDim(v), BCurveGetDim(that));
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
    for (int dim = 0; dim < VecGetDim(that->_ctrl[iCtrl]); ++dim)
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
void _BCurveScaleCenterScalar(BCurve* that, float c) {
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
void _BCurveTranslate(BCurve* that, VecFloat* v) {
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
  if (VecGetDim(v) != BCurveGetDim(that)) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "Dimension of 'v' is invalid (%d=%d)",
      VecGetDim(v), BCurveGetDim(that));
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
void _SCurveScaleOriginVector(SCurve* that, VecFloat* v) {
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
void _SCurveScaleOriginScalar(SCurve* that, float c) {
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
void _SCurveScaleStartVector(SCurve* that, VecFloat* v) {
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
  if (VecGetDim(v) != SCurveGetDim(that)) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'v' 's dimension is invalid (%d=%d)",
      VecGetDim(v), SCurveGetDim(that));
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
void _SCurveScaleStartScalar(SCurve* that, float c) {
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
void _SCurveScaleCenterVector(SCurve* that, VecFloat* v) {
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
  if (VecGetDim(v) != SCurveGetDim(that)) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'v' 's dimension is invalid (%d=%d)",
      VecGetDim(v), SCurveGetDim(that));
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
void _SCurveScaleCenterScalar(SCurve* that, float c) {
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
void _SCurveTranslate(SCurve* that, VecFloat* v) {
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
  if (VecGetDim(v) != SCurveGetDim(that)) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "Dimension of 'v' is invalid (%d=%d)",
      VecGetDim(v), SCurveGetDim(that));
    PBErrCatch(BCurveErr);
  }
#endif
  // Translate all the control points
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_ctrl));
  do {
    VecOp((VecFloat*)GSetIterGet(&iter), 1.0, v, 1.0);
  } while (GSetIterStep(&iter));
}

// Get the value of the SCurve at paramater 'u'
// The value is equal to the value of the floor(u)-th segment at
// value (u - floor(u))
// u can extend beyond [0.0, _nbSeg]
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
#endif
  // Declare a variable to memorize the relevant segment
  int iSeg = 0;
  // Get the segment the corresponding to 'u'
  if (u < 0.0)
    iSeg = 0;
  else if (u > that->_nbSeg) {
    iSeg = that->_nbSeg - 1;
    u -= (float)(that->_nbSeg - 1);
  } else { 
    iSeg = (int)floor(u);
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

// Set the attached SCurve of the SCurveIter 'that' to 'curve'
#if BUILDMODE != 0
inline
#endif 
void SCurveIterSetCurve(SCurveIter* that, SCurve* curve) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (curve == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'curve' is null");
    PBErrCatch(BCurveErr);
  }
#endif  
  that->_curve = curve;
}

// Set the delta of the SCurveIter 'that' to 'delta'
#if BUILDMODE != 0
inline
#endif 
void SCurveIterSetDelta(SCurveIter* that, float delta) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (delta <= 0.0) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "'delta' is invalid (%f>0)", delta);
    PBErrCatch(BCurveErr);
  }
#endif  
  that->_delta = delta;
}

// Get the attached curve of the SCurveIter 'that'
#if BUILDMODE != 0
inline
#endif 
SCurve* SCurveIterCurve(SCurveIter* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  return that->_curve;
}

// Get the delta of the SCurveIter 'that'
#if BUILDMODE != 0
inline
#endif 
float SCurveIterGetDelta(SCurveIter* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  return that->_delta;  
}

// Init the SCurveIter 'that'
#if BUILDMODE != 0
inline
#endif 
void SCurveIterInit(SCurveIter* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  that->_curPos = 0.0;
}

// Step the SCurveIter 'that'
// Return false if it couldn't step, true else
#if BUILDMODE != 0
inline
#endif 
bool SCurveIterStep(SCurveIter* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  if (that->_curPos > 
    SCurveGetMaxU(SCurveIterCurve(that)) - PBMATH_EPSILON)
    return false;
  that->_curPos += that->_delta;
  if (that->_curPos > SCurveGetMaxU(SCurveIterCurve(that)))
    that->_curPos = SCurveGetMaxU(SCurveIterCurve(that));
  return true;
}

// Step back the SCurveIter 'that'
// Return false if it couldn't step, true else
#if BUILDMODE != 0
inline
#endif 
bool SCurveIterStepP(SCurveIter* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  if (that->_curPos < PBMATH_EPSILON)
    return false;
  that->_curPos -= that->_delta;
  if (that->_curPos < 0.0)
    that->_curPos = 0.0;
  return true;
}

// Get the current value of the internal parameter of the 
// SCurveIter 'that'
#if BUILDMODE != 0
inline
#endif 
float SCurveIterGetPos(SCurveIter* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  return that->_curPos;  
}

// Get the current value of the attached SCurve at the current 
// internal position of the SCurveIter 'that'
#if BUILDMODE != 0
inline
#endif 
VecFloat* SCurveIterGet(SCurveIter* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  return SCurveGet(SCurveIterCurve(that), that->_curPos);  
}

// Set the value of the iCtrl-th control point to v
#if BUILDMODE != 0
inline
#endif 
void _BBodySetCtrl(BBody* that, VecShort* iCtrl, VecFloat* v) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (iCtrl == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'iCtrl' is null");
    PBErrCatch(BCurveErr);
  }
  if (v == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'v' is null");
    PBErrCatch(BCurveErr);
  }
  if (VecGetDim(iCtrl) != VecGet(&(that->_dim), 0)) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "Dimension of 'iCtrl' is invalid (%d=%d)",
      VecGetDim(iCtrl), VecGet(&(that->_dim), 0));
    PBErrCatch(BCurveErr);
  }
  if (VecGetDim(v) != VecGet(&(that->_dim), 1)) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "Dimension of 'v' is invalid (%d=%d)",
      VecGetDim(v), VecGet(&(that->_dim), 1));
    PBErrCatch(BCurveErr);
  }
#endif
  // Get the index of the ctrl
  int index = BBodyGetIndexCtrl(that, iCtrl);
  // If we could get the index
  if (index != -1)
    // Set the ctrl
    VecCopy(that->_ctrl[index], v);
}

// Get the number of control points of the BBody 'that'
#if BUILDMODE != 0
inline
#endif 
int BBodyGetNbCtrl(BBody* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // Return the number of control points
  return powi(that->_order + 1, VecGet(&(that->_dim), 0));
}

// Get the the 'iCtrl'-th control point of 'that'
#if BUILDMODE != 0
inline
#endif 
VecFloat* _BBodyCtrl(BBody* that, VecShort* iCtrl) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (iCtrl == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'iCtrl' is null");
    PBErrCatch(BCurveErr);
  }
  if (VecGetDim(iCtrl) != VecGet(&(that->_dim), 0)) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "Dimension of 'iCtrl' is invalid (%d=%d)",
      VecGetDim(iCtrl), VecGet(&(that->_dim), 0));
    PBErrCatch(BCurveErr);
  }
#endif
  // Get the index
  int index = BBodyGetIndexCtrl(that, iCtrl);
  // If we could get the index
  if (index != -1)
    // Return the control
    return that->_ctrl[index];
  // Else, we couldn't get the index
  else
    // Return NULL
    return NULL;
}

// Get the index in _ctrl of the 'iCtrl' control point of 'that'
#if BUILDMODE != 0
inline
#endif 
int _BBodyGetIndexCtrl(BBody* that, VecShort* iCtrl) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (iCtrl == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'iCtrl' is null");
    PBErrCatch(BCurveErr);
  }
  if (VecGetDim(iCtrl) != VecGet(&(that->_dim), 0)) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "Dimension of 'iCtrl' is invalid (%d=%d)",
      VecGetDim(iCtrl), VecGet(&(that->_dim), 0));
    PBErrCatch(BCurveErr);
  }
#endif
  for (int iDim = VecGetDim(iCtrl); iDim--;)
    if (VecGet(iCtrl, iDim) < 0 || 
      VecGet(iCtrl, iDim) > that->_order)
      return -1;
  // Declare a variable to memorize the dimension of input
  int dim = VecGetDim(iCtrl);
  // Get the index
  int index = 0;
  for (int iDim = 0; iDim < dim; ++iDim)
    index += index * that->_order + VecGet(iCtrl, iDim);
  // return the index
  return index;
}

// Get the order of the BBody 'that'
#if BUILDMODE != 0
inline
#endif 
int BBodyGetOrder(BBody* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  return that->_order;
}

// Get the dimensions of the BBody 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D* BBodyDim(BBody* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  return &(that->_dim);
}

// Get a copy of the dimensions of the BBody 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D BBodyGetDim(BBody* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  return that->_dim;
}

// Return the center of the BBody (average of control points)
#if BUILDMODE != 0
inline
#endif 
VecFloat* BBodyGetCenter(BBody* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // Sum all the control points
  VecFloat* center = VecFloatCreate(VecGet(BBodyDim(that), 1));
  for (int iCtrl = BBodyGetNbCtrl(that); iCtrl--;)
    VecOp(center, 1.0, that->_ctrl[iCtrl], 1.0);
  // Get the average
  VecScale(center, 1.0 / (float)(BBodyGetNbCtrl(that)));
  // Return the result
  return center;
}

// Translate the BBody by 'v'
#if BUILDMODE != 0
inline
#endif 
void _BBodyTranslate(BBody* that, VecFloat* v) {
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
  if (VecGetDim(v) != VecGet(BBodyDim(that), 1)) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "Dimension of 'v' is invalid (%d=%d)",
      VecGetDim(v), VecGet(BBodyDim(that), 1));
    PBErrCatch(BCurveErr);
  }
#endif
  // For each control point
  for (int iCtrl = BBodyGetNbCtrl(that); iCtrl--;)
    // Translate the control point
    VecOp(that->_ctrl[iCtrl], 1.0, v, 1.0);
}

// Scale the BBody by 'v' relatively to the origin
#if BUILDMODE != 0
inline
#endif 
void _BBodyScaleOriginVector(BBody* that, VecFloat* v) {
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
  if (VecGetDim(v) != VecGet(BBodyDim(that), 1)) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "Dimension of 'v' is invalid (%d=%d)",
      VecGetDim(v), VecGet(BBodyDim(that), 1));
    PBErrCatch(BCurveErr);
  }
#endif
  // For each control point
  for (int iCtrl = BBodyGetNbCtrl(that); iCtrl--;) {
    VecFloat* ctrl = that->_ctrl[iCtrl];
    // Scale the control point
    for (int dim = 0; dim < VecGetDim(ctrl); ++dim)
      VecSet(ctrl, dim, VecGet(ctrl, dim) * VecGet(v, dim));
  }
}

// Scale the BBody by 'c' relatively to the origin
#if BUILDMODE != 0
inline
#endif 
void _BBodyScaleOriginScalar(BBody* that, float c) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // For each control point
  for (int iCtrl = BBodyGetNbCtrl(that); iCtrl--;)
    // Scale the control point
    VecScale(that->_ctrl[iCtrl], c);
}

// Scale the BBody by 'v' relatively to its origin
// (first control point)
#if BUILDMODE != 0
inline
#endif 
void _BBodyScaleStartVector(BBody* that, VecFloat* v) {
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
  if (VecGetDim(v) != VecGet(BBodyDim(that), 1)) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "Dimension of 'v' is invalid (%d=%d)",
      VecGetDim(v), VecGet(BBodyDim(that), 1));
    PBErrCatch(BCurveErr);
  }
#endif
  // For each control point except the first one
  for (int iCtrl = BBodyGetNbCtrl(that); iCtrl-- && iCtrl != 0;) {
    VecFloat* ctrl = that->_ctrl[iCtrl];
    // Translate the control point
    VecOp(ctrl, 1.0, that->_ctrl[0], -1.0);
    // Scale the control point
    for (int dim = 0; dim < VecGetDim(that->_ctrl[iCtrl]); ++dim)
      VecSet(ctrl, dim, VecGet(ctrl, dim) * VecGet(v, dim));
    // Translate back the control point
    VecOp(ctrl, 1.0, that->_ctrl[0], 1.0);
  }
}

// Scale the BBody by 'c' relatively to its origin
// (first control point)
#if BUILDMODE != 0
inline
#endif 
void _BBodyScaleStartScalar(BBody* that, float c) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  // For each control point except the first one
  for (int iCtrl = BBodyGetNbCtrl(that); iCtrl-- && iCtrl != 0;) {
    VecFloat* ctrl = that->_ctrl[iCtrl];
    // Translate the control point
    VecOp(ctrl, 1.0, that->_ctrl[0], -1.0);
    // Scale the control point
    VecScale(ctrl, c);
    // Translate back the control point
    VecOp(ctrl, 1.0, that->_ctrl[0], 1.0);
  }
}

// Scale the BBody by 'v' relatively to its center
// (average of control points)
#if BUILDMODE != 0
inline
#endif 
void _BBodyScaleCenterVector(BBody* that, VecFloat* v) {
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
  if (VecGetDim(v) != VecGet(BBodyDim(that), 1)) {
    BCurveErr->_type = PBErrTypeInvalidArg;
    sprintf(BCurveErr->_msg, "Dimension of 'v' is invalid (%d=%d)",
      VecGetDim(v), VecGet(BBodyDim(that), 1));
    PBErrCatch(BCurveErr);
  }
#endif
  VecFloat* center = BBodyGetCenter(that);
  // For each control point
  for (int iCtrl = BBodyGetNbCtrl(that); iCtrl--;) {
    VecFloat* ctrl = that->_ctrl[iCtrl];
    // Translate the control point
    VecOp(ctrl, 1.0, center, -1.0);
    // Scale the control point
    for (int dim = 0; dim < VecGetDim(that->_ctrl[iCtrl]); ++dim)
      VecSet(ctrl, dim, VecGet(ctrl, dim) * VecGet(v, dim));
    // Translate back the control point
    VecOp(ctrl, 1.0, center, 1.0);
  }
  // Free memory
  VecFree(&center);
}

// Scale the BBody by 'c' relatively to its center
// (average of control points)
#if BUILDMODE != 0
inline
#endif 
void _BBodyScaleCenterScalar(BBody* that, float c) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
#endif
  VecFloat* center = BBodyGetCenter(that);
  // For each control point
  for (int iCtrl = BBodyGetNbCtrl(that); iCtrl--;) {
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

// Rotate the BBody by 'theta' relatively to the origin
// of the coordinates system around 'axis'
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotAxisOrigin(BBody* that, VecFloat3D* axis, float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (axis == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'axis' is null");
    PBErrCatch(BCurveErr);
  }
  if (VecGet(BBodyDim(that), 1) != 3) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' 's dimension is invalid (%d=3)",
      VecGet(BBodyDim(that), 1));
    PBErrCatch(BCurveErr);
  }
#endif
  // For each control point
  for (int iCtrl = BBodyGetNbCtrl(that); iCtrl--;) {
    VecFloat* ctrl = that->_ctrl[iCtrl];
    // Rotate the control point
    VecRotAxis((VecFloat3D*)ctrl, axis, theta);
  }
}

// Rotate the BBody by 'theta' relatively to the center
// of the body around 'axis'
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotAxisCenter(BBody* that, VecFloat3D* axis, float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (axis == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'axis' is null");
    PBErrCatch(BCurveErr);
  }
  if (VecGet(BBodyDim(that), 1) != 3) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' 's dimension is invalid (%d=3)",
      VecGet(BBodyDim(that), 1));
    PBErrCatch(BCurveErr);
  }
#endif
  VecFloat* center = BBodyGetCenter(that);
  // For each control point
  for (int iCtrl = BBodyGetNbCtrl(that); iCtrl--;) {
    VecFloat* ctrl = that->_ctrl[iCtrl];
    // Translate the control point
    VecOp(ctrl, 1.0, center, -1.0);
    // Rotate the control point
    VecRotAxis((VecFloat3D*)ctrl, axis, theta);
    // Translate back the control point
    VecOp(ctrl, 1.0, center, 1.0);
  }
  // Free memory
  VecFree(&center);
}

// Rotate the BBody by 'theta' relatively to the first control point
// of the body around 'axis'
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotAxisStart(BBody* that, VecFloat3D* axis, float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (axis == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'axis' is null");
    PBErrCatch(BCurveErr);
  }
  if (VecGet(BBodyDim(that), 1) != 3) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' 's dimension is invalid (%d=3)",
      VecGet(BBodyDim(that), 1));
    PBErrCatch(BCurveErr);
  }
#endif
  VecFloat* start = that->_ctrl[0];
  // For each control point except the first one
  for (int iCtrl = BBodyGetNbCtrl(that); iCtrl-- && iCtrl != 0;) {
    VecFloat* ctrl = that->_ctrl[iCtrl];
    // Translate the control point
    VecOp(ctrl, 1.0, start, -1.0);
    // Rotate the control point
    VecRotAxis((VecFloat3D*)ctrl, axis, theta);
    // Translate back the control point
    VecOp(ctrl, 1.0, start, 1.0);
  }
}

// Rotate the BBody by 'theta' relatively to the origin
// of the coordinates system around X
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotXOrigin(BBody* that, float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (VecGet(BBodyDim(that), 1) != 3) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' 's dimension is invalid (%d=3)",
      VecGet(BBodyDim(that), 1));
    PBErrCatch(BCurveErr);
  }
#endif
  // For each control point
  for (int iCtrl = BBodyGetNbCtrl(that); iCtrl--;) {
    VecFloat* ctrl = that->_ctrl[iCtrl];
    // Rotate the control point
    VecRotX((VecFloat3D*)ctrl, theta);
  }
}

// Rotate the BBody by 'theta' relatively to the center
// of the body around X
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotXCenter(BBody* that, float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (VecGet(BBodyDim(that), 1) != 3) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' 's dimension is invalid (%d=3)",
      VecGet(BBodyDim(that), 1));
    PBErrCatch(BCurveErr);
  }
#endif
  VecFloat* center = BBodyGetCenter(that);
  // For each control point
  for (int iCtrl = BBodyGetNbCtrl(that); iCtrl--;) {
    VecFloat* ctrl = that->_ctrl[iCtrl];
    // Translate the control point
    VecOp(ctrl, 1.0, center, -1.0);
    // Rotate the control point
    VecRotX((VecFloat3D*)ctrl, theta);
    // Translate back the control point
    VecOp(ctrl, 1.0, center, 1.0);
  }
  // Free memory
  VecFree(&center);
}

// Rotate the BBody by 'theta' relatively to the first control point
// of the body around X
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotXStart(BBody* that, float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (VecGet(BBodyDim(that), 1) != 3) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' 's dimension is invalid (%d=3)",
      VecGet(BBodyDim(that), 1));
    PBErrCatch(BCurveErr);
  }
#endif
  VecFloat* start = that->_ctrl[0];
  // For each control point except the first one
  for (int iCtrl = BBodyGetNbCtrl(that); iCtrl-- && iCtrl != 0;) {
    VecFloat* ctrl = that->_ctrl[iCtrl];
    // Translate the control point
    VecOp(ctrl, 1.0, start, -1.0);
    // Rotate the control point
    VecRotX((VecFloat3D*)ctrl, theta);
    // Translate back the control point
    VecOp(ctrl, 1.0, start, 1.0);
  }
}

// Rotate the BBody by 'theta' relatively to the origin
// of the coordinates system around Y
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotYOrigin(BBody* that, float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (VecGet(BBodyDim(that), 1) != 3) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' 's dimension is invalid (%d=3)",
      VecGet(BBodyDim(that), 1));
    PBErrCatch(BCurveErr);
  }
#endif
  // For each control point
  for (int iCtrl = BBodyGetNbCtrl(that); iCtrl--;) {
    VecFloat* ctrl = that->_ctrl[iCtrl];
    // Rotate the control point
    VecRotY((VecFloat3D*)ctrl, theta);
  }
}

// Rotate the BBody by 'theta' relatively to the center
// of the body around Y
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotYCenter(BBody* that, float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (VecGet(BBodyDim(that), 1) != 3) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' 's dimension is invalid (%d=3)",
      VecGet(BBodyDim(that), 1));
    PBErrCatch(BCurveErr);
  }
#endif
  VecFloat* center = BBodyGetCenter(that);
  // For each control point
  for (int iCtrl = BBodyGetNbCtrl(that); iCtrl--;) {
    VecFloat* ctrl = that->_ctrl[iCtrl];
    // Translate the control point
    VecOp(ctrl, 1.0, center, -1.0);
    // Rotate the control point
    VecRotY((VecFloat3D*)ctrl, theta);
    // Translate back the control point
    VecOp(ctrl, 1.0, center, 1.0);
  }
  // Free memory
  VecFree(&center);
}

// Rotate the BBody by 'theta' relatively to the first control point
// of the body around Y
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotYStart(BBody* that, float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (VecGet(BBodyDim(that), 1) != 3) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' 's dimension is invalid (%d=3)",
      VecGet(BBodyDim(that), 1));
    PBErrCatch(BCurveErr);
  }
#endif
  VecFloat* start = that->_ctrl[0];
  // For each control point except the first one
  for (int iCtrl = BBodyGetNbCtrl(that); iCtrl-- && iCtrl != 0;) {
    VecFloat* ctrl = that->_ctrl[iCtrl];
    // Translate the control point
    VecOp(ctrl, 1.0, start, -1.0);
    // Rotate the control point
    VecRotY((VecFloat3D*)ctrl, theta);
    // Translate back the control point
    VecOp(ctrl, 1.0, start, 1.0);
  }
}

// Rotate the BBody by 'theta' relatively to the origin
// of the coordinates system around Z
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotZOrigin(BBody* that, float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (VecGet(BBodyDim(that), 1) != 3) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' 's dimension is invalid (%d=3)",
      VecGet(BBodyDim(that), 1));
    PBErrCatch(BCurveErr);
  }
#endif
  // For each control point
  for (int iCtrl = BBodyGetNbCtrl(that); iCtrl--;) {
    VecFloat* ctrl = that->_ctrl[iCtrl];
    // Rotate the control point
    VecRotZ((VecFloat3D*)ctrl, theta);
  }
}

// Rotate the BBody by 'theta' relatively to the center
// of the body around Z
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotZCenter(BBody* that, float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (VecGet(BBodyDim(that), 1) != 3) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' 's dimension is invalid (%d=3)",
      VecGet(BBodyDim(that), 1));
    PBErrCatch(BCurveErr);
  }
#endif
  VecFloat* center = BBodyGetCenter(that);
  // For each control point
  for (int iCtrl = BBodyGetNbCtrl(that); iCtrl--;) {
    VecFloat* ctrl = that->_ctrl[iCtrl];
    // Translate the control point
    VecOp(ctrl, 1.0, center, -1.0);
    // Rotate the control point
    VecRotZ((VecFloat3D*)ctrl, theta);
    // Translate back the control point
    VecOp(ctrl, 1.0, center, 1.0);
  }
  // Free memory
  VecFree(&center);
}

// Rotate the BBody by 'theta' relatively to the first control point
// of the body around Z
// dim[1] of BBody must be 3
#if BUILDMODE != 0
inline
#endif 
void BBodyRotZStart(BBody* that, float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' is null");
    PBErrCatch(BCurveErr);
  }
  if (VecGet(BBodyDim(that), 1) != 3) {
    BCurveErr->_type = PBErrTypeNullPointer;
    sprintf(BCurveErr->_msg, "'that' 's dimension is invalid (%d=3)",
      VecGet(BBodyDim(that), 1));
    PBErrCatch(BCurveErr);
  }
#endif
  VecFloat* start = that->_ctrl[0];
  // For each control point except the first one
  for (int iCtrl = BBodyGetNbCtrl(that); iCtrl-- && iCtrl != 0;) {
    VecFloat* ctrl = that->_ctrl[iCtrl];
    // Translate the control point
    VecOp(ctrl, 1.0, start, -1.0);
    // Rotate the control point
    VecRotZ((VecFloat3D*)ctrl, theta);
    // Translate back the control point
    VecOp(ctrl, 1.0, start, 1.0);
  }
}
