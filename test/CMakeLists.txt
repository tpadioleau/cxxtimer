add_executable(simple simple.cpp)
target_link_libraries(simple PRIVATE cxxtimer)
add_test(NAME simple COMMAND simple)

add_executable(count_before_stop count_before_stop.cpp)
target_link_libraries(count_before_stop PRIVATE cxxtimer)
add_test(NAME count_before_stop COMMAND count_before_stop)
