//
// Created by 허온 on 2026. 2. 17..
//

#ifndef SOCCER_BOOTS_BAYES_PROJECT_APISERVER_H
#define SOCCER_BOOTS_BAYES_PROJECT_APISERVER_H

#include <crow.h>
#include <crow/middlewares/cors.h>

class ApiServer {
private:
    crow::App<crow::CORSHandler> app_;
    int port_;

public:
    // explicit 암시적 변환 막기
    explicit ApiServer(int port = 8080);

    // 컨트롤러가 라우트를 등록할 수 있도록 app 참조 반환
    crow::App<crow::CORSHandler>& getApp();

    void run();
};

#endif //SOCCER_BOOTS_BAYES_PROJECT_APISERVER_H
