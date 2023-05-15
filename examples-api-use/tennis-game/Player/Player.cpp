#include "Player.h"

Player::Player(int playerNumber) : _playerNumber(playerNumber) {
  _points = 0;
  _games = 0;
  _sets = 0;
  _matches = 0;
  _mode = 0;
  _setting = 0;
}
Player::~Player() {}

void Player::setPoints(int points) {
  _points = points;
}
int Player::getPoints() {
  return _points;
}

void Player::setGames(int games) {
  _games = games;
}
int Player::getGames() {
  return _games;
}

void Player::setSets(int sets) {
  _sets = sets;
}
int Player::getSets() {
  return _sets;
}

void Player::setMatches(int matches) {
  _matches = matches;
}
int Player::getMatches() {
  return _matches;
}

void Player::setMode(int mode) {
  _mode = mode;
}
int Player::getMode() {
  return _mode;
}

void Player::setSetting(int setting) {
  _setting = setting;
}
int Player::getSetting() {
  return _setting;
}

int Player::incrementSetting() {
  return ++_setting;
}

int Player::number() {
  return _playerNumber;
}
