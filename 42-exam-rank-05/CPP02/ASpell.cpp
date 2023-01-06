#include "ASpell.hpp"

void ASpell::launch(ATarget const &other) { other.getHitBySpell(*this); }
