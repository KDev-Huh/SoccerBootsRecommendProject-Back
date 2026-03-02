//
// Created by 허온 on 2026. 3. 2..
//

#include "RandomForestRecommender.h"
#include <iostream>
#include <algorithm>
#include <numeric>

RandomForestRecommender::RandomForestRecommender(const string& modelPath) {
    // ONNX Runtime 환경 초기화
    env = make_unique<Ort::Env>(ORT_LOGGING_LEVEL_WARNING, "RandomForestModel");
    sessionOptions = make_unique<Ort::SessionOptions>();
    sessionOptions->SetIntraOpNumThreads(1);

    // 모델 로드
    session = make_unique<Ort::Session>(*env, modelPath.c_str(), *sessionOptions);

    // 입력/출력 이름 가져오기
    Ort::AllocatorWithDefaultOptions allocator;

    // 입력 이름
    size_t numInputNodes = session->GetInputCount();
    for (size_t i = 0; i < numInputNodes; i++) {
        auto inputName = session->GetInputNameAllocated(i, allocator);
        inputNames.push_back(inputName.get());
    }

    // 출력 이름
    size_t numOutputNodes = session->GetOutputCount();
    for (size_t i = 0; i < numOutputNodes; i++) {
        auto outputName = session->GetOutputNameAllocated(i, allocator);
        outputNames.push_back(outputName.get());
    }
}

RandomForestRecommender::~RandomForestRecommender() = default;

void RandomForestRecommender::setBootsLabels(const vector<string>& labels) {
    bootsLabels = labels;
}

void RandomForestRecommender::setListVocabularies(const map<string, vector<string>>& vocabs) {
    listVocabularies = vocabs;
}

void RandomForestRecommender::setTextVocabularies(const map<string, vector<string>>& vocabs) {
    textVocabularies = vocabs;
}

void RandomForestRecommender::setNumericFeatureOrder(const vector<string>& order) {
    numericFeatureOrder = order;
}

vector<float> RandomForestRecommender::prepareInputData(
    const vector<pair<string, string>>& textInputs,
    const vector<pair<string, double>>& numInputs,
    const vector<pair<string, vector<string>>>& listInputs
) {
    vector<float> inputData;

    // 1. 수치형 데이터 추가 (순서대로)
    // numericFeatureOrder가 설정되어 있으면 그 순서대로, 아니면 입력 순서대로
    if (!numericFeatureOrder.empty()) {
        map<string, double> numMap;
        for (const auto& [key, value] : numInputs) {
            numMap[key] = value;
        }
        for (const auto& key : numericFeatureOrder) {
            inputData.push_back(static_cast<float>(numMap[key]));
        }
    } else {
        for (const auto& [key, value] : numInputs) {
            inputData.push_back(static_cast<float>(value));
        }
    }

    // 2. 텍스트형 데이터 인코딩 (LabelEncoder)
    // 현재는 textKeys가 비어있으므로 스킵
    for (const auto& [key, value] : textInputs) {
        if (textVocabularies.count(key)) {
            const auto& vocab = textVocabularies[key];
            auto it = find(vocab.begin(), vocab.end(), value);
            if (it != vocab.end()) {
                inputData.push_back(static_cast<float>(distance(vocab.begin(), it)));
            } else {
                inputData.push_back(0.0f); // 미등장 값은 0
            }
        }
    }

    // 3. 리스트형 데이터 처리 (MultiLabelBinarizer)
    // 각 리스트 컬럼에 대해 vocabulary의 각 값이 있으면 1, 없으면 0
    const vector<string> listColumnOrder = {"player_positions", "strengths", "weaknesses", "player_style"};

    for (const auto& columnName : listColumnOrder) {
        if (!listVocabularies.count(columnName)) {
            continue; // vocabulary가 설정되지 않았으면 스킵
        }

        // 입력에서 해당 컬럼 찾기
        vector<string> inputValues;
        for (const auto& [key, values] : listInputs) {
            if (key == columnName) {
                inputValues = values;
                break;
            }
        }

        // MultiLabelBinarizer: vocabulary의 각 값에 대해 0/1 설정
        const auto& vocab = listVocabularies[columnName];
        for (const auto& vocabValue : vocab) {
            bool found = find(inputValues.begin(), inputValues.end(), vocabValue) != inputValues.end();
            inputData.push_back(found ? 1.0f : 0.0f);
        }
    }

    return inputData;
}

pair<string, double> RandomForestRecommender::predict(
    const vector<pair<string, string>>& textInputs,
    const vector<pair<string, double>>& numInputs,
    const vector<pair<string, vector<string>>>& listInputs
) {
    try {
        // 입력 데이터 준비
        vector<float> inputData = prepareInputData(textInputs, numInputs, listInputs);

        // 입력 텐서 차원 설정
        vector<int64_t> inputShape = {1, static_cast<int64_t>(inputData.size())};

        // 메모리 정보 설정
        Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(
            OrtArenaAllocator, OrtMemTypeDefault);

        // 입력 텐서 생성
        Ort::Value inputTensor = Ort::Value::CreateTensor<float>(
            memoryInfo,
            inputData.data(),
            inputData.size(),
            inputShape.data(),
            inputShape.size()
        );

        // 입력/출력 이름을 const char* 배열로 변환
        vector<const char*> inputNamesPtr;
        for (const auto& name : inputNames) {
            inputNamesPtr.push_back(name.c_str());
        }

        vector<const char*> outputNamesPtr;
        for (const auto& name : outputNames) {
            outputNamesPtr.push_back(name.c_str());
        }

        // 추론 실행
        auto outputTensors = session->Run(
            Ort::RunOptions{nullptr},
            inputNamesPtr.data(),
            &inputTensor,
            1,
            outputNamesPtr.data(),
            outputNamesPtr.size()
        );

        // 출력 처리
        // output[0]: output_label (예측된 클래스 인덱스)
        // output[1]: output_probability (각 클래스별 확률들)

        // sklearn RandomForest ONNX 출력:
        // output[0]: label (int64 or string)
        // output[1]: probabilities (sequence of maps)

        // 레이블 가져오기
        auto& labelTensor = outputTensors[0];
        auto labelTypeInfo = labelTensor.GetTensorTypeAndShapeInfo();
        auto labelType = labelTypeInfo.GetElementType();

        size_t predictedIndex = 0;

        // label이 int64 타입인 경우
        if (labelType == ONNX_TENSOR_ELEMENT_DATA_TYPE_INT64) {
            int64_t* labelData = labelTensor.GetTensorMutableData<int64_t>();
            predictedIndex = static_cast<size_t>(labelData[0]);
        }
        // label이 string 타입인 경우 (sklearn에서 가능)
        else if (labelType == ONNX_TENSOR_ELEMENT_DATA_TYPE_STRING) {
            // string 타입은 직접 처리 어려움, 기본값 사용
            predictedIndex = 0;
        }

        // 확률 가져오기 (output[1]은 sequence of maps 형태)
        // RandomForest는 {class_id: probability} 형태의 맵을 반환
        double probability = 1.0;  // 기본값

        try {
            // output[1]이 텐서인 경우 (일부 변환에서는 텐서로 나옴)
            auto& probOutput = outputTensors[1];

            // IsTensor로 확인
            if (probOutput.IsTensor()) {
                auto probTypeInfo = probOutput.GetTensorTypeAndShapeInfo();
                auto probType = probTypeInfo.GetElementType();
                auto probShape = probTypeInfo.GetShape();

                if (probType == ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT) {
                    float* probData = probOutput.GetTensorMutableData<float>();

                    // [1, num_classes] 형태
                    if (probShape.size() == 2) {
                        size_t numClasses = probShape[1];
                        if (predictedIndex < numClasses) {
                            probability = static_cast<double>(probData[predictedIndex]);
                        }
                    }
                    // [num_classes] 형태
                    else if (probShape.size() == 1 && predictedIndex < static_cast<size_t>(probShape[0])) {
                        probability = static_cast<double>(probData[predictedIndex]);
                    }
                } else if (probType == ONNX_TENSOR_ELEMENT_DATA_TYPE_DOUBLE) {
                    double* probData = probOutput.GetTensorMutableData<double>();

                    if (probShape.size() == 2) {
                        size_t numClasses = probShape[1];
                        if (predictedIndex < numClasses) {
                            probability = probData[predictedIndex];
                        }
                    } else if (probShape.size() == 1 && predictedIndex < static_cast<size_t>(probShape[0])) {
                        probability = probData[predictedIndex];
                    }
                }
            }
        } catch (...) {
            // output[1]이 sequence of maps 형태면 텐서 접근 실패
            // 이 경우 확률을 1.0으로 설정 (또는 다른 처리)
            probability = 1.0;
        }

        // 레이블 반환
        string predictedBoot = "Unknown";
        if (predictedIndex < bootsLabels.size()) {
            predictedBoot = bootsLabels[predictedIndex];
        } else {
            predictedBoot = "Class_" + to_string(predictedIndex);
        }

        return {predictedBoot, probability};

    } catch (const Ort::Exception& e) {
        cerr << "ONNX Runtime Error: " << e.what() << endl;
        return {"Error", 0.0};
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return {"Error", 0.0};
    }
}
