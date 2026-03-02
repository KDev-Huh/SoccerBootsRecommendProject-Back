//
// Created by 허온 on 2026. 2. 17..
//

#ifndef SOCCER_BOOTS_BAYES_PROJECT_RECOMMENDATIONCONTROLLER_H
#define SOCCER_BOOTS_BAYES_PROJECT_RECOMMENDATIONCONTROLLER_H

#include <crow.h>
#include <crow/middlewares/cors.h>
#include "model/bayesian/SoccerBootsRecommender.h"
#include "model/random_forest/RandomForestRecommender.h"

class RecommendationController {
private:
    SoccerBootsRecommender& bayesianRecommender_;
    RandomForestRecommender& rfRecommender_;

    crow::response handlePostBayesian(const crow::request& req);
    crow::response handlePostRandomForest(const crow::request& req);

public:
    explicit RecommendationController(
        SoccerBootsRecommender& bayesianRecommender,
        RandomForestRecommender& rfRecommender
    );

    void registerRoutes(crow::App<crow::CORSHandler>& app);
};

#endif //SOCCER_BOOTS_BAYES_PROJECT_RECOMMENDATIONCONTROLLER_H
