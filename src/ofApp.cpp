#include "ofApp.h"

Params param; //Definition of global variable

void Params::setup(){
    eCenter = ofPoint(ofGetWidth()/2,ofGetHeight()/2);
    eRad = 4000;
    velRad = 2;
    lifeTime = 2;
    rotate = 0;
    force = -50;
    spinning = 800;
    friction = 0.01;
    
    beat.loadSound("8.mp3");
    beat.play();
    fftSmooth = new float[8912];
    for(int i=0; i < 8912; i++){
        bands= 1;
        beat.setLoop(true);
        beat.setVolume(1);
    }
   
}

Particle::Particle(){
    live = false;
}

ofPoint ramdomPointInCircle( float maxRad ){
    ofPoint pnt;
    float rad = ofRandom( 0, maxRad);
    float angle = ofRandom( 0, M_TWO_PI );
    pnt.x = cos( angle ) * rad;
    pnt.y = sin( angle ) * rad;
    return pnt;
}

void Particle::setup() {
    
    pos = param.eCenter + ramdomPointInCircle( param.eRad );
    vel = ramdomPointInCircle( param.velRad );
    time = 0;
    lifeTime = param.lifeTime;
    live = true;
 
}

void Particle::update( float dt ){
    if ( live ) {
        ofSoundUpdate();
        float * value = ofSoundGetSpectrum(param.bands);
        for (int i = 0 ; i < param.bands; i++) {
            param.fftSmooth [i] *=0.99f;
            if(param.fftSmooth[i] < value[i]){
                param.fftSmooth[i] = value[i];
            }
            
        //Rotate vel
        vel.rotate( 0, 0, param.rotate * dt );
        ofPoint acc; //Acceleration
        ofPoint delta = pos - param.eCenter + (param.fftSmooth[i]*8000);
        float len = delta.length();
        for(int i=0; i<param.bands;i++){
        if ( ofInRange( len, 0, param.eRad) ) {
            delta.normalize();
            //Attraction/repulsion force
            acc += delta * param.force + (param.fftSmooth[i]*11250) ;
            //Spinning force
            acc.x += -delta.y * param.spinning + (param.fftSmooth[i]*2550);
            acc.y += delta.x * param.spinning- (param.fftSmooth[i]*15000);
            }
        
        }
        vel += acc * dt; //Euler method
        vel *= ( 1 - param.friction ); //Friction
        //Update pos
        pos += vel * dt; //Euler method
        //check  die
        time += dt;
        if ( time >= lifeTime) {
            live = false;
        }
        }
    }
}



void Particle::draw(){
    if ( live ) {
        //size
        float size = ofMap(fabs(time - lifeTime/2), 0, lifeTime/2, 3, 1 );
        //color
        for(int i=0; i<param.bands;i++){
        ofColor color = ofColor::red;
        float hue = ofMap( time, 0, lifeTime, 100, 155 );
        
        color.setHue( hue );
        ofSetColor( color );
        ofPushMatrix();
        ofDrawCircle(pos, size);
        ofPopMatrix();
        }
    }
}

void ofApp::setup(){
 
    ofSetFrameRate( 60 ); //Set screen frame rate
    //Allocate drawing buffer
    int w = ofGetWidth();
    int h = ofGetHeight();
    fbo.allocate( w, h, GL_RGB32F_ARB );
    //Fill buffer with white color
    fbo.begin();
    ofBackground(255, 255, 255);
    fbo.end();
    //Set up parameters
    param.setup(); //Global parameters
    history = 0.6;
    bornCount = 0;     //number of the particle to born
    bornRate = 0;
    bornCount = 0;
    //ofEnableDepthTest();
    //ofSetVerticalSync(true);
    //cam.setDistance(160);
}

void ofApp::update(){
    
    ofSoundUpdate();
    float * value = ofSoundGetSpectrum(param.bands);
    for (int a = 0 ; a < param.bands; a++) {
        param.fftSmooth [a] *=0.99f;
        if(param.fftSmooth[a] < value[a]){
            param.fftSmooth[a] = value[a];
        }
    
    //Compute dt
    float time = ofGetElapsedTimef();
    float dt = ofClamp( time - time0, 0, 0.1 );
    time0 = time;
    //Delete inactive particles
    int i=0;
    while (i < p.size()) {
        if ( !p[i].live ) {
            p.erase( p.begin() + i );
        }
        else {
            i++;
        }
    }
    
    //Born new particles
    bornCount += dt * bornRate; //Update bornCount value
    if ( bornCount >= 1 ) { //It's time to born particle(s)
        int bornN = int( bornCount );//How many born
        bornCount -= bornN; //Correct bornCount value
        for (int i=0; i<bornN; i++) {
            Particle newP;
            newP.setup(); //Start a new particle
            p.push_back( newP ); //Add this particle to array
        }
    }
    
    //Update the particles
    for (int i=0; i<p.size(); i++) {
        p[i].update( dt );
    }
}
}
        
        
void ofApp::draw(){
        
        //Set white background
        ofBackground( 255, 255, 255 );
        fbo.begin();
        //Draw semi-transparent white rectangle
        //to slightly clearing a buffer (depends on history value)
        ofEnableAlphaBlending(); //Enable transparency
        float alpha = (1-history) * 255;
        ofSetColor( 0, 0, 0, alpha );
        ofFill();
        ofDrawRectangle( 0, 0, ofGetWidth(), ofGetHeight() );
        ofDisableAlphaBlending(); //Disable transparency
        //Draw the particle
        //Draw the particles
        ofFill();
        for (int i=0; i<p.size(); i++) {
        p[i].draw();
    }
        fbo.end();
        //2. Draw buffer on the screen
        ofSetColor( 255, 255, 255 );
        fbo.draw( 0, 0 );
        history = 0.8;
        for (int a = 0 ; a < param.bands; a++) {
        bornRate = (param.fftSmooth[a]*22000);
        }
    }


