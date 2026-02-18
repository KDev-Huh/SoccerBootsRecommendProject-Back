//
// Created by 허온 on 2026. 2. 18..
//

#include <iostream>
#include "data/reader/SoccerPlayerBootsDataCsvReader.h"
#include "data/splitter/SoccerBootsDataSplitter.h"
#include "model/SoccerBootsBayesianTrainer.h"
#include "model/SoccerBootsRecommender.h"
#include "evaluator/SoccerBootsEvaluator.h"

int main() {
    const std::vector<std::string> datasetPaths{
            "../datasets/germany-bundesliga-players_boots.csv",
            "../datasets/premier-league-players_boots.csv",
            "../datasets/spain-laliga-players_boots.csv"
    };

    SoccerPlayerBootsDataCsvReader reader;
    reader.readCsvData(datasetPaths);

    TrainTestSplit trainTestSplit = SoccerBootsDataSplitter::trainTestSplit(reader.getPlayerBoots(), 0.8, 42);

    SoccerBootsBayesianTrainer trainer;
    SoccerBootsBayesianModel model = trainer.fit(trainTestSplit.train);

    double trainAccuracy = SoccerBootsEvaluator::evaluateAccuracy(model, trainTestSplit.train);
    double testAccuracy = SoccerBootsEvaluator::evaluateAccuracy(model, trainTestSplit.test);
    cout << "훈련 데이터 정확도 : " << trainAccuracy << endl;
    cout << "테스트 데이터 정확도 : " << testAccuracy << endl;

    return 0;
}
