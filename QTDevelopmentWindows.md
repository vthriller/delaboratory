Qt package (with development environment):

http://download.qt-project.org/official_releases/qt/5.1/5.1.1/qt-windows-opensource-5.1.1-mingw48_opengl-x86-offline.exe

DLLs required to run:

```
2013-04-22  18:03        22 378 434 icudt51.dll
2013-04-22  18:03         3 369 922 icuin51.dll
2013-04-22  18:03         1 978 690 icuuc51.dll
2013-04-17  20:18           544 817 libgcc_s_dw2-1.dll
2013-04-17  20:19           989 805 libstdc++-6.dll
2013-04-17  19:26            73 901 libwinpthread-1.dll
2013-08-13  20:26         4 389 376 Qt5Core.dll
2013-07-02  10:01         4 450 304 Qt5Gui.dll
2013-07-02  10:06         6 146 560 Qt5Widgets.dll
```


In Qt project file this library is required (to compile in dcraw):

LIBS += -lws2\_32

static build requires recompilation of Qt which is very complex (lots of deps)