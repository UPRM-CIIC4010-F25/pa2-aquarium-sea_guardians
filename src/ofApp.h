#pragma once

#include "ofMain.h"
#include "Aquarium.h"

//Hitmarker added for fun
	struct hitmark{
		bool active = false;
		float x = 0, y = 0;
		int timer = 0;
		int maxTimer = 2;
		float sizePx = 64.0f;

		void trigger(float px, float py, int dur=30, float size=64.0f){ 
			x=px; y=py; 
			timer = maxTimer = dur;
			sizePx = size; 
			active = true; 
		}
		float alpha() const { return active ? 1.0f : 0.0f;  }
    	float scale() const { return 0.8f + 0.4f * (1.0f - alpha()); }
		};
class ofApp : public ofBaseApp{

	public:
		void setup() override;
		void update() override;
		void draw() override;
		void exit() override;

		void keyPressed(int key) override;
		void keyReleased(int key) override;
		void mouseMoved(int x, int y ) override;
		void mouseDragged(int x, int y, int button) override;
		void mousePressed(int x, int y, int button) override;
		void mouseReleased(int x, int y, int button) override;
		void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
		void mouseEntered(int x, int y) override;
		void mouseExited(int x, int y) override;
		void windowResized(int w, int h) override;
		void dragEvent(ofDragInfo dragInfo) override;
		void gotMessage(ofMessage msg) override;
	

		char moveDirection;
		int DEFAULT_SPEED = 5;


		AwaitFrames acuariumUpdate{5};

		ofTrueTypeFont gameOverTitle;
		GameEvent lastEvent;


		ofImage backgroundImage;
		hitmark hit;
		ofImage hitmarkImage;
		ofSoundPlayer hitmarkSound;

		std::unique_ptr<GameSceneManager> gameManager;
		std::shared_ptr<AquariumSpriteManager>spriteManager;
		std::shared_ptr<PlayerCreature> player;
		std::shared_ptr<Aquarium> myAquarium;
		
};
