//
// Created by Henk on 02.06.2025.
//

#ifndef IMENUEVENTLISTENER_H
#define IMENUEVENTLISTENER_H

struct StatsViewDTO;

class IMenuEventListener {
public:
    virtual ~IMenuEventListener() {};
    virtual void onGameCompleted() = 0;
    virtual void onStatsChanged(StatsViewDTO stats) = 0;
};

#endif //IMENUEVENTLISTENER_H
