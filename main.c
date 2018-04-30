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
  BCurve* curve = BCurveCreate(order, dim);
  if (curve->_dim != dim || curve->_order != order){
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BCurveCreate failed");
    PBErrCatch(BCurveErr);
  }
  VecFloat* v = VecFloatCreate(dim);
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
  BCurve* clone= BCurveClone(curve);
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
  BCurve* curve = BCurveCreate(order, dim);
  VecFloat* v = VecFloatCreate(dim);
  for (int iCtrl = order + 1; iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(v, iDim, iCtrl * dim + iDim);
    BCurveSetCtrl(curve, iCtrl, v);
  }
  BCurvePrint(curve, stdout);
  printf("\n");
  FILE* file = fopen("./bcurve.txt", "w");
  if (BCurveSave(curve, file) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BCurveSave failed");
    PBErrCatch(BCurveErr);
  }
  BCurve* load = BCurveCreate(order, dim);
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
  BCurve* curve = BCurveCreate(order, dim);
  VecFloat* v = VecFloatCreate(dim);
  for (int iCtrl = order + 1; iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(v, iDim, iCtrl * dim + iDim);
    BCurveSetCtrl(curve, iCtrl, v);
    if (VecIsEqual(curve->_ctrl[iCtrl], v) == false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "BCurveSetCtrl failed");
      PBErrCatch(BCurveErr);
    }
    VecFloat* w = BCurveGetCtrl(curve, iCtrl);
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
  BCurve* curve = BCurveCreate(order, dim);
  VecFloat* v = VecFloatCreate(dim);
  for (int iCtrl = order + 1; iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(v, iDim, iCtrl * dim + iDim);
    BCurveSetCtrl(curve, iCtrl, v);
  }
  for (float u = 0.0; u < 1.0 + PBMATH_EPSILON; u += 0.1) {
    VecFloat* w = BCurveGet(curve, u);
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
  BCurve* curve = BCurveCreate(order, dim);
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
  BCurve* curve = BCurveCreate(order, dim);
  VecFloat* v = VecFloatCreate(dim);
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
  VecFloat* center = BCurveGetCenter(curve);
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
  BCurve* curve = BCurveCreate(order, dim);
  VecFloat* v = VecFloatCreate(dim);
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
  BCurve* curve = BCurveCreate(order, dim);
  VecFloat* v = VecFloatCreate(dim);
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
  BCurve* curve = BCurveCreate(order, dim);
  VecFloat* v = VecFloatCreate(dim);
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
  BCurve* curve = BCurveCreate(order, dim);
  VecFloat* vA = VecFloatCreate(dim);
  VecSet(vA, 0, 0.0); VecSet(vA, 1, 0.0);
  BCurveSetCtrl(curve, 0, vA);
  VecFloat* vB = VecFloatCreate(dim);
  VecSet(vB, 0, 0.5); VecSet(vB, 1, 1.0);
  BCurveSetCtrl(curve, 1, vB);
  VecFloat* vC = VecFloatCreate(dim);
  VecSet(vC, 0, 1.0); VecSet(vC, 1, 0.0);
  BCurveSetCtrl(curve, 2, vC);
  GSetVecFloat* set = GSetVecFloatCreate();
  VecFree(&vB);
  vB = BCurveGet(curve, 0.5);
  GSetAppend(set, vA);
  GSetAppend(set, vB);
  GSetAppend(set, vC);
  BCurve* cloud = BCurveFromCloudPoint(set);
  if (cloud == NULL) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BCurveFromCloudPoint failed");
    PBErrCatch(BCurveErr);
  }
  for (float u = 0.0; u < 1.0 + PBMATH_EPSILON; u += 0.1) {
    VecFloat* wA = BCurveGet(curve, u);
    VecFloat* wB = BCurveGet(cloud, u);
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
  BCurve* curve = BCurveCreate(order, dim);
  VecFloat* vA = VecFloatCreate(dim);
  VecSet(vA, 0, 0.0); VecSet(vA, 1, 0.0);
  BCurveSetCtrl(curve, 0, vA);
  VecFloat* vB = VecFloatCreate(dim);
  VecSet(vB, 0, 0.5); VecSet(vB, 1, 1.0);
  BCurveSetCtrl(curve, 1, vB);
  VecFloat* vC = VecFloatCreate(dim);
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
    VecFloat* w = BCurveGetWeightCtrlPt(curve, u);
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
  BCurve* curve = BCurveCreate(order, dim);
  VecFloat* v = VecFloatCreate(dim);
  VecSet(v, 0, -0.5); VecSet(v, 1, -0.5);
  BCurveSetCtrl(curve, 0, v);
  VecSet(v, 0, 0.0); VecSet(v, 1, 1.0);
  BCurveSetCtrl(curve, 1, v);
  VecSet(v, 0, 1.0); VecSet(v, 1, 1.5);
  BCurveSetCtrl(curve, 2, v);
  VecSet(v, 0, 1.5); VecSet(v, 1, 0.0);
  BCurveSetCtrl(curve, 3, v);
  Facoid* bound = BCurveGetBoundingBox(curve);
  Facoid* check = FacoidCreate(dim);
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
  SCurve* curve = SCurveCreate(order, dim, nbSeg);
  if (curve->_dim != dim || curve->_order != order || 
    curve->_nbSeg != nbSeg || 
    GSetNbElem(&(curve->_ctrl)) != 1 + order * nbSeg){
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveCreate failed");
    PBErrCatch(BCurveErr);
  }
  VecFloat* v = VecFloatCreate(dim);
  GSetIterForward iter = GSetIterForwardCreateStatic(&(curve->_ctrl));
  do {
    VecFloat* ctrl = GSetIterGet(&iter);
    if (VecIsEqual(ctrl, v) == false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "SCurveCreate failed");
      PBErrCatch(BCurveErr);
    }
  } while (GSetIterStep(&iter));
  iter = GSetIterForwardCreateStatic(&(curve->_seg));
  VecFloat* prevCtrl = (VecFloat*)(curve->_ctrl._set._head->_data);
  do {
    BCurve* seg = GSetIterGet(&iter);
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
    VecFloat* ctrl = GSetIterGet(&iter);
    for (int iDim = dim; iDim--;)
      VecSet(ctrl, iDim, iCtrl * dim + iDim);
    ++iCtrl;
  } while (GSetIterStep(&iter));
  SCurve* clone= SCurveClone(curve);
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
    VecFloat* ctrl = GSetIterGet(&iter);
    VecFloat* ctrlClone = GSetIterGet(&iterClone);
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
  SCurve* curve = SCurveCreate(order, dim, nbSeg);
  GSetIterForward iter = GSetIterForwardCreateStatic(&(curve->_ctrl));
  int iCtrl = 0;
  do {
    VecFloat* ctrl = GSetIterGet(&iter);
    for (int iDim = dim; iDim--;)
      VecSet(ctrl, iDim, iCtrl * dim + iDim);
    ++iCtrl;
  } while (GSetIterStep(&iter));
  SCurvePrint(curve, stdout);
  printf("\n");
  FILE* file = fopen("./scurve.txt", "w");
  if (SCurveSave(curve, file) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveSave failed");
    PBErrCatch(BCurveErr);
  }
  SCurve* load = SCurveCreate(order, dim, nbSeg);
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
    VecFloat* ctrl = GSetIterGet(&iter);
    VecFloat* ctrlLoad = GSetIterGet(&iterLoad);
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
  SCurve* curve = SCurveCreate(order, dim, nbSeg);
  VecFloat* v = VecFloatCreate(dim);
  if (SCurveCtrls(curve) != &(curve->_ctrl)) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveCtrls failed");
    PBErrCatch(BCurveErr);
  }
  if (SCurveSegs(curve) != &(curve->_seg)) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveSegs failed");
    PBErrCatch(BCurveErr);
  }
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
    VecFloat* ctrl = GSetIterGet(&iter);
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
  SCurve* curve = SCurveCreate(order, dim, nbSeg);
  VecFloat* v = VecFloatCreate(dim);
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(v, iDim, iCtrl * dim + iDim);
    SCurveSetCtrl(curve, iCtrl, v);
  }
  for (int iSeg = SCurveGetNbSeg(curve); iSeg--;) {
    BCurve* seg = SCurveGetSeg(curve, iSeg);
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
    BCurve* seg = SCurveGetSeg(curve, iSeg);
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
    BCurve* seg = SCurveGetSeg(curve, iSeg);
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
  SCurve* curve = SCurveCreate(order, dim, nbSeg);
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(SCurveCtrl(curve, iCtrl), iDim, iCtrl * dim + iDim);
  }
  for (float u = 0.0; u < SCurveGetMaxU(curve) + PBMATH_EPSILON; 
    u += 0.1) {
    VecFloat* v = SCurveGet(curve, u);
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
  SCurve* curve = SCurveCreate(order, dim, nbSeg);
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
  SCurve* curve = SCurveCreate(order, dim, nbSeg);
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(SCurveCtrl(curve, iCtrl), iDim, iCtrl * dim + iDim);
  }
  VecFloat* center = SCurveGetCenter(curve);
  VecFloat* check = VecFloatCreate(dim);
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
  SCurve* curve = SCurveCreate(order, dim, nbSeg);
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
  SCurve* curve = SCurveCreate(order, dim, nbSeg);
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
  VecFloat* v = VecFloatCreate(dim);
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
  SCurve* curve = SCurveCreate(order, dim, nbSeg);
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(SCurveCtrl(curve, iCtrl), iDim, iCtrl * dim + iDim);
  }
  VecFloat* v = VecFloatCreate(dim);
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
  SCurve* curve = SCurveCreate(order, dim, nbSeg);
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    VecSet(SCurveCtrl(curve, iCtrl), 0, 
      cos(PBMATH_QUARTERPI * (float)iCtrl * 0.5));
    VecSet(SCurveCtrl(curve, iCtrl), 1, 
      sin(PBMATH_QUARTERPI * (float)iCtrl * 0.5));
  }
  Facoid* bound = SCurveGetBoundingBox(curve);
  if (ISEQUALF(VecGet(ShapoidPos(bound), 0), -1.0) == false ||
    ISEQUALF(VecGet(ShapoidPos(bound), 1), -0.382683) == false ||
    ISEQUALF(VecGet(ShapoidAxis(bound, 0), 0), 2.382684) == false ||
    ISEQUALF(VecGet(ShapoidAxis(bound, 0), 1), 0.0) == false ||
    ISEQUALF(VecGet(ShapoidAxis(bound, 1), 0), 0.0) == false ||
    ISEQUALF(VecGet(ShapoidAxis(bound, 1), 1), 1.765367) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveGetBoundingBox failed");
    PBErrCatch(BCurveErr);
  }
  ShapoidFree(&bound);
  SCurveFree(&curve);
  printf("UnitTestSCurveGetBoundingBox OK\n");
}

void UnitTestSCurveGetNewDim() {
  int order = 3;
  int dim = 3;
  int nbSeg = 2;
  SCurve* curve = SCurveCreate(order, dim, nbSeg);
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    VecSet(SCurveCtrl(curve, iCtrl), 0, (float)iCtrl);
    VecSet(SCurveCtrl(curve, iCtrl), 1, (float)iCtrl + 1);
    VecSet(SCurveCtrl(curve, iCtrl), 2, (float)iCtrl + 2);
  }
  SCurve* curveA = SCurveGetNewDim(curve, 2);
  if (SCurveGetDim(curveA) != 2 ||
    ISEQUALF(VecGet(SCurveCtrl(curveA, 0), 0), 0.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveA, 0), 1), 1.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveA, 1), 0), 1.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveA, 1), 1), 2.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveA, 2), 0), 2.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveA, 2), 1), 3.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveA, 3), 0), 3.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveA, 3), 1), 4.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveA, 4), 0), 4.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveA, 4), 1), 5.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveA, 5), 0), 5.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveA, 5), 1), 6.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveA, 6), 0), 6.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveA, 6), 1), 7.0) == false ||
    BCurveCtrl(SCurveSeg(curveA, 0), 0) != SCurveCtrl(curveA, 0) ||
    BCurveCtrl(SCurveSeg(curveA, 0), 1) != SCurveCtrl(curveA, 1) ||
    BCurveCtrl(SCurveSeg(curveA, 0), 2) != SCurveCtrl(curveA, 2) ||
    BCurveCtrl(SCurveSeg(curveA, 0), 3) != SCurveCtrl(curveA, 3) ||
    BCurveCtrl(SCurveSeg(curveA, 1), 0) != SCurveCtrl(curveA, 3) ||
    BCurveCtrl(SCurveSeg(curveA, 1), 1) != SCurveCtrl(curveA, 4) ||
    BCurveCtrl(SCurveSeg(curveA, 1), 2) != SCurveCtrl(curveA, 5) ||
    BCurveCtrl(SCurveSeg(curveA, 1), 3) != SCurveCtrl(curveA, 6)) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveGetNewDim failed");
    PBErrCatch(BCurveErr);
  }
  SCurve* curveB = SCurveGetNewDim(curve, 4);
  if (SCurveGetDim(curveB) != 4 ||
    ISEQUALF(VecGet(SCurveCtrl(curveB, 0), 0), 0.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveB, 0), 1), 1.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveB, 0), 2), 2.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveB, 0), 3), 0.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveB, 1), 0), 1.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveB, 1), 1), 2.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveB, 1), 2), 3.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveB, 1), 3), 0.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveB, 2), 0), 2.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveB, 2), 1), 3.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveB, 2), 2), 4.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveB, 2), 3), 0.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveB, 3), 0), 3.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveB, 3), 1), 4.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveB, 3), 2), 5.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveB, 3), 3), 0.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveB, 4), 0), 4.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveB, 4), 1), 5.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveB, 4), 2), 6.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveB, 4), 3), 0.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveB, 5), 0), 5.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveB, 5), 1), 6.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveB, 5), 2), 7.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveB, 5), 3), 0.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveB, 6), 0), 6.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveB, 6), 1), 7.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveB, 6), 2), 8.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curveB, 6), 3), 0.0) == false ||
    BCurveCtrl(SCurveSeg(curveB, 0), 0) != SCurveCtrl(curveB, 0) ||
    BCurveCtrl(SCurveSeg(curveB, 0), 1) != SCurveCtrl(curveB, 1) ||
    BCurveCtrl(SCurveSeg(curveB, 0), 2) != SCurveCtrl(curveB, 2) ||
    BCurveCtrl(SCurveSeg(curveB, 0), 3) != SCurveCtrl(curveB, 3) ||
    BCurveCtrl(SCurveSeg(curveB, 1), 0) != SCurveCtrl(curveB, 3) ||
    BCurveCtrl(SCurveSeg(curveB, 1), 1) != SCurveCtrl(curveB, 4) ||
    BCurveCtrl(SCurveSeg(curveB, 1), 2) != SCurveCtrl(curveB, 5) ||
    BCurveCtrl(SCurveSeg(curveB, 1), 3) != SCurveCtrl(curveB, 6)) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveGetNewDim failed");
    PBErrCatch(BCurveErr);
  }
  SCurveFree(&curve);
  SCurveFree(&curveA);
  SCurveFree(&curveB);
  printf("UnitTestSCurveGetNewDim OK\n");
}

void UnitTestSCurveCreateFromShapoid() {
  Facoid* facoid = FacoidCreate(2);
  Pyramidoid* pyramidoid = PyramidoidCreate(2);
  Spheroid* spheroid = SpheroidCreate(2);
  VecFloat2D v = VecFloatCreateStatic2D();
  VecSet(&v, 0, 1.0); VecSet(&v, 1, 2.0); 
  ShapoidSetPos(facoid, &v);
  ShapoidSetPos(pyramidoid, &v);
  ShapoidSetPos(spheroid, &v);
  VecSet(&v, 0, 3.0); VecSet(&v, 1, 4.0); 
  ShapoidSetAxis(facoid, 0, &v);
  ShapoidSetAxis(pyramidoid, 0, &v);
  ShapoidSetAxis(spheroid, 0, &v);
  VecSet(&v, 0, -5.0); VecSet(&v, 1, 6.0); 
  ShapoidSetAxis(facoid, 1, &v);
  ShapoidSetAxis(pyramidoid, 1, &v);
  ShapoidSetAxis(spheroid, 1, &v);
  SCurve* curve = SCurveCreateFromShapoid((Shapoid*)facoid);
  if (curve == NULL || SCurveGetDim(curve) != 2 ||
    SCurveGetOrder(curve) != 1 || SCurveGetNbSeg(curve) != 4) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveCreateFromFacoid failed");
    PBErrCatch(BCurveErr);
  }
  if (ISEQUALF(VecGet(SCurveCtrl(curve, 0), 0), 1.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 0), 1), 2.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 1), 0), 4.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 1), 1), 6.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 2), 0), -1.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 2), 1), 12.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 3), 0), -4.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 3), 1), 8.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 4), 0), 1.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 4), 1), 2.0) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveCreateFromFacoid failed");
    PBErrCatch(BCurveErr);
  }
  SCurveFree(&curve);
  curve = SCurveCreateFromShapoid((Shapoid*)pyramidoid);
  if (curve == NULL || SCurveGetDim(curve) != 2 ||
    SCurveGetOrder(curve) != 1 || SCurveGetNbSeg(curve) != 3) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveCreateFromPyramidoid failed");
    PBErrCatch(BCurveErr);
  }
  if (ISEQUALF(VecGet(SCurveCtrl(curve, 0), 0), 1.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 0), 1), 2.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 1), 0), 4.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 1), 1), 6.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 2), 0), -4.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 2), 1), 8.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 3), 0), 1.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 3), 1), 2.0) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveCreateFromPyramidoid failed");
    PBErrCatch(BCurveErr);
  }
  SCurveFree(&curve);
  curve = SCurveCreateFromShapoid((Shapoid*)spheroid);
  if (curve == NULL || SCurveGetDim(curve) != 2 ||
    SCurveGetOrder(curve) != 3 || SCurveGetNbSeg(curve) != 4) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveCreateFromSpheroid failed");
    PBErrCatch(BCurveErr);
  }
  if (ISEQUALF(VecGet(SCurveCtrl(curve, 0), 0), 2.5) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 0), 1), 4.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 1), 0), 1.119290) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 1), 1), 5.656852) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 2), 0), -0.671574) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 2), 1), 6.104568) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 3), 0), -1.5) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 3), 1), 5.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 4), 0), -2.328426) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 4), 1), 3.895432) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 5), 0), -1.880710) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 5), 1), 1.656852) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 6), 0), -0.5) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 6), 1), 0.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 7), 0), 0.880710) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 7), 1), -1.656852) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 8), 0), 2.671574) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 8), 1), -2.104568) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 9), 0), 3.5) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 9), 1), -1.0) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 10), 0), 4.328426) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 10), 1), 0.104568) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 11), 0), 3.880710) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 11), 1), 2.343148) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 12), 0), 2.5) == false ||
    ISEQUALF(VecGet(SCurveCtrl(curve, 12), 1), 4.0) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveCreateFromSpheroid failed");
    PBErrCatch(BCurveErr);
  }
  SCurveFree(&curve);
  ShapoidFree(&facoid);
  ShapoidFree(&pyramidoid);
  ShapoidFree(&spheroid);
  printf("UnitTestSCurveCreateFromShapoid OK\n");
}

void UnitTestSCurveGetDistToCurve() {
  int order = 1;
  int dim = 2;
  int nbSeg = 1;
  SCurve* curveA = SCurveCreate(order, dim, nbSeg);
  SCurve* curveB = SCurveCreate(order, dim, nbSeg);
  VecSet(SCurveCtrl(curveA, 0), 0, 0.0);
  VecSet(SCurveCtrl(curveA, 0), 1, 0.0);
  VecSet(SCurveCtrl(curveA, 1), 0, 1.0);
  VecSet(SCurveCtrl(curveA, 1), 1, 0.0);
  VecSet(SCurveCtrl(curveB, 0), 0, 0.0);
  VecSet(SCurveCtrl(curveB, 0), 1, 2.0);
  VecSet(SCurveCtrl(curveB, 1), 0, 1.0);
  VecSet(SCurveCtrl(curveB, 1), 1, 2.0);
  float dist = SCurveGetDistToCurve(curveA, curveB);
  if (ISEQUALF(dist, 2.0) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveGetDistToCurve failed");
    PBErrCatch(BCurveErr);
  }
  SCurveFree(&curveA);
  SCurveFree(&curveB);
  printf("UnitTestSCurveGetDDistToCurve OK\n");
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
  UnitTestSCurveGetNewDim();
  UnitTestSCurveCreateFromShapoid();
  UnitTestSCurveGetDistToCurve();
  printf("UnitTestSCurve OK\n");
}

void UnitTestSCurveIterCreate() {
  int order = 3;
  int dim = 2;
  int nbSeg = 3;
  SCurve* curve = SCurveCreate(order, dim, nbSeg);
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(SCurveCtrl(curve, iCtrl), iDim, iCtrl * dim + iDim);
  }
  float delta = 0.2;
  SCurveIter iter = SCurveIterCreateStatic(curve, delta);
  if (iter._curve != curve || ISEQUALF(iter._curPos, 0.0) == false ||
    ISEQUALF(iter._delta, delta) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveIterCreateStatic failed");
    PBErrCatch(BCurveErr);
  }
  SCurveFree(&curve);
  printf("UnitTestSCurveIterCreate OK\n");
}

void UnitTestSCurveIterSetGet() {
  int order = 3;
  int dim = 2;
  int nbSeg = 3;
  SCurve* curve = SCurveCreate(order, dim, nbSeg);
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(SCurveCtrl(curve, iCtrl), iDim, iCtrl * dim + iDim);
  }
  float delta = 0.2;
  SCurveIter iter = SCurveIterCreateStatic(curve, delta);
  SCurve* curveB = SCurveCreate(order, dim, nbSeg);
  SCurveIterSetCurve(&iter, curveB);
  if (iter._curve != curveB) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveIterSetCurve failed");
    PBErrCatch(BCurveErr);
  }
  if (SCurveIterCurve(&iter) != curveB) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveIterCurve failed");
    PBErrCatch(BCurveErr);
  }
  float deltaB = 0.3;
  SCurveIterSetDelta(&iter, deltaB);
  if (iter._delta != deltaB) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveIterSetDelta failed");
    PBErrCatch(BCurveErr);
  }
  if (SCurveIterGetDelta(&iter) != deltaB) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveIterGetDelta failed");
    PBErrCatch(BCurveErr);
  }
  SCurveIterSetCurve(&iter, curve);
  iter._curPos = 0.5;
  if (SCurveIterGetPos(&iter) != 0.5) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveIterGetPos failed");
    PBErrCatch(BCurveErr);
  }
  VecFloat* pos = SCurveIterGet(&iter);
  if (ISEQUALF(VecGet(pos, 0), 3.0) == false ||
    ISEQUALF(VecGet(pos, 1), 4.0) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveIterGet failed");
    PBErrCatch(BCurveErr);
  }
  VecFree(&pos);
  SCurveFree(&curve);
  SCurveFree(&curveB);
  printf("UnitTestSCurveIterSetGet OK\n");
}

void UnitTestSCurveIterStep() {
  int order = 3;
  int dim = 2;
  int nbSeg = 3;
  SCurve* curve = SCurveCreate(order, dim, nbSeg);
  for (int iCtrl = SCurveGetNbCtrl(curve); iCtrl--;) {
    for (int iDim = dim; iDim--;)
      VecSet(SCurveCtrl(curve, iCtrl), iDim, iCtrl * dim + iDim);
  }
  float delta = 3.0;
  SCurveIter iter = SCurveIterCreateStatic(curve, delta);
  bool ret = SCurveIterStep(&iter);
  if (ISEQUALF(SCurveIterGetPos(&iter), 3.0) == false ||
    ret == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveIterStep failed");
    PBErrCatch(BCurveErr);
  }
  ret = SCurveIterStep(&iter);
  if (ISEQUALF(SCurveIterGetPos(&iter), 3.0) == false ||
    ret == true) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveIterStep failed");
    PBErrCatch(BCurveErr);
  }
  ret = SCurveIterStepP(&iter);
  if (ISEQUALF(SCurveIterGetPos(&iter), 0.0) == false ||
    ret == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveIterStepP failed");
    PBErrCatch(BCurveErr);
  }
  ret = SCurveIterStepP(&iter);
  if (ISEQUALF(SCurveIterGetPos(&iter), 0.0) == false ||
    ret == true) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "SCurveIterStepP failed");
    PBErrCatch(BCurveErr);
  }
  SCurveFree(&curve);
  printf("UnitTestSCurveStep OK\n");
}

void UnitTestSCurveIter() {
  UnitTestSCurveIterCreate();
  UnitTestSCurveIterSetGet();
  UnitTestSCurveIterStep();

  printf("UnitTestSCurveIter OK\n");
}

void UnitTestBBodyCreateFree() {
  int order = 1;
  VecShort2D dim = VecShortCreateStatic2D();
  VecSet(&dim, 0, 2); VecSet(&dim, 1, 3);
  BBody* surf = BBodyCreate(order, &dim);
  if (VecGet(&(surf->_dim), 0) != VecGet(&dim, 0) ||
    VecGet(&(surf->_dim), 1) != VecGet(&dim, 1) ||
    surf->_order != order) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BBodyCreate failed");
    PBErrCatch(BCurveErr);
  }
  BBodyFree(&surf);
  printf("UnitTestBBodyCreateFree OK\n");
}

void UnitTestBBodyGetSet() {
  int order = 1;
  VecShort2D dim = VecShortCreateStatic2D();
  VecSet(&dim, 0, 2); VecSet(&dim, 1, 3);
  BBody* surf = BBodyCreate(order, &dim);
  if (BBodyGetOrder(surf) != 1) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BBodyGetOrder failed");
    PBErrCatch(BCurveErr);
  }
  if (VecIsEqual(BBodyDim(surf), &dim) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BBodyDim failed");
    PBErrCatch(BCurveErr);
  }
  VecShort2D dimB = VecShortCreateStatic2D();
  dimB = BBodyGetDim(surf);
  if (VecIsEqual(&dimB, &dim) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BBodyGetDim failed");
    PBErrCatch(BCurveErr);
  }
  if (BBodyGetNbCtrl(surf) != 4) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BBodyGetNbCtrl failed");
    PBErrCatch(BCurveErr);
  }
  VecShort2D iCtrl = VecShortCreateStatic2D();
  VecSet(&iCtrl, 0, 1); VecSet(&iCtrl, 1, 0); 
  if (BBodyGetIndexCtrl(surf, &iCtrl) != 2) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BBodyGetIndexCtrl failed");
    PBErrCatch(BCurveErr);
  }
  if (BBodyCtrl(surf, &iCtrl) != surf->_ctrl[2]) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BBodyCtrl failed");
    PBErrCatch(BCurveErr);
  }
  VecFloat3D v = VecFloatCreateStatic3D();
  VecSet(&v, 0, 1.0); VecSet(&v, 1, 2.0); VecSet(&v, 2, 3.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  if (VecIsEqual(BBodyCtrl(surf, &iCtrl), (VecFloat*)&v) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BBodySetCtrl failed");
    PBErrCatch(BCurveErr);
  }
  BBodyFree(&surf);
  printf("UnitTestBBodyGetSet OK\n");
}

void UnitTestBBodyGet() {
  int order = 1;
  VecShort2D dim = VecShortCreateStatic2D();
  VecSet(&dim, 0, 2); VecSet(&dim, 1, 3);
  BBody* surf = BBodyCreate(order, &dim);
  VecShort2D iCtrl = VecShortCreateStatic2D();
  VecSet(&iCtrl, 0, 0); VecSet(&iCtrl, 1, 0); 
  VecFloat3D v = VecFloatCreateStatic3D();
  VecSet(&v, 0, 0.0); VecSet(&v, 1, 0.0); VecSet(&v, 2, 0.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecSet(&iCtrl, 0, 1); VecSet(&iCtrl, 1, 0); 
  VecSet(&v, 0, 1.0); VecSet(&v, 1, 0.0); VecSet(&v, 2, 0.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecSet(&iCtrl, 0, 0); VecSet(&iCtrl, 1, 1); 
  VecSet(&v, 0, 0.0); VecSet(&v, 1, 1.0); VecSet(&v, 2, 0.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecSet(&iCtrl, 0, 1); VecSet(&iCtrl, 1, 1); 
  VecSet(&v, 0, 0.0); VecSet(&v, 1, 0.0); VecSet(&v, 2, 1.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecFloat2D u = VecFloatCreateStatic2D();
  float du = 0.2;
  int iCheck = 0;
  float check[75] = {
    0.0,0.0,0.0,0.0,0.2,0.0,0.0,0.4,0.0,0.0,0.6,0.0,0.0,0.8,0.0,
    0.2,0.0,0.0,0.16,0.16,0.04,0.12,0.32,0.08,0.08,0.48,0.12,0.04,
    0.64,0.16,0.4,0.0,0.0,0.32,0.12,0.08,0.24,0.24,0.16,0.16,0.36,
    0.24,0.08,0.48,0.32,0.6,0.0,0.0,0.48,0.08,0.12,0.36,0.16,0.24,
    0.24,0.24,0.36,0.12,0.32,0.48,0.8,0.0,0.0,0.64,0.04,0.16,0.48,
    0.08,0.32,0.32,0.12,0.48,0.16,0.16,0.64
    };
  for (VecSet(&u, 0, 0.0); VecGet(&u, 0) < 1.0; 
    VecSet(&u, 0, VecGet(&u, 0) + du)) {
    for (VecSet(&u, 1, 0.0); VecGet(&u, 1) < 1.0; 
      VecSet(&u, 1, VecGet(&u, 1) + du)) {
      VecFloat* p = BBodyGet(surf, &u);
      if (ISEQUALF(p->_val[0], check[iCheck]) == false ||
        ISEQUALF(p->_val[1], check[iCheck + 1]) == false ||
        ISEQUALF(p->_val[2], check[iCheck + 2]) == false) {
        BCurveErr->_type = PBErrTypeUnitTestFailed;
        sprintf(BCurveErr->_msg, "BBodyGet failed");
        PBErrCatch(BCurveErr);
      }
      iCheck += 3;
      VecFree(&p);
    }
  }
  BBodyFree(&surf);
  printf("UnitTestBBodyGet OK\n");
}

void UnitTestBBodyClone() {
  int order = 1;
  VecShort2D dim = VecShortCreateStatic2D();
  VecSet(&dim, 0, 2); VecSet(&dim, 1, 3);
  BBody* surf = BBodyCreate(order, &dim);
  VecShort2D iCtrl = VecShortCreateStatic2D();
  VecSet(&iCtrl, 0, 0); VecSet(&iCtrl, 1, 0); 
  VecFloat3D v = VecFloatCreateStatic3D();
  VecSet(&v, 0, 0.0); VecSet(&v, 1, 0.0); VecSet(&v, 2, 0.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecSet(&iCtrl, 0, 1); VecSet(&iCtrl, 1, 0); 
  VecSet(&v, 0, 1.0); VecSet(&v, 1, 0.0); VecSet(&v, 2, 0.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecSet(&iCtrl, 0, 0); VecSet(&iCtrl, 1, 1); 
  VecSet(&v, 0, 0.0); VecSet(&v, 1, 1.0); VecSet(&v, 2, 0.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecSet(&iCtrl, 0, 1); VecSet(&iCtrl, 1, 1); 
  VecSet(&v, 0, 0.0); VecSet(&v, 1, 0.0); VecSet(&v, 2, 1.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  BBody* clone = BBodyClone(surf);
  if (BBodyGetOrder(clone) != BBodyGetOrder(surf)) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BBodyClone failed");
    PBErrCatch(BCurveErr);
  }
  if (VecIsEqual(BBodyDim(clone), BBodyDim(surf)) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BBodyClone failed");
    PBErrCatch(BCurveErr);
  }
  for (int iCtrl = BBodyGetNbCtrl(clone); iCtrl--;) {
    if (VecIsEqual(clone->_ctrl[iCtrl], surf->_ctrl[iCtrl]) == false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "BBodyClone failed");
      PBErrCatch(BCurveErr);
    }
  }
  BBodyFree(&surf);
  BBodyFree(&clone);
  printf("UnitTestBBodyClone OK\n");
}
  
void UnitTestBBodyPrint() {
  int order = 1;
  VecShort2D dim = VecShortCreateStatic2D();
  VecSet(&dim, 0, 2); VecSet(&dim, 1, 3);
  BBody* surf = BBodyCreate(order, &dim);
  VecShort2D iCtrl = VecShortCreateStatic2D();
  VecSet(&iCtrl, 0, 0); VecSet(&iCtrl, 1, 0); 
  VecFloat3D v = VecFloatCreateStatic3D();
  VecSet(&v, 0, 0.0); VecSet(&v, 1, 0.0); VecSet(&v, 2, 0.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecSet(&iCtrl, 0, 1); VecSet(&iCtrl, 1, 0); 
  VecSet(&v, 0, 1.0); VecSet(&v, 1, 0.0); VecSet(&v, 2, 0.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecSet(&iCtrl, 0, 0); VecSet(&iCtrl, 1, 1); 
  VecSet(&v, 0, 0.0); VecSet(&v, 1, 1.0); VecSet(&v, 2, 0.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecSet(&iCtrl, 0, 1); VecSet(&iCtrl, 1, 1); 
  VecSet(&v, 0, 0.0); VecSet(&v, 1, 0.0); VecSet(&v, 2, 1.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  BBodyPrint(surf, stdout);
  printf("\n");
  BBodyFree(&surf);
  printf("UnitTestBBodyPrint OK\n");
}

void UnitTestBBodyLoadSave() {
  int order = 1;
  VecShort2D dim = VecShortCreateStatic2D();
  VecSet(&dim, 0, 2); VecSet(&dim, 1, 3);
  BBody* surf = BBodyCreate(order, &dim);
  VecShort2D iCtrl = VecShortCreateStatic2D();
  VecSet(&iCtrl, 0, 0); VecSet(&iCtrl, 1, 0); 
  VecFloat3D v = VecFloatCreateStatic3D();
  VecSet(&v, 0, 0.0); VecSet(&v, 1, 0.0); VecSet(&v, 2, 0.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecSet(&iCtrl, 0, 1); VecSet(&iCtrl, 1, 0); 
  VecSet(&v, 0, 1.0); VecSet(&v, 1, 0.0); VecSet(&v, 2, 0.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecSet(&iCtrl, 0, 0); VecSet(&iCtrl, 1, 1); 
  VecSet(&v, 0, 0.0); VecSet(&v, 1, 1.0); VecSet(&v, 2, 0.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecSet(&iCtrl, 0, 1); VecSet(&iCtrl, 1, 1); 
  VecSet(&v, 0, 0.0); VecSet(&v, 1, 0.0); VecSet(&v, 2, 1.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  FILE* file = fopen("./bbody.txt", "w");
  if (BBodySave(surf, file) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BBodySave failed");
    PBErrCatch(BCurveErr);
  }
  fclose(file);
  BBody* clone = NULL;
  file = fopen("./bbody.txt", "r");
  if (BBodyLoad(&clone, file) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BBodyLoad failed");
    PBErrCatch(BCurveErr);
  }
  fclose(file);
  if (BBodyGetOrder(clone) != BBodyGetOrder(surf)) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BBodyLoadSave failed");
    PBErrCatch(BCurveErr);
  }
  if (VecIsEqual(BBodyDim(clone), BBodyDim(surf)) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BBodyLoadSave failed");
    PBErrCatch(BCurveErr);
  }
  for (int iCtrl = BBodyGetNbCtrl(clone); iCtrl--;) {
    if (VecIsEqual(clone->_ctrl[iCtrl], surf->_ctrl[iCtrl]) == false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "BBodyLoadSave failed");
      PBErrCatch(BCurveErr);
    }
  }
  BBodyFree(&surf);
  BBodyFree(&clone);
  printf("UnitTestBBodyLoadSave OK\n");
}

void UnitTestBBodyGetCenter() {
  int order = 1;
  VecShort2D dim = VecShortCreateStatic2D();
  VecSet(&dim, 0, 2); VecSet(&dim, 1, 3);
  BBody* surf = BBodyCreate(order, &dim);
  VecShort2D iCtrl = VecShortCreateStatic2D();
  VecSet(&iCtrl, 0, 0); VecSet(&iCtrl, 1, 0); 
  VecFloat3D v = VecFloatCreateStatic3D();
  VecSet(&v, 0, 0.0); VecSet(&v, 1, 0.0); VecSet(&v, 2, 0.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecSet(&iCtrl, 0, 1); VecSet(&iCtrl, 1, 0); 
  VecSet(&v, 0, 1.0); VecSet(&v, 1, 0.0); VecSet(&v, 2, 0.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecSet(&iCtrl, 0, 0); VecSet(&iCtrl, 1, 1); 
  VecSet(&v, 0, 0.0); VecSet(&v, 1, 1.0); VecSet(&v, 2, 0.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecSet(&iCtrl, 0, 1); VecSet(&iCtrl, 1, 1); 
  VecSet(&v, 0, 0.0); VecSet(&v, 1, 0.0); VecSet(&v, 2, 1.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecFloat* center = BBodyGetCenter(surf);
  VecSet(&v, 0, 0.25); VecSet(&v, 1, 0.25); VecSet(&v, 2, 0.25); 
  if (VecIsEqual(center, (VecFloat*)&v) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BBodyGetCenter failed");
    PBErrCatch(BCurveErr);
  }
  BBodyFree(&surf);
  VecFree(&center);
  printf("UnitTestBBodyGetCenter OK\n");
}

void UnitTestBBodyTranslate() {
  int order = 1;
  VecShort2D dim = VecShortCreateStatic2D();
  VecSet(&dim, 0, 2); VecSet(&dim, 1, 3);
  BBody* surf = BBodyCreate(order, &dim);
  VecShort2D iCtrl = VecShortCreateStatic2D();
  VecSet(&iCtrl, 0, 0); VecSet(&iCtrl, 1, 0); 
  VecFloat3D v = VecFloatCreateStatic3D();
  VecSet(&v, 0, 0.0); VecSet(&v, 1, 0.0); VecSet(&v, 2, 0.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecSet(&iCtrl, 0, 1); VecSet(&iCtrl, 1, 0); 
  VecSet(&v, 0, 1.0); VecSet(&v, 1, 0.0); VecSet(&v, 2, 0.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecSet(&iCtrl, 0, 0); VecSet(&iCtrl, 1, 1); 
  VecSet(&v, 0, 0.0); VecSet(&v, 1, 1.0); VecSet(&v, 2, 0.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecSet(&iCtrl, 0, 1); VecSet(&iCtrl, 1, 1); 
  VecSet(&v, 0, 0.0); VecSet(&v, 1, 0.0); VecSet(&v, 2, 1.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecSet(&v, 0, 1.0); VecSet(&v, 1, 2.0); VecSet(&v, 2, 3.0); 
  BBodyTranslate(surf, &v);
  float check[12] = {
    1.0,2.0,3.0,
    1.0,3.0,3.0,
    2.0,2.0,3.0,
    1.0,2.0,4.0
    };
  for (int iCtrl = BBodyGetNbCtrl(surf); iCtrl--;) {
    if (ISEQUALF(check[3 * iCtrl], surf->_ctrl[iCtrl]->_val[0]) == 
      false ||
      ISEQUALF(check[3 * iCtrl + 1], surf->_ctrl[iCtrl]->_val[1]) == 
      false ||
      ISEQUALF(check[3 * iCtrl + 2], surf->_ctrl[iCtrl]->_val[2]) == 
      false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "BBodyTranslate failed");
      PBErrCatch(BCurveErr);
    }
  }
  BBodyFree(&surf);
  printf("UnitTestBBodyTranslate OK\n");
}

void UnitTestBBodyScale() {
  int order = 1;
  VecShort2D dim = VecShortCreateStatic2D();
  VecSet(&dim, 0, 2); VecSet(&dim, 1, 3);
  BBody* surf = BBodyCreate(order, &dim);
  VecShort2D iCtrl = VecShortCreateStatic2D();
  VecSet(&iCtrl, 0, 0); VecSet(&iCtrl, 1, 0); 
  VecFloat3D v = VecFloatCreateStatic3D();
  VecSet(&v, 0, 0.0); VecSet(&v, 1, 0.0); VecSet(&v, 2, 0.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecSet(&iCtrl, 0, 1); VecSet(&iCtrl, 1, 0); 
  VecSet(&v, 0, 1.0); VecSet(&v, 1, 0.0); VecSet(&v, 2, 0.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecSet(&iCtrl, 0, 0); VecSet(&iCtrl, 1, 1); 
  VecSet(&v, 0, 0.0); VecSet(&v, 1, 1.0); VecSet(&v, 2, 0.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecSet(&iCtrl, 0, 1); VecSet(&iCtrl, 1, 1); 
  VecSet(&v, 0, 0.0); VecSet(&v, 1, 0.0); VecSet(&v, 2, 1.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecSet(&v, 0, 1.0); VecSet(&v, 1, 2.0); VecSet(&v, 2, 3.0); 
  BBodyScaleCenter(surf, (VecFloat*)&v);
  float checka[12] = {
    0.0,-0.25,-0.5,
    0.0,1.75,-0.5,
    1.0,-0.25,-0.5,
    0.0,-0.25,2.5
    };
  for (int iCtrl = BBodyGetNbCtrl(surf); iCtrl--;) {
    if (ISEQUALF(checka[3 * iCtrl], surf->_ctrl[iCtrl]->_val[0]) == 
      false ||
      ISEQUALF(checka[3 * iCtrl + 1], surf->_ctrl[iCtrl]->_val[1]) == 
      false ||
      ISEQUALF(checka[3 * iCtrl + 2], surf->_ctrl[iCtrl]->_val[2]) == 
      false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "BBodyScaleCenter failed");
      PBErrCatch(BCurveErr);
    }
  }
  BBodyScaleOrigin(surf, (VecFloat*)&v);
  float checkb[12] = {
    0.0,-0.5,-1.5,
    0.0,3.5,-1.5,
    1.0,-0.5,-1.5,
    0.0,-0.5,7.5
    };
  for (int iCtrl = BBodyGetNbCtrl(surf); iCtrl--;) {
    if (ISEQUALF(checkb[3 * iCtrl], surf->_ctrl[iCtrl]->_val[0]) == 
      false ||
      ISEQUALF(checkb[3 * iCtrl + 1], surf->_ctrl[iCtrl]->_val[1]) == 
      false ||
      ISEQUALF(checkb[3 * iCtrl + 2], surf->_ctrl[iCtrl]->_val[2]) == 
      false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "BBodyScale failed");
      PBErrCatch(BCurveErr);
    }
  }
  BBodyScaleStart(surf, (VecFloat*)&v);
  float checkc[12] = {
    0.0,-0.5,-1.5,
    0.0,7.5,-1.5,
    1.0,-0.5,-1.5,
    0.0,-0.5,25.5
    };
  for (int iCtrl = BBodyGetNbCtrl(surf); iCtrl--;) {
    if (ISEQUALF(checkc[3 * iCtrl], surf->_ctrl[iCtrl]->_val[0]) == 
      false ||
      ISEQUALF(checkc[3 * iCtrl + 1], surf->_ctrl[iCtrl]->_val[1]) == 
      false ||
      ISEQUALF(checkc[3 * iCtrl + 2], surf->_ctrl[iCtrl]->_val[2]) == 
      false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "BBodyScale failed");
      PBErrCatch(BCurveErr);
    }
  }
  BBodyFree(&surf);
  printf("UnitTestBBodyScale OK\n");
}

void UnitTestBBodyGetBoundingBox() {
  int order = 1;
  VecShort2D dim = VecShortCreateStatic2D();
  VecSet(&dim, 0, 2); VecSet(&dim, 1, 3);
  BBody* surf = BBodyCreate(order, &dim);
  VecShort2D iCtrl = VecShortCreateStatic2D();
  VecSet(&iCtrl, 0, 0); VecSet(&iCtrl, 1, 0); 
  VecFloat3D v = VecFloatCreateStatic3D();
  VecSet(&v, 0, 0.0); VecSet(&v, 1, 0.0); VecSet(&v, 2, 0.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecSet(&iCtrl, 0, 1); VecSet(&iCtrl, 1, 0); 
  VecSet(&v, 0, 1.0); VecSet(&v, 1, 0.0); VecSet(&v, 2, 0.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecSet(&iCtrl, 0, 0); VecSet(&iCtrl, 1, 1); 
  VecSet(&v, 0, 0.0); VecSet(&v, 1, 1.0); VecSet(&v, 2, 0.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecSet(&iCtrl, 0, 1); VecSet(&iCtrl, 1, 1); 
  VecSet(&v, 0, 0.0); VecSet(&v, 1, 0.0); VecSet(&v, 2, 1.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  BBodyScaleCenter(surf, (float)2.0);
  Facoid* bound = BBodyGetBoundingBox(surf);
  VecSet(&v, 0, -0.25); VecSet(&v, 1, -0.25); VecSet(&v, 2, -0.25); 
  if (VecIsEqual(ShapoidPos(bound), (VecFloat*)&v) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BBodyGetBoundingBox failed");
    PBErrCatch(BCurveErr);
  }
  VecSet(&v, 0, 2.0); VecSet(&v, 1, 0.0); VecSet(&v, 2, 0.0); 
  if (VecIsEqual(ShapoidAxis(bound, 0), (VecFloat*)&v) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BBodyGetBoundingBox failed");
    PBErrCatch(BCurveErr);
  }
  VecSet(&v, 0, 0.0); VecSet(&v, 1, 2.0); VecSet(&v, 2, 0.0); 
  if (VecIsEqual(ShapoidAxis(bound, 1), (VecFloat*)&v) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BBodyGetBoundingBox failed");
    PBErrCatch(BCurveErr);
  }
  VecSet(&v, 0, 0.0); VecSet(&v, 1, 0.0); VecSet(&v, 2, 2.0); 
  if (VecIsEqual(ShapoidAxis(bound, 2), (VecFloat*)&v) == false) {
    BCurveErr->_type = PBErrTypeUnitTestFailed;
    sprintf(BCurveErr->_msg, "BBodyGetBoundingBox failed");
    PBErrCatch(BCurveErr);
  }
  ShapoidFree(&bound);
  BBodyFree(&surf);
  printf("UnitTestBBodyGetBoundingBox OK\n");
}

void UnitTestBBodyRotate() {
  int order = 1;
  VecShort2D dim = VecShortCreateStatic2D();
  VecSet(&dim, 0, 2); VecSet(&dim, 1, 3);
  BBody* surf = BBodyCreate(order, &dim);
  VecShort2D iCtrl = VecShortCreateStatic2D();
  VecSet(&iCtrl, 0, 0); VecSet(&iCtrl, 1, 0); 
  VecFloat3D v = VecFloatCreateStatic3D();
  VecSet(&v, 0, 0.0); VecSet(&v, 1, 0.0); VecSet(&v, 2, 0.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecSet(&iCtrl, 0, 1); VecSet(&iCtrl, 1, 0); 
  VecSet(&v, 0, 1.0); VecSet(&v, 1, 0.0); VecSet(&v, 2, 0.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecSet(&iCtrl, 0, 0); VecSet(&iCtrl, 1, 1); 
  VecSet(&v, 0, 0.0); VecSet(&v, 1, 1.0); VecSet(&v, 2, 0.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  VecSet(&iCtrl, 0, 1); VecSet(&iCtrl, 1, 1); 
  VecSet(&v, 0, 0.0); VecSet(&v, 1, 0.0); VecSet(&v, 2, 1.0); 
  BBodySetCtrl(surf, &iCtrl, &v);
  float theta = PBMATH_HALFPI;
  BBodyRotXCenter(surf, theta);
  float checka[12] = {
    0.0,0.5,0.0,
    0.0,0.5,1.0,
    1.0,0.5,0.0,
    0.0,-0.5,0.0
    };
  for (int iCtrl = BBodyGetNbCtrl(surf); iCtrl--;) {
    if (ISEQUALF(checka[3 * iCtrl], surf->_ctrl[iCtrl]->_val[0]) == 
      false ||
      ISEQUALF(checka[3 * iCtrl + 1], surf->_ctrl[iCtrl]->_val[1]) == 
      false ||
      ISEQUALF(checka[3 * iCtrl + 2], surf->_ctrl[iCtrl]->_val[2]) == 
      false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "BBodyRotXCenter failed");
      PBErrCatch(BCurveErr);
    }
  }
  BBodyRotXOrigin(surf, theta);
  float checkb[12] = {
    0.0,0.0,0.5,
    0.0,-1.0,0.5,
    1.0,0.0,0.5,
    0.0,0.0,-0.5
    };
  for (int iCtrl = BBodyGetNbCtrl(surf); iCtrl--;) {
    if (ISEQUALF(checkb[3 * iCtrl], surf->_ctrl[iCtrl]->_val[0]) == 
      false ||
      ISEQUALF(checkb[3 * iCtrl + 1], surf->_ctrl[iCtrl]->_val[1]) == 
      false ||
      ISEQUALF(checkb[3 * iCtrl + 2], surf->_ctrl[iCtrl]->_val[2]) == 
      false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "BBodyRotXOrigin failed");
      PBErrCatch(BCurveErr);
    }
  }
  BBodyRotXStart(surf, theta);
  float checkc[12] = {
    0.0,0.0,0.5,
    0.0,0.0,-0.5,
    1.0,0.0,0.5,
    0.0,1.0,0.5
    };
  for (int iCtrl = BBodyGetNbCtrl(surf); iCtrl--;) {
    if (ISEQUALF(checkc[3 * iCtrl], surf->_ctrl[iCtrl]->_val[0]) == 
      false ||
      ISEQUALF(checkc[3 * iCtrl + 1], surf->_ctrl[iCtrl]->_val[1]) == 
      false ||
      ISEQUALF(checkc[3 * iCtrl + 2], surf->_ctrl[iCtrl]->_val[2]) == 
      false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "BBodyRotXStart failed");
      PBErrCatch(BCurveErr);
    }
  }
  BBodyRotYCenter(surf, theta);
  float checkd[12] = {
    0.5,0.0,0.5,
    -0.5,0.0,0.5,
    0.5,0.0,-0.5,
    0.5,1.0,0.5
    };
  for (int iCtrl = BBodyGetNbCtrl(surf); iCtrl--;) {
    if (ISEQUALF(checkd[3 * iCtrl], surf->_ctrl[iCtrl]->_val[0]) == 
      false ||
      ISEQUALF(checkd[3 * iCtrl + 1], surf->_ctrl[iCtrl]->_val[1]) == 
      false ||
      ISEQUALF(checkd[3 * iCtrl + 2], surf->_ctrl[iCtrl]->_val[2]) == 
      false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "BBodyRotYCenter failed");
      PBErrCatch(BCurveErr);
    }
  }
  BBodyRotYOrigin(surf, theta);
  float checke[12] = {
    0.5,0.0,-0.5,
    0.5,0.0,0.5,
    -0.5,0.0,-0.5,
    0.5,1.0,-0.5
    };
  for (int iCtrl = BBodyGetNbCtrl(surf); iCtrl--;) {
    if (ISEQUALF(checke[3 * iCtrl], surf->_ctrl[iCtrl]->_val[0]) == 
      false ||
      ISEQUALF(checke[3 * iCtrl + 1], surf->_ctrl[iCtrl]->_val[1]) == 
      false ||
      ISEQUALF(checke[3 * iCtrl + 2], surf->_ctrl[iCtrl]->_val[2]) == 
      false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "BBodyRotYOrigin failed");
      PBErrCatch(BCurveErr);
    }
  }
  BBodyRotYStart(surf, theta);
  float checkf[12] = {
    0.5,0.0,-0.5,
    1.5,0.0,-0.5,
    0.5,0.0,0.5,
    0.5,1.0,-0.5
    };
  for (int iCtrl = BBodyGetNbCtrl(surf); iCtrl--;) {
    if (ISEQUALF(checkf[3 * iCtrl], surf->_ctrl[iCtrl]->_val[0]) == 
      false ||
      ISEQUALF(checkf[3 * iCtrl + 1], surf->_ctrl[iCtrl]->_val[1]) == 
      false ||
      ISEQUALF(checkf[3 * iCtrl + 2], surf->_ctrl[iCtrl]->_val[2]) == 
      false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "BBodyRotYStart failed");
      PBErrCatch(BCurveErr);
    }
  }
  BBodyRotZCenter(surf, theta);
  float checkg[12] = {
    1.0,0.0,-0.5,
    1.0,1.0,-0.5,
    1.0,0.0,0.5,
    0.0,0.0,-0.5
    };
  for (int iCtrl = BBodyGetNbCtrl(surf); iCtrl--;) {
    if (ISEQUALF(checkg[3 * iCtrl], surf->_ctrl[iCtrl]->_val[0]) == 
      false ||
      ISEQUALF(checkg[3 * iCtrl + 1], surf->_ctrl[iCtrl]->_val[1]) == 
      false ||
      ISEQUALF(checkg[3 * iCtrl + 2], surf->_ctrl[iCtrl]->_val[2]) == 
      false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "BBodyRotZCenter failed");
      PBErrCatch(BCurveErr);
    }
  }
  BBodyRotZOrigin(surf, theta);
  float checkh[12] = {
    0.0,1.0,-0.5,
    -1.0,1.0,-0.5,
    0.0,1.0,0.5,
    0.0,0.0,-0.5
    };
  for (int iCtrl = BBodyGetNbCtrl(surf); iCtrl--;) {
    if (ISEQUALF(checkh[3 * iCtrl], surf->_ctrl[iCtrl]->_val[0]) == 
      false ||
      ISEQUALF(checkh[3 * iCtrl + 1], surf->_ctrl[iCtrl]->_val[1]) == 
      false ||
      ISEQUALF(checkh[3 * iCtrl + 2], surf->_ctrl[iCtrl]->_val[2]) == 
      false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "BBodyRotZOrigin failed");
      PBErrCatch(BCurveErr);
    }
  }
  BBodyRotZStart(surf, theta);
  float checki[12] = {
    0.0,1.0,-0.5,
    0.0,0.0,-0.5,
    0.0,1.0,0.5,
    1.0,1.0,-0.5
    };
  for (int iCtrl = BBodyGetNbCtrl(surf); iCtrl--;) {
    if (ISEQUALF(checki[3 * iCtrl], surf->_ctrl[iCtrl]->_val[0]) == 
      false ||
      ISEQUALF(checki[3 * iCtrl + 1], surf->_ctrl[iCtrl]->_val[1]) == 
      false ||
      ISEQUALF(checki[3 * iCtrl + 2], surf->_ctrl[iCtrl]->_val[2]) == 
      false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "BBodyRotZStart failed");
      PBErrCatch(BCurveErr);
    }
  }
  VecFloat3D axis = VecFloatCreateStatic3D();
  VecSet(&axis, 0, 1.0); VecSet(&axis, 1, 1.0); VecSet(&axis, 2, 1.0);
  VecNormalise(&axis);
  BBodyRotAxisCenter(surf, &axis, theta);
  float checkj[12] = {
    -0.122009,0.666667,-0.044658,
    0.122008,0.333334,-0.955342,
    0.788675,0.422650,0.288675,
    0.211325,1.577350,-0.288675
    };
  for (int iCtrl = BBodyGetNbCtrl(surf); iCtrl--;) {
    if (ISEQUALF(checkj[3 * iCtrl], surf->_ctrl[iCtrl]->_val[0]) == 
      false ||
      ISEQUALF(checkj[3 * iCtrl + 1], surf->_ctrl[iCtrl]->_val[1]) == 
      false ||
      ISEQUALF(checkj[3 * iCtrl + 2], surf->_ctrl[iCtrl]->_val[2]) == 
      false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "BBodyRotAxisCenter failed");
      PBErrCatch(BCurveErr);
    }
  }
  BBodyRotAxisOrigin(surf, &axis, theta);
  float checkk[12] = {
    -0.244017,0.122008,0.622008,
    -0.910684,0.455342,-0.044658,
    0.422650,0.788675,0.288675,
    -0.577350,0.788675,1.288675
    };
  for (int iCtrl = BBodyGetNbCtrl(surf); iCtrl--;) {
    if (ISEQUALF(checkk[3 * iCtrl], surf->_ctrl[iCtrl]->_val[0]) == 
      false ||
      ISEQUALF(checkk[3 * iCtrl + 1], surf->_ctrl[iCtrl]->_val[1]) == 
      false ||
      ISEQUALF(checkk[3 * iCtrl + 2], surf->_ctrl[iCtrl]->_val[2]) == 
      false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "BBodyRotAxisOrigin failed");
      PBErrCatch(BCurveErr);
    }
  }
  BBodyRotAxisStart(surf, &axis, theta);
  float checkl[12] = {
    -0.244017,0.122008,0.622008,
    -1.154700,-0.211325,0.866026,
    -0.488034,1.032692,0.955342,
    0.089317,-0.122008,1.532692
    };
  for (int iCtrl = BBodyGetNbCtrl(surf); iCtrl--;) {
    if (ISEQUALF(checkl[3 * iCtrl], surf->_ctrl[iCtrl]->_val[0]) == 
      false ||
      ISEQUALF(checkl[3 * iCtrl + 1], surf->_ctrl[iCtrl]->_val[1]) == 
      false ||
      ISEQUALF(checkl[3 * iCtrl + 2], surf->_ctrl[iCtrl]->_val[2]) == 
      false) {
      BCurveErr->_type = PBErrTypeUnitTestFailed;
      sprintf(BCurveErr->_msg, "BBodyRotAxisStart failed");
      PBErrCatch(BCurveErr);
    }
  }
  BBodyFree(&surf);
  printf("UnitTestBBodyRotate OK\n");
}
  
void UnitTestBBody() {
  UnitTestBBodyCreateFree();
  UnitTestBBodyGetSet();
  UnitTestBBodyGet();
  UnitTestBBodyClone();
  UnitTestBBodyPrint();
  UnitTestBBodyLoadSave();
  UnitTestBBodyGetCenter();
  UnitTestBBodyTranslate();
  UnitTestBBodyScale();
  UnitTestBBodyGetBoundingBox();
  UnitTestBBodyRotate();
  printf("UnitTestBBody OK\n");
}

void UnitTestAll() {
  UnitTestBCurve();
  UnitTestSCurve();
  UnitTestSCurveIter();
  UnitTestBBody();
  printf("UnitTestAll OK\n");
}

int main() {
  UnitTestAll();
  // Return success code
  return 0;
}

