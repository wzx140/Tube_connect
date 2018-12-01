//
// Created by wzx on 18-11-30.
//

#ifndef TUBE_CONNECT_CELLUTIL_H
#define TUBE_CONNECT_CELLUTIL_H

#include <vtkSmartPointer.h>
#include <vtkDataArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataNormals.h>
#include <vtkCellData.h>

namespace CellUtil {

    /**
     * calculate normals of data's cells
     * @param data
     * @return three-components tuple
     */
    inline vtkSmartPointer<vtkDataArray> calculateNormals(vtkSmartPointer<vtkPolyData> data);

}

namespace CellUtil {

    vtkSmartPointer<vtkDataArray> calculateNormals(vtkSmartPointer<vtkPolyData> data) {
        auto normalFilter = vtkSmartPointer<vtkPolyDataNormals>::New();
        normalFilter->SetInputData(data);
        normalFilter->SetComputeCellNormals(1);
        normalFilter->Update();
        return normalFilter->GetOutput()->GetCellData()->GetNormals();

    }
}


#endif //TUBE_CONNECT_CELLUTIL_H
