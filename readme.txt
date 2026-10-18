// \English
In the Example\Demo directory there is test.exe test application ready for use for a Windows,
demonstrating the possibilities of the geometric kernel c3d.dll
After starting the application test.exe must enter the key and signature by:
Menu->Help->License_Key, Signature.

In the Example\Models directory there are examples of models in the form of files *.c3d, *.stp, *.x_t, *.stl,
can operate with a test application test.exe.

In the Example\Source directory there are the source code of test.exe test application,
and CMakeLists.txt file to generate the test application project/solution.

To compile the test application of the geometric kernel C3D, follow these steps.

1. Create the a new folder (for example "TestApp") in a comfortable working position.

2. Choose the archive corresponding to the development environment in the <C3D> catalog.

3. Copy <Include>, <Debug>, and <Release> folders from the chosen archive to a new folder.

4. Copy <Source> folder from the <Example> archive to a new folder.

5. Make sure that the new folder contains four directories: <Debug>, <Include>, <Release>, and <Source>.

6. Install CMake, the installation process by selecting the option "Add CMake to the system PATH for all users".

7. Create a solution for test application using the following steps.
   Run CMake version 3.24 or later.
   To "Where is the source code", specify the directory <path_to_testapp>\TestApp\Source.
   For "Where to build the binaries", specify the directory <path_to_testapp>\TestApp\Build.
   Click the "Configure" button to configure the project.
   At the request "Create Directory" confirm agreement to create the directory <path_to_testapp>\TestApp\Build.
   On request "Specify the generator for this project" select the appropriate version of C3D configuration development environment.
   Click the "Generate" to generate the project/solution files.
   Note: if the directory containing the QT5 distribution is added to the PATH, then by setting the MathTest_QT cmake option
   and reconfiguring the project, one can build a test application linked with the Qt5 libraries instead of MFC.

8.1. Under Windows run your solution TestApp\Build\Test.sln for the test application in the development environment.
8.2. Under Linux you may use commands
      cmake -S Source -B Build -DCMAKE_BUILD_TYPE=Release  // (optional, equivalent to step 7)
      cmake --build Build -- -j4
     (Qt5 packages must be preinstalled, e.g. for Ubuntu with 'sudo apt install qtbase5-dev qt5-qmake' command.)

9. If there are some troubles with installation of CMake, use project files in the Example.
   That project files were generated with Unicode!
   Copy files of your development environment: Test.vcxproj, Test.vcxproj.filters to the folder created by step 1.
   Run the project Test.vcxproj in the development environment.

10. To activate the C3D geometry core, you need to write the license key to the c3d.lic file.
    For Windows, the file is located in the %USERPROFILE% directory (C:\Users\user_name).
    For UNIX systems, the file is located in the HOME directory (/home/user_name).

11. After compilation, run the generated test application test.exe from the catalog
    Debug or Release, respectively.

12. Note: if you need to build a test application using the conan packages of kernel and its examples
   (the c3d/* and c3d-examples/* respectively), you can use the short instructions inside the
   conanfile-examples-usage.py file. To build, you will need to specify the kernel package version number.


// \Russian
В каталоге Example\Demo находится готовое для использования в системе Windows тестовое приложение test.exe, 
демонстрирующее возможности геометрического ядра c3d.dll
После запуска приложения test.exe необходимо ввести ключ и сигнатуру выбрав: 
Меню->Помощь->Лицензионный_ключ, Сигнатура.

В каталоге Example\Models находятся примеры моделей в виде файлов *.c3d, *.stp, *.x_t, *.stl, 
с которыми может работать тестовое приложение test.exe.

В каталоге Example\Source находятся исходные тексты тестового приложения test.exe, 
а также файл CMakeLists.txt для генерации проекта/решения тестового приложения.

Для компиляции тестового приложения геометрического ядра C3D выполните следующие действия.

1. Создайте в удобном для работы месте новый каталог, например, "TestApp".

2. В каталоге <C3D> выберите архив, соответствующий среде разработки.

3. Скопируйте из выбранного архива каталоги <Include>, <Debug> и <Release> в новый каталог. 

4. Скопируйте из архива <Example> каталог <Source> в новый каталог.

5. Убедитесь, что в новом каталоге находятся четыре папки: <Debug>, <Include>, <Release> и <Source>.

6. Установите CMake, выбрав в процессе установки опцию "Add CMake to the system PATH for all users".

7. Создайте проект/решение тестового приложения, выполнив следующие действия. 
   Запустите CMake версии не ниже 3.24.
   Для "Where is the source code" укажите каталог <path_to_testapp>\TestApp\Source.
   Для "Where to build the binaries" укажите каталог <path_to_testapp>\TestApp\Build.
   Нажмите кнопку "Configure" для конфигурирования проекта/решения.
   На запрос "Create Directory" подтвердите согласие на создание каталога <path_to_testapp>\TestApp\Build.
   По запросу "Specify the generator for this project" укажите соответствующую версии C3D конфигурацию среды разработки.
   Нажмите кнопку "Generate" для генерации файлов проекта/решения.
   Справочно: если в PATH добавлен каталог с дистрибутивом QT5, то задав cmake опцию MathTest_QT и переконфигурировав
   проект возможно собрать тестовое приложение слинкованное с библиотеками Qt5 вместо MFC.

8.1. Под Windows запустите созданное решение TestApp\Build\Test.sln тестового приложения в Вашей среде разработки.
8.2. Под Linux можно использовать команды
      cmake -S Source -B Build -DCMAKE_BUILD_TYPE=Release  // (опциональна, эквивалентна пункту 7)
      cmake --build Build -- -j4
      (Пакеты с Qt5 должны быть предустановлены, например, для Ubuntu командой 'sudo apt install qtbase5-dev qt5-qmake'.)

9. Если при установке CMake возникли проблемы, 
   то можете воспользоваться готовым проектом Test.vcxproj, расположенными в каталоге Example.
   Проект подготовлен для конфигурации Unicode!
   Для сборки проекта скопируйте файлы проекта: Test.vcxproj, Test.vcxproj.filters в каталог, созданный на шаге 1.
   Запустите проект Test.vcxproj тестового приложения в Вашей среде разработки.

10. Для активации геометрического ядра C3D необходимо прописать полученный лицензионный ключ в файл c3d.lic.
    Для Windows файл расположен в каталоге %USERPROFILE% (C:\Users\user_name).
    Для UNIX систем файл расположен в каталоге HOME (/home/user_name).

11. После компиляции запустите созданное тестовое приложение test.exe из каталога 
    Debug или Release, соответственно.

12. Если есть необходимость сборки тестового приложения с использованием conan пакетов ядра и примеров к нему
   (пакеты c3d/* и c3d-examples/*), можно воспользоваться краткой инструкцией внутри файла conanfile-examples-usage.py.
   Для сборки потребуется указать номер версии пакета ядра.
