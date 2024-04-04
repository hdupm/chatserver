cmake_minimum_required(VERSION 3.0)
project(chat) #工程名字

#配置编译选项 可执行文件可调试
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)

#设置可执行文件最终存储路径 
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)#全局变量 PROJECT_SOURCE_DIR是工程根目录

# 把.指定路径下即当前目录下的所有源文件放入变量名SRC_LIST
#aux_source_directory(. SRC_LIST) 

#配置头文件的搜索路径
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/server)
include_directories(${PROJECT_SOURCE_DIR}/include/server/db)
include_directories(${PROJECT_SOURCE_DIR}/include/server/model)
include_directories(${PROJECT_SOURCE_DIR}/include/server/redis)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty)

#加载子目录
add_subdirectory(src)

#表示生成可执行文件server 由SRC_LIST变量所定义的源文件编译而来
#add_executable(server ${SRC_LIST}) #生成server 依赖文件是变量SRC_LIST
#表示server这个目标程序，需要连接muduo_net muduo_base pthread这三个库
#target_link_libraries(server muduo_net muduo_base pthread)
