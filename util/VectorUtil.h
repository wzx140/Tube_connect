//
// Created by wzx on 18-11-30.
//

#ifndef TUBE_CONNECT_VECTORUTIL_H
#define TUBE_CONNECT_VECTORUTIL_H

#include <vtkMath.h>
#include <vtkDataArray.h>

namespace VectorUtil {
    /**
     * get vector from point1 -> point2
     * @param point1: three components
     * @param point2: three components
     * @param vector: the return vector
     */
    inline void getVector(const double point1[], const double point2[], double vector[]);

    /**
     * get angle of two vectors
     * @param vector1: three components
     * @param vector2: three components
     * @return deg
     */
    inline double getAngle(double vector1[], double vector2[]);

    /**
     * reverse vector
     * @param vector: three components
     */
    inline void reverseVector(double vector[]);

    /**
     *  regularize the vector
     * @param vector: three components
     */
    inline void regularize(double vector[]);

    /**
     * if the cells in tube are dense, there will be some equal normals of cells. this function designed to get different vector
     * @param normals
     * @param vector: three components
     * @return
     */
    inline double *getDifferentVector(vtkSmartPointer<vtkDataArray> normals, double vector[]);

    /**
     * compare two vector with 0.001 epsilon
     * @param vector1
     * @param vector2
     * @return
     */
    inline bool isEqual(double vector1[], double vector2[]);

}

namespace VectorUtil {
    void getVector(const double *point1, const double *point2, double vector[]) {
        for (int i = 0; i < 3; i++) {
            vector[i] = point2[i] - point1[i];
        }

    }

    double getAngle(double *vector1, double *vector2) {
        double tempCross[3];
        vtkMath::Cross(vector1, vector2, tempCross);
        double rad = atan2(vtkMath::Norm(tempCross), vtkMath::Dot(vector1, vector2));
        double deg = vtkMath::DegreesFromRadians(rad);
        return deg;
    }

    void reverseVector(double *vector) {
        for (int i = 0; i < 3; i++) {
            vector[i] = -vector[i];
        }
    }

    void regularize(double *vector) {
        double norm = vtkMath::Norm(vector);
        vector[0] = vector[0] / norm;
        vector[1] = vector[1] / norm;
        vector[2] = vector[2] / norm;
    }

    double *getDifferentVector(vtkSmartPointer<vtkDataArray> normals, double vector[]) {

        for (int i = 0; i < normals->GetNumberOfTuples(); i++) {
            if (!VectorUtil::isEqual(normals->GetTuple3(i), vector)) {
                return normals->GetTuple3(i);
            }
        }
        return nullptr;
    }

    bool isEqual(double *vector1, double *vector2) {
        double epsilon = 0.001;

        if (abs(vector1[0] - vector2[0]) < epsilon &&
            abs(vector1[1] - vector2[1]) < epsilon &&
            abs(vector1[2] - vector2[2]) < epsilon) {
            return true;
        } else {

            return false;
        }
    }
}

#endif //TUBE_CONNECT_VECTORUTIL_H
