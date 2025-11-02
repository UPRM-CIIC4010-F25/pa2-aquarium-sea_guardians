#include "Aquarium.h"
#include <cstdlib>
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include "ofApp.h"


string AquariumCreatureTypeToString(AquariumCreatureType t){
    switch(t){
        case AquariumCreatureType::BiggerFish:
            return "BiggerFish";
        case AquariumCreatureType::NPCreature:
            return "BaseFish";
              case AquariumCreatureType::PlaneFish:
            return "PlaneFish";
        case AquariumCreatureType::RainbowFish:
            return "RainbowFish";
        default:
            return "UknownFish";
    }
}


// PlayerCreature Implementation
PlayerCreature::PlayerCreature(float x, float y, int speed, std::shared_ptr<GameSprite> sprite)
: Creature(x, y, speed, 10.0f, 1, sprite) {}


void PlayerCreature::setDirection(float dx, float dy) {
    m_dx = dx;
    m_dy = dy;
    normalize();
}

void PlayerCreature::move() {
    m_x += m_dx * m_speed;
    m_y += m_dy * m_speed;
    this->bounce();

     //boundary + unstuck logic
 float halfW = (getCollisionRadius() > 0.0f) ? getCollisionRadius() : 10.0f;
    float halfH = halfW;
    float minX = halfW;
    float minY = halfH;
    float maxX = ofGetWidth() - halfW;
    float maxY = ofGetHeight() - halfH;

    if (m_x < minX) {
        m_x = minX;
        if (m_dx < 0) m_dx = fabs(m_dx);
        if (fabs(m_dx) < 0.01f) m_dx = 0.5f;
    }
    if (m_x > maxX) {
        m_x = maxX;
        if (m_dx > 0) m_dx = -fabs(m_dx);
        if (fabs(m_dx) < 0.01f) m_dx = -0.5f;
    }
    if (m_y < minY) {
        m_y = minY;
        if (m_dy < 0) m_dy = fabs(m_dy);
        if (fabs(m_dy) < 0.01f) m_dy = 0.5f;
    }
    if (m_y > maxY) {
        m_y = maxY;
        if (m_dy > 0) m_dy = -fabs(m_dy);
        if (fabs(m_dy) < 0.01f) m_dy = -0.5f;
    }

    normalize();
}

void PlayerCreature::reduceDamageDebounce() {
    if (m_damage_debounce > 0) {
        --m_damage_debounce;
    }
}

void PlayerCreature::update() {
    this->reduceDamageDebounce();
    this->move();
}


void PlayerCreature::draw() const {
    
    ofLogVerbose() << "PlayerCreature at (" << m_x << ", " << m_y << ") with speed " << m_speed << std::endl;
    if (this->m_damage_debounce > 0) {
        ofSetColor(ofColor::red); // Flash red if in damage debounce
    }
    if (m_sprite) {
        m_sprite->draw(m_x, m_y);
    }
    ofSetColor(ofColor::white); // Reset color

}

void PlayerCreature::changeSpeed(int speed) {
    m_speed = speed;
}

void PlayerCreature::loseLife(int debounce) {
    if (m_damage_debounce <= 0) {
        if (m_lives > 0) this->m_lives -= 1;
        m_damage_debounce = debounce; // Set debounce frames
        ofLogNotice() << "Player lost a life! Lives remaining: " << m_lives << std::endl;
    }
    // If in debounce period, do nothing
    if (m_damage_debounce > 0) {
        ofLogVerbose() << "Player is in damage debounce period. Frames left: " << m_damage_debounce << std::endl;
    }
}

// NPCreature Implementation
NPCreature::NPCreature(float x, float y, int speed, std::shared_ptr<GameSprite> sprite)
: Creature(x, y, speed, 30, 1, sprite) {
    m_dx = (rand() % 3 - 1); // -1, 0, or 1
    m_dy = (rand() % 3 - 1); // -1, 0, or 1
    normalize();

    m_creatureType = AquariumCreatureType::NPCreature;
     setRequiredPower(getValue());
}

void NPCreature::move() {
    // Simple AI movement logic (random direction)
    m_x += m_dx * m_speed;
    m_y += m_dy * m_speed;
    if(m_dx < 0 ){
        this->m_sprite->setFlipped(true);
    }else {
        this->m_sprite->setFlipped(false);
    }
    bounce();

    //boundary + unstuck logic
  float halfW = (getCollisionRadius() > 0.0f) ? getCollisionRadius() : 10.0f;
    float halfH = halfW;
    float minX = halfW;
    float minY = halfH;
    float maxX = ofGetWidth() - halfW;
    float maxY = ofGetHeight() - halfH;

    if (m_x < minX) {
        m_x = minX;
        if (m_dx < 0) m_dx = fabs(m_dx);
        if (fabs(m_dx) < 0.01f) m_dx = 0.25f;
    }
    if (m_x > maxX) {
        m_x = maxX;
        if (m_dx > 0) m_dx = -fabs(m_dx);
        if (fabs(m_dx) < 0.01f) m_dx = -0.25f;
    }
    if (m_y < minY) {
        m_y = minY;
        if (m_dy < 0) m_dy = fabs(m_dy);
        if (fabs(m_dy) < 0.01f) m_dy = 0.25f;
    }
     if (m_y > maxY) {
        m_y = maxY;
        if (m_dy > 0) m_dy = -fabs(m_dy);
        if (fabs(m_dy) < 0.01f) m_dy = -0.25f;
    }

    normalize();
}

void NPCreature::draw() const {
    ofLogVerbose() << "NPCreature at (" << m_x << ", " << m_y << ") with speed " << m_speed << std::endl;
    ofSetColor(ofColor::white);
    if (m_sprite) {
        m_sprite->draw(m_x, m_y);
    }
}


BiggerFish::BiggerFish(float x, float y, int speed, std::shared_ptr<GameSprite> sprite)
: NPCreature(x, y, speed, sprite) {
    m_dx = (rand() % 3 - 1);
    m_dy = (rand() % 3 - 1);
    normalize();

    setCollisionRadius(60); // Bigger fish have a larger collision radius
    m_value = 5; // Bigger fish have a higher value
    m_creatureType = AquariumCreatureType::BiggerFish;
}

void BiggerFish::move() {
    // Bigger fish might move slower or have different logic
    m_x += m_dx * (m_speed * 0.5); // Moves at half speed
    m_y += m_dy * (m_speed * 0.5);
    if(m_dx < 0 ){
        this->m_sprite->setFlipped(true);
    }else {
        this->m_sprite->setFlipped(false);
    }

    bounce();

     //boundary + unstuck logic
    float halfW = (getCollisionRadius() > 0.0f) ? getCollisionRadius() : 20.0f;
    float halfH = halfW;
    float minX = halfW;
    float minY = halfH;
    float maxX = ofGetWidth() - halfW;
    float maxY = ofGetHeight() - halfH;

    if (m_x < minX) {
        m_x = minX;
        if (m_dx < 0) m_dx = fabs(m_dx);
        if (fabs(m_dx) < 0.01f) m_dx = 0.2f;
    }
    if (m_x > maxX) {
        m_x = maxX;
        if (m_dx > 0) m_dx = -fabs(m_dx);
        if (fabs(m_dx) < 0.01f) m_dx = -0.2f;
    }
     if (m_y < minY) {
        m_y = minY;
        if (m_dy < 0) m_dy = fabs(m_dy);
        if (fabs(m_dy) < 0.01f) m_dy = 0.2f;
    }
    if (m_y > maxY) {
        m_y = maxY;
        if (m_dy > 0) m_dy = -fabs(m_dy);
        if (fabs(m_dy) < 0.01f) m_dy = -0.2f;
    }

    normalize();
}

void BiggerFish::draw() const {
    ofLogVerbose() << "BiggerFish at (" << m_x << ", " << m_y << ") with speed " << m_speed << std::endl;
    this->m_sprite->draw(this->m_x, this->m_y);
}

PlaneFish::PlaneFish(float x, float y, int speed, std::shared_ptr<GameSprite> sprite)
: NPCreature(x, y, speed, sprite) {
    m_dx = (rand() % 3 - 1);
    m_dy = (rand() % 3 - 1);
    normalize();
    setCollisionRadius(30);
    m_value = 3;
    setRequiredPower(1);
    m_creatureType = AquariumCreatureType::PlaneFish;
}

void PlaneFish::move() {
    // Dash movement - periodically speeds up
    m_dashTimer++;
    float currentSpeed = m_speed;
    
    if (m_dashTimer >= m_dashCooldown) {
        currentSpeed *= m_dashSpeed; // Dash!
        if (m_dashTimer >= m_dashCooldown + 30) { // 30 frames of dash
            m_dashTimer = 0;
        }
    }

    m_x += m_dx * currentSpeed;
    m_y += m_dy * currentSpeed;

    if(m_dx < 0) {
        m_sprite->setFlipped(true);
    } else {
        m_sprite->setFlipped(false);
    }
     bounce();
    
     float halfW = (getCollisionRadius() > 0.0f) ? getCollisionRadius() : 20.0f;
    float halfH = halfW;
    float minX = halfW;
    float minY = halfH;
    float maxX = ofGetWidth() - halfW;
    float maxY = ofGetHeight() - halfH;

    if (m_x < minX) {
        m_x = minX;
        if (m_dx < 0) m_dx = fabs(m_dx);
        if (fabs(m_dx) < 0.01f) m_dx = 0.2f;
    }
    if (m_x > maxX) {
        m_x = maxX;
        if (m_dx > 0) m_dx = -fabs(m_dx);
        if (fabs(m_dx) < 0.01f) m_dx = -0.2f;
    }
     if (m_y < minY) {
        m_y = minY;
        if (m_dy < 0) m_dy = fabs(m_dy);
        if (fabs(m_dy) < 0.01f) m_dy = 0.2f;
    }
    if (m_y > maxY) {
        m_y = maxY;
        if (m_dy > 0) m_dy = -fabs(m_dy);
        if (fabs(m_dy) < 0.01f) m_dy = -0.2f;
    }

    normalize();
}

void PlaneFish::draw() const {
    if (m_sprite) {
        // Flash white during dash
        if (m_dashTimer >= m_dashCooldown) {
            ofSetColor(255, 255, 255, 200);
        }
        m_sprite->draw(m_x, m_y);
        ofSetColor(255, 255, 255, 255);
    }
}

//Rainbow Fish

RainbowFish::RainbowFish(float x, float y, int speed, std::shared_ptr<GameSprite> sprite)
: NPCreature(x, y, speed, sprite) {
   
    do {
        m_dx = static_cast<float>((rand() % 201) - 100) / 100.0f; // [-1,1]
        m_dy = static_cast<float>((rand() % 201) - 100) / 100.0f; // [-1,1]
    } while (std::abs(m_dx) < 0.01f && std::abs(m_dy) < 0.01f);
    normalize();

    setCollisionRadius(38.0f);
    m_value = 10; 
    setRequiredPower(0);  
    m_creatureType = AquariumCreatureType::RainbowFish;

    // motion params
    m_phase = 0.0f;
    m_curveSpeed = 0.08f;    
    m_curveAmplitude = 2.5f;  
    m_turnTimer = 0;
    m_turnInterval = 120;    
}

void RainbowFish::move() {
   
    m_phase += m_curveSpeed;

   
    float vx = m_dx * m_speed;
    float vy = m_dy * m_speed;

   
    float sway = std::sin(m_phase) * m_curveAmplitude;
    float perpX = -m_dy * sway;
    float perpY =  m_dx * sway;

    m_x += vx + perpX;
    m_y += vy + perpY;

    // soft re-aim to a nearby heading every interval
    if (++m_turnTimer >= m_turnInterval) {
        m_turnTimer = 0;

  // rotate heading by a small random angle [-20°, 20°]
        float deg = static_cast<float>((rand() % 41) - 20);
        float rad = deg * static_cast<float>(M_PI / 180.0);
        float nx = m_dx * std::cos(rad) - m_dy * std::sin(rad);
        float ny = m_dx * std::sin(rad) + m_dy * std::cos(rad);
        m_dx = nx; m_dy = ny;
        normalize();
    }

    // flip sprite based on direction
    if (m_dx < 0) m_sprite->setFlipped(true);
    else          m_sprite->setFlipped(false);

  
    bounce();
   
   
    float halfW = (getCollisionRadius() > 0.0f) ? getCollisionRadius() : 20.0f;
    float halfH = halfW;
    float minX = halfW;
    float minY = halfH;
    float maxX = ofGetWidth() - halfW;
    float maxY = ofGetHeight() - halfH;

    if (m_x < minX) {
        m_x = minX;
        if (m_dx < 0) m_dx = fabs(m_dx);
        if (fabs(m_dx) < 0.01f) m_dx = 0.2f;
    }
    if (m_x > maxX) {
        m_x = maxX;
        if (m_dx > 0) m_dx = -fabs(m_dx);
        if (fabs(m_dx) < 0.01f) m_dx = -0.2f;
    }
     if (m_y < minY) {
        m_y = minY;
        if (m_dy < 0) m_dy = fabs(m_dy);
        if (fabs(m_dy) < 0.01f) m_dy = 0.2f;
    }
    if (m_y > maxY) {
        m_y = maxY;
        if (m_dy > 0) m_dy = -fabs(m_dy);
        if (fabs(m_dy) < 0.01f) m_dy = -0.2f;
    }

    normalize();
}

void RainbowFish::draw() const {
    if (!m_sprite) return;

    
    float phase = m_phase; 
    int hue = static_cast<int>(std::fmod(phase * 40.0f, 255.0f)); // 0..254
    ofColor tint = ofColor::fromHsb(hue, 200, 255, 255);

    ofPushStyle();
    ofSetColor(tint);
    m_sprite->draw(m_x, m_y);
    ofPopStyle();
}



// AquariumSpriteManager
AquariumSpriteManager::AquariumSpriteManager(){
    this->m_npc_fish = std::make_shared<GameSprite>("base-fish.png", 70,70);
    this->m_big_fish = std::make_shared<GameSprite>("bigger-fish.png", 120, 120);
    this->m_plane_fish = std::make_shared<GameSprite>("plane-fish.png", 60, 60); 
    this->m_rainbow_fish = std::make_shared<GameSprite>("rainbow-fish.png", 80, 80);
}



std::shared_ptr<GameSprite> AquariumSpriteManager::GetSprite(AquariumCreatureType t){
    switch(t){
        case AquariumCreatureType::BiggerFish:
            return std::make_shared<GameSprite>(*this->m_big_fish);
            
        case AquariumCreatureType::NPCreature:
            return std::make_shared<GameSprite>(*this->m_npc_fish);
        case AquariumCreatureType::PlaneFish:
            return std::make_shared<GameSprite>(*this->m_plane_fish);
         case AquariumCreatureType::RainbowFish:
            return std::make_shared<GameSprite>(*this->m_rainbow_fish);
        default:
            return nullptr;
    }
}


// Aquarium Implementation
Aquarium::Aquarium(int width, int height, std::shared_ptr<AquariumSpriteManager> spriteManager) : GameLevel(10), m_width(width), m_height(height) {
        m_sprite_manager =  spriteManager;
    }



void Aquarium::addCreature(std::shared_ptr<Creature> creature) {
    creature->setBounds(m_width - 20, m_height - 20);
    m_creatures.push_back(creature);
}

void Aquarium::addAquariumLevel(std::shared_ptr<AquariumLevel> level){
    if(level == nullptr){return;} // guard to not add noise
    this->m_aquariumlevels.push_back(level);
}

void Aquarium::update() {
    for (auto& creature : m_creatures) {
        creature->move();
    }
    this->Repopulate();
}

void Aquarium::draw() const {
    for (const auto& creature : m_creatures) {
        creature->draw();
    }
}


void Aquarium::removeCreature(std::shared_ptr<Creature> creature) {
    auto it = std::find(m_creatures.begin(), m_creatures.end(), creature);
    if (it != m_creatures.end()) {
        ofLogVerbose() << "removing creature " << endl;
        int selectLvl = this->currentLevel % this->m_aquariumlevels.size();
        auto npcCreature = std::static_pointer_cast<NPCreature>(creature);
        this->m_aquariumlevels.at(selectLvl)->ConsumePopulation(npcCreature->GetType(), npcCreature->getValue());
        m_creatures.erase(it);
    }
}

void Aquarium::clearCreatures() {
    m_creatures.clear();
}

std::shared_ptr<Creature> Aquarium::getCreatureAt(int index) {
    if (index < 0 || size_t(index) >= m_creatures.size()) {
        return nullptr;
    }
    return m_creatures[index];
}



void Aquarium::SpawnCreature(AquariumCreatureType type) {
    int x = rand() % this->getWidth();
    int y = rand() % this->getHeight();
    int speed = 1 + rand() % 25; // Speed between 1 and 25

    switch (type) {
        case AquariumCreatureType::NPCreature:
            this->addCreature(std::make_shared<NPCreature>(x, y, speed, this->m_sprite_manager->GetSprite(AquariumCreatureType::NPCreature)));
            break;
        case AquariumCreatureType::BiggerFish:
            this->addCreature(std::make_shared<BiggerFish>(x, y, speed, this->m_sprite_manager->GetSprite(AquariumCreatureType::BiggerFish)));
            break;
             case AquariumCreatureType::PlaneFish:
            this->addCreature(std::make_shared<PlaneFish>(x, y, speed, this->m_sprite_manager->GetSprite(AquariumCreatureType::PlaneFish)));
            break;
              case AquariumCreatureType::RainbowFish:
            this->addCreature(std::make_shared<RainbowFish>(x, y, speed, this->m_sprite_manager->GetSprite(AquariumCreatureType::RainbowFish)));
            break;
            
        default:
            ofLogError() << "Unknown creature type to spawn!";
            break;
    }

}



void Aquarium::Repopulate() {
    // log entry consistently using stream style
    ofLogVerbose() << "entering phase repopulation" << std::endl;

    // make sure we have levels to avoid division by zero
    if (this->m_aquariumlevels.empty()) {
        ofLogVerbose() << "no aquarium levels available to repopulate" << std::endl;
        return;
    }

    // compute the selected level index in a circular manner
    int selectedLevelIdx = this->currentLevel % static_cast<int>(this->m_aquariumlevels.size());
    ofLogVerbose() << "the current index: " << selectedLevelIdx << std::endl;

    std::shared_ptr<AquariumLevel> level = this->m_aquariumlevels.at(selectedLevelIdx);

    if (level->isCompleted()) {
        level->levelReset();
        this->currentLevel += 1;
        selectedLevelIdx = this->currentLevel % static_cast<int>(this->m_aquariumlevels.size());
        ofLogNotice() << "new level reached : " << selectedLevelIdx << std::endl;
        level = this->m_aquariumlevels.at(selectedLevelIdx);
        this->clearCreatures();
    }

    // determine which creatures to respawn for this level
    std::vector<AquariumCreatureType> toRespawn = level->Repopulate();
    ofLogVerbose() << "amount to repopulate : " << toRespawn.size() << std::endl;
    if (toRespawn.empty()) {
        return; // nothing to do
    }

    for (AquariumCreatureType newCreatureType : toRespawn) {
        this->SpawnCreature(newCreatureType);
    }
}


// Aquarium collision detection
std::shared_ptr<GameEvent> DetectAquariumCollisions(std::shared_ptr<Aquarium> aquarium, std::shared_ptr<PlayerCreature> player) {
    if (!aquarium || !player) return nullptr;
    
    for (int i = 0; i < aquarium->getCreatureCount(); ++i) {
        std::shared_ptr<Creature> npc = aquarium->getCreatureAt(i);
        if (npc && checkCollision(player, npc)) {
            return std::make_shared<GameEvent>(GameEventType::COLLISION, player, npc);
        }
    }
    return nullptr;
};

// Implementation of the AquariumScene


void AquariumGameScene::Draw() {
    this->m_player->draw();
    this->m_aquarium->draw();
    this->paintAquariumHUD();

}




void AquariumGameScene::Update(){
  
   if (!m_musicStarted) {
        if (!m_ambientSound.isLoaded()) {
            std::vector<std::string> candidates = {
                "music/bgm_o.mp3",
                "music/bgm_o.ogg",
                "music/bgm_o.wav",
                "music/bgm.mp3",
                "music/bgm.ogg",
                "music/bgm.wav"
            };
            for (auto &p : candidates) {
                ofFile f(ofToDataPath(p, true));
                if (!f.exists()) continue;
                if (m_ambientSound.load(p)) {
                    m_ambientSound.setMultiPlay(false);
                    m_ambientSound.setLoop(true);
                    m_ambientSound.setVolume(0.0f);
                    break;
                }
            }
 }

        if (m_ambientSound.isLoaded()) {
            m_ambientSound.play();
            m_musicStarted = true;
            m_currentVolume = 0.0f;
            m_ambientSound.setVolume(0.0f);
        }
    } else {
        if (m_currentVolume < m_targetVolume && m_ambientSound.isLoaded()) {
            m_currentVolume = std::min(m_targetVolume, m_currentVolume + m_fadeSpeed);
            m_ambientSound.setVolume(m_currentVolume);
        }
        if (m_ambientSound.isLoaded() && !m_ambientSound.isPlaying()) {
            m_ambientSound.play();
            m_ambientSound.setVolume(m_currentVolume);
        }
    }

   
    std::shared_ptr<GameEvent> event;

    this->m_player->update();
    //itemblock
    float now = ofGetElapsedTimef();
    float dt  = ofGetLastFrameTime();

    m_aquarium->updateItemBox(now, dt);
    m_aquarium->checkItemBoxCollision(now, m_player);
    m_player->updatePowerUps(now);
/////////////////////////////////////////////////////////////////
    if (this->updateControl.tick()) {
        event = DetectAquariumCollisions(this->m_aquarium, this->m_player);
        if (event != nullptr && event->isCollisionEvent()) {
            ofLogVerbose() << "Collision detected between player and NPC!" << std::endl;
            if(event->creatureB != nullptr){
                event->print();
                //hitmark
                auto baseApp = ofGetAppPtr();
                    auto* app = dynamic_cast<ofApp*>(baseApp);
                    const float SPRITE_TO_COLLISION = 3.5f;
                    float creatureDia = event->creatureB->getCollisionRadius() * 2.0f;
                    float hitSize = creatureDia * SPRITE_TO_COLLISION;
                    if (app) {
                    app->hit.trigger(
                    event->creatureB->getX() + event->creatureB->getCollisionRadius(),
                    event->creatureB->getY() + event->creatureB->getCollisionRadius(),
                    2, hitSize);
                    if (app->hitmarkSound.isLoaded()) app->hitmarkSound.play();
                    }
                if(this->m_player->getPower() < event->creatureB->getRequiredPower()){
                    ofLogNotice() << "Player is too weak to eat the creature!" << std::endl;
                    this->m_player->loseLife(3*60);
                    if(this->m_player->getLives() <= 0){
                        this->m_lastEvent = std::make_shared<GameEvent>(GameEventType::GAME_OVER, this->m_player, nullptr);
                        return;
                    }
                }
                else{
                    this->m_aquarium->removeCreature(event->creatureB);
                    this->m_player->addToScore(1, event->creatureB->getValue());
                    if (this->m_player->getScore() % 25 == 0){
                        this->m_player->increasePower(1);
                        ofLogNotice() << "Player power increased to " << this->m_player->getPower() << "!" << std::endl;
                    }
                }
            } else {
                ofLogError() << "Error: creatureB is null in collision event." << std::endl;
            }
        }
        this->m_aquarium->update();
    }
}






void AquariumGameScene::paintAquariumHUD(){
    float panelWidth = ofGetWindowWidth() - 150;
    ofDrawBitmapString("Score: " + std::to_string(this->m_player->getScore()), panelWidth, 20);
    ofDrawBitmapString("Power: " + std::to_string(this->m_player->getPower()), panelWidth, 30);
    ofDrawBitmapString("Lives: " + std::to_string(this->m_player->getLives()), panelWidth, 40);
    for (int i = 0; i < this->m_player->getLives(); ++i) {
        ofSetColor(ofColor::red);
        ofDrawCircle(panelWidth + i * 20, 50, 5);
    }
    ofSetColor(ofColor::white); // Reset color to white for other drawings
}

void AquariumLevel::populationReset(){
    for(auto node: this->m_levelPopulation){
        node->currentPopulation = 0; // need to reset the population to ensure they are made a new in the next level
    }
}

void AquariumLevel::ConsumePopulation(AquariumCreatureType creatureType, int power){
    for(std::shared_ptr<AquariumLevelPopulationNode> node: this->m_levelPopulation){
        ofLogVerbose() << "consuming from this level creatures" << endl;
        if(node->creatureType == creatureType){
            ofLogVerbose() << "-cosuming from type: " << AquariumCreatureTypeToString(node->creatureType) <<" , currPop: " << node->currentPopulation << endl;
            if(node->currentPopulation == 0){
                return;
            } 
            node->currentPopulation -= 1;
            ofLogVerbose() << "+cosuming from type: " << AquariumCreatureTypeToString(node->creatureType) <<" , currPop: " << node->currentPopulation << endl;
            this->m_level_score += power;
            return;
        }
    }
}

bool AquariumLevel::isCompleted(){
    return this->m_level_score >= this->m_targetScore;
}


           




void Aquarium::scheduleNextItemBox(float now) {
    nextSpawnAt = now + ofRandom(8.0f, 15.0f);
}

void Aquarium::spawnItemBox() {
    float margin = 80.0f;
    float x = ofRandom(margin, ofGetWidth()  - margin);
    float y = ofRandom(margin, ofGetHeight() - margin);
    itemBox.spawn(x, y);
    itemBoxPresent = true;
}

void Aquarium::updateItemBox(float now, float dt) {
    if (!itemBoxPresent && now >= nextSpawnAt) {
        spawnItemBox();
    }
    itemBox.update(dt);

    // expira doble puntos
    if (scoreMultiplier > 1.0f && now >= scoreMultUntil) {
        scoreMultiplier = 1.0f;
    }
}

void Aquarium::drawItemBox() {
    if (itemBoxPresent) itemBox.draw();
}

void Aquarium::checkItemBoxCollision(float now, std::shared_ptr<PlayerCreature> player) {
    if (!itemBoxPresent || !itemBox.active || !player) return;

    float dist = ofDist(player->getX(), player->getY(), itemBox.pos.x, itemBox.pos.y);
    if (dist <= player->getCollisionRadius() + itemBox.radius) {
        itemBox.playSound();

        PowerUpEffect effect = RandomPowerUpEffect();

        const float D_SPEED  = 5.0f;
        const float D_SIZE   = 5.0f;
        const float D_DBLPTS = 8.0f;

        switch (effect) {
            case PowerUpEffect::DoublePoints:
                scoreMultiplier = 2.0f;
                scoreMultUntil  = now + D_DBLPTS;
                break;
            case PowerUpEffect::Speed:
                player->applyPowerUp(PowerUpEffect::Speed, now, D_SPEED);
                break;
            case PowerUpEffect::Dash:
                player->applyPowerUp(PowerUpEffect::Dash, now, 0.0f);
                break;
            case PowerUpEffect::Size:
                player->applyPowerUp(PowerUpEffect::Size, now, D_SIZE);
                break;
        }

        itemBox.active = false;
        itemBoxPresent = false;
        scheduleNextItemBox(now);
    }
}
bool Aquarium::isCompleted() { return false; }
void PlayerCreature::applyPowerUp(PowerUpEffect effect, float now, float durationSec) {
    switch (effect) {
        case PowerUpEffect::Speed:
            speedBoostOn = true;
            speedBoostUntil = now + durationSec;
            m_speed = baseSpeed * 2.0f;
            break;

        case PowerUpEffect::Size:
            sizeBoostOn = true;
            sizeBoostUntil = now + durationSec;
            setCollisionRadius(baseRadius * 2.0f); 
            break;

        case PowerUpEffect::Dash:
            dashUnlocked = true;
            tryDash(now); 
            break;

        case PowerUpEffect::DoublePoints:
            break;
    }
}

void PlayerCreature::updatePowerUps(float now) {
    if (speedBoostOn && now >= speedBoostUntil) {
        speedBoostOn = false;
        m_speed = baseSpeed;
    }
    if (sizeBoostOn && now >= sizeBoostUntil) {
        sizeBoostOn = false;
        setCollisionRadius(baseRadius);
    }
    if (dashActive && now >= dashActiveUntil) {
        dashActive = false;
    }
}

void PlayerCreature::tryDash(float now) {
    if (!dashUnlocked) return;
    if (now < dashCooldownUntil || dashActive) return;

    dashActive = true;
    dashActiveUntil   = now + dashDuration;
    dashCooldownUntil = now + dashCooldown;
    baseSpeed = m_speed;
    baseRadius = getCollisionRadius();
}
