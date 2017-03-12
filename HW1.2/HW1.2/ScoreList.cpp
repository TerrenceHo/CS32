//
//  ScoreList.cpp
//  HW1
//
//  Created by Kevin on 1/22/17.
//  Copyright © 2017 Kevin. All rights reserved.
//

#include "ScoreList.h"
ScoreList::ScoreList():m_scoreList(){}

bool ScoreList::add(unsigned long score){
    if(score <=100){
        m_scoreList.insert(score);
        return true;
    }
    return false;
}

bool ScoreList::remove(unsigned long score){
    if(m_scoreList.find(score) != -1){
        m_scoreList.erase(m_scoreList.find(score));
        return true;
    }
    return false;
}

int ScoreList::size() const{
    return m_scoreList.size();
}

unsigned long ScoreList::minimum() const{
    if(m_scoreList.empty())
        return NO_SCORE;
    unsigned long minScore = 100;
    unsigned long temp = 100;
    for(int i = 0; i < m_scoreList.size();i++){
        m_scoreList.get(i,temp);
        if(temp < minScore)
            minScore = temp;
    }
    return minScore;
}

unsigned long ScoreList::maximum() const{
    if(m_scoreList.empty())
        return NO_SCORE;
    unsigned long maxScore = 0;
    unsigned long temp = 0;
    for(int i = 0; i < m_scoreList.size();i++){
        m_scoreList.get(i,temp);
        if(temp > maxScore)
            maxScore = temp;
    }
    return maxScore;
}
