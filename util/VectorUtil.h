//
// Created by wzx on 18-11-30.
//

#ifndef TUBE_CONNECT_VECTORUTIL_H
#define TUBE_CONNECT_VECTORUTIL_H

#include <vtkMath.h>
#include <vtkDataArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataNormals.h>
#include <vtkCellData.h>
#include <vtkDataArray.h>
#include <array>

using std::array;

namespace VectorUtil {
    /**
     * get vector from point1 -> point2
     * @param point1: three components
     * @param point2: three components
     * @return the return vector
     */
    inline array<double, 3> getVector(const array<double, 3> &point1, const array<double, 3> &point2);

    /**
     * get angle of two vectors
     * @param vector1: three components
     * @param vector2: three components
     * @return deg
     */
    inline double getAngle(const array<double, 3> &vector1, const array<double, 3> &vector2);

    /**
     * reverse vector
     * @param vector: three components
     */
    inline void reverseVector(array<double, 3> &vector);

    /**
     *  regularize the vector
     * @param vector: three components
     */
    inline void regularize(array<double, 3> &vector);

    /**
     * if the cells in tube are dense, there will be some equal normals of cells. this function designed to get different vector
     * @param normals
     * @param vector: three components
     * @return
     */
    inline array<double, 3> getDifferentVector(vtkSmartPointer<vtkDataArray> normals, const array<double, 3> &vector);

    /**
     * compare two vector with 0.001 epsilon
     * @param vector1
     * @param vector2
     * @return
     */
    inline bool isEqual(const array<double, 3> &vector1, const array<double, 3> &vector2);

    /**
     * calculate normals of data's cells
     * @param data
     * @return three-components tuple
     */
    inline vtkSmartPointer<vtkDataArray> calculateNormals(vtkSmartPointer<vtkPolyData> data);

}

namespace VectorUtil {
    array<double, 3> getVector(const array<double, 3> &point1, const array<double, 3> &point2) {
        array<double, 3> vector = {0};
        for (int i = 0; i < 3; i++) {
            vector[i] = point2[i] - point1[i];
        }
        return vector;
    }


    double getAngle(const array<double, 3> &vector1, const array<double, 3> &vector2) {
        array<double, 3> tempCross = {0};
        vtkMath::Cross(vector1.data(), vector2.data(), tempCross.data());
        double rad = atan2(vtkMath::Norm(tempCross.data()), vtkMath::Dot(vector1.data(), vector2.data()));
        double deg = vtkMath::DegreesFromRadians(rad);
        return deg;
    }

    void reverseVector(array<double, 3> &vector) {
        for (int i = 0; i < 3; i++) {
            vector[i] = -vector[i];
        }
    }

    void regularize(array<double, 3> &vector) {
        double norm = vtkMath::Norm(vector.data());
        vector[0] = vector[0] / norm;
        vector[1] = vector[1] / norm;
        vector[2] = vector[2] / norm;
    }

    array<double, 3> getDifferentVector(vtkSmartPointer<vtkDataArray> normals, const array<double, 3> &vector) {

        array<double, 3> diffVector = {0};
        array<double, 3> normal = {0};
        for (int i = 0; i < normals->GetNumberOfTuples(); i++) {
            normals->GetTuple(i, normal.data());
            if (!VectorUtil::isEqual(normal, vector)) {
                normals->GetTuple(i, diffVector.data());
            }
        }
        return diffVector;
    }

    bool isEqual(const array<double, 3> &vector1, const array<double, 3> &vector2) {
        double epsilon = 0.001;

        if (abs(vector1[0] - vector2[0]) < epsilon &&
            abs(vector1[1] - vector2[1]) < epsilon &&
            abs(vector1[2] - vector2[2]) < epsilon) {
            return true;
        } else {

            return false;
        }
    }

    vtkSmartPointer<vtkDataArray> calculateNormals(vtkSmartPointer<vtkPolyData> data) {
        auto normalFilter = vtkSmartPointer<vtkPolyDataNormals>::New();
        normalFilter->SetInputData(data);
        normalFilter->SetComputeCellNormals(1);
        normalFilter->Update();
        return normalFilter->GetOutput()->GetCellData()->GetNormals();

    }
}

#endif //TUBE_CONNECT_VECTORUTIL_H
