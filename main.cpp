#include "data/reader/SoccerPlayerBootsDataCsvReader.h"
#include "model/bayesian/SoccerBootsBayesianTrainer.h"
#include "model/bayesian/SoccerBootsRecommender.h"
#include "model/random_forest/RandomForestRecommender.h"
#include "api/ApiServer.h"
#include "api/RecommendationController.h"

// 수치형 피처 순서 설정
std::vector<std::string> getNumericFeatureOrder() {
    std::vector<std::string> order;
    for (const auto& [key, _] : SoccerPlayerBoots::numKeys) {
        order.push_back(key);
    }
    return order;
}

int main() {
    std::cout << "--- SERVER STARTING ---" << std::endl;
    std::cout.flush();
    // 1. 데이터 로드
    const std::vector<std::string> datasetPaths{
        "./datasets/germany-bundesliga-players_boots.csv",
        "./datasets/premier-league-players_boots.csv",
        "./datasets/spain-laliga-players_boots.csv"
    };

    SoccerPlayerBootsDataCsvReader reader;
    reader.readCsvData(datasetPaths);

    std::cout << "--- SERVER READ ---" << std::endl;
    std::cout.flush();

    // 2. 베이지안 모델 학습
    SoccerBootsBayesianTrainer trainer;
    SoccerBootsBayesianModel model = trainer.fit(reader.getPlayerBoots());

    // 3. 베이지안 추천기 생성
    SoccerBootsRecommender bayesianRecommender(model);

    // 4. 랜덤포레스트 추천기 생성
    RandomForestRecommender rfRecommender("./model/random_forest/rf_model.onnx");

    // 축구화 레이블 설정 (베이지안 모델에서 가져오기)
    rfRecommender.setBootsLabels(model.getBootsNames());

    // Vocabulary 설정 (MultiLabelBinarizer용)
    rfRecommender.setListVocabularies(SoccerPlayerBoots::listVocabularies);

    // 수치형 피처 순서 설정
    rfRecommender.setNumericFeatureOrder(getNumericFeatureOrder());

    std::cout << "--- SERVER PREPARE ---" << std::endl;
    std::cout.flush();

    // 5. API 서버 설정 및 실행
    RecommendationController controller(bayesianRecommender, rfRecommender);

    ApiServer server(8080);
    controller.registerRoutes(server.getApp());
    server.run();

    return 0;
}
