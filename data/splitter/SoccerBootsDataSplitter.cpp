//
// Created by 허온 on 2026. 2. 17..
//

#include "SoccerBootsDataSplitter.h"
#include <algorithm>
#include <random>

TrainTestSplit SoccerBootsDataSplitter::trainTestSplit(const vector<SoccerPlayerBoots> &data, double trainRatio, int seed) {
    vector<SoccerPlayerBoots> shuffled = data;

    // 운영체제에서 제공하는 진짜 난수(엔트로피) 가져오기(seed를 주기 위해 사용)
    std::random_device rd;
    // 메르센 트위스터(Mersenne Twister) 의사난수 생성기
    std::mt19937 gen(seed == -1 ? rd() : seed);

    // 셔플
    std::shuffle(
            shuffled.begin(),
            shuffled.end(),
            gen
    );

    size_t trainSize = static_cast<size_t>(shuffled.size() * trainRatio);
    vector<SoccerPlayerBoots> train(shuffled.begin(), shuffled.begin() + trainSize);
    vector<SoccerPlayerBoots> test(shuffled.begin() + trainSize, shuffled.end());

    return TrainTestSplit(train, test);
}
