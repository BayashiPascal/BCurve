#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "pberr.h"
#include "bcurve.h"

#define RANDOMSEED 0

void UnitTestBCurveCreateCloneFree() {
  int order = 3;
  int dim = 2;
  BCurve *curve = BCurveCreate(order, dim);
  if (curve->_dim != dim || curve->_order != order){
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BCurveCreate failed");
    PBErrCatch(BCurveErr);
  }
  VecFloat *v = VecFloatCreate(dim);
  for (int iCtrl = order + 1; iCtrl--;) {
    if (VecIsEqual(curve->_ctrl[iCtrl], v) == false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "BCurveCreate failed");
      PBErrCatch(BCurveErr);
    }
  }
  for (int iCtrl = order + 1; iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(v, iDim, iCtrl * dim + iDim);
    BCurveSetCtrl(curve, iCtrl, v);
  }
  BCurve *clone= BCurveClone(curve);
  if (clone->_dim != dim || clone->_order != order){
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BCurveClone failed");
    PBErrCatch(BCurveErr);
  }
  for (int iCtrl = order + 1; iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(v, iDim, iCtrl * dim + iDim);
    if (VecIsEqual(clone->_ctrl[iCtrl], v) == false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "BCurveClone failed");
      PBErrCatch(BCurveErr);
    }
  }
  BCurveFree(&curve);
  if (curve != NULL) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BCurveFree failed");
    PBErrCatch(BCurveErr);
  }
  BCurveFree(&clone);
  VecFree(&v);
  printf("UnitTestBCurveCreateCloneFree OK\n");
}

void UnitTestBCurveLoadSavePrint() {
  int order = 3;
  int dim = 2;
  BCurve *curve = BCurveCreate(order, dim);
  VecFloat *v = VecFloatCreate(dim);
  for (int iCtrl = order + 1; iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(v, iDim, iCtrl * dim + iDim);
    BCurveSetCtrl(curve, iCtrl, v);
  }
  BCurvePrint(curve, stdout);
  printf("\n");
  FILE *file = fopen("./bcurve.txt", "w");
  if (BCurveSave(curve, file) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BCurveSave failed");
    PBErrCatch(BCurveErr);
  }
  BCurve *load = BCurveCreate(order, dim);
  fclose(file);
  file = fopen("./bcurve.txt", "r");
  if (BCurveLoad(&load, file) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BCurveLoad failed");
    PBErrCatch(BCurveErr);
  }
  fclose(file);
  if (load->_dim != dim || load->_order != order) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BCurveLoad failed");
    PBErrCatch(BCurveErr);
  }
  for (int iCtrl = order + 1; iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(v, iDim, iCtrl * dim + iDim);
    if (VecIsEqual(load->_ctrl[iCtrl], v) == false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "BCurveLoad failed");
      PBErrCatch(BCurveErr);
    }
  }
  BCurveFree(&curve);
  BCurveFree(&load);
  VecFree(&v);
  printf("UnitTestBCurveLoadSavePrint OK\n");
}

void UnitTestBCurveGetSetCtrl() {
  int order = 3;
  int dim = 2;
  BCurve *curve = BCurveCreate(order, dim);
  VecFloat *v = VecFloatCreate(dim);
  for (int iCtrl = order + 1; iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(v, iDim, iCtrl * dim + iDim);
    BCurveSetCtrl(curve, iCtrl, v);
    if (VecIsEqual(curve->_ctrl[iCtrl], v) == false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "BCurveSetCtrl failed");
      PBErrCatch(BCurveErr);
    }
    VecFloat *w = BCurveGetCtrl(curve, iCtrl);
    if (VecIsEqual(w, v) == false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "BCurveGetCtrl failed");
      PBErrCatch(BCurveErr);
    }
    VecFree(&w);
    if (VecIsEqual(BCurveCtrl(curve, iCtrl), v) == false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "BCurveCtrl failed");
      PBErrCatch(BCurveErr);
    }
  }
  BCurveFree(&curve);
  VecFree(&v);
  printf("UnitTestBCurveGetSetCtrl OK\n");
}

void UnitTestBCurveGet() {
  int order = 3;
  int dim = 2;
  BCurve *curve = BCurveCreate(order, dim);
  VecFloat *v = VecFloatCreate(dim);
  for (int iCtrl = order + 1; iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(v, iDim, iCtrl * dim + iDim);
    BCurveSetCtrl(curve, iCtrl, v);
  }
  for (float u = 0.0; u < 1.0 + PBMATH_EPSILON; u += 0.1) {
    VecFloat *w = BCurveGet(curve, u);
    if (ISEQUALF(VecGet(w, 0), u * 6.0) == false ||
      ISEQUALF(VecGet(w, 1), u * 6.0 + 1.0) == false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "BCurveGet failed");
      PBErrCatch(BCurveErr);
    }
    VecFree(&w);
  }
  BCurveFree(&curve);
  VecFree(&v);
  printf("UnitTestBCurveGet OK\n");
}

void UnitTestBCurveGetOrderDim() {
  int order = 3;
  int dim = 2;
  BCurve *curve = BCurveCreate(order, dim);
  if (BCurveGetOrder(curve) != order) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BCurveGetOrder failed");
    PBErrCatch(BCurveErr);
  }
  if (BCurveGetDim(curve) != dim) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BCurveGetDim failed");
    PBErrCatch(BCurveErr);
  }
  BCurveFree(&curve);
  printf("UnitTestBCurveGetOrderDim OK\n");
}

void UnitTestBCurveGetApproxLenCenter() {
  int order = 3;
  int dim = 2;
  BCurve *curve = BCurveCreate(order, dim);
  VecFloat *v = VecFloatCreate(dim);
  for (int iCtrl = order + 1; iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(v, iDim, iCtrl * dim + iDim);
    BCurveSetCtrl(curve, iCtrl, v);
  }
  float len = BCurveGetApproxLen(curve);
  if (ISEQUALF(len, 8.485281) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BCurveGetApproxLen failed");
    PBErrCatch(BCurveErr);
  }
  VecFloat *center = BCurveGetCenter(curve);
  VecSet(v, 0, 3.0);
  VecSet(v, 1, 4.0);
  if (VecIsEqual(v, center) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BCurveGetCenter failed");
    PBErrCatch(BCurveErr);
  }
  VecFree(&center);
  BCurveFree(&curve);
  VecFree(&v);
  printf("UnitTestBCurveGetApproxLenCenter OK\n");
}

void UnitTestBCurveRot() {
  int order = 3;
  int dim = 2;
  BCurve *curve = BCurveCreate(order, dim);
  VecFloat *v = VecFloatCreate(dim);
  for (int iCtrl = order + 1; iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(v, iDim, iCtrl * dim + iDim);
    BCurveSetCtrl(curve, iCtrl, v);
  }
  float theta = PBMATH_HALFPI;
  BCurveRotOrigin(curve, theta);
  float pa[8] = {-1.0, 0.0, -3.0, 2.0, -5.0, 4.0, -7.0, 6.0};
  for (int iCtrl = order + 1; iCtrl--;)
    for (int iDim = dim; iDim--;)
      if (ISEQUALF(VecGet(BCurveCtrl(curve, iCtrl), iDim), 
        pa[iCtrl * dim + iDim]) == false) {
        BCurveErr->_type = PBErrTypeUnitTestFailed;
        sprintf(BCurveErr->_msg, "BCurveRotOrigin failed");
        PBErrCatch(BCurveErr);
      }
  BCurveRotStart(curve, theta);
  float pb[8] = {-1.0, 0.0, -3.0,-2.0, -5.0, -4.0, -7.0, -6.0};
  for (int iCtrl = order + 1; iCtrl--;)
    for (int iDim = dim; iDim--;)
      if (ISEQUALF(VecGet(BCurveCtrl(curve, iCtrl), iDim), 
        pb[iCtrl * dim + iDim]) == false) {
        BCurveErr->_type = PBErrTypeUnitTestFailed;
        sprintf(BCurveErr->_msg, "BCurveRotStart failed");
        PBErrCatch(BCurveErr);
      }
  BCurveRotCenter(curve, theta);
  float pc[8] = {-7.0, 0.0, -5.0, -2.0, -3.0, -4.0, -1.0, -6.0};
  for (int iCtrl = order + 1; iCtrl--;)
    for (int iDim = dim; iDim--;)
      if (ISEQUALF(VecGet(BCurveCtrl(curve, iCtrl), iDim), 
        pc[iCtrl * dim + iDim]) == false) {
        BCurveErr->_type = PBErrTypeUnitTestFailed;
        sprintf(BCurveErr->_msg, "BCurveRotCenter failed");
        PBErrCatch(BCurveErr);
      }
  BCurveFree(&curve);
  VecFree(&v);
  printf("UnitTestBCurveRot OK\n");
}

void UnitTestBCurveScale() {
  int order = 3;
  int dim = 2;
  BCurve *curve = BCurveCreate(order, dim);
  VecFloat *v = VecFloatCreate(dim);
  for (int iCtrl = order + 1; iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(v, iDim, iCtrl * dim + iDim);
    BCurveSetCtrl(curve, iCtrl, v);
  }
  float scale = 2.0;
  BCurveScaleOrigin(curve, scale);
  float pa[8] = {0.0,2.0, 4.0, 6.0, 8.0, 10.0, 12.0, 14.0};
  for (int iCtrl = order + 1; iCtrl--;)
    for (int iDim = dim; iDim--;)
      if (ISEQUALF(VecGet(BCurveCtrl(curve, iCtrl), iDim), 
        pa[iCtrl * dim + iDim]) == false) {
        BCurveErr->_type = PBErrTypeUnitTestFailed;
        sprintf(BCurveErr->_msg, "BCurveScaleOrigin failed");
        PBErrCatch(BCurveErr);
      }
  BCurveScaleStart(curve, scale);
  float pb[8] = {0.0, 2.0, 8.0, 10.0, 16.0, 18.0, 24.0, 26.0};
  for (int iCtrl = order + 1; iCtrl--;)
    for (int iDim = dim; iDim--;)
      if (ISEQUALF(VecGet(BCurveCtrl(curve, iCtrl), iDim), 
        pb[iCtrl * dim + iDim]) == false) {
        BCurveErr->_type = PBErrTypeUnitTestFailed;
        sprintf(BCurveErr->_msg, "BCurveScaleStart failed");
        PBErrCatch(BCurveErr);
      }
  BCurveScaleCenter(curve, scale);
  float pc[8] = {-12.0, -10.0, 4.0, 6.0, 20.0, 22.0, 36.0, 38.0};
  for (int iCtrl = order + 1; iCtrl--;)
    for (int iDim = dim; iDim--;)
      if (ISEQUALF(VecGet(BCurveCtrl(curve, iCtrl), iDim), 
        pc[iCtrl * dim + iDim]) == false) {
        BCurveErr->_type = PBErrTypeUnitTestFailed;
        sprintf(BCurveErr->_msg, "BCurveScaleCenter failed");
        PBErrCatch(BCurveErr);
      }
  BCurveFree(&curve);
  VecFree(&v);
  printf("UnitTestBCurveScale OK\n");
}

void UnitTestBCurveTranslate() {
  int order = 3;
  int dim = 2;
  BCurve *curve = BCurveCreate(order, dim);
  VecFloat *v = VecFloatCreate(dim);
  for (int iCtrl = order + 1; iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(v, iDim, iCtrl * dim + iDim);
    BCurveSetCtrl(curve, iCtrl, v);
  }
  VecSet(v, 0, -1.0);
  VecSet(v, 1, -2.0);
  BCurveTranslate(curve, v);
  for (int iCtrl = order + 1; iCtrl--;) {
    for (int iDim = dim; iDim--;) {
      VecSet(v, iDim, iCtrl * dim + iDim);
      if (ISEQUALF(VecGet(BCurveCtrl(curve, iCtrl), iDim), 
        VecGet(v, iDim) - (float)(iDim + 1)) == false) {
        BCurveErr->_type = PBErrTypeUnitTestFailed;
        sprintf(BCurveErr->_msg, "BCurveTranslate failed");
        PBErrCatch(BCurveErr);
      }
    }
  }
  BCurveFree(&curve);
  VecFree(&v);
  printf("UnitTestBCurveTranslate OK\n");
}

void UnitTestBCurveFromCloudPoint() {
  int order = 2;
  int dim = 2;
  BCurve *curve = BCurveCreate(order, dim);
  VecFloat *vA = VecFloatCreate(dim);
  VecSet(vA, 0, 0.0); VecSet(vA, 1, 0.0);
  BCurveSetCtrl(curve, 0, vA);
  VecFloat *vB = VecFloatCreate(dim);
  VecSet(vB, 0, 0.5); VecSet(vB, 1, 1.0);
  BCurveSetCtrl(curve, 1, vB);
  VecFloat *vC = VecFloatCreate(dim);
  VecSet(vC, 0, 1.0); VecSet(vC, 1, 0.0);
  BCurveSetCtrl(curve, 2, vC);
  GSet *set = GSetCreate();
  VecFree(&vB);
  vB = BCurveGet(curve, 0.5);
  GSetAppend(set, vA);
  GSetAppend(set, vB);
  GSetAppend(set, vC);
  BCurve *cloud = BCurveFromCloudPoint(set);
  if (cloud == NULL) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BCurveFromCloudPoint failed");
    PBErrCatch(BCurveErr);
  }
  for (float u = 0.0; u < 1.0 + PBMATH_EPSILON; u += 0.1) {
    VecFloat *wA = BCurveGet(curve, u);
    VecFloat *wB = BCurveGet(cloud, u);
    if (VecIsEqual(wA, wB) == false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "BCurveFromCloudPoint failed");
      PBErrCatch(BCurveErr);
    }
    VecFree(&wA);
    VecFree(&wB);
  }
  GSetFree(&set);
  BCurveFree(&curve);
  BCurveFree(&cloud);
  VecFree(&vA);
  VecFree(&vB);
  VecFree(&vC);
  printf("UnitTestBCurveFromCloudPoint OK\n");
}

void UnitTestBCurveGetWeightCtrlPt() {
  int order = 2;
  int dim = 2;
  BCurve *curve = BCurveCreate(order, dim);
  VecFloat *vA = VecFloatCreate(dim);
  VecSet(vA, 0, 0.0); VecSet(vA, 1, 0.0);
  BCurveSetCtrl(curve, 0, vA);
  VecFloat *vB = VecFloatCreate(dim);
  VecSet(vB, 0, 0.5); VecSet(vB, 1, 1.0);
  BCurveSetCtrl(curve, 1, vB);
  VecFloat *vC = VecFloatCreate(dim);
  VecSet(vC, 0, 1.0); VecSet(vC, 1, 0.0);
  BCurveSetCtrl(curve, 2, vC);
  float pa[11] = 
    {1.0, 0.81, 0.64, 0.49, 0.36, 0.25, 0.16, 0.09, 0.04, 0.01, 0.0};
  float pb[11] = 
    {0.0, 0.18, 0.32, 0.42, 0.48, 0.5, 0.48, 0.42, 0.32, 0.18, 0.0};
  float pc[11] = 
    {0.0, 0.01, 0.04, 0.09, 0.16, 0.25, 0.36, 0.49, 0.64, 0.81, 1.0};
  int iArr = 0;
  for (float u = 0.0; u < 1.0 + PBMATH_EPSILON; u += 0.1, ++iArr) {
    VecFloat *w = BCurveGetWeightCtrlPt(curve, u);
    if (ISEQUALF(VecGet(w, 0), pa[iArr]) == false ||
      ISEQUALF(VecGet(w, 1), pb[iArr]) == false ||
      ISEQUALF(VecGet(w, 2), pc[iArr]) == false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "BCurveGetWeightCtrlPt failed");
      PBErrCatch(BCurveErr);
    }
    VecFree(&w);
  }
  BCurveFree(&curve);
  VecFree(&vA);
  VecFree(&vB);
  VecFree(&vC);
  printf("UnitTestBCurveGetWeightCtrlPt OK\n");
}

void UnitTestBCurveGetBoundingBox() {
  int order = 3;
  int dim = 2;
  BCurve *curve = BCurveCreate(order, dim);
  VecFloat *v = VecFloatCreate(dim);
  VecSet(v, 0, -0.5); VecSet(v, 1, -0.5);
  BCurveSetCtrl(curve, 0, v);
  VecSet(v, 0, 0.0); VecSet(v, 1, 1.0);
  BCurveSetCtrl(curve, 1, v);
  VecSet(v, 0, 1.0); VecSet(v, 1, 1.5);
  BCurveSetCtrl(curve, 2, v);
  VecSet(v, 0, 1.5); VecSet(v, 1, 0.0);
  BCurveSetCtrl(curve, 3, v);
  Facoid *bound = BCurveGetBoundingBox(curve);
  Facoid *check = FacoidCreate(dim);
  float scale = 2.0;
  ShapoidScale(check, scale);
  VecSet(v, 0, -0.5); VecSet(v, 1, -0.5);
  ShapoidTranslate(check, v);
  if (ShapoidIsEqual(bound, check) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BCurveGetBoundingBox failed");
    PBErrCatch(BCurveErr);
  }
  ShapoidFree(&bound);
  ShapoidFree(&check);
  BCurveFree(&curve);
  VecFree(&v);
  printf("UnitTestBCurveGetBoundingBox OK\n");
}

void UnitTestBCurve() {
  UnitTestBCurveCreateCloneFree();
  UnitTestBCurveLoadSavePrint();
  UnitTestBCurveGetSetCtrl();
  UnitTestBCurveGet();
  UnitTestBCurveGetOrderDim();
  UnitTestBCurveGetApproxLenCenter();
  UnitTestBCurveRot();
  UnitTestBCurveScale();
  UnitTestBCurveTranslate();
  UnitTestBCurveFromCloudPoint();
  UnitTestBCurveGetWeightCtrlPt();
  UnitTestBCurveGetBoundingBox();
  printf("UnitTestBCurve OK\n");
}

void UnitTestSCurveCreateCloneFree() {
  int order = 3;
  int dim = 2;
  int nbSeg = 3;
  SCurve *curve = SCurveCreate(order, dim, nbSeg);
  if (curve->_dim != dim || curve->_order != order || 
    curve->_nbSeg != nbSeg || 
    GSetNbElem(&(curve->_ctrl)) != 1 + order * nbSeg){
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveCreate failed");
    PBErrCatch(BCurveErr);
  }
  VecFloat *v = VecFloatCreate(dim);
  GSetIterForward iter = GSetIterForwardCreateStatic(&(curve->_ctrl));
  do {
    VecFloat *ctrl = GSetIterGet(&iter);
    if (VecIsEqual(ctrl, v) == false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "SCurveCreate failed");
      PBErrCatch(BCurveErr);
    }
  } while (GSetIterStep(&iter));
  iter = GSetIterForwardCreateStatic(&(curve->_seg));
  VecFloat *prevCtrl = (VecFloat*)(curve->_ctrl._head->_data);
  do {
    BCurve *seg = GSetIterGet(&iter);
    if (seg->_ctrl[0] != prevCtrl) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "SCurveCreate failed");
      PBErrCatch(BCurveErr);
    }
    prevCtrl = seg->_ctrl[order];
  } while (GSetIterStep(&iter));
  iter = GSetIterForwardCreateStatic(&(curve->_ctrl));
  int iCtrl = 0;
  do {
    VecFloat *ctrl = GSetIterGet(&iter);
    for (int iDim = dim; iDim--;)
      VecSet(ctrl, iDim, iCtrl * dim + iDim);
    ++iCtrl;
  } while (GSetIterStep(&iter));
  SCurve *clone= SCurveClone(curve);
  if (clone->_dim != dim || clone->_order != order || 
    clone->_nbSeg != nbSeg){
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveClone failed");
    PBErrCatch(BCurveErr);
  }
  iter = GSetIterForwardCreateStatic(&(curve->_ctrl));
  GSetIterForward iterClone = 
    GSetIterForwardCreateStatic(&(clone->_ctrl));
  do {
    VecFloat *ctrl = GSetIterGet(&iter);
    VecFloat *ctrlClone = GSetIterGet(&iterClone);
    if (VecIsEqual(ctrl, ctrlClone) == false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "SCurveClone failed");
      PBErrCatch(BCurveErr);
    }
  } while (GSetIterStep(&iter) && GSetIterStep(&iterClone));
  SCurveFree(&curve);
  if (curve != NULL) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveFree failed");
    PBErrCatch(BCurveErr);
  }
  SCurveFree(&clone);
  VecFree(&v);
  printf("UnitTestSCurveCreateCloneFree OK\n");
}

void UnitTestSCurveLoadSavePrint() {
  int order = 3;
  int dim = 2;
  int nbSeg = 3;
  SCurve *curve = SCurveCreate(order, dim, nbSeg);
  GSetIterForward iter = GSetIterForwardCreateStatic(&(curve->_ctrl));
  int iCtrl = 0;
  do {
    VecFloat *ctrl = GSetIterGet(&iter);
    for (int iDim = dim; iDim--;)
      VecSet(ctrl, iDim, iCtrl * dim + iDim);
    ++iCtrl;
  } while (GSetIterStep(&iter));
  SCurvePrint(curve, stdout);
  printf("\n");
  FILE *file = fopen("./scurve.txt", "w");
  if (SCurveSave(curve, file) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveSave failed");
    PBErrCatch(BCurveErr);
  }
  SCurve *load = SCurveCreate(order, dim, nbSeg);
  fclose(file);
  file = fopen("./scurve.txt", "r");
  if (SCurveLoad(&load, file) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveLoad failed");
    PBErrCatch(BCurveErr);
  }
  fclose(file);
  if (load->_dim != dim || load->_order != order || 
    load->_order != order) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveLoad failed");
    PBErrCatch(BCurveErr);
  }
  iter = GSetIterForwardCreateStatic(&(curve->_ctrl));
  GSetIterForward iterLoad = 
    GSetIterForwardCreateStatic(&(load->_ctrl));
  do {
    VecFloat *ctrl = GSetIterGet(&iter);
    VecFloat *ctrlLoad = GSetIterGet(&iterLoad);
    if (VecIsEqual(ctrl, ctrlLoad) == false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "SCurveLoad failed");
      PBErrCatch(BCurveErr);
    }
  } while (GSetIterStep(&iter) && GSetIterStep(&iterLoad));
  SCurveFree(&curve);
  SCurveFree(&load);
  printf("UnitTestSCurveLoadSavePrint OK\n");
}

void UnitTestSCurveGetSetCtrl() {
  int order = 3;
  int dim = 2;
  int nbSeg = 3;
  SCurve *curve = SCurveCreate(order, dim, nbSeg);
  VecFloat *v = VecFloatCreate(dim);
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(v, iDim, iCtrl * dim + iDim);
    SCurveSetCtrl(curve, iCtrl, v);
  }
  GSetIterForward iter = GSetIterForwardCreateStatic(&(curve->_ctrl));
  int iCtrl = 0;
  do {
    for (int iDim = dim; iDim--;)
      VecSet(v, iDim, iCtrl * dim + iDim);
    VecFloat *ctrl = GSetIterGet(&iter);
    if (VecIsEqual(ctrl, v) == false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "SCurveSetCtrl failed");
      PBErrCatch(BCurveErr);
    }
    if (ctrl != SCurveCtrl(curve, iCtrl)) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "SCurveCtrl failed");
      PBErrCatch(BCurveErr);
    }
    ctrl = SCurveGetCtrl(curve, iCtrl);
    if (VecIsEqual(ctrl, v) == false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "SCurveGetCtrl failed");
      PBErrCatch(BCurveErr);
    }
    VecFree(&ctrl);
    ++iCtrl;
  } while (GSetIterStep(&iter));
  VecFree(&v);
  SCurveFree(&curve);
  printf("UnitTestSCurveGetSetCtrl OK\n");
}

void UnitTestSCurveGetAddRemoveSeg() {
  int order = 3;
  int dim = 2;
  int nbSeg = 3;
  SCurve *curve = SCurveCreate(order, dim, nbSeg);
  VecFloat *v = VecFloatCreate(dim);
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(v, iDim, iCtrl * dim + iDim);
    SCurveSetCtrl(curve, iCtrl, v);
  }
  for (int iSeg = SCurveGetNbSeg(curve); iSeg--;) {
    BCurve *seg = SCurveGetSeg(curve, iSeg);
    if (BCurveGetDim(seg) != dim || BCurveGetOrder(seg) != order) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "SCurveGetSeg failed");
      PBErrCatch(BCurveErr);
    }
    for (int iCtrl = order + 1; iCtrl--;) {
      int jCtrl = iSeg * order + iCtrl;
      if (VecIsEqual(BCurveCtrl(seg, iCtrl), 
        SCurveCtrl(curve, jCtrl)) == false) {
        BCurveErr->_type = PBErrTypeUnitTestFailed;
        sprintf(BCurveErr->_msg, "SCurveGetSeg failed");
        PBErrCatch(BCurveErr);
      }
      if (BCurveCtrl(SCurveSeg(curve, iSeg), iCtrl) != 
        SCurveCtrl(curve, jCtrl)) {
        BCurveErr->_type = PBErrTypeUnitTestFailed;
        sprintf(BCurveErr->_msg, "SCurveSeg failed");
        PBErrCatch(BCurveErr);
      }
    }
    BCurveFree(&seg);
  }
  SCurveAddSegHead(curve);
  SCurveAddSegTail(curve);
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(v, iDim, iCtrl * dim + iDim);
    SCurveSetCtrl(curve, iCtrl, v);
  }
  for (int iSeg = SCurveGetNbSeg(curve); iSeg--;) {
    BCurve *seg = SCurveGetSeg(curve, iSeg);
    if (BCurveGetDim(seg) != dim || BCurveGetOrder(seg) != order) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "SCurveGetSeg failed1");
      PBErrCatch(BCurveErr);
    }
    for (int iCtrl = order + 1; iCtrl--;) {
      int jCtrl = iSeg * order + iCtrl;
      if (VecIsEqual(BCurveCtrl(seg, iCtrl), 
        SCurveCtrl(curve, jCtrl)) == false) {
        BCurveErr->_type = PBErrTypeUnitTestFailed;
        sprintf(BCurveErr->_msg, "SCurveGetSeg failed2");
        PBErrCatch(BCurveErr);
      }
      if (BCurveCtrl(SCurveSeg(curve, iSeg), iCtrl) != 
        SCurveCtrl(curve, jCtrl)) {
        BCurveErr->_type = PBErrTypeUnitTestFailed;
        sprintf(BCurveErr->_msg, "SCurveSeg failed");
        PBErrCatch(BCurveErr);
      }
    }
    BCurveFree(&seg);
  }
  SCurveRemoveHeadSeg(curve);
  SCurveRemoveTailSeg(curve);
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(v, iDim, iCtrl * dim + iDim);
    SCurveSetCtrl(curve, iCtrl, v);
  }
  for (int iSeg = SCurveGetNbSeg(curve); iSeg--;) {
    BCurve *seg = SCurveGetSeg(curve, iSeg);
    if (BCurveGetDim(seg) != dim || BCurveGetOrder(seg) != order) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "SCurveGetSeg failed");
      PBErrCatch(BCurveErr);
    }
    for (int iCtrl = order + 1; iCtrl--;) {
      int jCtrl = iSeg * order + iCtrl;
      if (VecIsEqual(BCurveCtrl(seg, iCtrl), 
        SCurveCtrl(curve, jCtrl)) == false) {
        BCurveErr->_type = PBErrTypeUnitTestFailed;
        sprintf(BCurveErr->_msg, "SCurveGetSeg failed");
        PBErrCatch(BCurveErr);
      }
      if (BCurveCtrl(SCurveSeg(curve, iSeg), iCtrl) != 
        SCurveCtrl(curve, jCtrl)) {
        BCurveErr->_type = PBErrTypeUnitTestFailed;
        sprintf(BCurveErr->_msg, "SCurveSeg failed");
        PBErrCatch(BCurveErr);
      }
    }
    BCurveFree(&seg);
  }
  VecFree(&v);
  SCurveFree(&curve);
  printf("UnitTestSCurveGetAddRemoveSeg OK\n");
}

void UnitTestSCurveGet() {
  int order = 3;
  int dim = 2;
  int nbSeg = 3;
  SCurve *curve = SCurveCreate(order, dim, nbSeg);
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(SCurveCtrl(curve, iCtrl), iDim, iCtrl * dim + iDim);
  }
  for (float u = 0.0; u < SCurveGetMaxU(curve) + PBMATH_EPSILON; 
    u += 0.1) {
    VecFloat *v = SCurveGet(curve, u);
    if (ISEQUALF(VecGet(v, 0), u * 6.0) == false || 
      ISEQUALF(VecGet(v, 1), 1.0 + u * 6.0) == false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "SCurveGet failed");
      PBErrCatch(BCurveErr);
    }
    VecFree(&v);
  }
  SCurveFree(&curve);
  printf("UnitTestSCurveGet OK\n");
}

void UnitTestSCurveGetOrderDimNbSegMaxUNbCtrl() {
  int order = 3;
  int dim = 2;
  int nbSeg = 3;
  SCurve *curve = SCurveCreate(order, dim, nbSeg);
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(SCurveCtrl(curve, iCtrl), iDim, iCtrl * dim + iDim);
  }
  if (SCurveGetOrder(curve) != order) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveGetOrder failed");
    PBErrCatch(BCurveErr);
  }
  if (SCurveGetDim(curve) != dim) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveGetDim failed");
    PBErrCatch(BCurveErr);
  }
  if (SCurveGetNbSeg(curve) != nbSeg) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveGetNbSeg failed");
    PBErrCatch(BCurveErr);
  }
  if (ISEQUALF(SCurveGetMaxU(curve), (float)(curve->_nbSeg)) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveGetMaxU failed");
    PBErrCatch(BCurveErr);
  }
  if (SCurveGetNbCtrl(curve) != nbSeg * order + 1) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveGetNbCtrl failed");
    PBErrCatch(BCurveErr);
  }
  SCurveFree(&curve);
  printf("UnitTestSCurveGetOrderDimNbSegMaxUNbCtrl OK\n");
}

void UnitTestSCurveGetApproxLenCenter() {
  int order = 3;
  int dim = 2;
  int nbSeg = 3;
  SCurve *curve = SCurveCreate(order, dim, nbSeg);
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(SCurveCtrl(curve, iCtrl), iDim, iCtrl * dim + iDim);
  }
  VecFloat *center = SCurveGetCenter(curve);
  VecFloat *check = VecFloatCreate(dim);
  VecSet(check, 0, 9.0);
  VecSet(check, 1, 10.0);
  if (VecIsEqual(center, check) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveGetCenter failed");
    PBErrCatch(BCurveErr);
  }
  VecFree(&check);
  VecFree(&center);
  float len = 25.455843;
  if (ISEQUALF(SCurveGetApproxLen(curve), len) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveGetApproxLen failed");
    PBErrCatch(BCurveErr);
  }
  SCurveFree(&curve);
  printf("UnitTestSCurveGetApproxLenCenter OK\n");
}

void UnitTestSCurveRot() {
  int order = 3;
  int dim = 2;
  int nbSeg = 3;
  SCurve *curve = SCurveCreate(order, dim, nbSeg);
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(SCurveCtrl(curve, iCtrl), iDim, iCtrl * dim + iDim);
  }
  float theta = PBMATH_HALFPI;
  SCurveRotStart(curve, theta);
  float pa[20] = {0.0, 1.0, -2.0, 3.0, -4.0, 5.0, -6.0, 7.0, -8.0, 9.0,
      -10.0, 11.0, -12.0, 13.0, -14.0, 15.0, -16.0, 17.0, -18.0, 19.0};
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    if (ISEQUALF(VecGet(SCurveCtrl(curve, iCtrl), 0), 
      pa[iCtrl * 2]) == false ||
      ISEQUALF(VecGet(SCurveCtrl(curve, iCtrl), 1), 
      pa[iCtrl * 2 + 1]) == false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "SCurveRotStart failed");
      PBErrCatch(BCurveErr);
    }
  }
  SCurveRotOrigin(curve, theta);
  float pb[20] = {-1.0, 0.0, -3.0, -2.0, -5.0, -4.0, -7.0, -6.0, -9.0, 
      -8.0, -11.0, -10.0, -13.0, -12.0, -15.0, -14.0, -17.0, -16.0, 
      -19.0, -18.0};
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    if (ISEQUALF(VecGet(SCurveCtrl(curve, iCtrl), 0), 
      pb[iCtrl * 2]) == false ||
      ISEQUALF(VecGet(SCurveCtrl(curve, iCtrl), 1), 
      pb[iCtrl * 2 + 1]) == false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "SCurveRotOrigin failed");
      PBErrCatch(BCurveErr);
    }
  }
  SCurveRotCenter(curve, theta);
  float pc[20] = {-19.0, 0.0, -17.0, -2.0, -15.0, -4.0, -13.0, -6.0, 
      -11.0, -8.0, -9.0, -10.0, -7.0, -12.0, -5.0, -14.0, -3.0, -16.0,
      -1.0, -18.0};
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    if (ISEQUALF(VecGet(SCurveCtrl(curve, iCtrl), 0), 
      pc[iCtrl * 2]) == false ||
      ISEQUALF(VecGet(SCurveCtrl(curve, iCtrl), 1), 
      pc[iCtrl * 2 + 1]) == false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "SCurveRotCenter failed");
      PBErrCatch(BCurveErr);
    }
  }
  SCurveFree(&curve);
  printf("UnitTestSCurveRot OK\n");
}

void UnitTestSCurveScale() {
  int order = 3;
  int dim = 2;
  int nbSeg = 3;
  SCurve *curve = SCurveCreate(order, dim, nbSeg);
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(SCurveCtrl(curve, iCtrl), iDim, iCtrl * dim + iDim);
  }
  float scale = 2.0;
  SCurveScaleStart(curve, scale);
  float pa[20] = {0.0, 1.0, 4.0, 5.0, 8.0, 9.0, 12.0, 13.0, 16.0, 17.0, 
    20.0, 21.0, 24.0, 25.0, 28.0, 29.0, 32.0, 33.0, 36.0, 37.0};
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    if (ISEQUALF(VecGet(SCurveCtrl(curve, iCtrl), 0), 
      pa[iCtrl * 2]) == false ||
      ISEQUALF(VecGet(SCurveCtrl(curve, iCtrl), 1), 
      pa[iCtrl * 2 + 1]) == false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "SCurveScaleStart failed");
      PBErrCatch(BCurveErr);
    }
  }
  SCurveScaleOrigin(curve, scale);
  float pb[20] = {0.0, 2.0, 8.0, 10.0, 16.0, 18.0, 24.0, 26.0, 32.0, 
      34.0, 40.0, 42.0, 48.0, 50.0, 56.0, 58.0, 64.0, 66.0, 72.0, 74.0};
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    if (ISEQUALF(VecGet(SCurveCtrl(curve, iCtrl), 0), 
      pb[iCtrl * 2]) == false ||
      ISEQUALF(VecGet(SCurveCtrl(curve, iCtrl), 1), 
      pb[iCtrl * 2 + 1]) == false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "SCurveScaleOrigin failed");
      PBErrCatch(BCurveErr);
    }
  }
  SCurveScaleCenter(curve, scale);
  float pc[20] = {-36.0, -34.0, -20.0, -18.0, -4.0, -2.0, 12.0, 14.0, 
      28.0, 30.0, 44.0, 46.0, 60.0, 62.0, 76.0, 78.0, 92.0, 94.0, 
      108.0, 110.0};
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    if (ISEQUALF(VecGet(SCurveCtrl(curve, iCtrl), 0), 
      pc[iCtrl * 2]) == false ||
      ISEQUALF(VecGet(SCurveCtrl(curve, iCtrl), 1), 
      pc[iCtrl * 2 + 1]) == false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "SCurveScaleCenter failed");
      PBErrCatch(BCurveErr);
    }
  }
  SCurveFree(&curve);
  curve = SCurveCreate(order, dim, nbSeg);
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(SCurveCtrl(curve, iCtrl), iDim, iCtrl * dim + iDim);
  }
  VecFloat *v = VecFloatCreate(dim);
  VecSet(v, 0, 2.0);
  VecSet(v, 1, -1.0);
  SCurveScaleStart(curve, v);
  float pd[20] = {0.0, 1.0, 4.0, -1.0, 8.0, -3.0, 12.0, -5.0, 16.0, 
      -7.0, 20.0, -9.0, 24.0, -11.0, 28.0, -13.0, 32.0, -15.0, 36.0, 
      -17.0};
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    if (ISEQUALF(VecGet(SCurveCtrl(curve, iCtrl), 0), 
      pd[iCtrl * 2]) == false ||
      ISEQUALF(VecGet(SCurveCtrl(curve, iCtrl), 1), 
      pd[iCtrl * 2 + 1]) == false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "SCurveScaleStart failed");
      PBErrCatch(BCurveErr);
    }
  }
  SCurveScaleOrigin(curve, v);
  float pe[20] = {0.0, -1.0, 8.0, 1.0, 16.0, 3.0, 24.0, 5.0, 32.0, 
      7.0, 40.0, 9.0, 48.0, 11.0, 56.0, 13.0, 64.0, 15.0, 72.0, 17.0};
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    if (ISEQUALF(VecGet(SCurveCtrl(curve, iCtrl), 0), 
      pe[iCtrl * 2]) == false ||
      ISEQUALF(VecGet(SCurveCtrl(curve, iCtrl), 1), 
      pe[iCtrl * 2 + 1]) == false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "SCurveScaleOrigin failed");
      PBErrCatch(BCurveErr);
    }
  }
  SCurveScaleCenter(curve, v);
  float pf[20] = {-36.0, 17.0, -20.0, 15.0, -4.0, 13.0, 12.0, 11.0, 
      28.0, 9.0, 44.0, 7.0, 60.0, 5.0, 76.0, 3.0, 92.0, 1.0, 108.0, 
      -1.0};
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    if (ISEQUALF(VecGet(SCurveCtrl(curve, iCtrl), 0), 
      pf[iCtrl * 2]) == false ||
      ISEQUALF(VecGet(SCurveCtrl(curve, iCtrl), 1), 
      pf[iCtrl * 2 + 1]) == false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "SCurveScaleCenter failed");
      PBErrCatch(BCurveErr);
    }
  }
  SCurveFree(&curve);
  VecFree(&v);
  printf("UnitTestSCurveScale OK\n");
}

void UnitTestSCurveTranslate() {
  int order = 3;
  int dim = 2;
  int nbSeg = 3;
  SCurve *curve = SCurveCreate(order, dim, nbSeg);
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(SCurveCtrl(curve, iCtrl), iDim, iCtrl * dim + iDim);
  }
  VecFloat *v = VecFloatCreate(dim);
  VecSet(v, 0, -1.0);
  VecSet(v, 1, 2.0);
  SCurveTranslate(curve, v);
  float p[20] = {-1.0, 3.0, 1.0, 5.0, 3.0, 7.0, 5.0, 9.0, 7.0, 11.0, 
    9.0, 13.0, 11.0, 15.0, 13.0, 17.0, 15.0, 19.0, 17.0, 21.0};
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    if (ISEQUALF(VecGet(SCurveCtrl(curve, iCtrl), 0), 
      p[iCtrl * 2]) == false ||
      ISEQUALF(VecGet(SCurveCtrl(curve, iCtrl), 1), 
      p[iCtrl * 2 + 1]) == false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "SCurveTranslate failed");
      PBErrCatch(BCurveErr);
    }
  }
  SCurveFree(&curve);
  VecFree(&v);
  printf("UnitTestSCurveTranslate OK\n");
}

void UnitTestSCurveGetBoundingBox() {
  int order = 3;
  int dim = 2;
  int nbSeg = 3;
  SCurve *curve = SCurveCreate(order, dim, nbSeg);
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    VecSet(SCurveCtrl(curve, iCtrl), 0, 
      cos(PBMATH_QUARTERPI * (float)iCtrl * 0.5));
    VecSet(SCurveCtrl(curve, iCtrl), 1, 
      sin(PBMATH_QUARTERPI * (float)iCtrl * 0.5));
  }
  Facoid *bound = SCurveGetBoundingBox(curve);
  if (ISEQUALF(VecGet(ShapoidPos(bound), 0), -1.0) == false ||
    ISEQUALF(VecGet(ShapoidPos(bound), 1), -0.382683) == false ||
    ISEQUALF(VecGet(ShapoidAxis(bound, 0), 0), 2.0) == false ||
    ISEQUALF(VecGet(ShapoidAxis(bound, 0), 1), 0.0) == false ||
    ISEQUALF(VecGet(ShapoidAxis(bound, 1), 0), 0.0) == false ||
    ISEQUALF(VecGet(ShapoidAxis(bound, 1), 1), 1.382683) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveGetBoundingBox failed");
    PBErrCatch(BCurveErr);
  }
  ShapoidFree(&bound);
  SCurveFree(&curve);
  printf("UnitTestSCurveGetBoundingBox OK\n");
}

void UnitTestSCurve() {
  UnitTestSCurveCreateCloneFree();
  UnitTestSCurveLoadSavePrint();
  UnitTestSCurveGetSetCtrl();
  UnitTestSCurveGetAddRemoveSeg();
  UnitTestSCurveGet();
  UnitTestSCurveGetOrderDimNbSegMaxUNbCtrl();
  UnitTestSCurveGetApproxLenCenter();
  UnitTestSCurveRot();
  UnitTestSCurveScale();
  UnitTestSCurveTranslate();
  UnitTestSCurveGetBoundingBox();
  printf("UnitTestSCurve OK\n");
}

void UnitTestAll() {
  UnitTestBCurve();
  UnitTestSCurve();
  printf("UnitTestAll OK\n");
}

int main() {
  UnitTestAll();
  // Return success code
  return 0;
}

