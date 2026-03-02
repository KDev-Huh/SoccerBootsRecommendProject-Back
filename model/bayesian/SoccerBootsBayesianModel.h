//
// Created by 허온 on 2026. 2. 17..
//

#ifndef SOCCER_BOOTS_BAYES_PROJECT_SOCCERBOOTSBAYESIANMODEL_H
#define SOCCER_BOOTS_BAYES_PROJECT_SOCCERBOOTSBAYESIANMODEL_H

#include <map>
#include <string>
#include <vector>

using namespace std;

class SoccerBootsBayesianModel {
private:
    // 결과 축구화 리스트
    vector<string> bootsNames;
    // 해당 축구화 신는 사람들의 인원
    map<string, int> bootsCount;
    // 각 리스트 컬럼 별 고유 값 갯수 (어휘 크기)
    map<string, int> listCategoryCount;
    // 각 텍스트 컬럼 별 고유 값 갯수 (라플라스 스무딩용)
    map<string, int> textCategoryCount;
    // [축구화이름][키] = 해당 부츠타입+키 조합의 총 단어 수
    map<string, map<string, int>> listCategoryTotalWords;

    map<string, double> priors;
    // [축구화이름][스탯이름] = {평균, 분산}
    map<string, map<string, pair<double, double>>> numericLikelihoods;
    // 예: ["머큐리얼"]["position"]["FW"] = 45회
    // 예: ["머큐리얼"]["strengths"]["Speed"] = 30회
    map<string, map<string, map<string, int>>> categoryLikelihoods;

public:
    SoccerBootsBayesianModel(
            const vector<string>& bootsNames,
            const map<string, int>& bootsCount,
            const map<string, int>& listCategoryCount,
            const map<string, int>& textCategoryCount,
            const map<string, map<string, int>>& listCategoryTotalWords,
            const map<string, double>& priors,
            const map<string, map<string, pair<double, double>>>& numericLikelihoods,
            const map<string, map<string, map<string, int>>>& categoryLikelihoods
    )
    : bootsNames(bootsNames), priors(priors), bootsCount(bootsCount),
    listCategoryCount(listCategoryCount), textCategoryCount(textCategoryCount),
    numericLikelihoods(numericLikelihoods), categoryLikelihoods(categoryLikelihoods),
    listCategoryTotalWords(listCategoryTotalWords) {};

    [[nodiscard]] map<string, double> getPriors() const;
    [[nodiscard]] map<string, map<string, pair<double, double>>> getNumericLikelihoods() const;
    [[nodiscard]] map<string, map<string, map<string, int>>> getCategoryLikelihoods() const;
    [[nodiscard]] map<string, map<string, int>> getListCategoryTotalWords() const;
    [[nodiscard]] vector<string> getBootsNames() const;
    [[nodiscard]] map<string, int> getBootsCount() const;
    [[nodiscard]] map<string, int> getListCategoryCount() const;
    [[nodiscard]] map<string, int> getTextCategoryCount() const;
};


#endif //SOCCER_BOOTS_BAYES_PROJECT_SOCCERBOOTSBAYESIANMODEL_H
