//
// Created by 허온 on 2026. 2. 17..
//

#ifndef SOCCER_BOOTS_BAYES_PROJECT_SOCCERBOOTSDATASPLITTER_H
#define SOCCER_BOOTS_BAYES_PROJECT_SOCCERBOOTSDATASPLITTER_H

#include "data/SoccerPlayerBoots.h"

struct TrainTestSplit {
    vector<SoccerPlayerBoots> train;
    vector<SoccerPlayerBoots> test;

    TrainTestSplit(
            const vector<SoccerPlayerBoots>& train,
            const vector<SoccerPlayerBoots>& test
    ) : train(train), test(test) {};
};

class SoccerBootsDataSplitter {
public:
    static TrainTestSplit trainTestSplit(const vector<SoccerPlayerBoots>& data, double trainRatio=0.8, int seed=-1);
};


#endif //SOCCER_BOOTS_BAYES_PROJECT_SOCCERBOOTSDATASPLITTER_H
