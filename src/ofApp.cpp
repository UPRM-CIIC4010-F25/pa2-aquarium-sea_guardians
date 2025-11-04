#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetFrameRate(60);
    ofSetBackgroundColor(ofColor::blue);

    
    gameManager = std::make_unique<GameSceneManager>();
    backgroundImage.load("background.png");
    backgroundImage.resize(ofGetWindowWidth(), ofGetWindowHeight());
    spriteManager = std::make_shared<AquariumSpriteManager>();
    gameManager->AddScene(std::make_shared<GameIntroScene>(
        GameSceneKindToString(GameSceneKind::GAME_INTRO),
        std::make_shared<GameSprite>("title.png", ofGetWindowWidth(), ofGetWindowHeight())
    ));

    //Crea Aquarium 
    myAquarium = std::make_shared<Aquarium>(ofGetWindowWidth(), ofGetWindowHeight(), spriteManager);
    player = std::make_shared<PlayerCreature>(
        ofGetWindowWidth()/2 - 50,
        ofGetWindowHeight()/2 - 50,
        DEFAULT_SPEED,
        this->spriteManager->GetSprite(AquariumCreatureType::NPCreature)
    );
    player->setDirection(0, 0);
    player->setBounds(ofGetWindowWidth() - 20, ofGetWindowHeight() - 20);

    // Niveles y población inicial
    myAquarium->addAquariumLevel(std::make_shared<Level_0>(0, 10));
    myAquarium->addAquariumLevel(std::make_shared<Level_1>(1, 15));
    myAquarium->addAquariumLevel(std::make_shared<Level_2>(2, 20));
    myAquarium->Repopulate();

    gameManager->AddScene(std::make_shared<AquariumGameScene>(
        player, myAquarium, GameSceneKindToString(GameSceneKind::AQUARIUM_GAME)
    ));

    //Escena de GAME OVER
    gameManager->AddScene(std::make_shared<GameOverScene>(
        GameSceneKindToString(GameSceneKind::GAME_OVER),
        std::make_shared<GameSprite>("game-over.png", ofGetWindowWidth(), ofGetWindowHeight())
    ));

    // hitmark setup
    bool ok = hitmarkImage.load("hitmark.png");
    hitmarkImage.setAnchorPercent(0.5f, 0.5f);
    hitmarkSound.load("hitmarkSound.mp3");
    hitmarkSound.setMultiPlay(true);
    hitmarkSound.setVolume(0.9f);
    if (hitmarkSound.isLoaded()) {
    }
    ofSetLogLevel(OF_LOG_NOTICE);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if(gameManager->GetActiveSceneName() == GameSceneKindToString(GameSceneKind::GAME_OVER)){
        return; // Stop updating if game is over or exiting
    }

    if(gameManager->GetActiveSceneName() == GameSceneKindToString(GameSceneKind::AQUARIUM_GAME)){
        auto gameScene = std::static_pointer_cast<AquariumGameScene>(gameManager->GetActiveScene());
        if(gameScene->GetLastEvent() != nullptr && gameScene->GetLastEvent()->isGameOver()){
            hit.active = false; // disable hitmarker
            hit.timer = 0;
            gameManager->Transition(GameSceneKindToString(GameSceneKind::GAME_OVER));
            return;
        }
        
    }

    gameManager->UpdateActiveScene();

 //update hitmarker 
 if(hit.active && --hit.timer <= 0) hit.active = false;
 }


//--------------------------------------------------------------
void ofApp::draw(){
    backgroundImage.draw(0, 0);
    gameManager->DrawActiveScene();
    
    //draw hitmarker
    if (gameManager->GetActiveSceneName() == GameSceneKindToString(GameSceneKind::AQUARIUM_GAME)) {
        if (hit.active && hitmarkImage.isAllocated()){
        ofPushStyle();
        ofEnableAlphaBlending();
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        float a = 255.0f * hit.alpha();
        float imgW = hitmarkImage.getWidth();
        float imgH = hitmarkImage.getHeight();
        float base = (hit.sizePx > 0 ? hit.sizePx : imgW);
        float shrink = 0.3f; 
        float w = base * shrink;
        float h = (base * imgH / imgW) * shrink;
        ofSetColor(255,255,255,(int)a);
        hitmarkImage.draw(hit.x, hit.y, w, h);
        ofPopStyle();
        }
    }
}

//--------------------------------------------------------------
void ofApp::exit(){
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (lastEvent.isGameExit()) { 
        ofLogNotice() << "Game has ended. Press ESC to exit." << std::endl;
        return; // Ignore other keys after game over
    }
    if(gameManager->GetActiveSceneName() == GameSceneKindToString(GameSceneKind::AQUARIUM_GAME)){
        auto gameScene = std::static_pointer_cast<AquariumGameScene>(gameManager->GetActiveScene());
        switch(key){
            case OF_KEY_UP:
                gameScene->GetPlayer()->setDirection(gameScene->GetPlayer()->isXDirectionActive()?gameScene->GetPlayer()->getDx():0, -1);
                break;
            case OF_KEY_DOWN:
                gameScene->GetPlayer()->setDirection(gameScene->GetPlayer()->isXDirectionActive()?gameScene->GetPlayer()->getDx():0, 1);
                break;
            case OF_KEY_LEFT:
                gameScene->GetPlayer()->setDirection(-1, gameScene->GetPlayer()->isYDirectionActive()?gameScene->GetPlayer()->getDy():0);
                gameScene->GetPlayer()->setFlipped(true);
                break;
            case OF_KEY_RIGHT:
                gameScene->GetPlayer()->setDirection(1, gameScene->GetPlayer()->isYDirectionActive()?gameScene->GetPlayer()->getDy():0);
                gameScene->GetPlayer()->setFlipped(false);
                break;
            case 'b':
            case 'B': {
                auto gameScene = std::static_pointer_cast<AquariumGameScene>(gameManager->GetActiveScene());
                if (gameScene) {
                    auto aq = gameScene->GetAquarium();
                    auto pl = gameScene->GetPlayer();
                    if (aq && pl) {
                        aq->itemBlock.load();                 // ensure assets loaded
                        aq->itemBlock.spawn(pl->getX()+80.f, pl->getY());
                        aq->itemBlock.playSound();
                        aq->itemBlockPresent = true;
                    }
                }
                break;
            }
            
            case OF_KEY_SHIFT: {
                auto gameScene = std::static_pointer_cast<AquariumGameScene>(gameManager->GetActiveScene());
                if (gameScene) {
                    gameScene->GetPlayer()->tryDash(ofGetElapsedTimef());
                }
                break;
            }
            default:
                break;
        }
    
        
    
        gameScene->GetPlayer()->move();
        return;

    }

    if(gameManager->GetActiveSceneName() == GameSceneKindToString(GameSceneKind::GAME_INTRO)){
        switch (key)
        {
        case OF_KEY_SPACE:
            gameManager->Transition(GameSceneKindToString(GameSceneKind::AQUARIUM_GAME));
            break;
        
        default:
            break;
        }
    }


    
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(gameManager->GetActiveSceneName() == GameSceneKindToString(GameSceneKind::AQUARIUM_GAME)){
        auto gameScene = std::static_pointer_cast<AquariumGameScene>(gameManager->GetActiveScene());
    if( key == OF_KEY_UP || key == OF_KEY_DOWN){
        gameScene->GetPlayer()->setDirection(gameScene->GetPlayer()->isXDirectionActive()?gameScene->GetPlayer()->getDx():0, 0);
        gameScene->GetPlayer()->move();
        return;
    }
    
    if(key == OF_KEY_LEFT || key == OF_KEY_RIGHT){
        gameScene->GetPlayer()->setDirection(0, gameScene->GetPlayer()->isYDirectionActive()?gameScene->GetPlayer()->getDy():0);
        gameScene->GetPlayer()->move();
        return;
    }

    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    backgroundImage.resize(w, h);
    auto aquariumScene = std::static_pointer_cast<AquariumGameScene>(gameManager->GetScene(GameSceneKindToString(GameSceneKind::AQUARIUM_GAME)));
    aquariumScene->GetAquarium()->setBounds(w,h);
    aquariumScene->GetPlayer()->setBounds(w - 20, h - 20);

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
