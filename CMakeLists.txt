cmake_minimum_required(VERSION 3.10)
project(prao-compresser)
set(CMAKE_CXX_STANDARD 14)
find_package(OpenCL REQUIRED)
add_executable(prao-compresser main.cpp
        Calibration/CalibrationDataInput.h Calibration/CalibrationDataInput.cpp
        Calibration/CalibrationData.h Calibration/CalibrationData.cpp
        Calibration/CalibrationDataStorage.h Calibration/CalibrationDataStorage.cpp Calibration/test.cpp Calibration/test.h
        Reader.cpp Reader.h FileHeader.cpp FileHeader.h
        Reader/DataHeader.h Reader/DataHeader.cpp Reader/testReader.h Reader/testReader.cpp Reader/DataReader.h
        Reader/DataReader.cpp Reader/FilesListItem.h Reader/FilesListItem.cpp
        Point/Point.h
        Time/Time.h Time/Time.cpp
        Processing/GPUContext.cpp Processing/GPUContext.h Processing/Kernels/SortKernel.cl
        Processing/SignalProcessor.cpp Processing/SignalProcessor.h)
target_link_libraries(prao-compresser OpenCL::OpenCL)