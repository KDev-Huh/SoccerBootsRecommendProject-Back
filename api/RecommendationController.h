//
// Created by 허온 on 2026. 2. 17..
//

#ifndef SOCCER_BOOTS_BAYES_PROJECT_RECOMMENDATIONCONTROLLER_H
#define SOCCER_BOOTS_BAYES_PROJECT_RECOMMENDATIONCONTROLLER_H

#include <crow.h>
#include "model/bayesian/SoccerBootsRecommender.h"
#include "model/random_forest/RandomForestRecommender.h"

class RecommendationController {
private:
    SoccerBootsRecommender& bayesianRecommender_;
    RandomForestRecommender& rfRecommender_;

    static void addCorsHeaders(crow::response& res);

    // [[nodiscard]]는 반환값 사용 강제함
    [[nodiscard]] crow::response handleOptions(const crow::request& req) const;
    crow::response handlePostBayesian(const crow::request& req);
    crow::response handlePostRandomForest(const crow::request& req);

public:
    explicit RecommendationController(
        SoccerBootsRecommender& bayesianRecommender,
        RandomForestRecommender& rfRecommender
    );

    void registerRoutes(crow::SimpleApp& app);
};

#endif //SOCCER_BOOTS_BAYES_PROJECT_RECOMMENDATIONCONTROLLER_H
