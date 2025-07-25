
#ifndef TIME_H
#define TIME_H

class Time {
public:
    double getDeltaTime() const;
    static double getTime();

    void update();

    static void init();
    static Time* get();

private:
    double _delta_time;
    double _last_time;
    Time();
    static Time* s_instance;
};

#endif
