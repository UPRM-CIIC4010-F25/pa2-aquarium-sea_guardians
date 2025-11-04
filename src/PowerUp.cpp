
#include "PowerUp.h"
#include <cstdlib>

PowerUpEffect RandomPowerUpEffect() {
    int r = std::rand() % 4;
    switch (r) {
        case 0: return PowerUpEffect::DoublePoints;
        case 1: return PowerUpEffect::Speed;
        case 2: return PowerUpEffect::Dash;
        default: return PowerUpEffect::Size;
    }
}
