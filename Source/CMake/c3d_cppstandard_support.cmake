macro(c3d_set_cpp_standard_msvc proj_name required_cpp_standard)
  if(MSVC)
    message(STATUS "Requested C++ standard: " ${required_cpp_standard})
    # CMake: For Visual Studio Generators, CMake cannot precisely control the placement of the language standard flag on the compiler command line

    if(${required_cpp_standard} GREATER 2022)
      target_compile_definitions(${proj_name} PUBLIC C3D_STANDARD_CXX_LIMIT=2023)
      target_compile_features(${proj_name} PUBLIC cxx_std_23)
      message(STATUS "${proj_name}: The compiler ${CMAKE_CXX_COMPILER} has /std:c++23 support.")

    elseif(${required_cpp_standard} GREATER 2019)
      target_compile_definitions(${proj_name} PUBLIC C3D_STANDARD_CXX_LIMIT=2020)
      target_compile_features(${proj_name} PUBLIC cxx_std_20)
      message(STATUS "${proj_name}: The compiler ${CMAKE_CXX_COMPILER} has /std:c++20 support.")

    elseif(${required_cpp_standard} GREATER 2016)
      target_compile_definitions(${proj_name} PUBLIC C3D_STANDARD_CXX_LIMIT=2017)
      target_compile_features(${proj_name} PUBLIC cxx_std_17)
      message(STATUS "${proj_name}: The compiler ${CMAKE_CXX_COMPILER} has /std:c++17 support.")

    elseif(${required_cpp_standard} GREATER 2013)
      target_compile_definitions(${proj_name} PUBLIC C3D_STANDARD_CXX_LIMIT=2014)
      target_compile_features(${proj_name} PUBLIC cxx_std_14)
      message(STATUS "${proj_name}: The compiler ${CMAKE_CXX_COMPILER} has /std:c++14 support.")

    # The Microsoft C++ compiler in Visual Studio 2017 and later versions doesn't support C++ standards modes earlier than C++14 (/std:c++14).
    elseif(${required_cpp_standard} GREATER 2010)
      target_compile_definitions(${proj_name} PUBLIC C3D_STANDARD_CXX_LIMIT=2011)
      target_compile_features(${proj_name} PUBLIC cxx_std_11)
      message(STATUS "${proj_name}: The compiler ${CMAKE_CXX_COMPILER} has /std:c++11 support.")

    else()
      message(FATAL_ERROR "${proj_name}: /std:c++11 is the minimum supported version.")
    endif()
  endif()
endmacro()

macro(c3d_set_cpp_standard proj_name required_cpp_standard)
  if (NOT MSVC)
    include(CheckCXXCompilerFlag)
    # These checks are clang/gcc specific and expected to fail on MSVC.
    CHECK_CXX_COMPILER_FLAG(-std=c++23 COMPILER_SUPPORTS_CXX23)
    CHECK_CXX_COMPILER_FLAG(-std=c++20 COMPILER_SUPPORTS_CXX20)
    CHECK_CXX_COMPILER_FLAG(-std=c++17 COMPILER_SUPPORTS_CXX17)
    CHECK_CXX_COMPILER_FLAG(-std=c++14 COMPILER_SUPPORTS_CXX14)
    CHECK_CXX_COMPILER_FLAG(-std=c++11 COMPILER_SUPPORTS_CXX11)
    message(STATUS "Requested C++ standard: " ${required_cpp_standard})

    if(COMPILER_SUPPORTS_CXX23 AND ${required_cpp_standard} GREATER 2022)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23")
      target_compile_definitions(${proj_name} PUBLIC C3D_STANDARD_CXX_LIMIT=2023)
      message(STATUS "${proj_name}: The compiler ${CMAKE_CXX_COMPILER} has std=c++23 support.")

    elseif(COMPILER_SUPPORTS_CXX20 AND ${required_cpp_standard} GREATER 2019)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
      target_compile_definitions(${proj_name} PUBLIC C3D_STANDARD_CXX_LIMIT=2020)
      message(STATUS "${proj_name}: The compiler ${CMAKE_CXX_COMPILER} has std=c++20 support.")

    elseif(COMPILER_SUPPORTS_CXX17 AND ${required_cpp_standard} GREATER 2016)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
      target_compile_definitions(${proj_name} PUBLIC C3D_STANDARD_CXX_LIMIT=2017)
      message(STATUS "${proj_name}: The compiler ${CMAKE_CXX_COMPILER} has std=c++17 support.")

    elseif(COMPILER_SUPPORTS_CXX14 AND ${required_cpp_standard} GREATER 2013)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
      target_compile_definitions(${proj_name} PUBLIC C3D_STANDARD_CXX_LIMIT=2014)
      message(STATUS "${proj_name}: The compiler ${CMAKE_CXX_COMPILER} has std=c++14 support.")

    elseif(COMPILER_SUPPORTS_CXX11 AND ${required_cpp_standard} GREATER 2010)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
      target_compile_definitions(${proj_name} PUBLIC C3D_STANDARD_CXX_LIMIT=2011)
      message(STATUS "${proj_name}: The compiler ${CMAKE_CXX_COMPILER} has std=c++11 support.")

    else()
      message(FATAL_ERROR "${proj_name}: -std=c++11 is the minimum supported version.")
    endif()
  endif()
endmacro()

macro(c3d_set_c_standard proj_name)
  if (NOT MSVC)
    include(CheckCCompilerFlag)
    CHECK_C_COMPILER_FLAG("-std=c18" COMPILER_SUPPORTS_C18)
    CHECK_C_COMPILER_FLAG("-std=c11" COMPILER_SUPPORTS_C11)

    if(COMPILER_SUPPORTS_C18)
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c18")
      message(STATUS "${proj_name}: The compiler ${CMAKE_C_COMPILER} has std=c18 support.")

    elseif(COMPILER_SUPPORTS_C11)
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
      message(STATUS "${proj_name}: The compiler ${CMAKE_C_COMPILER} has std=c11 support.")

    else()
      message(WARNING "${proj_name}: The compiler ${CMAKE_C_COMPILER} has no C11 support. Please use a different C compiler.")
    endif()
  endif()
endmacro()
