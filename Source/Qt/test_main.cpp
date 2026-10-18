#ifdef __USE_QT__ 

#pragma warning(disable: 4127)
#pragma warning(disable: 4512)


#include <QApplication>
#include <QIcon>
#include <test_service.h>
#include <math_namespace.h>
#include <tool_cstring.h>
#include <last.h>

using namespace c3d;


//------------------------------------------------------------------------------
// Обработка сообщений окна 
// ---
int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(test);
    QApplication app(argc, argv);
    app.setWindowIcon( QIcon(":/res/crystal.ico") );

    QApplication::setAttribute(Qt::AA_NativeWindows);

    ::CreateFrameWindow();

    std::cout << "Точка запуска найдена!" << std::endl;

    if ( argc > 0 )
    {
      if ( argc == 2 )
      {
        ReadModel( c3d::ToC3Dstring( argv[1] ).c_str() );
      }
    }

    int retVal = app.exec();
    ::DestroyFrameWindow();

    return retVal;
}


#endif // __USE_QT__ 
