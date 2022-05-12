#ifndef PLAYERS_MANAGER_H
#define PLAYERS_MANAGER_H

#include "library1.h"
#include "Group.hpp"
#include <memory>

class PlayersManager { 
    
    std::unique_ptr<AVLTree<GroupID, Group, compareInt>> groups;
    std::unique_ptr<AVLTree<GroupID, Group, compareInt>> active_Groups;
    std::unique_ptr<AVLTree<PlayerID, Player, compareInt>> players;
    std::unique_ptr<AVLTree<Player, Player, comaparePlayer>> players_by_level; 
    Player highest_level_player;
    
    public: 
    PlayersManager();
    PlayersManager(const PlayersManager&) = delete;
    PlayersManager& operator=(const PlayersManager&) = delete;
    ~PlayersManager();

    StatusType AddGroup(int GroupID);
    StatusType AddPlayer(int PlayerID, int GroupID, int Level);
    StatusType RemovePlayer(int PlayerID);
    StatusType ReplaceGroup(int GroupID, int ReplacementID);
    StatusType IncreaseLevel(int PlayerID, int LevelIncrease);
    StatusType GetHighestLevel(int GroupID, int *PlayerID);
    StatusType GetAllPlayersByLevel(int GroupID, int **Players, int *numOfPlayers);
    StatusType GetGroupsHighestLevel(int numOfGroups, int **Players); 

};

#endif
