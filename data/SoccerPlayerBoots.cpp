//
// Created by 허온 on 2026. 2. 7..
//

#include <sstream>
#include "SoccerPlayerBoots.h"

const vector<pair<string, string>> SoccerPlayerBoots::textKeys{
};

const vector<pair<string, string>> SoccerPlayerBoots::numKeys{
        {"player_age",    "player_age"},
        {"player_height", "player_height"},
        {"apps",          "Apps"},
        {"mins",          "Mins"},
        {"goals",         "Goals"},
        {"assists",       "Assists"},
        {"yel",           "Yel"},
        {"red",           "Red"},
        {"spg",           "SpG"},
        {"ps",            "PS"},
        {"aerials_won",   "AerialsWon"},
        {"motm",          "MotM"},
        {"rating",        "Rating"},
        {"tackles",       "Tackles"},
        {"inter",         "Inter"},
        {"fouls",         "Fouls"},
        {"offsides",      "Offsides"},
        {"clear",         "Clear"},
        {"drb",           "Drb"},
        {"blocks",        "Blocks"},
        {"own_g",         "OwnG"},
        {"key_p",         "KeyP"},
        {"fouled",        "Fouled"},
        {"off",           "Off"},
        {"disp",          "Disp"},
        {"uns_tch",       "UnsTch"},
        {"avg_p",         "AvgP"},
        {"crosses",       "Crosses"},
        {"long_b",        "LongB"},
        {"thr_b",         "ThrB"},
};

const vector<pair<string, string>> SoccerPlayerBoots::listKeys{
        {"player_positions",    "player_positions"},
        {"strengths", "strengths"},
        {"weaknesses", "weaknesses"},
        {"player_style", "player_style"}
};

const map<string, vector<string>> SoccerPlayerBoots::listVocabularies{
    {"player_positions", {
        "Attacking Midfielder",
        "Defender",
        "Defensive Midfielder",
        "Forward",
        "Goalkeeper",
        "Midfielder"
    }},
    {"strengths", {
        "Aerial Duels",
        "Ball interception",
        "Blocking the ball",
        "Concentration",
        "Crossing",
        "Defensive contribution",
        "Direct free-kicks",
        "Dribbling",
        "Finishing",
        "Headed attempts",
        "Holding on to the ball",
        "Key passes",
        "Long passing",
        "Long shots",
        "Passing",
        "Penalty saving",
        "Saving close range shots",
        "Saving long shots",
        "Shot stopping (Reflexes)",
        "Tackling",
        "Taking set-pieces",
        "Through balls"
    }},
    {"weaknesses", {
        "Aerial Duels",
        "Catching crosses",
        "Concentration",
        "Crossing",
        "Defensive contribution",
        "Discipline",
        "Finishing",
        "Holding on to the ball",
        "Long passing",
        "Offside awareness",
        "Passing",
        "Saving close range shots",
        "Saving long shots",
        "Tackling"
    }},
    {"player_style", {
        "Clears the ball out of defence often",
        "Comes off his line often",
        "Commits fouls often",
        "Counter attack threat",
        "Does not dive into tackles",
        "Gets fouled often",
        "Indirect set-piece threat",
        "Likes to cross",
        "Likes to cut inside",
        "Likes to do flick ons",
        "Likes to do layoffs",
        "Likes to dribble",
        "Likes to play long balls",
        "Likes to play short passes",
        "Likes to punch the ball",
        "Likes to shoot from distance",
        "Likes to tackle",
        "Plays the ball off the ground often"
    }}
};

void SoccerPlayerBoots::addNumeric(const string& key, const double& value) {
    numericStats[key] = value;
}

void SoccerPlayerBoots::addText(const string& key, const string& value) {
    textStats[key] = value;
}

void SoccerPlayerBoots::setLabel(const string& value) {
    label = value;
}

void SoccerPlayerBoots::addList(const string& key, string listStr) {
    listStats[key]; // 빈 리스트라도 키가 반드시 존재하도록 초기화

    // [ ] ' , 문자들을 제거하고 콤마로 분리하는 간단한 로직
    for(char &c : listStr)
        if(c == '[' || c == ']' || c == '\'') c = ' ';

    stringstream ss(listStr);
    string segment;
    while (getline(ss, segment, ',')) {
        // 앞뒤 공백 제거 후 추가
        segment.erase(0, segment.find_first_not_of(" "));
        segment.erase(segment.find_last_not_of(" ") + 1);
        if(!segment.empty()) listStats[key].push_back(segment);
    }
}

double SoccerPlayerBoots::getNum(const string& key) const {
    return numericStats.at(key);
}

string SoccerPlayerBoots::getText(const string& key) const {
    return textStats.at(key);
}

string SoccerPlayerBoots::getLabel() const {
    return label;
}

vector<string> SoccerPlayerBoots::getList(const string& key) const {
    return listStats.at(key);
}

vector<pair<string, double>> SoccerPlayerBoots::convertNumVectorPair() const {
    vector<pair<string, double>> results;
    for (const auto &[k, v]: numericStats)
        results.emplace_back(k, v);
    return results;
}

vector<pair<string, string>> SoccerPlayerBoots::convertTextVectorPair() const {
    vector<pair<string, string>> results;
    for(const auto& [k, v] : textStats)
        results.emplace_back(k, v);
    return results;
}

vector<pair<string, vector<string>>> SoccerPlayerBoots::convertListVectorPair() const {
    vector<pair<string, vector<string>>> results;
    for(const auto& [k, v] : listStats)
        results.emplace_back(k, v);
    return results;
}

void SoccerPlayerBoots::removeText(const std::string &key) {
    textStats.erase(key);
}