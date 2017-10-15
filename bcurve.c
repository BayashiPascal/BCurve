// ============ BCURVE.C ================

// ================= Include =================

#include "bcurve.h"

// ================= Define ==================

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
  // If we couldn't allocate memory
  if (v == NULL)
    return NULL;
  // Declare a variable for calcul
  float *val = (float*)malloc(sizeof(float) * (that->_order + 1));
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
VecFloat* BCurveGetWeightCtrlPt(BCurve *curve, float t) {
  // Check arguments
  if (curve == NULL || t < 0.0 || t > 1.0)
    return NULL;
  // Declare a variable to memorize the result
  VecFloat *res = VecFloatCreate(curve->_order + 1);
  // If we could allocate memory
  if (res != NULL) {
    // Initilize the two first weights
    VecSet(res, 0, 1.0 - t);
    VecSet(res, 1, t);
    // For each higher order
    for (int order = 1; order < curve->_order; ++order) {
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

