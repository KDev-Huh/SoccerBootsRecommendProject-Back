//
// Created by 허온 on 2026. 2. 17..
//

#include "RecommendationController.h"

#include <string>
#include <vector>
#include <utility>

using namespace std;

RecommendationController::RecommendationController(
    SoccerBootsRecommender& bayesianRecommender,
    RandomForestRecommender& rfRecommender
) : bayesianRecommender_(bayesianRecommender), rfRecommender_(rfRecommender) {}


crow::response RecommendationController::handlePostBayesian(const crow::request& req) {
    const auto body = crow::json::load(req.body);

    if (!body) {
        return crow::response(400);
    }

    // 텍스트 입력 파싱
    vector<pair<string, string>> textInputs;
    for (const auto& item : body["textInputs"]) {
        textInputs.emplace_back(item["key"].s(), item["value"].s());
    }

    // 수치 입력 파싱
    vector<pair<string, double>> numInputs;
    for (const auto& item : body["numInputs"]) {
        numInputs.emplace_back(item["key"].s(), item["value"].d());
    }

    // 리스트 입력 파싱
    vector<pair<string, vector<string>>> listInputs;
    for (const auto& item : body["listInputs"]) {
        vector<string> values;
        for (const auto& v : item["value"]) {
            values.push_back(v.s());
        }
        listInputs.emplace_back(item["key"].s(), std::move(values));
    }

    const auto [bootsName, probability] = bayesianRecommender_.predict(textInputs, numInputs, listInputs);

    crow::json::wvalue resBody;
    resBody["boots"] = bootsName;
    resBody["prob"]  = probability;

    return crow::response{resBody};
}

crow::response RecommendationController::handlePostRandomForest(const crow::request& req) {
    const auto body = crow::json::load(req.body);

    if (!body) {
        return crow::response(400);
    }

    // 텍스트 입력 파싱
    vector<pair<string, string>> textInputs;
    for (const auto& item : body["textInputs"]) {
        textInputs.emplace_back(item["key"].s(), item["value"].s());
    }

    // 수치 입력 파싱
    vector<pair<string, double>> numInputs;
    for (const auto& item : body["numInputs"]) {
        numInputs.emplace_back(item["key"].s(), item["value"].d());
    }

    // 리스트 입력 파싱
    vector<pair<string, vector<string>>> listInputs;
    for (const auto& item : body["listInputs"]) {
        vector<string> values;
        for (const auto& v : item["value"]) {
            values.push_back(v.s());
        }
        listInputs.emplace_back(item["key"].s(), std::move(values));
    }

    const auto [bootsName, probability] = rfRecommender_.predict(textInputs, numInputs, listInputs);

    crow::json::wvalue resBody;
    resBody["boots"] = bootsName;
    resBody["prob"]  = probability;

    return crow::response{resBody};
}

void RecommendationController::registerRoutes(crow::App<crow::CORSHandler>& app) {
    // 축구화 추천 요청 처리 - Bayesian
    CROW_ROUTE(app, "/recommend/soccer-boots/bayesian")
        .methods("POST"_method)
        ([this](const crow::request& req) {
            return handlePostBayesian(req);
        });

    // 축구화 추천 요청 처리 - Random Forest
    CROW_ROUTE(app, "/recommend/soccer-boots/random-forest")
        .methods("POST"_method)
        ([this](const crow::request& req) {
            return handlePostRandomForest(req);
        });
}
