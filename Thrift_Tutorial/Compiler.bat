@echo off
echo 开始编译,请稍等.......

.\thrift-0.9.3.exe --gen cpp Interface.thrift

copy  gen-cpp\* .\Thrift_Client\
copy  gen-cpp\* .\Thrift_Server\

rd /s /q .\gen-cpp


echo 编译结束..............

pause