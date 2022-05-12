#ifndef Player_H
#define Player_H

#include <memory>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <string.h>


typedef int PlayerID;
typedef int PlayerLevel;
typedef int GroupID;

class player_t;


typedef std::shared_ptr<player_t> Player;

class player_t{
    private:
        PlayerID id;
        PlayerLevel level; 
        GroupID group_id;  

    public:
        player_t(PlayerID Id, PlayerLevel level, GroupID group_Id) : id(Id), level(level), group_id(group_Id) {}
        void icreaseLevel(PlayerLevel level_upgrade);
        const PlayerID getId() const;
        PlayerLevel getLevel() const;
        GroupID getGroupId() const;
        void setGroupId(GroupID id);
        bool operator>(const player_t& player2) const; 
        void print(){
            std::cout<< id;
            std::cout<<"  ";
            std::cout << level ;
            std::cout<<"  ";
            std::cout<< group_id; 
            std::cout<<"  " << std::endl;
        }
};

class compareInt{
    public:
    bool operator()(const int& a, int& b){
        return a > b;
    }
};

class comaparePlayer{
    public:
    bool operator()(const Player& p1,const Player& p2){
        return *p1 > *p2;
    }
};
  
#endif