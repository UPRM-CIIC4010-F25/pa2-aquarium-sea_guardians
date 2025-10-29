#include "PowerUp.h"

PowerUpEffect RandomPowerUpEffect() {
    int r = static_cast<int>(ofRandom(0, 4));
    switch (r) {
        case 0: return PowerUpEffect::DoublePoints;
        case 1: return PowerUpEffect::Speed;
        case 2: return PowerUpEffect::Dash;
        default: return PowerUpEffect::Size;
    }
}
