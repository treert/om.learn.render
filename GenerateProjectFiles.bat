:: 判断文件夹是否存在
if not exist build ( md build )

cd build
:: https://cmake.org/cmake/help/v3.16/generator/Visual%20Studio%2016%202019.html#platform-selection
cmake -A Win32 ..

pause