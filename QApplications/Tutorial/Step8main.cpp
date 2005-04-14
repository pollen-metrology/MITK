#include <Step8.h>

#include <qapplication.h>
#include <itksys/SystemTools.hxx>
int main(int argc, char* argv[])
{
  QApplication qtapplication( argc, argv );

  if(argc<2)
  {
    fprintf( stderr, "Usage:   %s [filename1] [filename2] ...\n\n", itksys::SystemTools::GetFilenameName(argv[0]).c_str() );
    return 1;
  }

  Step8 mainWindow(argc, argv, NULL, "mainwindow");
  mainWindow.Initialize();
  qtapplication.setMainWidget(&mainWindow);
  mainWindow.show();

  return qtapplication.exec();
}

/**
\example Step8main.cpp
*/
