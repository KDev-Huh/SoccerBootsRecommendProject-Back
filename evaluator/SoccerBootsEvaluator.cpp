//
// Created by 허온 on 2026. 2. 17..
//

#include "SoccerBootsEvaluator.h"
#include <iostream>

double SoccerBootsEvaluator::evaluateAccuracy(const SoccerBootsBayesianModel &model, const vector<SoccerPlayerBoots>& testData) {
    SoccerBootsRecommender recommender(model);

    cout << "테스트 데이터 갯수 : " << testData.size() << endl;

    double correctCount = 0;
    int count = 1;
    for(auto data : testData) {
        string soccerBoots = data.getText("boots_name");
        data.removeText("boots_name");
        data.removeText("player_name");
        data.removeText("player_shirt_number");

        pair<string, double> result = recommender.predict(
                data.convertTextVectorPair(),
                data.convertNumVectorPair(),
                data.convertListVectorPair()
                );

        if(result.first == soccerBoots) correctCount++;
        cout << result.first << " : " << soccerBoots << endl;
        cout << count++ << "번째 데이터 완료" << endl;
    }

    return correctCount / testData.size();
}
