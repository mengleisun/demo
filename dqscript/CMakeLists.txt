cmake_minimum_required( VERSION 2.6 )

set(VTK_DIR "/Users/mengleis/CoreCode/VTK/build")
set(CMAKE_CXX_FLAGS "-std=c++11")

find_package(GSL)


find_package(VTK)
if(VTK_FOUND)
    include(${VTK_USE_FILE})
endif(VTK_FOUND)
find_package(Binning REQUIRED)
find_package(Maths REQUIRED)
find_package(KaLi REQUIRED)
find_package(Tabree REQUIRED)
find_package(Desaster REQUIRED)
find_package(Kommon REQUIRED)

include_directories(
   ${Desaster_INCLUDE_DIRS}
   ${KaLi_INCLUDE_DIRS}
   ${Tabree_INCLUDE_DIRS}
)



set(TABREE_BINS
  db-read
  test-criteria  
  timeseries-direct-adei
)

set( GSL_LIB /usr/local/Cellar/gsl/2.6/lib)

foreach(TABREE_TARGET ${TABREE_BINS})
  add_executable(${TABREE_TARGET} ${TABREE_TARGET}.cxx)
  target_link_libraries(${TABREE_TARGET} Tabree ${Kommon_LIBRARIES} ${Desaster_LIBRARIES} ${KaLi_LIBRARIES})
  set_source_files_properties(${TABREE_TARGET}.cxx PROPERTIES COMPILE_FLAGS -Wno-unused-parameter)
endforeach()

