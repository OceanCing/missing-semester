
cmakelist结尾加
```
add_executable(my_ssr_node src/my_ssr_pkg.cpp)  
  
add_dependencies(my_ssr_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})  
  
target_link_libraries(my_ssr_node  
        ${catkin_LIBRARIES}  
)
```


## 设置.py为可执行文件
```
chmod +x 脚本路径
```