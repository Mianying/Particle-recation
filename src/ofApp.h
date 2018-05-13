#pragma once

#include "ofMain.h"

class Particle {
public:
    Particle();
    void setup();
    void update(float dt);
    void draw();
    
 
    ofPoint pos;
    ofPoint vel;
    float time;
    float lifeTime;
    ofColor color;
    bool live;
   

};


class Params {
public:
    void setup();
    ofPoint eCenter;    //Emitter center
    float eRad;         //Emitter radius
    float velRad;       //Initial velocity limit
    float lifeTime;     //Lifetime in seconds
    float rotate;
    float force;
    float spinning;
    float friction;

    
    ofSoundPlayer beat;
    float * fftSmooth;
    int bands;
    
    void keyPressed(int key);
};


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    // create something new
    vector<Particle> p;
    float bornRate;
    float bornCount;
    ofFbo fbo;              //Offscreen buffer for trails
    float history;          //Control parameter for trails
    float time0;            //Time value for computing dt
    
    
   

};
