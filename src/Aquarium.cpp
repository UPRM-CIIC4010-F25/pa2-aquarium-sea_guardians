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
: Creature(x, y, speed, 10.0f, 1, sprite) {
    initPowerUpBases();  // Add this line
}


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
    float now = ofGetElapsedTimef();
    this->reduceDamageDebounce();
    this->updatePowerUps(now);  // Add this line
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
    setRequiredPower(m_value);
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

    // new sprites (place files in data/)
    this->m_jellyfish     = std::make_shared<GameSprite>("jellyfish.png", 64, 64);
    this->m_shark         = std::make_shared<GameSprite>("shark.png", 140, 140);
    this->m_cleaner_fish  = std::make_shared<GameSprite>("cleaner-fish.png", 50, 50);
    this->m_coral         = std::make_shared<GameSprite>("coral.png", 90, 90);
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
        case AquariumCreatureType::Jellyfish:
            return std::make_shared<GameSprite>(*this->m_jellyfish);
        case AquariumCreatureType::Shark:
            return std::make_shared<GameSprite>(*this->m_shark);
        case AquariumCreatureType::CleanerFish:
            return std::make_shared<GameSprite>(*this->m_cleaner_fish);
        case AquariumCreatureType::Coral:
            return std::make_shared<GameSprite>(*this->m_coral);
        default:
            return nullptr;
    }
}

// New: simple Jellyfish (slow drifting; medium collision radius)
class Jellyfish : public NPCreature {
public:
    Jellyfish(float x, float y, int speed, std::shared_ptr<GameSprite> sprite)
    : NPCreature(x, y, speed, sprite) {
        m_dx = 0.0f; m_dy = 0.0f;
        setCollisionRadius(36.0f);
        m_value = 0;             // not worth points, hazard instead
        setRequiredPower(99);    // effectively cannot be eaten normally
        m_creatureType = AquariumCreatureType::Jellyfish;
        m_sway = 0.0f;
    }
    void move() override {
        // slow vertical/horizontal sway
        m_sway += 0.02f;
        m_x += std::sin(m_sway) * 0.4f;
        m_y += std::cos(m_sway * 0.7f) * 0.3f;
        if (m_dx < 0) this->m_sprite->setFlipped(true);
        else this->m_sprite->setFlipped(false);
        bounce();
    }
private:
    float m_sway;
};

// New: Shark (fast predator, larger collision radius, higher value)
class Shark : public NPCreature {
public:
    Shark(float x, float y, int speed, std::shared_ptr<GameSprite> sprite)
    : NPCreature(x, y, speed, sprite) {
        m_dx = (rand()%3 -1); m_dy = (rand()%3 -1);
        normalize();
        setCollisionRadius(80.0f);
        m_value = 15;
        setRequiredPower(2);
        m_creatureType = AquariumCreatureType::Shark;
    }
    void move() override {
        // faster movement, occasional burst
        int burst = rand() % 200;
        float factor = (burst == 0) ? 2.5f : 1.0f;
        m_x += m_dx * (m_speed * factor);
        m_y += m_dy * (m_speed * factor);
        if (m_dx < 0) this->m_sprite->setFlipped(true);
        else this->m_sprite->setFlipped(false);
        bounce();
    }
};

// New: CleanerFish (friendly, small value, helps player progress)
class CleanerFish : public NPCreature {
public:
    CleanerFish(float x, float y, int speed, std::shared_ptr<GameSprite> sprite)
    : NPCreature(x, y, speed, sprite) {
        m_dx = (rand()%3 -1); m_dy = (rand()%3 -1);
        normalize();
        setCollisionRadius(18.0f);
        m_value = 1;
        setRequiredPower(0);
        m_creatureType = AquariumCreatureType::CleanerFish;
    }
    void move() override {
        m_x += m_dx * (m_speed * 0.8f);
        m_y += m_dy * (m_speed * 0.8f);
        if (m_dx < 0) this->m_sprite->setFlipped(true);
        else this->m_sprite->setFlipped(false);
        bounce();
    }
};

// New: Coral (stationary obstacle)
class Coral : public NPCreature {
public:
    Coral(float x, float y, std::shared_ptr<GameSprite> sprite)
    : NPCreature(x, y, 0, sprite) {
        setCollisionRadius(50.0f);
        m_value = 0;
        setRequiredPower(99); // can't eat
        m_creatureType = AquariumCreatureType::Coral;
    }
    void move() override {
        // stationary
    }
    void draw() const override {
        if (m_sprite) m_sprite->draw(m_x, m_y);
    }
};



// Aquarium Implementation
Aquarium::Aquarium(int width, int height, std::shared_ptr<AquariumSpriteManager> spriteManager)
    : m_width(width), m_height(height) {
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


    //=============================
    float now = ofGetElapsedTimef();
    float dt  = ofGetLastFrameTime();
    updateItemBlock(now, dt);
    //=============================

    if (scoreMultiplier > 1.0 && now >= scoreMultUntil) {
        scoreMultiplier = 1.0;
    }

}

void Aquarium::draw() const {
    for (const auto& creature : m_creatures) {
        creature->draw();
    }

    //=============================
    const_cast<Aquarium*>(this)->drawItemBlock();
    //=============================

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

// Add spawn cases in SpawnCreature
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
        case AquariumCreatureType::Jellyfish:
            this->addCreature(std::make_shared<Jellyfish>(x, y, std::max(1, speed/4), this->m_sprite_manager->GetSprite(AquariumCreatureType::Jellyfish)));
            break;
        case AquariumCreatureType::Shark:
            this->addCreature(std::make_shared<Shark>(x, y, std::max(4, speed), this->m_sprite_manager->GetSprite(AquariumCreatureType::Shark)));
            break;
        case AquariumCreatureType::CleanerFish:
            this->addCreature(std::make_shared<CleanerFish>(x, y, std::max(1, speed/2), this->m_sprite_manager->GetSprite(AquariumCreatureType::CleanerFish)));
            break;
        case AquariumCreatureType::Coral:
            // coral is stationary; use zero speed constructor variant
            this->addCreature(std::make_shared<Coral>(x, y, this->m_sprite_manager->GetSprite(AquariumCreatureType::Coral)));
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

    //=========================================
    float now = ofGetElapsedTimef();
    float dt  = ofGetLastFrameTime();
    m_aquarium->updateItemBlock(now, dt);
    m_aquarium->checkItemBlockCollision(now, m_player);
    //=========================================
    

    
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



                    int mult = (int)std::round(m_aquarium->getScoreMultiplier());
                    if (mult < 1) mult = 1;
                    this->m_player->addToScore(mult, event->creatureB->getValue());
                    this->m_aquarium->onScoreUpdated(this->m_player->getScore(), now);



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

    float now = ofGetElapsedTimef();
    std::string ptxt = m_player->debugPowerUpText(now);

    // line for player’s own buffs
    ofDrawBitmapString("PowerUp: " + ptxt, ofGetWindowWidth() - 150, 70);

    // line for Double Points (Aquarium-level buff)
    float mult = m_aquarium->getScoreMultiplier();
    if (mult > 1.0f) {
        int sec = (int)std::ceil(m_aquarium->getScoreMultRemaining(now));
        ofDrawBitmapString("Points x" + std::to_string((int)mult) + " (" + std::to_string(sec) + "s)",
                        ofGetWindowWidth() - 150, 80);
    } else {
        ofDrawBitmapString("Points x1", ofGetWindowWidth() - 150, 80);
    }

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

void PlayerCreature::applyPowerUp(PowerUpEffect effect, float now, float durationSec) {
    float until = now + durationSec;
    
    switch(effect) {
        case PowerUpEffect::DoublePoints:
            // Handle double points logic if needed
            break;
            
        case PowerUpEffect::Speed:
            if (!speedBoostOn) {
                baseSpeed = m_speed;
                m_speed *= 2;
                speedBoostOn = true;
            }
            speedBoostUntil = until;
            break;
            
        case PowerUpEffect::Dash:
            dashUnlocked = true;
            dashCharges = 1;
            dashCooldownUntil = now;
            break;
    }
}

void PlayerCreature::updatePowerUps(float now) {
    // Handle speed boost
    if (speedBoostOn && now >= speedBoostUntil) {
        m_speed = baseSpeed;
        speedBoostOn = false;
    }
    
    // Handle size boost
    if (sizeBoostOn && now >= sizeBoostUntil) {
        setCollisionRadius(baseRadius);
        sizeBoostOn = false;
    }
    
    // Handle dash cooldown
    if (dashActive && now >= dashActiveUntil) {
        dashActive = false;
        m_speed = baseSpeed;
        if (dashCharges <= 0) dashUnlocked = false;
    }
}

void PlayerCreature::tryDash(float now) {
    if (!dashUnlocked || dashActive || now < dashCooldownUntil || dashCharges <= 0) {
        return;
    }
    
    dashActive = true;
    dashActiveUntil = now + dashDuration;
    dashCooldownUntil = now + dashCooldown;
    
    baseSpeed = m_speed;
    m_speed *= dashSpeedFactor;

    dashCharges -= 1;
}

std::string PlayerCreature::debugPowerUpText(float now) const {
    std::vector<std::string> parts;

    if (speedBoostOn && now < speedBoostUntil) {
        int sec = (int)std::ceil(speedBoostUntil - now);
        parts.push_back("Speed x2 (" + std::to_string(sec) + "s)");
    }
    if (sizeBoostOn && now < sizeBoostUntil) {
        int sec = (int)std::ceil(sizeBoostUntil - now);
        parts.push_back("Size x2 (" + std::to_string(sec) + "s)");
    }
    if (dashUnlocked) {
        if (dashActive) {
            int sec = (int)std::ceil(dashActiveUntil - now);
            parts.push_back("Dash ACTIVE (" + std::to_string(sec) + "s)");
        } else if (now < dashCooldownUntil) {
            int sec = (int)std::ceil(dashCooldownUntil - now);
            parts.push_back("Dash CD (" + std::to_string(sec) + "s)");
        } else if (dashCharges > 0) {
            parts.push_back("Dash READY");
        }
    }

    if (parts.empty()) return "None";
    std::string out = parts[0];
    for (size_t i = 1; i < parts.size(); ++i) out += " | " + parts[i];
    return out;
}


void Aquarium::scheduleNextItemBlock(float now) {
    float delay = 8.0f + static_cast<float>(rand() % 7);
    nextSpawnAt = now + delay;
}
void Aquarium::spawnItemBlock() {
    if (!m_width || !m_height) return;
    float margin = 60.0f;
    float x = margin + (float)rand()/RAND_MAX * (m_width  - 2*margin);
    float y = margin + (float)rand()/RAND_MAX * (m_height - 2*margin);
    itemBlock.spawn(x, y);
    itemBlock.playSound();
    itemBlockPresent = true;
}
void Aquarium::updateItemBlock(float now, float dt) {
    static bool loaded = false;
    if (!loaded) { itemBlock.load(); loaded = true; scheduleNextItemBlock(now); }
    if (!itemBlockPresent && now >= nextSpawnAt) spawnItemBlock();
    if (itemBlockPresent) itemBlock.update(dt);
}
void Aquarium::drawItemBlock() {
    if (itemBlockPresent) itemBlock.draw();
}
void Aquarium::checkItemBlockCollision(float now, std::shared_ptr<PlayerCreature> player) {
    if (!itemBlockPresent || !player) return;
    float dx = (player->getX() - (itemBlock.pos.x - 30));
    float dy = (player->getY() - (itemBlock.pos.y - 35));
    float r  = player->getCollisionRadius() + itemBlock.radius;
    if (dx*dx + dy*dy <= r*r) {
        PowerUpEffect eff = RandomPowerUpEffect();


        if (eff == PowerUpEffect::DoublePoints) {
        scoreMultiplier = 2.0f;
        scoreMultUntil  = now + 10.0f; 
        } else {
            player->applyPowerUp(eff, now, 10.0f);
        }

        itemBlockPresent = false;
        scheduleNextItemBlock(now);
    }
}
void Aquarium::onScoreUpdated(int score, float now) {
    if (score > 0 && score % 15 == 0 && !itemBlockPresent)
        nextSpawnAt = std::min(nextSpawnAt, now + 3.0f);
}

float Aquarium::getScoreMultiplier() const {
    return (ofGetElapsedTimef() < scoreMultUntil) ? scoreMultiplier : 1.0f;
}



float Aquarium::getScoreMultRemaining(float now) const {
    return std::max(0.0f, scoreMultUntil - now);
}
