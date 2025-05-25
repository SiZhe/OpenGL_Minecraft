file(GLOB_RECURSE MC ./ *.cpp)

add_library(mc ${MC})

# 添加对 glframework 模块的链接
target_link_libraries(mc PRIVATE fw)