﻿#pragma once
#pragma once
#include <Matrix4x4.h>
#include <Vector3.h>

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
Vector3 Transform_Move(const Vector3& translate, const Vector3& move);

// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

Matrix4x4 Multiply(const Matrix4x4 m1, const Matrix4x4 m2);

Matrix4x4 MakeRotateXMatrix(float radian);

Matrix4x4 MakeRotateYMatrix(float radian);

Matrix4x4 MakeRotateZMatrix(float radian);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

Matrix4x4 MakeOrthographicMatrix(
    float left, float top, float right, float bottom, float nearClip, float farClip);

Matrix4x4 Inverse(const Matrix4x4& m);

Matrix4x4 MakeViewportMatrix(
    float left, float top, float width, float height, float minDepth, float maxDepth);

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

Vector3 Normalize(const Vector3& v);

Vector3 Add(const Vector3& posa, const Vector3& posb);

// 線形補間
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

Vector3 SLerp(const Vector3& v1, const Vector3& v2, float t);

// 内績
float Dot(const Vector3& v1, const Vector3& v2);
