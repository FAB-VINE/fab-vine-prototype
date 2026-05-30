#  Neighbor Communication

This docs refers to the experiments performed in the files of the following folder: 
[NODE-LOGIC/fab-vine-node/V1/firmware/neighbor-communication](NODE-LOGIC/fab-vine-node/V1/firmware/neighbor-communication)

This is an attempt for crating a mesh of cells that know their neighbors and can react to their state, in this case the state is linked with the delay for the blink. And it will change towards the highest of the group.

It follows this goals:
- 3 boards each with 2 connections (neighbors)
- Initialize state with random integer values
- each board runs this code:
    - keep sending its state to both neighbors
    - search for neighbors
        - if found
            - get state from neighbor
            - compare with current state
                - if neighbor state is bigger
                - change state to bigger one
- after multiple clock cycles they all should converge to have same state
