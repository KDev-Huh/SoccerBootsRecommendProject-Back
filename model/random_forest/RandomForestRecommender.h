//
// Created by 허온 on 2026. 3. 2..
//

#ifndef SOCCER_BOOTS_BAYES_PROJECT_RANDOMFORESTRECOMMENDER_H
#define SOCCER_BOOTS_BAYES_PROJECT_RANDOMFORESTRECOMMENDER_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <onnxruntime/onnxruntime_cxx_api.h>

using namespace std;

class RandomForestRecommender {
private:
    unique_ptr<Ort::Env> env;
    unique_ptr<Ort::Session> session;
    unique_ptr<Ort::SessionOptions> sessionOptions;

    vector<string> inputNames;
    vector<string> outputNames;
    vector<string> bootsLabels;

    // MultiLabelBinarizer용 vocabulary
    // key: 컬럼 이름 (예: "player_positions")
    // value: 해당 컬럼의 모든 가능한 값들 (정렬된 순서)
    map<string, vector<string>> listVocabularies;

    // LabelEncoder용 vocabulary (텍스트형)
    map<string, vector<string>> textVocabularies;

    // 수치형 피처 순서
    vector<string> numericFeatureOrder;

    // 입력 데이터를 float 배열로 변환
    vector<float> prepareInputData(
        const vector<pair<string, string>>& textInputs,
        const vector<pair<string, double>>& numInputs,
        const vector<pair<string, vector<string>>>& listInputs
    );

public:
    explicit RandomForestRecommender(const string& modelPath);
    ~RandomForestRecommender();

    // 예측 수행
    pair<string, double> predict(
        const vector<pair<string, string>>& textInputs,
        const vector<pair<string, double>>& numInputs,
        const vector<pair<string, vector<string>>>& listInputs
    );

    // 레이블 설정 (축구화 이름 목록)
    void setBootsLabels(const vector<string>& labels);

    // Vocabulary 설정 (학습 데이터로부터 추출)
    void setListVocabularies(const map<string, vector<string>>& vocabs);
    void setTextVocabularies(const map<string, vector<string>>& vocabs);
    void setNumericFeatureOrder(const vector<string>& order);
};

#endif //SOCCER_BOOTS_BAYES_PROJECT_RANDOMFORESTRECOMMENDER_H
