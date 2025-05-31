//
// Created by Henk on 31.05.2025.
//

#ifndef IGAMEEVENTLISTENER_H
#define IGAMEEVENTLISTENER_H

struct MoveViewDTO;

class IGameEventListener {
public:
    virtual ~IGameEventListener() = default;
    virtual void onMoveCompleted(const MoveViewDTO& move) = 0;
};

#endif
