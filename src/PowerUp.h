#pragma once
#include "ofMain.h"

enum class PowerUpEffect { DoublePoints, Speed, Dash, Size };

class PowerUpBox {
public:
    ofImage sprite;
    ofSoundPlayer sound; 
    ofVec2f pos{0,0};
    float radius = 28.0f;
    bool active = false;
    float spinDeg = 0.0f;

    void load() {
        sprite.load("itemBlock.png");
        sound.load("itemBlockSound.mp3");
        sound.setMultiPlay(true);
    }

    void spawn(float x, float y) {
        pos.set(x, y);
        active = true;
        spinDeg = 0.0f;
    }

    void update(float dt) {
        if (!active) return;
        spinDeg += 60.0f * dt;
        if (spinDeg >= 360.0f) spinDeg -= 360.0f;
    }

    void draw() {
        if (!active) return;
        ofPushMatrix();
        ofTranslate(pos.x, pos.y);
        ofRotateDeg(spinDeg);
        sprite.draw(-sprite.getWidth()/2, -sprite.getHeight()/2);
        ofPopMatrix();
    }

    void playSound() {
        sound.play();
    }
};
PowerUpEffect RandomPowerUpEffect();
