#ifndef NODE_STATE_H
#define NODE_STATE_H

struct NodeState {
  bool positionKnown = false;
  int8_t pos[3] = {0, 0, 0};
  bool isSeed = false;
  uint16_t rounds = 0;

  NodeState(bool isSeedNode){
    isSeed = isSeedNode;
  }
};

#endif // NODE_STATE_H