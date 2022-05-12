#ifndef Group_H
#define Group_H


#include "AVLTree.hpp"
#include "Player.hpp"

class group_t;

typedef std::shared_ptr<group_t> Group;
 
class PlayerToArray;
class group_t {

    GroupID id; 
    std::unique_ptr<AVLTree<Player, Player, comaparePlayer>> players_in_group;
    Player highest_player_in_group; 

    public:
    group_t (GroupID id) ;
    group_t (const group_t& other_group) = delete;
    group_t& operator= (const group_t&) = delete;
    ~group_t(); 

    Player find(Player player) const;
    void insert(Player player);
    void remove(Player player);
    const GroupID getId() const ;
    int getSize() const;
    void replace(group_t& group_to_remove);
    PlayerID getHighestPlayerId() const;
    void reverseInOrder(PlayerToArray& player_to_array);

    bool isEmpty() const { return players_in_group->isEmpty(); }

};

class PlayerToArray{
    int* playerIdArray;
    int num_of_groups;
    int index;
    
    public:
    PlayerToArray(int* playerIdArray, int num_of_groups = -1) : playerIdArray(playerIdArray), num_of_groups(num_of_groups), index(0){}
    void operator()(Player &player){
        playerIdArray[index] = player->getId();
        index++;
    }
    void operator()(Group &group){
        if (group->isEmpty()) // necessary?
            return;
        if (num_of_groups == 0) {
            return;
        }
        playerIdArray[index] = group->getHighestPlayerId();
        index++;
        num_of_groups--;
    }
    int getIndex() const{
        return index;
    }
};

class UpdatePlayersGroup {
        GroupID group_id;
    public:
        UpdatePlayersGroup(GroupID group_id) : group_id(group_id) {}
        void operator()(Player player) {
            player->setGroupId(group_id);
        }
};


#endif  
