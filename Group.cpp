#include "Group.hpp"

group_t::group_t (GroupID id) : id (id) , players_in_group(new AVLTree<Player,Player, comaparePlayer>), highest_player_in_group(nullptr) {}
group_t::~group_t(){
    highest_player_in_group = nullptr;
}
Player group_t::find(Player player) const {
    return players_in_group->find(player);
}
void group_t::insert(Player player) {
    players_in_group->insert(player, player);
    highest_player_in_group = players_in_group->findMax();
}
void group_t::remove(Player player) {
    if (players_in_group == nullptr){
        return;
    }
    players_in_group->remove(player);
    highest_player_in_group = players_in_group->findMax();
}
const GroupID group_t::getId() const {
    return id; 
}
int group_t::getSize() const {
    if (players_in_group == nullptr){
        return 0;
    }
    return players_in_group->getSize();
}
void group_t::replace(group_t& group_to_remove) {
    players_in_group->merge(*(group_to_remove.players_in_group));
    group_to_remove.players_in_group = nullptr;
    highest_player_in_group = players_in_group->findMax();
    UpdatePlayersGroup update_players_group(this->getId());
    players_in_group->inOrder(update_players_group);
}
PlayerID group_t::getHighestPlayerId() const {
    return highest_player_in_group->getId();
}
void group_t::reverseInOrder(PlayerToArray& player_to_array){
    players_in_group->reverseInOrder(player_to_array);
}
