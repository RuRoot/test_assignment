# conanfile-examples-usage.py is a ready to use prototype to build with conan 'test' examples source code packaged by 'c3d-examples' package.

# For manual build user should:
#
# 1. Have installed conan package manager.
# 2. Install at least c3d and c3d-examples conan packages.
# 3. Get installed c3d-examples package folder path with:
#    conan cache path c3d-examples/2024.1.11820.4-7b249167
#    where kernel version is 2024.1.11820.4-7b249167 (but use actual).
# 4. Copy this file from the c3d-examples package folder e.g.
#    /home/user_name/.conan2/p/b/c3d-<hashtag-from-previous-step>/p/src/Test
#    to any other folder,
# 5. Set FORCE_LINK_TEST_WITH_QT defined below to True to link with the Qt package under Windows if required.
# 6. Build by this recipe 'test' example source code located in the c3d-examples package folder.


# Under Linux in virtual machine it may be necessary to install conan and to work with conan out of activated virtual environment.
# To create virtual environment for python3 user may run once the following commands:
#
# $> mkdir -p ~/.venv/vuser_conan
# $> python3 -m venv ~/.venv/vuser_conan
# $> ~/.venv/vuser_conan/bin/python -m pip install conan
#
# To activate the virtual environment run
#
# $> source ~/.venv/vuser_conan/bin/activate
#
# To upgrade conan run
# $> pip install conan --upgrade.


## Build examples source code e.g. with the following commands
#
# $> conan build conanfile-examples-usage.py -of build-release --build=missing --options:h="&:with_unicode=True" --options="c3d/*:shared=True" -pr:h default -s build_type=Release -c user.pkg_libs:deploy=True -c user.c3d:pkg_version="2024.1.11820.4-7b249167"
#
# Note. One may use custom conan profile instead of 'default' (i.e. based on the same parameters with which
# the referenced kernel package was deployed - or create your own) and use required c3d package version.
#
# Hint: to peek into build parameters for the referenced kernel,
#  1) find its package hash (package id) with
#    $> conan list c3d/2024.1.11820.4-7b249167:*
#    under 'packages' phrase, e.g. 885772c04a786754e428081308041af2645799ea
#  2) find path to conaninfo.txt file and print it
#     $> cat `conan cache path c3d/2024.1.11820.4-7b249167:885772c04a786754e428081308041af2645799ea`/conaninfo.txt
#  3) create your custom conan profile based on the data above.
#
# Here
# user.pkg_libs:deploy=False   config defines if required to deploy dependent packages libraries or not
# user.c3d:pkg_version         config defines kernel package version in local conan repository to link against
# Deployment is mandatory on Windows in order to run test.exe.
#
# 'test' executable and config files will be created in 'build-release/Release' folder.
# To run under Linux call 'sh run.sh' command in the terminal from the folder where run.sh is located, under Windows just click 'test.exe'.
#
# Note: only linkage against _shared_ c3d package kernel library (c3d.dll/libc3d.so) is supported by this recipe.
#

## If necessary, export dependent packages to separate folder (use one of the direct_deploy, full_deploy, runtime_deploy options)
#
# $> conan install conanfile-examples-usage.py -of build-release --deployer-folder=deployed-packages --deployer=direct_deploy --options:h="&:with_unicode=True" --options="c3d/*:shared=True" -pr:h default -s build_type=Release -c user.pkg_libs:deploy=False -c user.c3d:pkg_version="2024.1.11820.4-7b249167"
#

## If necessary, export the built 'test' to the conan repository package with:
#
#  $> conan export-pkg conanfile-examples-usage.py -of build-release --options:h="&:with_unicode=True" --options="c3d/*:shared=True" -pr:h default -s build_type=Release -c user.c3d:pkg_version="2024.1.11820.4-7b249167"
#

import os
import shutil
from conan import ConanFile, tools
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake, cmake_layout
from conan.tools.files import copy, save
from conan import conan_version
if conan_version >= '2.12.0':
  from conan.internal.model.conanfile_interface import ConanFileInterface
else:
  from conans.model.conanfile_interface import ConanFileInterface

required_conan_version = '>=2.0.5'

# Set to True to link with Qt package under Windows
FORCE_LINK_TEST_WITH_QT = False

class C3DConanExamplesUsage(ConanFile):
  name = 'c3d-examples-usage'
  license = 'Commercial'
  author = 'C3D Labs LLC'
  homepage = 'https://c3dlabs.com'
  url = 'https://c3dlabs.com'
  description = 'Recipe to build c3d-examples package test example source code.'
  settings = 'os', 'compiler', 'build_type', 'arch'

  options = {  # Are passed transparately to the requested c3d package options
              'with_unicode': [True, False], 'cmake_options': ['ANY'] }

  default_options = { 'with_unicode': True, 'cmake_options': [None] }

  package_type = 'application'

  version = '1.0.0'

  c3d_pkg_version = 'undefined'


  def requirements(self):
    print('>> Processing requirements...')

    self.c3d_pkg_version = f"{self.conf.get('user.c3d:pkg_version')}"
    c3d_version: str = f'Passed c3d kernel package version: c3d/{self.c3d_pkg_version}'
    if (self.c3d_pkg_version == 'None'):
      self.output.error(c3d_version)
    else:
      self.output.info(c3d_version)

    self.requires(f'c3d/{self.c3d_pkg_version}',
      options = {
        'shared': True,
        'with_unicode': self.options.with_unicode.value,
        'cmake_options': self.options.cmake_options.value },
      package_id_mode='full_mode')

    self.requires(f'c3d-examples/{self.c3d_pkg_version}',
      options = {
        'shared': True,
        'with_unicode': self.options.with_unicode.value,
        'cmake_options': self.options.cmake_options.value })

    if self.settings.os != 'Windows' or FORCE_LINK_TEST_WITH_QT:
      self.requires('qt/5.15.16', options={'shared': True, 'with_odbc': False, 'with_pq': False, 'with_mysql': False, 'with_sqlite3': False})
      self.requires('freetype/2.13.3', override=True)  # Since qt/5.15.16 expects older freetype/2.13.2 and we want to minimize compiled binaries quantity
      self.requires('openssl/1.1.1w', override=True)  # Use the same version as expected by the c3d kernel package
      self.requires('opengl/system')


  @property
  def _c_compiler(self):
    return os.getenv('CC', None)

  @property
  def _cxx_compiler(self):
    return os.getenv('CXX', None)

  @property
  def _min_cppstd(self):
    return 17


  def _configure_cmake(self, cmake):
    if self.settings.compiler == 'msvc':
      # See https://learn.microsoft.com/en-us/cpp/overview/compiler-versions?view=msvc-170 for the table of correspondence
      if self.settings.compiler.version == 192:
        cmake.generator = 'Visual Studio 16 2019'
      elif (self.settings.compiler.version == 193 or self.settings.compiler.version == 194):
        cmake.generator = 'Visual Studio 17 2022'
      else:
        self.output.error(f'CMake generator for MSVC compiler version {self.settings.compiler.version} is not supported.')
    else:
      cmake.generator = 'Unix Makefiles'


  def build(self):
    print('>> build...')
    cmake = CMake(self)
    self._configure_cmake(cmake)
    print(f'Using {cmake.generator} cmake.generator.')

    host_c3d_examples_pkg = f'c3d-examples/{self.c3d_pkg_version}'
    c3d_examples: ConanFileInterface = self.dependencies.host[host_c3d_examples_pkg]
    self.output.info(f"Building 'test' example source code from the \'{host_c3d_examples_pkg}\' package ({c3d_examples.package_folder})")

    cmake.configure(build_script_folder=os.path.normpath(f'{c3d_examples.package_folder}/src/Test'))
    cmake.build()

    dependent_libs_deploy: bool = (f"{self.conf.get('user.pkg_libs:deploy')}" == 'True')
    if dependent_libs_deploy:
      print('Deploying dependent package libraries:')
    else:
      print('Built without deployment of dependent package libraries.')

    # Deploy libc3d.so shared lib to the recipe build folder
    host_c3d_pkg = f'c3d/{self.c3d_pkg_version}'
    c3d: ConanFileInterface = self.dependencies.host[host_c3d_pkg]

    if dependent_libs_deploy:
      self.output.info(f"Deploying \'{host_c3d_pkg}\' package shared library from the {c3d.package_folder}")
      if self.settings.os == 'Windows':
        for item in [ '*.dll' ]:  # 'c3d.dll'
            copy(self, item,
                src=os.path.normpath(f'{c3d.package_folder}/bin'),
                dst=os.path.normpath(f'{self.build_folder}/{self.settings.build_type.value}'), keep_path=False)
      else:
        for item in [ '*.so.*', '*.so', '*.so.*.*', '*.dylib' ]:  # 'libc3d.so', 'libc3d.dylib'
          copy(self, item,
              src=os.path.normpath(f'{c3d.package_folder}/lib'),
              dst=os.path.normpath(f'{self.build_folder}/{self.settings.build_type.value}'), keep_path=False)

    if self.settings.os != 'Windows' or FORCE_LINK_TEST_WITH_QT:
      # Create qt.conf to point to the Qt package plugins folder
      host_qtx_pkg = f"qt/{self.dependencies['qt'].ref.version}"
      qtx: ConanFileInterface = self.dependencies.host[host_qtx_pkg]

      QT_CONF_FILE = 'qt.conf'
      output_dir: str = os.path.normpath(f'{self.build_folder}/{self.settings.build_type.value}')

      conf_line: str = 'Prefix=.'
      save(self, os.path.normpath(f'{output_dir}/{QT_CONF_FILE}'), '[Paths]\n')
      if not dependent_libs_deploy:
        if self.settings.os == 'Windows':
          conf_line = (f'Prefix={qtx.package_folder}').replace('\\', '/')  # Only forward slash '\' does for Windows
        else:
          conf_line = f'Prefix={qtx.package_folder}'  # Backslash does for Linux
      save(self, path=os.path.normpath(f'{output_dir}/{QT_CONF_FILE}'), content=os.path.normpath(f'{conf_line}'), append=True)


      # Create run.sh script to setup LD_LIBRARY_PATH for 'test' run
      if not self.settings.os == 'Windows':
        RUN_SH_FILE = 'run.sh'
        save(self, os.path.normpath(f'{output_dir}/{RUN_SH_FILE}'), '#!/bin/sh\n')
        save(self, os.path.normpath(f'{output_dir}/{RUN_SH_FILE}'),
             f'# NOTE: call \'sh run.sh\' in terminal from the folder where it is located to run the \'test\'\n', append=True)
        if not dependent_libs_deploy:
          conf_line = os.path.normpath(f'LD_LIBRARY_PATH={qtx.package_folder}/lib:{c3d.package_folder}/lib\n')
        else:
          conf_line = 'LD_LIBRARY_PATH=.\n'
        save(self, os.path.normpath(f'{output_dir}/{RUN_SH_FILE}'), f'{conf_line}', append=True)
        save(self, os.path.normpath(f'{output_dir}/{RUN_SH_FILE}'), 'export LD_LIBRARY_PATH\n', append=True)
        save(self, os.path.normpath(f'{output_dir}/{RUN_SH_FILE}'), 'exec ./test $@ &\n', append=True)


      # Deploy linked Qt libraries and plugins
      if dependent_libs_deploy:
        self.output.info(f"Deploying '{host_qtx_pkg}' package shared libraries from the {qtx.package_folder}")
        qt_dst_dir: str = os.path.normpath(f'{self.build_folder}/{self.settings.build_type.value}')
        shutil.copytree(src=os.path.normpath(f'{qtx.package_folder}/plugins'),
          dst=os.path.normpath(f'{qt_dst_dir}/plugins'), dirs_exist_ok=True)
        if self.settings.os == 'Windows':
          for item in [ '*.dll' ]:
              copy(self, item,
                  src=os.path.normpath(f'{qtx.package_folder}/bin'),
                  dst=os.path.normpath(f'{qt_dst_dir}'), keep_path=False)
        else:
          for item in [ '*.so.*', '*.so', '*.so.*.*', '*.dylib' ]:
              copy(self, item,
                  src=os.path.normpath(f'{qtx.package_folder}/lib'),
                  dst=os.path.normpath(f'{qt_dst_dir}'), keep_path=False)


  def generate(self):
    print('>> Generating project buildsystem...')
    deps = CMakeDeps(self)  # helper will generate one xxxx-config.cmake file per dependency, together with other necessary .cmake files
    deps.generate()

    tc = CMakeToolchain(self)

    tc.cache_variables['CMAKE_BUILD_TYPE'] = self.settings.build_type.value
    # NOTE. The booleans assigned to variables/cache_variables will be translated to ON and OFF symbols in CMake
    tc.cache_variables['MathTest_WITH_QT5_PKG'] = True
    tc.cache_variables['MathTest_USING_UNICODE'] = (self.options.get_safe('with_unicode').value == 'True')

    if self.settings.os != 'Windows' or FORCE_LINK_TEST_WITH_QT:
      tc.cache_variables['MathTest_QT'] = True

    tc.cache_variables['MathTest_PROJECT_VERSION'] = self.c3d_pkg_version[:14]  # xxxx.x.xxxxx.x or xxxx.x.xxxxx.x-xxxxxxxx
    tc.cache_variables['MathTest_BUILD_ID'] = self.c3d_pkg_version  # e.g. 2024.1.11820.4-7b249167
    tc.cache_variables['math_target'] = 'c3d::c3d'
    tc.cache_variables['Test_OUTPUT'] = 'test'

    if self._c_compiler != None:
      tc.cache_variables['CMAKE_C_COMPILER'] = self._c_compiler
    if self._cxx_compiler != None:
      tc.cache_variables['CMAKE_CXX_COMPILER'] = self._cxx_compiler
    if tools.apple.is_apple_os(self):
      tc.cache_variables['CMAKE_FIND_ROOT_PATH_MODE_PROGRAM'] = 'BOTH'
      tc.cache_variables['CMAKE_FIND_ROOT_PATH_MODE_INCLUDE'] = 'BOTH'
      tc.cache_variables['CMAKE_FIND_ROOT_PATH_MODE_LIBRARY'] = 'BOTH'
      tc.cache_variables['CMAKE_FIND_ROOT_PATH_MODE_PACKAGE'] = 'NEVER'
      tc.cache_variables['CMAKE_MACOSX_BUNDLE'] = False
      tc.cache_variables['CMAKE_OSX_ARCHITECTURES'] = tools.apple.to_apple_arch(self)
    tc.generate()


  def layout(self):
    print('>> Creating layout...')
    # Create layout setting source and build subfolders
    cmake_layout(self, build_folder='.')
    self.folders.build = '.'
    self.folders.generators = 'generators'


  def package(self):
    print('>> Packaging project...')

    artifacts_dir = os.path.normpath(self.build_folder)
    if self.settings.os == 'Windows':
      artifacts_dir = os.path.join(artifacts_dir, self.settings.build_type.value)

    print(f'artifacts_dir {artifacts_dir}')

    # Create standard folders layout for the package artifacts folder in conan repository (e.g. in ~/.conan2/p/b/c3d<hash>/p)
    # copy executables
    for item in ['test.exe', 'test', 'qt.conf']:
      copy(self, item, src=artifacts_dir, dst=os.path.join(self.package_folder, 'bin'), keep_path=False)


  def package_id(self):
    print('>> package_id...')
    del self.info.options.cmake_options
    if self.info.settings.build_type == 'RelWithDebInfo':
      self.info.settings.build_type = 'Release'
    print(f'Package id: {self.info.package_id()}')
