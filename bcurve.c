// ============ BCURVE.C ================

// ================= Include =================

#include "bcurve.h"

// ================= Define ==================

// ================ Functions declaration ====================

// Recursive function to calculate the value of a BSurf
VecFloat* BSurfGetRec(BSurf *that, BCurve *curve, 
  VecShort *iCtrl, VecFloat *uSafe, int iDimIn);

// ================ Functions implementation ====================

// Create a new BCurve of order 'order' and dimension 'dim'
// Return NULL if we couldn't create the BCurve
BCurve* BCurveCreate(int order, int dim) {
  // Check arguments
  if (order < 0 || dim < 1)
    return NULL;
  // Allocate memory
  BCurve *that = (BCurve*)malloc(sizeof(BCurve));
  //If we could allocate memory
  if (that != NULL) {
    // Set the values
    that->_dim = dim;
    that->_order = order;
    // Allocate memory for the array of control points
    that->_ctrl = (VecFloat**)malloc(sizeof(VecFloat*) * (order + 1));
    // If we couldn't allocate memory
    if (that->_ctrl == NULL) {
      // Free memory
      free(that);
      // Stop here
      return NULL;
    }
    // For each control point
    for (int iCtrl = 0; iCtrl < order + 1; ++iCtrl)
      // Initialize the pointer
      that->_ctrl[iCtrl] = NULL;
    // For each control point
    for (int iCtrl = 0; iCtrl < order + 1; ++iCtrl) {
      // Allocate memory
      that->_ctrl[iCtrl] = VecFloatCreate(dim);
      // If we couldn't allocate memory
      if (that->_ctrl[iCtrl] == NULL) {
        // Free memory
        BCurveFree(&that);
        // Stop here
        return NULL;
      }
    }
  }
  // Return the new BCurve
  return that;
}

// Clone the BCurve
// Return NULL if we couldn't clone the BCurve
BCurve* BCurveClone(BCurve *that) {
  // Check argument
  if (that == NULL)
    return NULL;
  // Allocate memory for the clone
  BCurve *clone = (BCurve*)malloc(sizeof(BCurve));
  // If we could allocate memory
  if (clone != NULL) {
    // Clone the properties
    clone->_dim = that->_dim;
    clone->_order = that->_order;
    // Allocate memory for the array of control points
    clone->_ctrl = (VecFloat**)malloc(sizeof(VecFloat*) * 
      (clone->_order + 1));
    // If we couldn't allocate memory
    if (that->_ctrl == NULL) {
      // Free memory
      free(clone);
      // Stop here
      return NULL;
    }
    // For each control point
    for (int iCtrl = 0; iCtrl < clone->_order + 1; ++iCtrl) {
      // Clone the control point
      clone->_ctrl[iCtrl] = VecClone(that->_ctrl[iCtrl]);
      // If we couldn't clone the control point
      if (clone->_ctrl[iCtrl] == NULL) {
        // Free memory
        BCurveFree(&clone);
        // Stop here
        return NULL;
      }
    }
  }
  // Return the clone
  return clone;
}

// Load the BCurve from the stream
// If the BCurve is already allocated, it is freed before loading
// Return 0 in case of success, or:
// 1: invalid arguments
// 2: can't allocate memory
// 3: invalid data
// 4: fscanf error
// 5: VecLoad error
int BCurveLoad(BCurve **that, FILE *stream) {
  // Check arguments
  if (that == NULL || stream == NULL)
    return 1;
  // If 'that' is already allocated
  if (*that != NULL) {
    // Free memory
    BCurveFree(that);
  }
  // Read the order and dimension
  int order;
  int dim;
  int ret = fscanf(stream, "%d %d", &order, &dim);
  // If we couldn't read
  if (ret == EOF) {
    return 4;
  }
  // Allocate memory
  *that = BCurveCreate(order, dim);
  // If we coudln't allocate memory
  if (*that == NULL) {
    return 2;
  }
  // For each control point
  for (int iCtrl = 0; iCtrl < (order + 1); ++iCtrl) {
    // Load the control point
    ret = VecLoad((*that)->_ctrl + iCtrl, stream);
    // If we couldn't read the control point or the conrtol point
    // is not of the correct dimension
    if (ret != 0 || VecDim((*that)->_ctrl[iCtrl]) != (*that)->_dim) {
      // Free memory
      BCurveFree(that);
      // Stop here
      return 5;
    }
  }
  // Return success code
  return 0;
}

// Save the BCurve to the stream
// Return 0 upon success, or
// 1: invalid arguments
// 2: fprintf error
// 3: VecSave error
int BCurveSave(BCurve *that, FILE *stream) {
  // Check arguments
  if (that == NULL || stream == NULL)
    return 1;
  // Save the order and dimension
  int ret = fprintf(stream, "%d %d\n", that->_order, that->_dim);
  // If the fprintf failed
  if (ret < 0) {
    // Stop here
    return 2;
  }
  // For each control point
  for (int iCtrl = 0; iCtrl < that->_order + 1; ++iCtrl) {
    // Save the control point
    ret = VecSave(that->_ctrl[iCtrl], stream);
    // If we couldn't save the control point
    if (ret != 0) {
      // Stop here
      return 3;
    }
  }
  // Return success code
  return 0;
}

// Free the memory used by a BCurve
// Do nothing if arguments are invalid
void BCurveFree(BCurve **that) {
  // Check argument
  if (that == NULL || *that == NULL)
    return;
  // If there are control points
  if ((*that)->_ctrl != NULL) {
    // For each control point
    for (int iCtrl = 0; iCtrl < (*that)->_order + 1; ++iCtrl) {
      // Free the control point
      VecFree((*that)->_ctrl + iCtrl);
    }
  }
  // Free the array of control points
  free((*that)->_ctrl);
  // Free memory
  free(*that);
  *that = NULL;
}

// Print the BCurve on 'stream'
// Do nothing if arguments are invalid
void BCurvePrint(BCurve *that, FILE *stream) {
  // Check arguments
  if (that == NULL || stream == NULL)
    return;
  // Print the order and dim
  fprintf(stream, "order(%d) dim(%d) ", that->_order, that->_dim);
  // For each control point
  for (int iCtrl = 0; iCtrl < that->_order + 1; ++iCtrl) {
    VecPrint(that->_ctrl[iCtrl], stream);
    fprintf(stream, " ");
  }
}

// Set the value of the iCtrl-th control point to v
// Do nothing if arguments are invalid
void BCurveSet(BCurve *that, int iCtrl, VecFloat *v) {
  // Check arguments
  if (that == NULL || v == NULL || iCtrl < 0 || 
    iCtrl > that->_order || VecDim(v) != BCurveDim(that))
    return;
  // Set the values
  VecCopy(that->_ctrl[iCtrl], v);
}

// Get the value of the BCurve at paramater 'u' (in [0.0, 1.0])
// Return NULL if arguments are invalid or malloc failed
// if 'u' < 0.0 it is replaced by 0.0
// if 'u' > 1.0 it is replaced by 1.0
VecFloat* BCurveGet(BCurve *that, float u) {
  // Check arguments
  if (that == NULL)
    return NULL;
  if (u < 0.0)
    u = 0.0;
  if (u > 1.0)
    u = 1.0;
  // Allocate memory for the result
  VecFloat *v = VecFloatCreate(that->_dim);
  // Declare a variable for calcul
  float *val = (float*)malloc(sizeof(float) * (that->_order + 1));
  // If we couldn't allocate memory
  if (v == NULL || val == NULL) {
    VecFree(&v);
    if (val == NULL) 
      free(val);
    return NULL;
  }
  // Loop on dimension
  for (int dim = that->_dim; dim--;) {
    // Initialise the temporary variable with the value in current
    // dimension of the control points
    for (int iCtrl = 0; iCtrl < that->_order + 1; ++iCtrl)
      val[iCtrl] = VecGet(that->_ctrl[iCtrl], dim);
    // Loop on order
    int subOrder = that->_order;
    while (subOrder != 0) {
      // Loop on sub order
      for (int order = 0; order < subOrder; ++order) {
        val[order] = (1.0 - u) * val[order] + u * val[order + 1];
      }
      --subOrder;
    }
    // Set the value for the current dim
    VecSet(v, dim, val[0]);
  }
  // Free memory
  free(val);
  // Return the result
  return v;
}

// Get the order of the BCurve
// Return -1 if argument is invalid
int BCurveOrder(BCurve *that) {
  // Check arguments
  if (that == NULL)
    return -1;
  return that->_order;
}

// Get the dimension of the BCurve
// Return 0 if argument is invalid
int BCurveDim(BCurve *that) {
  // Check arguments
  if (that == NULL)
    return 0;
  return that->_dim;
}

// Get the approximate length of the BCurve (sum of dist between
// control points)
// Return 0.0 if argument is invalid
float BCurveApproxLen(BCurve *that) {
  // Check arguments
  if (that == NULL)
    return 0.0;
  // Declare a variable to calculate the length
  float res = 0.0;
  // Calculate the length
  for (int iCtrl = 0; iCtrl < that->_order; ++iCtrl)
    res += VecDist(that->_ctrl[iCtrl], that->_ctrl[iCtrl + 1]);
  // Return the length
  return res;
}

// Rotate the curve CCW by 'theta' radians relatively to the origin
// Do nothing if arguments are invalid
void BCurveRot2D(BCurve *that, float theta) {
  // Check arguments
  if (that == NULL || that->_dim != 2)
    return;
  // For each control point
  for (int iCtrl = 0; iCtrl <= that->_order; ++iCtrl)
    // Rotate the control point
    VecRot2D(that->_ctrl[iCtrl], theta);
}

// Scale the curve by 'v' relatively to the origin
// Do nothing if arguments are invalid
void BCurveScale(BCurve *that, VecFloat *v) {
  // Check arguments
  if (that == NULL || v == NULL)
    return;
  // For each control point
  for (int iCtrl = 0; iCtrl <= that->_order; ++iCtrl)
    // Scale the control point
    for (int dim = 0; dim < VecDim(that->_ctrl[iCtrl]); ++dim)
      VecSet(that->_ctrl[iCtrl], dim, 
        VecGet(that->_ctrl[iCtrl], dim) * VecGet(v, dim));
}

// Translate the curve by 'v'
// Do nothing if arguments are invalid
void BCurveTranslate(BCurve *that, VecFloat *v) {
  // Check arguments
  if (that == NULL || v == NULL)
    return;
  // For each control point
  for (int iCtrl = 0; iCtrl <= that->_order; ++iCtrl)
    // Translate the control point
    VecOp(that->_ctrl[iCtrl], 1.0, v, 1.0);
}

// Create a BCurve which pass through the points given in the GSet 'set'
// The GSet must contains VecFloat of same dimensions
// The BCurve pass through the points in the order they are given
// in the GSet. The points don't need to be uniformly distributed
// The created BCurve is of same dimension as the VecFloat and of order 
// equal to the number of VecFloat in 'set' minus one
// Return NULL if it couldn't create the BCurve or the arguments are
// invalid
BCurve* BCurveFromCloudPoint(GSet *set) {
  // Check arguments
  if (set == NULL || set->_nbElem < 1)
    return NULL;
  // Declare a variable to memorize the result
  int order = set->_nbElem - 1;
  int dim = VecDim((VecFloat*)(set->_head->_data));
  BCurve *curve = BCurveCreate(order, dim);
  // If we could allocate memory
  if (curve != NULL) {
    // Set the first control point to the first point in the point cloud
    BCurveSet(curve, 0, (VecFloat*)(set->_head->_data));
    // If the order is greater than 0
    if (order > 0) {
      // Set the last control point to the last point in the point cloud
      BCurveSet(curve, order, (VecFloat*)(set->_tail->_data));
      // If the order is greater than 1
      if (order > 1) {
        // Calculate the t values for intermediate control points
        // They are equal to the relative distance on the polyline
        // linking the point in the point cloud
        // Declare a variable to memorize the dimension of the matrix
        // in the linear system to solve
        VecShort *dimMat = VecShortCreate(2);
        // Declare a variable to memorize the t values
        VecFloat *t = VecFloatCreate(order + 1);
        // If we could allocate memory
        if (t != NULL && dimMat != NULL) {
          // Set the dimensions of the matrix of the linear system
          VecSet(dimMat, 0, order - 1);
          VecSet(dimMat, 1, order - 1);
          // For each point 
          GSetElem *elem = set->_head->_next;
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
            MatFloat *m = MatFloatCreate(dimMat);
            VecFloat *v = VecFloatCreate(VecGet(dimMat, 0));
            // If we could allocate memory
            if (m != NULL && v != NULL) {
              // Set the values of the linear system
              // For each line (equivalent to each intermediate point 
              // in point cloud)
              for (VecSet(dimMat, 1, 0); 
                VecGet(dimMat, 1) < order - 1;
                VecSet(dimMat, 1, VecGet(dimMat, 1) + 1)) {
                // Get the weight of the control point at the value 
                // of t for this point
                VecFloat *weight = 
                  BCurveGetWeightCtrlPt(curve, VecGet(t, 
                  VecGet(dimMat, 1) + 1));
                // If we could get the weights
                if (weight != NULL) {
                  // For each intermediate control point
                  for (VecSet(dimMat, 0, 0); 
                    VecGet(dimMat, 0) < order - 1;
                    VecSet(dimMat, 0, VecGet(dimMat, 0) + 1))
                    // Set the matrix value with the corresponding
                    // weight
                    MatSet(m, dimMat, VecGet(weight, 
                      VecGet(dimMat, 0) + 1));
                }
                // Set the vector value with the corresponding point
                // coordinate
                float x = VecGet((VecFloat*)(GSetGet(set, 
                  VecGet(dimMat, 1) + 1)), iDim);
                x -= VecGet(weight, 0) * 
                  VecGet((VecFloat*)(set->_head->_data), iDim);
                x -= VecGet(weight, order) * 
                  VecGet((VecFloat*)(set->_tail->_data), iDim);
                VecSet(v, VecGet(dimMat, 1), x);
                // Free memory
                VecFree(&weight);
              }
              // Declare a variable to memorize the linear system
              EqLinSys *sys = EqLinSysCreate(m, v);
              // If we could allocate memory
              if (sys != NULL) {
                // Solve the system
                VecFloat *solSys = EqLinSysSolve(sys);
                // If we could solve the linear system
                if (solSys != NULL) {
                  // Memorize the values of control points for the 
                  // current dimension
                  for (int iCtrl = 1; iCtrl < order; ++iCtrl)
                    VecSet(curve->_ctrl[iCtrl], iDim, 
                      VecGet(solSys, iCtrl - 1));
                  // Free memory
                  VecFree(&solSys);
                }
              }
              // Free memory
              EqLinSysFree(&sys);
              VecFree(&v);
              MatFree(&m);
            }
          }
        }
        // Free memory
        VecFree(&dimMat);
        VecFree(&t);
      }
    }
  }
  // Return the result
  return curve;
}

// Get a VecFloat of dimension equal to the number of control points
// Values of the VecFloat are the weight of each control point in the 
// BCurve given the curve's order and the value of 't' (in [0.0,1.0])
// Return null if the arguments are invalid or memory allocation failed
VecFloat* BCurveGetWeightCtrlPt(BCurve *that, float t) {
  // Check arguments
  if (that == NULL || t < 0.0 || t > 1.0)
    return NULL;
  // Declare a variable to memorize the result
  VecFloat *res = VecFloatCreate(that->_order + 1);
  // If we could allocate memory
  if (res != NULL) {
    // Initilize the two first weights
    VecSet(res, 0, 1.0 - t);
    VecSet(res, 1, t);
    // For each higher order
    for (int order = 1; order < that->_order; ++order) {
      // For each control point at this order, starting by the last one
      // to avoid using a temporary buffer
      for (int iCtrl = order + 2; iCtrl--;) {
        // Calculate the weight of this control point
        // VecGet(v, - 1) = 0.0 and VecFloat is initialized to 0.0
        // => no need to check for border cases
        VecSet(res, iCtrl, 
          (1.0 - t) * VecGet(res, iCtrl) + t * VecGet(res, iCtrl - 1));
      }
    }
  }
  // Return the result
  return res;
}

// Get the bounding box of the BCurve.
// Return a Facoid whose axis are aligned on the standard coordinate 
// system.
// Return NULL if arguments are invalid.
Shapoid* BCurveGetBoundingBox(BCurve *that) {
  // Check argument
  if (that == NULL)
    return NULL;
  // Declare a variable to memorize the result
  Shapoid *res = FacoidCreate(that->_dim);
  // If we could allocate memory
  if (res != NULL) {
    // For each dimension
    for (int iDim = that->_dim; iDim--;) {
      // For each control point
      for (int iCtrl = that->_order + 1; iCtrl--;) {
        // If it's the first control point in this dimension
        if (iCtrl == that->_order) {
          // Initialise the bounding box
          VecSet(res->_pos, iDim, VecGet(that->_ctrl[iCtrl], iDim));
          VecSet(res->_axis[iDim], iDim, 
            VecGet(that->_ctrl[iCtrl], iDim));
        // Else, it's not the first control point in this dimension
        } else {
          // Update the bounding box
          if (VecGet(that->_ctrl[iCtrl], iDim) < 
            VecGet(res->_pos, iDim))
            VecSet(res->_pos, iDim, VecGet(that->_ctrl[iCtrl], iDim));
          if (VecGet(that->_ctrl[iCtrl], iDim) > 
            VecGet(res->_axis[iDim], iDim))
            VecSet(res->_axis[iDim], iDim,
              VecGet(that->_ctrl[iCtrl], iDim));
        }
      }
      VecSet(res->_axis[iDim], iDim,
        VecGet(res->_axis[iDim], iDim) - VecGet(res->_pos, iDim));
    }
  }
  // Return the result
  return res;
}

// Create a new SCurve of dimension 'dim'
// Return NULL if we couldn't create the SCurve
SCurve* SCurveCreate(int dim) {
  // Check arguments
  if (dim <= 0)
    return NULL;
  // Declare a variable for the returned SCurve
  SCurve *ret = (SCurve*)malloc(sizeof(SCurve));
  // If we could allocate memory
  if (ret != NULL) {
    // Set the properties
    ret->_dim = dim;
    // Create the set
    ret->_curves = GSetCreate();
    // If we couldn't allocate memory
    if (ret->_curves == NULL) {
      // Free memory and stop here
      SCurveFree(&ret);
      return NULL;
    }
  }
  // Return the new SCurve
  return ret; 
}

// Clone the SCurve
// Return NULL if we couldn't clone the SCurve
SCurve* SCurveClone(SCurve *that) {
  // Check arguments
  if (that == NULL)
    return NULL;
  // Allocate memory
  SCurve *ret = SCurveCreate(SCurveDim(that));
  // If we could allocate memory
  if (ret != NULL) {
    // Declare a pointer to the elements of the set
    GSetElem *ptr = that->_curves->_head;
    // Loop on elements
    while (ptr != NULL) {
      // Clone the BCurve and add it to the clone of SCurve
      GSetAppend(ret->_curves, BCurveClone((BCurve*)(ptr->_data)));
      // Move to the next element
      ptr = ptr->_next;
    }
  }
  // Return the cloned SCurve
  return ret;
}

// Load the SCurve from the stream
// If the SCurve is already allocated, it is freed before loading
// Return 0 in case of success, or:
// 1: invalid arguments
// 2: can't allocate memory
// 3: invalid data
// 4: fscanf error
// 5: BCurveLoad error
int SCurveLoad(SCurve **that, FILE *stream) {
  // Check arguments
  if (that == NULL || stream == NULL)
    return 1;
  // If 'that' is already allocated
  if (*that != NULL) {
    // Free memory
    SCurveFree(that);
  }
  // Read the dimension and number of curve
  int dim = 0;
  int nbCurve = 0;
  int ret = fscanf(stream, "%d %d", &dim, &nbCurve);
  // If we couldn't read
  if (ret == EOF) {
    return 4;
  }
  // Allocate memory
  *that = SCurveCreate(dim);
  // If we couldn't allocate memory
  if (*that == NULL) {
    return 2;
  }
  // Loop on curves
  for (int iCurve = 0; iCurve < nbCurve; ++iCurve) {
    // Declare a variable to load the BCurve
    BCurve *curve = NULL;
    // Load the BCurve
    ret = BCurveLoad(&curve, stream);
    // If we couldn't load the BCurve
    if (ret != 0)
      return 5;
    // Check the dimension of the curve
    if (BCurveDim(curve) != dim)
      return 3;
    // Add the BCurve to the SCurve
    SCurveAdd(*that, curve);
  }
  return 0;
}

// Save the SCurve to the stream
// Return 0 upon success, else
// 1: invalid arguments
// 2: fprintf error
// 3: BCurveSave error
int SCurveSave(SCurve *that, FILE *stream) {
  // Check arguments
  if (that == NULL || stream == NULL)
    return 1;
  // Save the dimension and number of curve
  int ret = fprintf(stream, "%d %d\n", that->_dim, 
    that->_curves->_nbElem);
  // If the fprintf failed
  if (ret < 0)
    // Stop here
    return 2;
  // Declare a pointer on elements of the set of curves
  GSetElem *ptr = that->_curves->_head;
  // Loop on elements
  while (ptr != NULL) {
    // Save the BCurve
    BCurveSave((BCurve*)(ptr->_data), stream);
    // Move to the next BCurve
    ptr = ptr->_next;
  }
  return 0;
}

// Free the memory used by a SCurve
// Do nothing if arguments are invalid
void SCurveFree(SCurve **that) {
  // Check argument
  if (that == NULL || *that == NULL)
    return;
  // Declare a pointer on elements of the set of curves
  GSetElem *ptr = (*that)->_curves->_head;
  // Loop on elements
  while (ptr != NULL) {
    // Free the BCurve
    BCurveFree((BCurve**)(&(ptr->_data)));
    // Move to the next BCurve
    ptr = ptr->_next;
  }
  // Free memory
  GSetFree(&((*that)->_curves));
  free(*that);
  *that = NULL;
}

// Print the SCurve on 'stream'
// Do nothing if arguments are invalid
void SCurvePrint(SCurve *that, FILE *stream) {
  // Check argument
  if (that == NULL || stream == NULL)
    return;
  // Declare a pointer on elements of the set of curves
  GSetElem *ptr = that->_curves->_head;
  // Loop on elements
  while (ptr != NULL) {
    // Print the BCurve
    BCurvePrint((BCurve*)(ptr->_data), stream);
    fprintf(stream, "\n");
    // Move to the next BCurve
    ptr = ptr->_next;
  }
}

// Set the 'iCurve'-th BCurve to a clone of 'curve'
// 'iCurve' must be in [0, current number of BCurve]
// 'curve' 's dimension must be equal to SCurve's dimension
// Do nothing if arguments are invalid
void SCurveSet(SCurve *that, int iCurve, BCurve *curve) {
  // Check arguments
  if (that == NULL || curve == NULL || iCurve < 0 || 
    iCurve > that->_curves->_nbElem)
    return;
  // Clone the curve
  BCurve *clone = BCurveClone(curve);
  // If we could clone)
  if (clone != NULL)
    // Insert a clone of the curve
    GSetInsert(that->_curves, clone, iCurve);
}

// Append a clone of 'curve'
// 'curve' 's dimension must be equal to SCurve's dimension
// Do nothing if arguments are invalid
void SCurveAdd(SCurve *that, BCurve *curve) {
  // Check arguments
  if (that == NULL || curve == NULL)
    return;
  // Append the curve
  SCurveSet(that, that->_curves->_nbElem, curve);
}

// Remove the 'iCurve'-th BCurve from the SCurve
// Return NULL if arguments are invalid
BCurve* SCurveRemove(SCurve *that, int iCurve) {
  // Check arguments
  if (that == NULL)
    return NULL;
  // Get the BCurve out of the set
  BCurve *curve = (BCurve*)GSetRemove(that->_curves, iCurve);
  // Return the curve
  return curve;
}

// Get the 'iCurve'-th BCurve of the SCurve, without removing it
// Return NULL if arguments are invalid
BCurve* SCurveGet(SCurve *that, int iCurve) {
  // Check arguments
  if (that == NULL)
    return NULL;
  // Return the BCurve
  return (BCurve*)(GSetGet(that->_curves, iCurve));
}

// Get the number of BCurve in the SCurve
// Return 0 if arguments are invalid
int SCurveGetNbCurve(SCurve *that) {
  // Check arguments
  if (that == NULL)
    return 0;
  // Return the number of BCurves
  return that->_curves->_nbElem;
}

// Get the dimension of the SCurve
// Return 0 if argument is invalid
int SCurveDim(SCurve *that) {
  // Check arguments
  if (that == NULL)
    return 0;
  // Return the dimension
  return that->_dim;
}

// Get the approximate length of the SCurve (sum of approxLen 
// of its BCurves)
// Return 0.0 if argument is invalid
float SCurveApproxLen(SCurve *that) {
  // Check arguments
  if (that == NULL)
    return 0.0;
  // Declare a variable to calculate the length
  float length = 0.0;
  // Declare a pointer on elements of the set of curves
  GSetElem *ptr = that->_curves->_head;
  // Loop on elements
  while (ptr != NULL) {
    // Add the approximate length of this BCurve
    length += BCurveApproxLen((BCurve*)(ptr->_data));
    // Move to the next BCurve
    ptr = ptr->_next;
  }
  // Return the length
  return length;
}

// Rotate the SCurve CCW by 'theta' radians relatively to the origin
// Do nothing if arguments are invalid
void SCurveRot2D(SCurve *that, float theta) {
  // Check arguments
  if (that == NULL)
    return;
  // Declare a pointer on elements of the set of curves
  GSetElem *ptr = that->_curves->_head;
  // Loop on elements
  while (ptr != NULL) {
    // Rotate the BCurve
    BCurveRot2D((BCurve*)(ptr->_data), theta);
    // Move to the next BCurve
    ptr = ptr->_next;
  }
}

// Scale the SCurve by 'v' relatively to the origin
// Do nothing if arguments are invalid
void SCurveScale(SCurve *that, VecFloat *v) {
  // Check arguments
  if (that == NULL || v == NULL)
    return;
  // Declare a pointer on elements of the set of curves
  GSetElem *ptr = that->_curves->_head;
  // Loop on elements
  while (ptr != NULL) {
    // Rotate the BCurve
    BCurveScale((BCurve*)(ptr->_data), v);
    // Move to the next BCurve
    ptr = ptr->_next;
  }
}

// Translate the SCurve by 'v'
// Do nothing if arguments are invalid
void SCurveTranslate(SCurve *that, VecFloat *v) {
  // Check arguments
  if (that == NULL || v == NULL)
    return;
  // Declare a pointer on elements of the set of curves
  GSetElem *ptr = that->_curves->_head;
  // Loop on elements
  while (ptr != NULL) {
    // Translate the BCurve
    BCurveTranslate((BCurve*)(ptr->_data), v);
    // Move to the next BCurve
    ptr = ptr->_next;
  }
}

// Get the bounding box of the SCurve.
// Return a Facoid whose axis are aligned on the standard coordinate 
// system.
// Return NULL if arguments are invalid.
Shapoid* SCurveGetBoundingBox(SCurve *that) {
  // Check arguments
  if (that == NULL)
    return NULL;
  // Allocate memory for the set of bounding boxes of BCurve
  GSet *set = GSetCreate();
  // If we couldn't allocate memory
  if (set == NULL) {
    return NULL;
  }
  // Add the bounding box of each BCurve
  GSetElem *ptr = set->_head;
  while (ptr != NULL) {
    GSetAppend(set, BCurveGetBoundingBox((BCurve*)(ptr->_data)));
    ptr = ptr->_next;
  }
  // Get the bounding box of the set of bounding boxes of BCurve
  Shapoid *ret = ShapoidGetBoundingBoxSet(set);
  // Free memory used by the set of bounding boxes of BCurve
  ptr = set->_head;
  while (ptr != NULL) {
    ShapoidFree((Shapoid**)(&(ptr->_data)));
    ptr = ptr->_next;
  }
  GSetFree(&set);
  // Return the result
  return ret;
}

// Create a new BSurf of order 'order' and dimension 'dim'
// Controls are initialized with null vectors
// Return NULL if we couldn't create the BSurf
BSurf* BSurfCreate(int order, VecShort *dim) {
  // Check arguments
  if (order < 0 || dim == NULL || VecDim(dim) != 2)
    return NULL;
  for (int iDim = 2; iDim--;)
    if (VecGet(dim, iDim) <= 0)
      return NULL;
  // Allocate memory for the new BSurf
  BSurf *that = (BSurf*)malloc(sizeof(BSurf));
  // If we could allocate memory
  if (that != NULL) {
    // Init pointers
    that->_dim = NULL;
    that->_ctrl = NULL;
    // Init properties
    that->_order = order;
    that->_dim = VecClone(dim);
    if (that->_dim == NULL) {
      BSurfFree(&that);
      return NULL;
    }
    // Init the control
    int nbCtrl = BSurfGetNbCtrl(that);
    that->_ctrl = (VecFloat**)malloc(sizeof(VecFloat*) * nbCtrl);
    if (that->_ctrl == NULL) {
      BSurfFree(&that);
      return NULL;
    }
    for (int iCtrl = nbCtrl; iCtrl--;)
      that->_ctrl[iCtrl] = NULL;
    for (int iCtrl = nbCtrl; iCtrl--;) {
      that->_ctrl[iCtrl] = VecFloatCreate(VecGet(dim, 1));
      if (that->_ctrl[iCtrl] == NULL) {
        BSurfFree(&that);
        return NULL;
      }
    }
  }
  // Return the new BSurf
  return that;
}

// Free the memory used by a BSurf
// Do nothing if arguments are invalid
void BSurfFree(BSurf **that) {
  // Check arguments
  if (that == NULL || *that == NULL)
    return;
  // Get the number of ctrl
  int nbCtrl = BSurfGetNbCtrl(*that);
  // Free memory
  VecFree(&((*that)->_dim));
  for (int iCtrl = nbCtrl; iCtrl--;) {
    VecFree((*that)->_ctrl + iCtrl);
  }
  free((*that)->_ctrl);
  free(*that);
  *that = NULL;
}

// Set the value of the iCtrl-th control point to v
// Do nothing if arguments are invalid
void BSurfSet(BSurf *that, VecShort *iCtrl, VecFloat *v) {
  // Check arguments
  if (that == NULL || iCtrl == NULL || v == NULL ||
    VecDim(iCtrl) != VecGet(that->_dim, 0) || 
    VecDim(v) != VecGet(that->_dim, 1))
    return;
  // Get the index of the ctrl
  int index = BSurfGetIndexCtrl(that, iCtrl);
  // If we could get the index
  if (index != -1)
    // Set the ctrl
    VecCopy(that->_ctrl[index], v);
}

// Get the value of the BSurf at paramater 'u' (in [0.0, 1.0])
// Return NULL if arguments are invalid or malloc failed
// Components of 'u' < 0.0 are replaced by 0.0
// Components of 'u' > 1.0 are replaced by 1.0
VecFloat* BSurfGet(BSurf *that, VecFloat *u) {
  // Check arguments
  if (that == NULL || u == NULL || VecDim(u) != VecGet(that->_dim, 0))
    return NULL;
  // Declare variables to memorize the nb of dimension
  int nbDimIn = VecGet(that->_dim, 0);
  int nbDimOut = VecGet(that->_dim, 1);
  // Create a clone of u to be checked for components interval
  VecFloat *uSafe = VecClone(u);
  // Declare a vector to memorize the index of the ctrl
  VecShort *iCtrl = VecShortCreate(nbDimIn);
  // Declare a BCurve used for calculation
  BCurve *curve = BCurveCreate(that->_order, nbDimOut);
  // If we couldn't allocate memory
  if (uSafe == NULL || curve == NULL || iCtrl == NULL) {
    VecFree(&uSafe);
    VecFree(&iCtrl);
    BCurveFree(&curve);
    return NULL;
  }
  // Check components
  for (int iDim = nbDimIn; iDim--;) {
    if (VecGet(uSafe, iDim) < 0.0)
      VecSet(uSafe, iDim, 0.0);
    if (VecGet(uSafe, iDim) > 1.0)
      VecSet(uSafe, iDim, 1.0);
  }
  // Calculate recursively the result value
  VecFloat *res = BSurfGetRec(that, curve, iCtrl, uSafe, 0);
  // Free memory
  VecFree(&uSafe);
  VecFree(&iCtrl);
  BCurveFree(&curve);
  // Return the result
  return res;
}

// Recursive function to calculate the value of SCurve
VecFloat* BSurfGetRec(BSurf *that, BCurve *curve, 
  VecShort *iCtrl, VecFloat *u, int iDimIn) {
  // Declare a variable for the result
  VecFloat *res = NULL;
  // If we are at the last dimension in the recursion, 
  // the curve controls are the controls of the surface at current
  // position in control's space
  if (iDimIn == VecGet(that->_dim, 0) - 1) {
    for (int i = that->_order + 1; i--;) {
      VecSet(iCtrl, iDimIn, i); 
      BCurveSet(curve, i, BSurfGetCtrl(that, iCtrl));
    }
  // Else, we are not at the last dimension in control's space
  } else {
    // Clone the position (to edit the lower dimension at lower 
    // level of the recursion)
    VecShort *jCtrl = VecClone(iCtrl);
    // If we couldn't clone
    if (jCtrl == NULL)
      // Return null
      return NULL;
    // Declare an array of VecFloat to memorize the control at 
    // the current level
    VecFloat **tmpCtrl = 
      (VecFloat**)malloc(sizeof(VecFloat*) * (that->_order + 1));
    // If we couldn't allocate memory
    if (tmpCtrl == NULL)
      // Return null
      return NULL;
    // For chaque control
    for (int i = that->_order + 1; i--;) {
      // Update the control position
      VecSet(jCtrl, iDimIn, i);
      // Get recursively the control (equal to the BCurve value at 
      // lower level)
      tmpCtrl[i] = 
        BSurfGetRec(that, curve, jCtrl, u, iDimIn + 1);
    }
    // Set the control of the curve at current level
    // Use a temporary instead of affecting directly into curve
    // because it is shared between recursion level and affecting
    // directly would lead to overwritting during the process
    for (int i = that->_order + 1; i--;)
      BCurveSet(curve, i, tmpCtrl[i]);
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

// Get the number of control point of the BSurf 'that'
// Return 0 if arguments are invalid
int BSurfGetNbCtrl(BSurf *that) {
  if (that == NULL)
    return 0;
  // Get the result
  int nb = powi(that->_order + 1, VecGet(that->_dim, 0));
  // Return the result;
  return nb;
}

// Get the index in _ctrl of the 'iCtrl' control point of 'that'
// ctrl are ordered as follow: 
// (0,0,0),(0,0,1),...,(0,0,order+1),(0,1,0),(0,1,1),...
// Return -1 if arguments are invalid
int BSurfGetIndexCtrl(BSurf *that, VecShort *iCtrl) {
  // Check arguments
  if (that == NULL || iCtrl == NULL ||
    VecDim(iCtrl) != VecGet(that->_dim, 0))
    return -1;
  for (int iDim = VecDim(iCtrl); iDim--;)
    if (VecGet(iCtrl, iDim) < 0 || 
      VecGet(iCtrl, iDim) > that->_order)
      return -1;
  // Declare a variable to memorize the dimension of input
  int dim = VecDim(iCtrl);
  // Get the index
  int index = 0;
  for (int iDim = 0; iDim < dim; ++iDim)
    index += index * that->_order + VecGet(iCtrl, iDim);
  // return the index
  return index;
}

// Get the the 'iCtrl'-th control point of 'that'
// ctrl are ordered as follow: 
// (0,0,0),(0,0,1),...,(0,0,order+1),(0,1,0),(0,1,1),...
// Return NULL if arguments are invalid
VecFloat* BSurfGetCtrl(BSurf *that, VecShort *iCtrl) {
  // Check arguments
  if (that == NULL || iCtrl == NULL)
    return NULL;
  // Get the index
  int index = BSurfGetIndexCtrl(that, iCtrl);
  // If we could get the index
  if (index != -1)
    // Return the control
    return that->_ctrl[index];
  // Else, we couldn't get the index
  else
    // Return NULL
    return NULL;
}
