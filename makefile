#使用.o替换.cpp
OBJS=$(patsubst %.cpp,%.o,$(wildcard *.cpp))
#链接的库文件
LIBS=
#指定链接库的目录
LDFLAGS=
#编译器
CXX=g++
#指定头文件搜索目录
INCPATH=
#编译器选项
CXXFLAGS=-std=c++11 -O2 -Wall
#编译规则
TARGET=
$(TARGET):$(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)
.cpp.o:
	$(CXX) $(INCPATH) -c $^ $(CXXFLAGS)
.PHONY:clean
clean:
	rm -f $(TARGET) *.o