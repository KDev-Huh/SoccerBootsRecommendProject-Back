//
// Created by 허온 on 2026. 2. 17..
//

#include "ApiServer.h"

ApiServer::ApiServer(int port)
    : port_(port) {
    // Global CORS 설정
    auto& cors = app_.get_middleware<crow::CORSHandler>();
    cors.global()
        .origin("*")
        .methods("POST"_method, "GET"_method, "OPTIONS"_method)
        .headers("Content-Type", "Accept", "Origin");
}

crow::App<crow::CORSHandler>& ApiServer::getApp() {
    return app_;
}

void ApiServer::run() {
    app_.port(port_).multithreaded().run();
}
