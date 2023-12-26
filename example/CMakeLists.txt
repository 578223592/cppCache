# set(SRC_LIST userservice.cc ../user.pb.cc)


add_executable(run_test run_test.cpp)
add_executable(per_test per_test.cpp)
target_link_libraries(run_test lruCache lfuCache arcCache)
target_link_libraries(per_test lruCache lfuCache arcCache)