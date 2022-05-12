#include "PlayersManager.hpp"

    PlayersManager::PlayersManager() :
         groups(new AVLTree<GroupID, Group, compareInt>),
          active_Groups(new AVLTree<GroupID, Group, compareInt>),
           players(new  AVLTree <PlayerID, Player,compareInt >),
            players_by_level(new AVLTree <Player, Player, comaparePlayer>),
             highest_level_player(nullptr) {} 

    PlayersManager::~PlayersManager(){
        highest_level_player = nullptr;
    }
    StatusType PlayersManager::AddGroup(int GroupID){
        if (GroupID <= 0 ){
            return INVALID_INPUT; 
        }
        if (groups->find(GroupID) != nullptr){
            return FAILURE; 
        }
        try {
            Group new_group = Group(new group_t(GroupID));
            groups->insert(GroupID, new_group);
        }   catch (std::bad_alloc& e) {
		        return ALLOCATION_ERROR; 
            }
        return SUCCESS;
    }
    StatusType PlayersManager::AddPlayer(int PlayerID, int GroupID, int Level){
        if (GroupID <= 0 || PlayerID <= 0 || Level < 0){
            return INVALID_INPUT; 
        }
        Group the_group = groups->find(GroupID);
        Player exsisting_player = players->find(PlayerID);
        if (the_group == nullptr || (exsisting_player != nullptr)){
            return FAILURE;
        }
        try{
            Player new_player = Player(new player_t(PlayerID, Level, GroupID));
            players->insert(PlayerID, new_player);
            players_by_level->insert(new_player, new_player);
            highest_level_player = players_by_level->findMax();
            the_group->insert(new_player);
            if (the_group->getSize() == 1){
                active_Groups->insert(GroupID, the_group);
            }
        }   catch (std::bad_alloc& e) {
		        return ALLOCATION_ERROR; 
            }
        return SUCCESS;
    }
    StatusType PlayersManager::RemovePlayer(int PlayerID){
        if ( PlayerID <= 0 ){
            return INVALID_INPUT; 
        }
        Player player_to_remove = players->find(PlayerID);
        if (player_to_remove == nullptr){
            return FAILURE;
        }
        players->remove(PlayerID);
        players_by_level->remove(player_to_remove);
        highest_level_player = players_by_level->findMax(); //if empty - should return nullptr
        assert(player_to_remove != nullptr);
        Group the_group = active_Groups->find(player_to_remove->getGroupId());
        the_group->remove(player_to_remove);
        if (the_group->getSize() == 0){
            active_Groups->remove(the_group->getId());
        }
        return SUCCESS;
    }
    StatusType PlayersManager::ReplaceGroup(int GroupID, int ReplacementID){
        if (GroupID <= 0 || ReplacementID <= 0 || GroupID == ReplacementID){
            return INVALID_INPUT;
        }
        Group group_to_remove = groups->find(GroupID);
        Group replacement_group = groups->find(ReplacementID);
        if (group_to_remove == nullptr || replacement_group == nullptr){
            return FAILURE;
        }
        try{
        if (replacement_group->isEmpty() == true && group_to_remove->isEmpty() == true) {
            groups->remove(GroupID); // group to remove has no players, so it's not in active_Groups
            return SUCCESS;
        }
        else if (replacement_group->isEmpty() == true) {
            active_Groups->insert(replacement_group->getId(), replacement_group);
        }
        replacement_group->replace(*group_to_remove);
    }   catch (std::bad_alloc& e) {
            return ALLOCATION_ERROR;
        }
        assert(group_to_remove->getSize() == 0);
        groups->remove(GroupID);
        active_Groups->remove(GroupID);
        return SUCCESS;    
    }
    StatusType PlayersManager::IncreaseLevel(int PlayerID, int LevelIncrease){
        if ( PlayerID <= 0 || LevelIncrease <= 0 ){
            return INVALID_INPUT; 
        }
        Player player_to_increase = players->find(PlayerID);
        if (player_to_increase == nullptr){
            return FAILURE;
        }
        int id = player_to_increase->getId();
        GroupID group_id = player_to_increase->getGroupId();
        PlayerLevel level = player_to_increase->getLevel();
        RemovePlayer(PlayerID);
        AddPlayer(id, group_id, level + LevelIncrease);
        return SUCCESS;
    }
    StatusType PlayersManager::GetHighestLevel(int GroupID, int *PlayerID){
        if (GroupID==0 || PlayerID == nullptr){
            return INVALID_INPUT;
        }
        if (GroupID < 0){
            if (highest_level_player == nullptr){
                *(PlayerID) = -1;
                return SUCCESS;
            }
            *(PlayerID) = highest_level_player->getId();
             return SUCCESS;
        }
        Group the_group = groups->find(GroupID);
        assert(GroupID > 0 );
        if (the_group == nullptr){
            return FAILURE;
        }
        if (the_group->getSize() == 0){
            *(PlayerID) = -1;
            return SUCCESS;
        }
        *(PlayerID) = the_group->getHighestPlayerId();
        return SUCCESS;
    }
    StatusType PlayersManager::GetAllPlayersByLevel(int GroupID, int **Players, int *numOfPlayers){
        if (GroupID == 0 || Players == nullptr || numOfPlayers == nullptr){
            return INVALID_INPUT;
        }
        if(players->getSize() == 0){
            Group the_group = groups->find(GroupID);
            if (GroupID > 0 && the_group == nullptr) {
                return FAILURE;
            }
            
            *Players = nullptr;
            *(numOfPlayers) = 0;
            return SUCCESS;
        }
        int number_of_players = 0;
        if (GroupID < 0){
             number_of_players = players->getSize();
        }
        else {
            Group the_group = groups->find(GroupID);
            if (the_group == nullptr) {
                return FAILURE;
            }
            if(the_group->getSize() == 0){
                *Players = nullptr;
                *(numOfPlayers) = 0;
                return SUCCESS;
            }
            number_of_players = the_group->getSize();
        }
        *Players = (int*)malloc(sizeof(int)*(number_of_players + 1));
        if (*Players == nullptr){
            return ALLOCATION_ERROR;
        }
        *numOfPlayers = number_of_players;
        PlayerToArray playerToArray(*Players);
        if (GroupID < 0){
            players_by_level->reverseInOrder(playerToArray);
        }
        else {
            (groups->find(GroupID))->reverseInOrder(playerToArray);
        }    
        return SUCCESS;
    }
    StatusType PlayersManager::GetGroupsHighestLevel(int numOfGroups, int **Players){
        if (numOfGroups < 1 || Players == nullptr){
            return INVALID_INPUT;
        }
        if (numOfGroups > active_Groups->getSize()){
            return FAILURE;
        }
        *Players = (int*) malloc(sizeof(int) * (numOfGroups+1));
        if (*Players == nullptr){
            return ALLOCATION_ERROR;
        }
        PlayerToArray playerToArray(*Players, numOfGroups);
        active_Groups->inOrder(playerToArray);

        return SUCCESS;
    }
