cmake_minimum_required(VERSION 3.29)
project(soccer_boots_bayes_project)

set(CMAKE_CXX_STANDARD 20)

# Project include directories
include_directories(${CMAKE_SOURCE_DIR})

# Crow include 디렉토리 추가
include_directories(${CMAKE_SOURCE_DIR}/Crow/include)

# Find ASIO (installed via Homebrew)
find_path(ASIO_INCLUDE_DIR asio.hpp HINTS /opt/homebrew/include /usr/local/include)
if(ASIO_INCLUDE_DIR)
    include_directories(${ASIO_INCLUDE_DIR})
endif()

# Find ONNX Runtime (installed via Homebrew)
find_path(ONNXRUNTIME_INCLUDE_DIR onnxruntime/onnxruntime_cxx_api.h
    HINTS /opt/homebrew/Cellar/onnxruntime/1.24.2_1/include /opt/homebrew/include /usr/local/include)
find_library(ONNXRUNTIME_LIB onnxruntime
    HINTS /opt/homebrew/Cellar/onnxruntime/1.24.2_1/lib /opt/homebrew/lib /usr/local/lib)

if(ONNXRUNTIME_INCLUDE_DIR AND ONNXRUNTIME_LIB)
    include_directories(${ONNXRUNTIME_INCLUDE_DIR})
    message(STATUS "Found ONNX Runtime: ${ONNXRUNTIME_LIB}")
else()
    message(FATAL_ERROR "ONNX Runtime not found. Install it via: brew install onnxruntime")
endif()

# Crow에서 필요한 threads 라이브러리 찾기
find_package(Threads REQUIRED)

add_library(core_library
        data/SoccerPlayerBoots.cpp
        data/SoccerPlayerBoots.h
        data/reader/SoccerPlayerBootsDataCsvReader.cpp
        data/reader/SoccerPlayerBootsDataCsvReader.h
        model/bayesian/SoccerBootsBayesianTrainer.cpp
        model/bayesian/SoccerBootsBayesianTrainer.h
        model/bayesian/SoccerBootsRecommender.cpp
        model/bayesian/SoccerBootsRecommender.h
        model/random_forest/RandomForestRecommender.cpp
        model/random_forest/RandomForestRecommender.h
        api/RecommendationController.cpp
        api/RecommendationController.h
        api/ApiServer.cpp
        api/ApiServer.h
        model/bayesian/SoccerBootsBayesianModel.cpp
        model/bayesian/SoccerBootsBayesianModel.h
        evaluator/SoccerBootsEvaluator.cpp
        evaluator/SoccerBootsEvaluator.h
        data/splitter/SoccerBootsDataSplitter.cpp
        data/splitter/SoccerBootsDataSplitter.h
)

add_executable(soccer_boots_bayes_project main.cpp)
target_link_libraries(soccer_boots_bayes_project core_library ${ONNXRUNTIME_LIB} Threads::Threads)

add_executable(evaluate evaluator/evaluateModel.cpp)
target_link_libraries(evaluate core_library)
