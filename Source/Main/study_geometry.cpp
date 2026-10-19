#include "study_geometry.h"
//#include <debugapi.h>
#include <tool_cstring.h>
#include <Windows.h>
#include <op_shell_parameter.h>
#include <array>
#include <action_solid.h>
#include <test_window.h>
#include <mb_vector3d.h>
using namespace c3d;

SolidSPtr CreateBlock(double w, double l, double h, const MbPlacement3D& place, int id) {
    MbSNameMaker nameMaker(id, MbSNameMaker::i_SideNone, 20);
    MbElementarySolidParams params(nameMaker);

    params.InitBlock(place, w, l, h, 0.0, MbElementarySolidParams::BlockPointType::faceCenter);

    MbElementarySolidResults results{};
    if (::ElementarySolid(params, results) == rt_Success) {
        return results.GetResultSolid();
    }
    return nullptr;
}

SolidSPtr CreateCylinder(double radius, double height, const MbPlacement3D& place, int id) {
    MbSNameMaker nameMaker(id, MbSNameMaker::i_SideNone, 20);
    MbElementarySolidParams params(nameMaker);

    params.InitCylinder(place, radius, height,height);

    MbElementarySolidResults results{};
    if (::ElementarySolid(params, results) == rt_Success) {
        return results.GetResultSolid();
    }
    return nullptr;
}



void study_area::create_specific_initial_scene(WorkWindow& window) {

    // 1. Построение элементарных тел

    // 1.1 Построение параллелепипеда
    MbPlacement3D place1;
    if (SolidSPtr block = CreateBlock(40, 50, 60, place1, 1)) {
        window.AddItem(*block, nullptr, true);
    }

    // 1.2 Построение цилиндра
    MbCartPoint3D place2 = MbCartPoint3D(100.0, 0.0, 0.0);
    MbPlacement3D cylinder_place(place2);
    if (SolidSPtr cylinder = CreateCylinder(20, 20, cylinder_place, 2)) {
        window.AddItem(*cylinder, nullptr, true);
    }

}
