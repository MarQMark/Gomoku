#ifndef IGAMEEVENTLISTENER_H
#define IGAMEEVENTLISTENER_H

struct MoveViewDTO;

class IBoardEventListener {
public:
    virtual ~IBoardEventListener() = default;
    virtual void onMoveCompleted(const MoveViewDTO& move) = 0;
};

#endif
