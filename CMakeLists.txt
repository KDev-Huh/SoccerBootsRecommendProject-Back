cmake_minimum_required(VERSION 3.29)
project(soccer_boots_bayes_project)

set(CMAKE_CXX_STANDARD 20)

# Project include directories
include_directories(${CMAKE_SOURCE_DIR})

# Crow include 디렉토리 추가
include_directories(${CMAKE_SOURCE_DIR}/Crow/include)

# Find ASIO
find_path(ASIO_INCLUDE_DIR asio.hpp
        HINTS /usr/include /usr/local/include /opt/homebrew/include)
if(ASIO_INCLUDE_DIR)
    include_directories(${ASIO_INCLUDE_DIR})
endif()

# --- ONNX Runtime 설정 시작 ---
# 1. 헤더 파일 찾기 (경로 유연성 확보)
find_path(ONNXRUNTIME_INCLUDE_DIR
        NAMES onnxruntime_cxx_api.h onnxruntime/onnxruntime_cxx_api.h
        HINTS /usr/local/include /usr/include /opt/homebrew/include)

# 2. 라이브러리 파일 찾기
find_library(ONNXRUNTIME_LIB
        NAMES onnxruntime
        HINTS /usr/local/lib /usr/lib /opt/homebrew/lib)

if(ONNXRUNTIME_INCLUDE_DIR AND ONNXRUNTIME_LIB)
    include_directories(${ONNXRUNTIME_INCLUDE_DIR})
    # 만약 헤더가 'onnxruntime/...' 안에 있다면 상위 폴더도 포함
    if(EXISTS "${ONNXRUNTIME_INCLUDE_DIR}/onnxruntime")
        include_directories("${ONNXRUNTIME_INCLUDE_DIR}/onnxruntime")
    endif()
    message(STATUS "Found ONNX Runtime: ${ONNXRUNTIME_LIB}")
    message(STATUS "ONNX Include Dir: ${ONNXRUNTIME_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "ONNX Runtime not found. Checked /usr/local/include and /usr/local/lib.")
endif()
# --- ONNX Runtime 설정 끝 ---

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