#使用.o替换.cpp
SRC:=$(wildcard *.cpp)
OBJS:=$(patsubst %.cpp,%.o,$(notdir $(SRC)))
#链接的库文件
LIBS:=
#指定链接库的目录
LDFLAGS:=
#编译器
CXX:=g++
#指定头文件搜索目录
INCPATH:=
#编译器选项
CXXFLAGS:=-std=c++11 -O2 -Wall
#编译规则
TARGET:=
$(TARGET):$(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)
$(OBJS):$(SRC)
	$(CXX) $(INCPATH) -c $^ $(CXXFLAGS)
.PHONY:clean
clean:
	rm -f $(TARGET) *.o