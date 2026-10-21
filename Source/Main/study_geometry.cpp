#include "study_geometry.h"
//#include <tool_cstring.h>
//#include <Windows.h>
//#include <op_shell_parameter.h>
//#include <array>
#include <action_solid.h>
#include <test_window.h>
//#include <mb_vector3d.h>
//#include <mb_operation_result.h>
//#include <mb_data.h>
#include <action_polymesh.h>
//#include <mesh.h>
#include <op_polymesh_parameter.h>
//#include <test_edit_space.h>
//#include <polymesh.h>
//#include <mesh_provider.h>
#include <polymesh_topo.h>

using namespace c3d;

SolidSPtr create_block(double w, double l, double h, const MbPlacement3D& place, int id) {
    MbSNameMaker nameMaker(id, MbSNameMaker::i_SideNone, 20);
    MbElementarySolidParams params(nameMaker);

    params.InitBlock(place, w, l, h, 0.0, MbElementarySolidParams::BlockPointType::faceCenter);

    MbElementarySolidResults results{};
    if (::ElementarySolid(params, results) == rt_Success) {
        return results.GetResultSolid();
    }
    return nullptr;
}

SolidSPtr create_cylinder(double radius, double height, const MbPlacement3D& place, int id) {
    MbSNameMaker nameMaker(id, MbSNameMaker::i_SideNone, 20);
    MbElementarySolidParams params(nameMaker);

    params.InitCylinder(place, radius, height,height);

    MbElementarySolidResults results{};
    if (::ElementarySolid(params, results) == rt_Success) {
        return results.GetResultSolid();
    }
    return nullptr;
}

SolidSPtr create_UN(SolidSPtr& l_operand_figure, SolidSPtr& r_operand_figure) {
    MbSolid* left_Solid = static_cast<MbSolid*>(&l_operand_figure->Duplicate());
    MbSolid* right_Solid = static_cast<MbSolid*>(&r_operand_figure->Duplicate());

    RPArray<MbSolid> wrkSolids(2, 1);
    wrkSolids.push_back(left_Solid);
    wrkSolids.push_back(right_Solid);

    MbSNameMaker operNames(ct_UnionSolid, MbSNameMaker::i_SideNone, 0);
    bool isArray = false;
    MbSolid* unionSolid = nullptr;
    RPArray<MbSolid> notGluedSolids;

    ::UnionSolid(wrkSolids, cm_Same, true, operNames, isArray, unionSolid, &notGluedSolids);

    if (unionSolid == nullptr) {
        ::UnionSolid(wrkSolids, operNames, unionSolid);
   
    }

    if (unionSolid != nullptr) {
        return SolidSPtr(unionSolid);
    }

    delete left_Solid;
    delete right_Solid;

    return nullptr;
}

// построение скругления для Block(20.0, 800.0, 20.0)
SolidSPtr make_fillet(SolidSPtr& figure) {
    if (figure == nullptr) {
        return nullptr;
    }
    c3d::EdgesSPtrVector baseEdges{};

    figure->GetEdges(baseEdges);

    SmoothValues c3dDistanceParametrs{};

    // @warning 21.10.26 11:11 Позднее поля будут перенесены в закрытую секцию, будет необходима редакция кода!!!

    c3dDistanceParametrs.InitEquidistantFillet(5.0, 5.0); 

    c3dDistanceParametrs.SetBegLength() = 100.0;
    c3dDistanceParametrs.SetEndLength() = 100.0;

    MbSNameMaker filletMaker(ct_FullFilletSolid, MbSNameMaker::i_SideNone, 0);
    c3d::FacesSPtrVector facesAdjacentToEdges{}; 

    MbShellFilletValues c3dFilleParametrs(baseEdges, facesAdjacentToEdges, c3dDistanceParametrs, filletMaker);
    c3d::SolidSPtr filletResult{};

    MbResultType filletCode = ::FilletSolid(*figure, cm_Copy, c3dFilleParametrs, filletResult);

    if (filletCode == rt_Success && filletResult != nullptr) {
          return(filletResult);
    }
       
    return nullptr;
}

MbItem* create_poly_model_triang(SolidSPtr& figure){
    MbStepData obj;
    MbFormNote form;
    if (figure == nullptr) {
        return nullptr;
    }
    MbItem* meshResult = figure->CreateMesh(obj, form, nullptr);
    if (meshResult != nullptr) {
        return meshResult;
    }
    return nullptr;
}

void study_area::create_specific_initial_scene(WorkWindow& window) {

    // 1. Построение элементарных тел

    // 1.1 Построение параллелепипеда
    MbCartPoint3D place1 = MbCartPoint3D(800.0, 0.0, 0.0);
    MbPlacement3D block_place(place1);
    SolidSPtr block = create_block(20.0, 800, 20.0, block_place, 1);
    if (block) {
        window.AddItem(*block, nullptr, true);
    }

    // 1.2 Построение цилиндра
    MbCartPoint3D place2 = MbCartPoint3D(0.0, 800.0, 0.0);
    MbPlacement3D cylinder_place(place2);
    SolidSPtr cylinder = create_cylinder(20, 20, cylinder_place, 2);
    if (cylinder) {
        window.AddItem(*cylinder, nullptr, true);
    }

    // 2.0  Построение тела с помощью булевой операции
    
    // 2.1 Первое тело опреции
    MbCartPoint3D place3 = MbCartPoint3D(30.0, 0.0, 0.0);
    MbPlacement3D block_operand_place(place3);
    SolidSPtr block_operand = create_block(20.0, 800, 20.0, block_operand_place, 3);
   

    // 2.2 Второе тело опреции
    MbCartPoint3D place4 = MbCartPoint3D(30.0, 0.0, 0.0);
    MbPlacement3D cylinder_operand_place(place4);
    SolidSPtr cylinder_operand = create_cylinder(20, 20, cylinder_operand_place, 4);
   

    // 2.3 Применение опреации к операндам
    
    if (SolidSPtr operand_result = create_UN(block_operand, cylinder_operand)) {
        window.AddItem(*operand_result, nullptr, true);
    }
    
    // 3.0 Построение полигональной модели при помощи триангуляции

    // 3.1 Построение базовго тела для триангуляции

    MbCartPoint3D place5 = MbCartPoint3D(60.0, 0.0, 800.0);
    MbPlacement3D block_poly_triag_place(place5);

    // 3.2 Создание полигональной модели при помощи триангуляции на основе базового тела

    SolidSPtr block_poly_triag = create_block(20.0, 800, 20.0, block_poly_triag_place, 5);
    MbItem* triag_mesh = create_poly_model_triang(block_poly_triag);
    if (triag_mesh != nullptr) {
        window.AddItem(*triag_mesh, nullptr, true);
    }

    // 4.0 Создание полигональной модели тела с помощью операции ремешинга

    MbCartPoint3D place6 = MbCartPoint3D(0.0, 900.0, 880.0);
    MbPlacement3D block_poly_remesh_place(place6);

    MbItem* meshItem = triag_mesh;

    if (meshItem != nullptr && meshItem->IsA() == st_Mesh) {

        MbMesh* originalMesh = static_cast<MbMesh*>(meshItem);

        MbRemeshParams obj{};
        c3d::IndicesVector fixedEdges{};
        obj.InitIncrementalUniform(2.0, fixedEdges, 45.0);

        MbPolymeshTopo* topo = MbPolymeshTopo::Create(*originalMesh, 0.001);

        MbPolymesh mesh(topo);

        MbResultType res = ::Remesh(obj, mesh);

        if (res == rt_Success) {
            MbMesh* remeshedMesh = new MbMesh();
            MbStepData stepData;
            MbFormNote note;
            MbMatrix3D transformMatrix;
            
            mesh.CalculateMesh(stepData, note, *remeshedMesh);
            block_poly_remesh_place.GetMatrixInto(transformMatrix);

            remeshedMesh->Transform(transformMatrix);
            window.AddItem(*remeshedMesh, nullptr, true);
        }

    }

}


