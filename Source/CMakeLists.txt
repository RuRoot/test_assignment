cmake_minimum_required(VERSION 3.24)

project(Test VERSION 2025.1.11805.8)

# Setup project-specified CMake modules.
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../CMake")
  message(STATUS "${PROJECT_NAME}: Adding CMake directory from the parent dir... Done.")
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../CMake")
endif()
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

if(NOT DEFINED Test_OUTPUT)  # Case of build flow as is given by readme.txt
  set(Test_OUTPUT "test")

  get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
  if(isMultiConfig)
    set_property(CACHE CMAKE_CONFIGURATION_TYPES PROPERTY VALUE "Release;Debug")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/..")
  else()
    if(NOT DEFINED CMAKE_BUILD_TYPE OR NOT CMAKE_BUILD_TYPE)
      set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    endif()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../${CMAKE_BUILD_TYPE}")
  endif()

  set(CMAKE_SUPPRESS_REGENERATION ON)
  option(MathTest_USING_UNICODE "Enable Unicode support" ON)
  option(MathTest_L10N "Enable localization" OFF)
  option(MathTest_QT "Use Qt Library" OFF)

  set(math_target "c3d")
  include_directories(${CMAKE_SOURCE_DIR}/../Include)
else()
  option(MathTest_WITH_QT5_PKG "Link test application with Qt5 conan package" OFF)
  # c3d kernel as a standalone conan package is used for linkage only if building test example source code with the conan-examples-usage.py recipe
  find_package(c3d QUIET)
endif()

if(UNIX)
  if(POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
  endif()
  set(MathTest_QT ON CACHE BOOL "Use Qt Library" FORCE)
endif()

if(NOT DEFINED MathTest_STANDARD_CXX)
  set(MathTest_STANDARD_CXX 2017 CACHE STRING "C++ standard")
endif()

if(MathTest_QT)
  if(MathTest_WITH_QT5_PKG)
    find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets OpenGL)
  else()
    find_package(Qt5Core)
    find_package(Qt5Gui)
    find_package(Qt5OpenGL)
  endif()
endif()

# Add Source-groups.

set(Create_SRC
  ./Create/test_constraint.cpp
  ./Create/test_arc.cpp
  ./Create/test_assembly.cpp
  ./Create/test_contour.cpp
  ./Create/test_curve.cpp
  ./Create/test_plane.h
  ./Create/test_line.cpp
  ./Create/test_multiline.cpp
  ./Create/test_point.cpp
  ./Create/test_curve3d.cpp
  ./Create/test_sheet.cpp
  ./Create/test_shell.cpp
  ./Create/test_map.cpp
  ./Create/test_solid.cpp
  ./Create/test_space.h
  ./Create/test_varsect.h
  ./Create/test_surface_.cpp
  ./Create/test_point3d.cpp
  ./Create/test_surface.cpp
  ./Create/test_user.cpp
  ./Create/test_multithreading.cpp
  ./Create/test_mesh.cpp
  ./Create/test_mesh.h
  )
source_group(Create FILES ${Create_SRC})

set(Edit_SRC
  ./Edit/test_edit_contour.cpp
  ./Edit/test_edit_curve.cpp
  ./Edit/test_edit_plane.h
  ./Edit/test_edit_multiline.cpp
  ./Edit/test_edit_curve3d.cpp
  ./Edit/test_edit_map.cpp
  ./Edit/test_edit_solid.cpp
  ./Edit/test_edit_space.cpp
  ./Edit/test_edit_space.h
  ./Edit/test_edit_surface.cpp
  ./Edit/test_edit_wireframe.cpp
  )
source_group(Edit FILES ${Edit_SRC})

set(Main_SRC
  ./Main/test_draw.cpp
  ./Main/test_draw.h
  ./Main/test_frame.h
  ./Main/test_frame1.cpp
  ./Main/test_frame2.cpp
  ./Main/test_frame3.cpp
  ./Main/test_service.cpp
  ./Main/test_service.h
  ./Main/test_variables.cpp
  ./Main/test_variables.h
 ./Main/study_geometry.h
 ./Main/study_geometry.cpp
  )
source_group(Main FILES ${Main_SRC})

set(Make_SRC
  ./Make/test_computation.cpp
  ./Make/test_computation.h
  ./Make/test_converter.cpp
  ./Make/test_converter.h
  ./Make/test_mates.cpp
  ./Make/test_mates.h
  ./Make/test_rendering.cpp
  ./Make/test_rendering.h
  ./Make/test_rendering_.cpp
  ./Make/test_rendering_context.cpp
  ./Make/test_rendering_context.h
  )
source_group(Make FILES ${Make_SRC})

set(Manager_SRC
  ./Manager/test_comanager.h
  ./Manager/test_comanager.cpp
  ./Manager/test_gr_draw.cpp
  ./Manager/test_gr_draw.h
  ./Manager/test_manager.cpp
  ./Manager/test_manager.h
  ./Manager/test_property.cpp
  ./Manager/test_property.h
  ./Manager/test_property_title.h
  ./Manager/test_style.h
  ./Manager/test_temporal_plane.cpp
  ./Manager/test_temporal_plane.h
  ./Manager/test_temporal.cpp
  ./Manager/test_temporal.h
  ./Manager/test_tree.cpp
  ./Manager/test_tree.h
  ./Manager/test_tree_filter.cpp
  ./Manager/test_tree_filter.h
  ./Manager/test_window.cpp
  ./Manager/test_window.h
  ./Manager/test_window_add.cpp
  ./Manager/test_window_find.cpp
  ./Manager/test_window_move.cpp
  )
source_group(Manager FILES ${Manager_SRC})

set(Samples_SRC
  ./Samples/test_samples.h
  ./Samples/test_sample_animation_of_bending.cpp
  ./Samples/test_sample_attributes.cpp
  ./Samples/test_sample_user_attributes.h
  ./Samples/test_sample_user_attributes.cpp
  ./Samples/test_sample_solid_elementary.cpp
  ./Samples/test_sample_solid_splitting.cpp
  ./Samples/test_sample_solids_classification.cpp
  ./Samples/test_sample_parametric_sketch.cpp
  ./Samples/test_sample_read_write_constraints.cpp
  ./Samples/test_sample_read_write_c3d_as_part.cpp
  ./Samples/test_sample_spinning_block.cpp
  ./Samples/test_sample_check_collisions.cpp
  ./Samples/test_sample_collision_detection.cpp
  ./Samples/test_sample_wireframe.cpp
  ./Samples/test_sample_converter.cpp
  ./Samples/test_sample_pmi.cpp
  ./Samples/test_sample_lofted_bend.cpp
  ./Samples/test_sample_dodecahedron.cpp
  ./Samples/test_sample_close_corner.cpp
  ./Samples/test_sample_several_bends.cpp
  ./Samples/test_sample_polymesh.cpp
  )
source_group(Samples FILES ${Samples_SRC})

if(MathTest_QT)
  set(QtTest_SRC
    ./Qt/test_main.cpp
    ./Qt/test_main_window.h
    ./Qt/test_main_window.cpp
    ./Qt/test_child_window.h
    ./Qt/test_child_window.cpp
    ./Qt/test_property_dialogs.h
    ./Qt/test_property_dialogs.cpp
    ./Qt/test_main_dialogs.h
    ./Qt/test_main_dialogs.cpp
  )
  source_group(QtTest FILES ${QtTest_SRC})

  set(MOC_Files
    ./Qt/test_main_window.h
    ./Qt/test_child_window.h
    ./Qt/test_property_dialogs.h
    ./Qt/test_main_dialogs.h
  )

  QT5_WRAP_CPP(MOC_Gui
    ${MOC_Files}
  )

  set(RCC_Files
    ./Qt/test.qrc
  )

  QT5_ADD_RESOURCES(RCC_Gui
    ${RCC_Files}
    )

else(MathTest_QT)
  set(WinTest_SRC
    ./Win/test.cpp
    ./Win/test.h
    ./Win/test.rc
    ./Win/test_dialogs.cpp
    ./Win/test_frame.cpp
    ./Win/test_prompt.h
    ./Win/test_set_filter.cpp
    ./Win/test_set_property.cpp
    ./Win/test_set_tree.cpp
    ./Win/test_std_afx.cpp
    ./Win/test_std_afx.h
    ./Win/test_window_graphic.cpp
  )
  source_group(WinTest FILES ${WinTest_SRC})
endif(MathTest_QT)

include_directories(
  ${VLD_INCLUDE_DIRS}
  ${CMAKE_INSTALL_PREFIX}/include/c3d
  ${Test_SOURCE_DIR}/Create
  ${Test_SOURCE_DIR}/Edit
  ${Test_SOURCE_DIR}/Main
  ${Test_SOURCE_DIR}/Make
  ${Test_SOURCE_DIR}/Manager
  ${Test_SOURCE_DIR}/Samples
  ${Test_SOURCE_DIR}/Win
  ${Test_SOURCE_DIR}/Qt
)
link_directories(
  "${CMAKE_SOURCE_DIR}/../$<CONFIG>"
)

if(MathTest_USING_UNICODE)
  add_definitions(
    -DUNICODE
    -D_UNICODE
    )
endif()

if(WIN32)
  add_definitions(
    -D_WIN32_WINNT=0X0502
    -D_CRT_SECURE_NO_WARNINGS
    )
endif()

if(MathTest_QT)
  add_definitions(
    -D__USE_QT__
    )
endif()

if(MathTest_WITH_VLD)
  add_definitions(
    -DENABLE_VLD
    )
endif()

# These strings are necessary to define version info and test app main window title.
include(c3d_version_info OPTIONAL RESULT_VARIABLE _version_module)
if(_version_module)
  message(STATUS "Module ${_version_module} was found")
  c3d_version_define(False)
else()
  message(STATUS "Module \'c3d_version_info\' was skipped")
endif()

if(MathTest_QT)
  if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS_INIT} /ENTRY:mainCRTStartup")
  endif()

  add_executable(${Test_OUTPUT} WIN32
    ${Create_SRC}
    ${Edit_SRC}
    ${Main_SRC}
    ${Make_SRC}
    ${Manager_SRC}
    ${Samples_SRC}
    ${QtTest_SRC}
    ${MOC_Gui}
    ${RCC_Gui}
    )
else()
  add_executable(${Test_OUTPUT} WIN32
    ${Create_SRC}
    ${Edit_SRC}
    ${Main_SRC}
    ${Make_SRC}
    ${Manager_SRC}
    ${Samples_SRC}
    ${Win_SRC}
    ${WinTest_SRC}
    )
endif()

# Set relevant C++ std.
include(c3d_cppstandard_support)
if(WIN32 AND MSVC)
  c3d_set_cpp_standard_msvc(${Test_OUTPUT} ${MathTest_STANDARD_CXX})
elseif(UNIX)
  c3d_set_cpp_standard(${Test_OUTPUT} ${MathTest_STANDARD_CXX})
  c3d_set_c_standard(${Test_OUTPUT})
endif()

# Set compiler and linker flags depending on the platform, OS, and compiler.
if(WIN32)
  if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG_INIT} -D_DEBUG -D_DRAWGI /Zi /openmp /W4 /MP ${CMAKE_CXX_FLAGS_DEBUG_OVERRIDE}")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE_INIT} -D_ITERATOR_DEBUG_LEVEL=0 /Zi /openmp /W4 /MP ${CMAKE_CXX_FLAGS_RELEASE_OVERRIDE}")
    set_property(TARGET ${Test_OUTPUT} APPEND PROPERTY LINK_FLAGS_RELEASE /DEBUG)
  endif()
elseif(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations -fno-operator-names -fvisibility=hidden")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated-declarations -fno-operator-names -fvisibility=hidden")
  if(${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
  elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
    endif()
  endif()
  if(MathTest_WITH_ADDRESS_SANITIZER OR MathTest_WITH_THREAD_SANITIZER OR MathTest_WITH_UB_SANITIZER)
    # Use sanitizers
    include(c3d_sanitizers OPTIONAL RESULT_VARIABLE _sanitizers_module)
    if(_sanitizers_module)
      message(STATUS "Module ${_sanitizers_module} was found")
      c3d_embed_sanitizer(${Test_OUTPUT})
    else()
      message(STATUS "Module \'c3d_sanitizers\' was skipped")
    endif()
  else()
    set(CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG_INIT} -D_DRAWGI")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE_INIT} -D_ITERATOR_DEBUG_LEVEL=0 -g")
    set(CMAKE_C_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG_INIT} -D_DRAWGI")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE_INIT} -D_ITERATOR_DEBUG_LEVEL=0 -g")
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_OVERRIDE}")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_OVERRIDE}")
endif()
include(print_cxx_c_flags OPTIONAL RESULT_VARIABLE _print_cxx_c_flags_module)
if(_print_cxx_c_flags_module)
  print_cxx_c_flags(${Test_OUTPUT})
else()
  message(STATUS "Test App Compile C++ Flags: " ${CMAKE_CXX_FLAGS})
  message(STATUS "Test App Compile C Flags: " ${CMAKE_C_FLAGS})
endif()

if(MathTest_QT)
  if(MathTest_WITH_QT5_PKG)
    target_link_libraries(
      ${Test_OUTPUT}
      Qt5::Core Qt5::Gui Qt5::Widgets Qt5::OpenGL
    )
  else()
    qt5_use_modules(${Test_OUTPUT} Core Gui OpenGL)
  endif()
endif()

if(WIN32)
  target_link_libraries(
    ${Test_OUTPUT}
    ${math_target}
    ${VLD_LIBRARIES}
    opengl32
    glu32
    comctl32
  )
else()
  find_package(OpenGL REQUIRED)
  set(THREAD_LIBRARY)
  if(${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD")
    set(THREAD_LIBRARY thr)
  endif()
  target_link_libraries(
    ${Test_OUTPUT}
    ${math_target}
    ${VLD_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLU_LIBRARY}
    ${THREAD_LIBRARY}
  )
endif()
if(NOT MathTest_WITH_QT5_PKG)
  target_link_libraries(
    ${Test_OUTPUT}
    ${Qt5Core_LIBRARIES}
    ${Qt5Gui_LIBRARIES}
    ${Qt5OpenGL_LIBRARIES}
  )
endif()

# Make optional since Test folder is packaged and distributed in-source separately.
include(strip_debug_symbols_support OPTIONAL RESULT_VARIABLE _strip_debug_symbols_module)
if(_strip_debug_symbols_module)
  message(STATUS "Module ${_strip_debug_symbols_module} was found")
  # Strip debug symbols before installation
  if(NOT MathTest_MATH_STATIC AND NOT MathTest_EMSCRIPTEN)
    strip_debug_symbols(${Test_OUTPUT} False ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
  endif()
else()
  message(STATUS "Module \'strip_debug_symbols_support\' was skipped")
endif()
install(TARGETS ${Test_OUTPUT}
        CONFIGURATIONS Release Debug
        RUNTIME DESTINATION $<CONFIG>
        ARCHIVE DESTINATION $<CONFIG>
)
if(MSVC)
  set_property(TARGET ${Test_OUTPUT} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}")
  if(NOT MathTest_MATH_STATIC)
    # TARGET_PDB_FILE is allowed only for targets with linker created artifacts.
    install(FILES $<TARGET_PDB_FILE:${Test_OUTPUT}>
            CONFIGURATIONS Release Debug
            DESTINATION $<CONFIG>
            OPTIONAL)
  endif()
endif()
if(UNIX)
  set(debug_sym_ext ".debug")
  if(APPLE)
    set(debug_sym_ext ".dSYM")
  endif()
  if(NOT MathTest_MATH_STATIC AND NOT MathTest_EMSCRIPTEN)
    install(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${Test_OUTPUT}${debug_sym_ext}
            CONFIGURATIONS Release Debug
            DESTINATION $<CONFIG>
            OPTIONAL)
  endif()
endif()
