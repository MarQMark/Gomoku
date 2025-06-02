#ifndef IGAMEEVENTLISTENER_H
#define IGAMEEVENTLISTENER_H

struct MoveViewDTO;
struct GameCompleteViewDTO;

class IBoardEventListener {
public:
    virtual ~IBoardEventListener() = default;
    virtual void onGameStarted() = 0;
    virtual void onMoveCompleted(const MoveViewDTO& move) = 0;
    virtual void onGameCompleted(GameCompleteViewDTO completeView) = 0;
};

#endif
