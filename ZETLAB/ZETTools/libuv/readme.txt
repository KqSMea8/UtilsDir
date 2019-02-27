libuv - event loop library

Событийно-ориентированная библиотека асинхронного ввода/вывода.
Под Windows работает на основе I/O Competion Ports.
Аналогичные библиотеки: libevent, libev.

Основной сайт: http://libuv.org
Разработка: https://github.com/libuv/libuv

Как собрать статические библиотеки:

1. libuv-mt.lib:
   1.1. Запустить
        > vcbuild.bat release x86 static

2. libuv-mtd.lib:
   2.1. Запустить
        > vcbuild.bat debug x86 static

3. libuv-md.lib:
   3.1. В common.gypi изменить строчку
                'RuntimeLibrary': 0, # static debug
        на
                'RuntimeLibrary': 2, # static debug
   3.2. Запустить
        > vcbuild.bat release x86 static

4. libuv-mdd.lib:
   4.1. В common.gypi изменить строчку
                'RuntimeLibrary': 1, # static debug
        на
                'RuntimeLibrary': 3, # static debug
   4.2. Запустить
        > vcbuild.bat debug x86 static
