#ifndef __MYMATH_H__
#define __MYMATH_H__
#include "framework.h"
#include <vector>

using namespace std;

class CatmullRom {
public:
    vector<vec3> cps;        // control points 
    vector<float>  ts;    // parameter (knot) values
private:
    vec3 Hermite(vec3 p0, vec3 v0, float t0,
        vec3 p1, vec3 v1, float t1, float t) {
        const float dt = t1 - t0;

        vec3 a0 = p0;
        vec3 a1 = v0;
        vec3 a2 = (((p1 - p0) * (float)3) / (dt*dt)) - ((v1 + v0 * 2) / dt);
        vec3 a3 = (((p0 - p1)* (float)2) / (dt*dt*dt)) + ((v1 + v0) / (dt*dt));

        return a3 * pow(t - t0, 3) + a2 * pow(t - t0, 2) + a1 * (t - t0) + a0;
    }

    vec3 v_i(vec3 r0, vec3 r1, vec3 r2, float t0, float t1, float t2)
    {
        return ((r2 - r1) / (t2 - t1) + ((r1 - r0) / (t1 - t0))) / 2;
    }

public:
    void AddControlPoint(vec3 cp, float t)
    {
        cps.push_back(cp);
        ts.push_back(t);
    }

    vec3 r(float t) {

        if (cps.size() > 1)
        {
            vec3 v0 = v_i(vec3(0, 0, 0), cps[0], cps[1], 0, ts[0], ts[1]);
            vec3 v1;

            for (unsigned i = 0; i < cps.size() - 1; i++)
            {
                if (i + 2 == cps.size())
                {
                    v1 = v0;
                }
                else
                {
                    v1 = v_i(cps[i], cps[i + 1], cps[i + 2], ts[0], ts[1], ts[2]);
                }

                if (ts[i] <= t && t <= ts[i + 1])
                {
                    return Hermite(cps[i], v0, ts[i], cps[i + 1], v1, ts[i + 1], t);
                }

                v0 = v1;
            }

            //throw std::exception("Invalid t operator!"); TODO
        }
        else if (cps.size() == 1)
        {
            return cps[0];
        }

        //throw std::exception("No control points!"); TODO
    }
};

#endif