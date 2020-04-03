 /* FILE NAME: TS5_VEC.H
  * PROGRAMMER: TS5 
  * DATE: 07.06.2010
  * PURPOSE: 
  *          
  */
#ifndef _VEC_H_
#define _VEC_H_

#include <math.h>
#include "TS5_ANIM.H"

#define PI 3.14159265358979323846
#define D2R(A) ((A) * (PI / 180.0))
#define Degree2Radian(a) D2R(a)

/* Объявление основного вещественного типа */
typedef double DBL;
/* Объявлеие векторного типа */
typedef struct tagVEC
{
  DBL X, Y, Z;
} VEC;
/* Объявление матричного типа */
typedef struct tagMATR
{
  DBL A[4][4];
} MATR;

/*********************************************************************************************************************/
/* Вектора */

/* Установка вектора */
__inline VEC VecSet( DBL X, DBL Y, DBL Z )
{
  VEC V;

  V.X = X;
  V.Y = Y;
  V.Z = Z;
  return V;
} /* End of 'VecSet' function */


/* Сложение векторов */
__inline VEC VecAddVec( VEC V1, VEC V2 )
{ 
  VEC V;

  V.X = V1.X + V2.X;
  V.Y = V1.Y + V2.Y;
  V.Z = V1.Z + V2.Z;

  return V;
}/* End of 'VecAddVec' function */
__inline VEC VecAddVecEq( VEC *VRes, VEC V2 )
{
  VRes->X += V2.X;
  VRes->Y += V2.Y;
  VRes->Z += V2.Z;
  return *VRes;
}/* End of 'VecAddVecEq' function */


/* Вычитание векторов */
__inline VEC VecSubVec( VEC V1, VEC V2 )
{
  VEC V;

  V.X = V1.X - V2.X;
  V.Y = V1.Y - V2.Y;
  V.Z = V1.Z - V2.Z;

  return V;
}/* End of 'VecSubVec' function */
__inline VEC VecSubVecEq( VEC *VRes, VEC V2 )
{
  VRes->X -= V2.X;
  VRes->Y -= V2.Y;
  VRes->Z -= V2.Z;
  return *VRes;
}/* End of 'VecSubVecEq' function */


/* Умножение векторов */
__inline VEC VecMulNum( VEC V1, DBL N )          
{
  VEC V;

  V.X = V1.X * N;
  V.Y = V1.Y * N;
  V.Z = V1.Z * N;

  return V;
}/* End of 'VecMulNum' function */
__inline VEC VecMulNumEq( VEC *VRes, DBL N )
{
  VRes->X *= N;
  VRes->Y *= N;
  VRes->Z *= N;
  return *VRes;
}/* End of 'VecMulNumEq' function */


/* Деление векторов */
__inline VEC VecDivNum( VEC V1, DBL N )
{ 
  VEC V;

  V.X = V1.X / N;
  V.Y = V1.Y / N;
  V.Z = V1.Z / N;

  return V;
}/* End of 'VecDivNum' function */
__inline VEC VecDivNumEq( VEC *VRes, DBL N )
{
  VRes->X /= N;
  VRes->Y /= N;
  VRes->Z /= N;
  return *VRes;
}/* End of 'VecDivNumEq' function */


VEC VecNeg( VEC V );
VEC VecNegEq( VEC *VRes );


DBL VecDotVec( VEC V1, VEC V2 );
VEC VecCrossVec( VEC V1, VEC V2 );

DBL VecLen2( VEC V );
DBL VecLen( VEC V );

VEC VecNormalize( VEC V );
VEC VecNormalizeEq( VEC *VRes );

__inline VEC VecMulMatr( VEC V, MATR M )
{
  VEC v;

  v.X = V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0];
  v.Y = V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1];
  v.Z = V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2];

  return v;
}
__inline VEC VecMulMatrEq( VEC *V, MATR M )
{
  DBL v[2];

  v[0] = V->X * M.A[0][0] + V->Y * M.A[1][0] + V->Z * M.A[2][0];
  v[1] = V->X * M.A[0][1] + V->Y * M.A[1][1] + V->Z * M.A[2][1];
  V->Z = V->X * M.A[0][2] + V->Y * M.A[1][2] + V->Z * M.A[2][2];
  V->X = v[0];
  V->Y = v[1];

  return *V;
}

/*************************************************************************************************************************/
/* Матрицы */

/* единичная матрица */
static MATR UnitMatrix =
{
  {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  }
};

/* Установка единичной матрицы */
__inline MATR MatrSetUnit( VOID )
{
  return UnitMatrix;
}/* End of 'MatrSetUnit' function */
__inline MATR MatrSetUnitEq( MATR *M )
{
  return *M = UnitMatrix;
}/* End of 'MatrSetUnitEq' function */


/* Матрица Параллельного переноса */
__inline MATR MatrSetTranslate( DBL Dx, DBL Dy, DBL Dz )
{
  MATR m = UnitMatrix;

  m.A[3][0] = Dx;
  m.A[3][1] = Dy;
  m.A[3][2] = Dz;
  return m;
}/* End of 'MatrTranslate' function */
__inline MATR MatrSetTranslateEq( MATR *M, DBL Dx, DBL Dy, DBL Dz )
{
  *M = UnitMatrix;
  M->A[3][0] = Dx;
  M->A[3][1] = Dy;
  M->A[3][2] = Dz;
  return *M;
}/* End of 'MatrTranslateEq' function */


/* Матрица Масштабирования */
__inline MATR MatrSetScale( DBL Sx, DBL Sy, DBL Sz )
{
  MATR m;  

  m.A[0][0] = Sx;
  m.A[0][1] = 0;
  m.A[0][2] = 0;

  m.A[1][0] = 0;
  m.A[1][1] = Sy;
  m.A[1][2] = 0;

  m.A[2][0] = 0;
  m.A[2][1] = 0;
  m.A[2][2] = Sz;

  return m;
}/* End of 'MatrSetScale' function */
__inline MATR MatrSetScaleEq( MATR *M, DBL Sx, DBL Sy, DBL Sz )
{ 
  M->A[0][0] = Sx;
  M->A[0][1] = 0;
  M->A[0][2] = 0;

  M->A[1][0] = 0;
  M->A[1][1] = Sy;
  M->A[1][2] = 0;

  M->A[2][0] = 0;
  M->A[2][1] = 0;
  M->A[2][2] = Sz;

  return *M;     
}/* End of 'MatrSetScaleEq' function */


/* Поворот вокруг оси Х */
__inline MATR MatrSetRotateX( DBL si, DBL co )
{
  MATR m;

  m.A[0][0] = 1;
  m.A[0][1] = 0;
  m.A[0][2] = 0;

  m.A[1][0] = 0;
  m.A[1][1] = co;
  m.A[1][2] = si;

  m.A[2][0] = 0;
  m.A[2][1] = -si;
  m.A[2][2] = co;

  return m;
}/* End of 'MatrSetRotateX' function */
__inline MATR MatrSetRotateXEq( MATR *M, DBL si, DBL co )
{ 
  M->A[0][0] = 1;
  M->A[0][1] = 0;
  M->A[0][2] = 0;

  M->A[1][0] = 0;
  M->A[1][1] = co;
  M->A[1][2] = si;

  M->A[2][0] = 0;
  M->A[2][1] = -si;
  M->A[2][2] = co;

  return *M;
}/* End of 'MatrSetRotateXEq' function */


/* Поворот вокруг оси Y */
__inline MATR MatrSetRotateY( DBL si, DBL co )
{
  MATR m;

  m.A[0][0] = co;
  m.A[0][1] = 0;
  m.A[0][2] = -si;

  m.A[1][0] = 0;
  m.A[1][1] = 1;
  m.A[1][2] = 0;

  m.A[2][0] = si;
  m.A[2][1] = 0;
  m.A[2][2] = co;

  return m;
}/* End of 'MatrSetRotateY' function */
__inline MATR MatrSetRotateYEq( MATR *M, DBL si, DBL co )
{  
  M->A[0][0] = co;
  M->A[0][1] = 0;
  M->A[0][2] = -si;

  M->A[1][0] = 0;
  M->A[1][1] = 1;
  M->A[1][2] = 0;

  M->A[2][0] = si;
  M->A[2][1] = 0;
  M->A[2][2] = co;

  return *M;
}/* End of 'MatrSetRotateYEq' function */


/* Поворот вокруг оси Z */
__inline MATR MatrSetRotateZ( DBL si, DBL co )
{     
  MATR m;

  m.A[0][0] = co;
  m.A[0][1] = si;
  m.A[0][2] = 0;

  m.A[1][0] = -si;
  m.A[1][1] = co;
  m.A[1][2] = 0;

  m.A[2][0] = 0;
  m.A[2][1] = 0;
  m.A[2][2] = 1;

  return m;
}/* End of 'MatrSetRotateZ' function */
__inline MATR MatrSetRotateZEq( MATR *M, DBL si, DBL co )
{  
  M->A[0][0] = co;
  M->A[0][1] = si;
  M->A[0][2] = 0;

  M->A[1][0] = -si;
  M->A[1][1] = co;
  M->A[1][2] = 0;

  M->A[2][0] = 0;
  M->A[2][1] = 0;
  M->A[2][2] = 1;

  return *M;
}/* End of 'MatrSetRotateZEq' function */


/* Матрица поворота вокруг произвольной оси */
__inline MATR MatrSetRotate( DBL si, DBL co,
                DBL X, DBL Y, DBL Z )
{
  DBL len;
  MATR m = UnitMatrix;


  /* определили длину вектора */
  len = X * X + Y * Y + Z * Z;
  if (len == 0)
    len = 1;
  else
    if (len != 1)
      len = sqrt(len);

  X *= si / len;
  Y *= si / len;
  Z *= si / len;

  m.A[0][0] = 1 - 2 * (Y * Y + Z * Z);
  m.A[0][1] = 2 * X * Y - 2 * co * Z;
  m.A[0][2] = 2 * co * Y + 2 * X * Z;

  m.A[1][0] = 2 * X * Y + 2 * co * Z;
  m.A[1][1] = 1 - 2 * (X * X + Z * Z);
  m.A[1][2] = 2 * Y * Z - 2 * co * X;

  m.A[2][0] = 2 * X * Z - 2 * co * Y;
  m.A[2][1] = 2 * co * X + 2 * Y * Z;
  m.A[2][2] = 1 - 2 * (X * X + Y * Y);

  return m;
}/* End of 'MatrSetRotate' function */    
__inline MATR MatrSetRotateEq( MATR *M, DBL si, DBL co,
                      DBL X, DBL Y, DBL Z )
{
  DBL len;
  *M = UnitMatrix;

  /* определили длину вектора */
  len = X * X + Y * Y + Z * Z;
  if (len == 0)
    len = 1;
  else
    if (len != 1)
      len = sqrt(len);

  X *= si / len;
  Y *= si / len;
  Z *= si / len;

  M->A[0][0] = 1 - 2 * (Y * Y + Z * Z);
  M->A[0][1] = 2 * X * Y - 2 * co * Z;
  M->A[0][2] = 2 * co * Y + 2 * X * Z;

  M->A[1][0] = 2 * X * Y + 2 * co * Z;
  M->A[1][1] = 1 - 2 * (X * X + Z * Z);
  M->A[1][2] = 2 * Y * Z - 2 * co * X;

  M->A[2][0] = 2 * X * Z - 2 * co * Y;
  M->A[2][1] = 2 * co * X + 2 * Y * Z;
  M->A[2][2] = 1 - 2 * (X * X + Y * Y);

  return *M;
}/* End of 'MatrSetRotateEq' function */


/* Перемножение матриц */
__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  MATR r;

  r.A[0][0] = M1.A[0][0] * M2.A[0][0] + M1.A[0][1] * M2.A[1][0] + M1.A[0][2] * M2.A[2][0];
  r.A[0][1] = M1.A[0][0] * M2.A[0][1] + M1.A[0][1] * M2.A[1][1] + M1.A[0][2] * M2.A[2][1];
  r.A[0][2] = M1.A[0][0] * M2.A[0][2] + M1.A[0][1] * M2.A[1][2] + M1.A[0][2] * M2.A[2][2];
  r.A[0][3] = 0;

  r.A[1][0] = M1.A[1][0] * M2.A[0][0] + M1.A[1][1] * M2.A[1][0] + M1.A[1][2] * M2.A[2][0];
  r.A[1][1] = M1.A[1][0] * M2.A[0][1] + M1.A[1][1] * M2.A[1][1] + M1.A[1][2] * M2.A[2][1];
  r.A[1][2] = M1.A[1][0] * M2.A[0][2] + M1.A[1][1] * M2.A[1][2] + M1.A[1][2] * M2.A[2][2];
  r.A[1][3] = 0;

  r.A[2][0] = M1.A[2][0] * M2.A[0][0] + M1.A[2][1] * M2.A[1][0] + M1.A[2][2] * M2.A[2][0];
  r.A[2][1] = M1.A[2][0] * M2.A[0][1] + M1.A[2][1] * M2.A[1][1] + M1.A[2][2] * M2.A[2][1];
  r.A[2][2] = M1.A[2][0] * M2.A[0][2] + M1.A[2][1] * M2.A[1][2] + M1.A[2][2] * M2.A[2][2];
  r.A[2][3] = 0;

  r.A[3][0] = M1.A[3][0] * M2.A[0][0] + M1.A[3][1] * M2.A[1][0] + M1.A[3][2] * M2.A[2][0] + M2.A[3][0];
  r.A[3][1] = M1.A[3][0] * M2.A[0][1] + M1.A[3][1] * M2.A[1][1] + M1.A[3][2] * M2.A[2][1] + M2.A[3][1];
  r.A[3][2] = M1.A[3][0] * M2.A[0][2] + M1.A[3][1] * M2.A[1][2] + M1.A[3][2] * M2.A[2][2] + M2.A[3][2];
  r.A[3][3] = 1;

  return r;
}/* End of 'MatrMulMatr' function */
__inline MATR MatrMulMatrEq( MATR *MRes, MATR M2 )
{
  MRes->A[0][0] = MRes->A[0][0] * M2.A[0][0] + MRes->A[0][1] * M2.A[1][0] + MRes->A[0][2] * M2.A[2][0];
  MRes->A[0][1] = MRes->A[0][0] * M2.A[0][1] + MRes->A[0][1] * M2.A[1][1] + MRes->A[0][2] * M2.A[2][1];
  MRes->A[0][2] = MRes->A[0][0] * M2.A[0][2] + MRes->A[0][1] * M2.A[1][2] + MRes->A[0][2] * M2.A[2][2];
  MRes->A[0][3] = 0;

  MRes->A[1][0] = MRes->A[1][0] * M2.A[0][0] + MRes->A[1][1] * M2.A[1][0] + MRes->A[1][2] * M2.A[2][0];
  MRes->A[1][1] = MRes->A[1][0] * M2.A[0][1] + MRes->A[1][1] * M2.A[1][1] + MRes->A[1][2] * M2.A[2][1];
  MRes->A[1][2] = MRes->A[1][0] * M2.A[0][2] + MRes->A[1][1] * M2.A[1][2] + MRes->A[1][2] * M2.A[2][2];
  MRes->A[1][3] = 0;

  MRes->A[2][0] = MRes->A[2][0] * M2.A[0][0] + MRes->A[2][1] * M2.A[1][0] + MRes->A[2][2] * M2.A[2][0];
  MRes->A[2][1] = MRes->A[2][0] * M2.A[0][1] + MRes->A[2][1] * M2.A[1][1] + MRes->A[2][2] * M2.A[2][1];
  MRes->A[2][2] = MRes->A[2][0] * M2.A[0][2] + MRes->A[2][1] * M2.A[1][2] + MRes->A[2][2] * M2.A[2][2];
  MRes->A[2][3] = 0;

  MRes->A[3][0] = MRes->A[3][0] * M2.A[0][0] + MRes->A[3][1] * M2.A[1][0] + MRes->A[3][2] * M2.A[2][0] + M2.A[3][0];
  MRes->A[3][1] = MRes->A[3][0] * M2.A[0][1] + MRes->A[3][1] * M2.A[1][1] + MRes->A[3][2] * M2.A[2][1] + M2.A[3][1];
  MRes->A[3][2] = MRes->A[3][0] * M2.A[0][2] + MRes->A[3][1] * M2.A[1][2] + MRes->A[3][2] * M2.A[2][2] + M2.A[3][2];
  MRes->A[3][3] = 1;

  return *MRes;
}/* End of 'MatrMulMatrEq' function */


/* Транспонирование матрицы */
__inline MATR MatrTranspose( MATR M )
{
  MATR m;

  m.A[0][0] = M.A[0][0];
  m.A[0][1] = M.A[1][0];
  m.A[0][2] = M.A[2][0];
  m.A[0][3] = M.A[3][0];

  m.A[1][0] = M.A[0][1];
  m.A[1][1] = M.A[1][1];
  m.A[1][2] = M.A[2][1];
  m.A[1][3] = M.A[3][1];

  m.A[2][0] = M.A[0][2];
  m.A[2][1] = M.A[1][2];
  m.A[2][2] = M.A[2][2];
  m.A[2][3] = M.A[3][2];

  m.A[3][0] = M.A[0][3];
  m.A[3][1] = M.A[1][3];
  m.A[3][2] = M.A[2][3];
  m.A[3][3] = M.A[3][3];

  return m;
}/* End of 'MatrTranspose' function */
__inline MATR MatrTransposeEq( MATR *MRes )
{ 
  DBL m[6];
  
  m[0] = MRes->A[0][1];
  m[1] = MRes->A[0][2];
  m[2] = MRes->A[0][3];
  
  m[3] = MRes->A[1][2];
  m[4] = MRes->A[1][3];

  m[5] = MRes->A[2][3];  

  MRes->A[0][1] = MRes->A[1][0];
  MRes->A[0][2] = MRes->A[2][0];
  MRes->A[0][3] = MRes->A[3][0];

  MRes->A[1][0] = m[0]; 
  MRes->A[1][2] = MRes->A[2][1];
  MRes->A[1][3] = MRes->A[3][1];

  MRes->A[2][0] = m[1];
  MRes->A[2][1] = m[3];  
  MRes->A[2][3] = MRes->A[3][2];

  MRes->A[3][0] = m[2];
  MRes->A[3][1] = m[4];
  MRes->A[3][2] = m[5];   

  return *MRes;
}/* End of 'MatrTransposeEq' function */


/* Определитель 3х3 */
__inline DBL GetDeterm3x3( DBL A11, DBL A12, DBL A13,
                  DBL A21, DBL A22, DBL A23,
                  DBL A31, DBL A32, DBL A33 )
{
  return A11 * A22 * A33 +
         A12 * A23 * A31 +
         A13 * A21 * A32 -
         A11 * A23 * A32 -
         A12 * A21 * A33 -
         A13 * A22 * A31;
}/* End of 'GetDeterm3x3' function */


/* Обратная матрица */
__inline MATR MatrInverse( MATR M )
{
  DBL det =
    GetDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                 M.A[1][0], M.A[1][1], M.A[1][2],
                 M.A[2][0], M.A[2][1], M.A[2][2]);
  MATR r;

  if (det == 0)
    return UnitMatrix;

  /* строим присоединенную матрицу */
  r.A[0][0] = 
    GetDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                 M.A[2][1], M.A[2][2], M.A[2][3],
                 M.A[3][1], M.A[3][2], M.A[3][3]);
  r.A[1][0] = 
    -GetDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                  M.A[2][0], M.A[2][2], M.A[2][3],
                  M.A[3][0], M.A[3][2], M.A[3][3]);
  r.A[2][0] = 
    GetDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                 M.A[2][0], M.A[2][1], M.A[2][3],
                 M.A[3][0], M.A[3][1], M.A[3][3]);
  r.A[3][0] = 
    -GetDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                  M.A[2][0], M.A[2][1], M.A[2][2],
                  M.A[3][0], M.A[3][1], M.A[3][2]);

  r.A[0][1] = 
    -GetDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                  M.A[2][1], M.A[2][2], M.A[2][3],
                  M.A[3][1], M.A[3][2], M.A[3][3]);
  r.A[1][1] = 
    GetDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                 M.A[2][0], M.A[2][2], M.A[2][3],
                 M.A[3][0], M.A[3][2], M.A[3][3]);
  r.A[2][1] = 
    -GetDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                  M.A[2][0], M.A[2][1], M.A[2][3],
                  M.A[3][0], M.A[3][1], M.A[3][3]);
  r.A[3][1] = 
    GetDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                 M.A[2][0], M.A[2][1], M.A[2][2],
                 M.A[3][0], M.A[3][1], M.A[3][2]);

  r.A[0][2] = 
    GetDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                 M.A[1][1], M.A[1][2], M.A[1][3],
                 M.A[3][1], M.A[3][2], M.A[3][3]);
  r.A[1][2] = 
    -GetDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                  M.A[1][0], M.A[1][2], M.A[1][3],
                  M.A[3][0], M.A[3][2], M.A[3][3]);
  r.A[2][2] = 
    GetDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                 M.A[1][0], M.A[1][1], M.A[1][3],
                 M.A[3][0], M.A[3][1], M.A[3][3]);
  r.A[3][2] = 
    -GetDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                  M.A[1][0], M.A[1][1], M.A[1][2],
                  M.A[3][0], M.A[3][1], M.A[3][2]);

  /* делим на определитель */
  r.A[0][0] /= det;
  r.A[0][1] /= det;
  r.A[0][2] /= det;
  r.A[1][0] /= det;
  r.A[1][1] /= det;
  r.A[1][2] /= det;
  r.A[2][0] /= det;
  r.A[2][1] /= det;
  r.A[2][2] /= det;
  r.A[3][0] /= det;
  r.A[3][1] /= det;
  r.A[3][2] /= det;

  r.A[0][3] = 0;
  r.A[1][3] = 0;
  r.A[2][3] = 0;
  r.A[3][3] = 1;

  return r;
}/* End of 'MatrInverse' function */

#endif /* _VEC_H_ */