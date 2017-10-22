#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "bcurve.h"

float CloudCurveX(float t) {
  return t * t;
  //return 2.0 * cos(t * PBMATH_HALFPI);
}

float CloudCurveY(float t) {
  return sqrt(t);
  //return sin(t * PBMATH_HALFPI);
}

int main(int argc, char **argv) {
  // Create a BCurve
  int order = 3;
  int dim = 2;
  BCurve *curve = BCurveCreate(order, dim);
  // If we couldn't create the BCurve
  if (curve == NULL) {
    // Print a message
    fprintf(stderr, "BCurveCreate failed\n");
    // Stop here
    return 1;
  }
  // Print the BCurve
  BCurvePrint(curve, stdout);
  fprintf(stdout, "\n");
  // Create a VecFloat to set the values
  VecFloat *v = VecFloatCreate(dim);
  // If we couldn't create the VecFloat
  if (v == NULL) {
    // Release memory
    BCurveFree(&curve);
    // Stop here
    return 2;
  }
  // Set the control points
  float ctrlPts[8] = {0.0, 1.0, 2.0, 5.0, 4.0, 3.0, 6.0, 7.0};
  for (int iCtrl = 0; iCtrl < order + 1; ++iCtrl) {
    VecSet(v, 0, ctrlPts[2 * iCtrl]);
    VecSet(v, 1, ctrlPts[2 * iCtrl + 1]);
    BCurveSet(curve, iCtrl, v);
  }
  // Print the BCurve
  BCurvePrint(curve, stdout);
  fprintf(stdout, "\n");
  // Save the curve
  FILE *file = fopen("./curve.txt", "w");
  // If we couldn't open the file
  if (file == NULL) {
    // Print a message
    fprintf(stderr, "Can't open file\n");
    // Free memory
    VecFree(&v);
    BCurveFree(&curve);
    // Stop here
    return 3;
  }
  int ret = BCurveSave(curve, file);
  // If we couldn't save
  if (ret != 0) {
    // Print a message
    fprintf(stderr, "BCurveSave failed (%d)\n", ret);
    // Free memory
    VecFree(&v);
    BCurveFree(&curve);
    // Stop here
    return 4;
  }
  fclose(file);
  // Load the curve
  file = fopen("./curve.txt", "r");
  // If we couldn't open the file
  if (file == NULL) {
    // Print a message
    fprintf(stderr, "Can't open file\n");
    // Free memory
    VecFree(&v);
    BCurveFree(&curve);
    // Stop here
    return 5;
  }
  BCurve *loaded = NULL;
  ret = BCurveLoad(&loaded, file);
  // If we couldn't load
  if (ret != 0) {
    // Print a message
    fprintf(stderr, "BCurveLoad failed (%d)\n", ret);
    // Free memory
    VecFree(&v);
    BCurveFree(&curve);
    BCurveFree(&loaded);
    // Stop here
    return 6;
  }
  fclose(file);
  // Print the loaded curve
  BCurvePrint(loaded, stdout);
  fprintf(stdout, "\n");
  // Get some values of the curve
  for (float u = 0.0; u <= 1.01; u += 0.1) {
    VecFloat *w = BCurveGet(curve, u);
    // If we couldn't get the values
    if (w == NULL) {
      // Free memory
      VecFree(&v);
      BCurveFree(&curve);
      BCurveFree(&loaded);
      // Stop here 
      return 7;
    }
    fprintf(stdout, "%.1f: ", u);
    VecPrint(w, stdout);
    fprintf(stdout, "\n");
    VecFree(&w);
  }
  // Scale the curve
  VecSet(v, 0, 0.5);
  VecSet(v, 1, 1.0);
  BCurveScale(curve, v);
  // Rotate the curve
  BCurveRot2D(curve, PBMATH_PI * 0.5);
  // Translate the curve
  VecSet(v, 0, -0.5);
  VecSet(v, 1, 1.0);
  BCurveTranslate(curve, v);
  // Get some values of the curve
  fprintf(stdout, "After transformation:\n");
  for (float u = 0.0; u <= 1.01; u += 0.1) {
    VecFloat *w = BCurveGet(curve, u);
    // If we couldn't get the values
    if (w == NULL) {
      // Free memory
      VecFree(&v);
      BCurveFree(&curve);
      BCurveFree(&loaded);
      // Stop here 
      return 7;
    }
    fprintf(stdout, "%.1f: ", u);
    VecPrint(w, stdout);
    fprintf(stdout, "\n");
    VecFree(&w);
  }  
  // Print the curve approximate length
  fprintf(stdout, "approx length: %.3f\n", BCurveApproxLen(curve));
  // Print the weight of control points
  fprintf(stdout, "Control points weight:\n");
  for (float t = 0.0; t <= 1.01; t += 0.05) {
    if (t > 1.0) t = 1.0;
    VecFloat *w = BCurveGetWeightCtrlPt(curve, t);
    if (w != NULL) {
      fprintf(stdout, "%.3f ", t);
      VecPrint(w, stdout);
      fprintf(stdout, "\n");
    }
    VecFree(&w);
  }
  // Get a curve from a cloud point
  GSet *cloud = GSetCreate();
  if (cloud != NULL) {
    VecFloat *w = NULL;
    fprintf(stdout, "cloud:\n");
    //for (float t = 0.0; t < 1.01; t += 0.25) {
    //for (float t = 0.0; t < 1.01; t += 0.334) {
    for (float t = 0.0; t < 1.01; t += 0.5) {
      w = VecFloatCreate(2);
      GSetAppend(cloud, w);
      VecSet(w, 0, CloudCurveX(t));
      VecSet(w, 1, CloudCurveY(t));
      VecPrint(w, stdout);
      fprintf(stdout, "\n");
    }
    w = NULL;
    BCurve *cloudCurve = BCurveFromCloudPoint(cloud);
    if (cloudCurve == NULL) {
      fprintf(stdout, "Couldn't get curve from cloud\n");
      return 8;
    }
    fprintf(stdout, "cloudCurve: ");
    BCurvePrint(cloudCurve, stdout);
    fprintf(stdout, "\n");
    for (float t = 0.0; t < 1.01; t += 0.1) {
      if (t > 1.0) t = 1.0;
      fprintf(stdout, "%.3f ", t);
      w = BCurveGet(cloudCurve, t);
      VecPrint(w, stdout);
      fprintf(stdout, "\n");
      VecFree(&w);
    }
    BCurveFree(&cloudCurve);
  }
  // Get the bounding box of the curve
  Shapoid *bound = BCurveGetBoundingBox(curve);
  if (bound == NULL) {
    fprintf(stdout, "Couldn't get the bounding box\n");
    return 9;
  }
  fprintf(stdout, "bounding box of \n");
  BCurvePrint(curve, stdout);
  fprintf(stdout, "\nis\n");
  ShapoidPrint(bound, stdout);
  ShapoidFree(&bound);
  // Free memory
  GSetElem *elem = cloud->_head;
  while (elem != NULL) {
    VecFree((VecFloat**)(&(elem->_data)));
    elem = elem->_next;
  }
  GSetFree(&cloud);
  VecFree(&v);
  BCurveFree(&curve);
  BCurveFree(&loaded);
  // Return success code
  return 0;
}

