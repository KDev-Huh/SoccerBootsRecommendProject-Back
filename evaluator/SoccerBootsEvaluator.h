//
// Created by 허온 on 2026. 2. 17..
//

#ifndef SOCCER_BOOTS_BAYES_PROJECT_SOCCERBOOTSEVALUATOR_H
#define SOCCER_BOOTS_BAYES_PROJECT_SOCCERBOOTSEVALUATOR_H


#include "data/SoccerPlayerBoots.h"
#include "model/SoccerBootsRecommender.h"

class SoccerBootsEvaluator {
public:
    static double evaluateAccuracy(const SoccerBootsBayesianModel& model, const vector<SoccerPlayerBoots>& testData);
};


#endif //SOCCER_BOOTS_BAYES_PROJECT_SOCCERBOOTSEVALUATOR_H
