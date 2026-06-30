/*
===============================================================================
  input.h — Gestion des entrées (moteur AKA)
-------------------------------------------------------------------------------
  Rôle :
    - Fournir une API générique pour lire les boutons.
    - Encapsuler l’implémentation Gamebuino.
    - Offrir des helpers : pressions, maintien, pressions longues.
    - Ne contient AUCUNE logique gameplay (réservée à main/).
===============================================================================
*/

#pragma once
#include <cstdint>
#include "gamebuino.h"   // Fournit gb_buttons et GB_KEY_*

// -----------------------------------------------------------------------------
// Boutons génériques du moteur AKA
// -----------------------------------------------------------------------------
enum {
    BTN_A     = GB_KEY_A,
    BTN_B     = GB_KEY_B,
    BTN_C     = GB_KEY_C,
    BTN_D     = GB_KEY_D,	
    BTN_LEFT  = GB_KEY_LEFT,
    BTN_RIGHT = GB_KEY_RIGHT,
    BTN_UP    = GB_KEY_UP,
    BTN_DOWN  = GB_KEY_DOWN,
	BTN_RUN   = GB_KEY_RUN,
    BTN_MENU  = GB_KEY_MENU,
    BTN_L1    = GB_KEY_L1,
    BTN_R1    = GB_KEY_R1
};

// -----------------------------------------------------------------------------
// API d’entrée
// -----------------------------------------------------------------------------
void input_init();       // Réinitialise l’état interne
void input_poll();       // Met à jour l’état des boutons (à appeler chaque frame)

bool button_pressed(int btn);   // Front montant
bool button_down(int btn);      // Bouton maintenu
bool button_held(int btn);      // Alias propre pour bouton maintenu
bool input_ready();             // Anti-repeat simple
bool isLongPress(int btn);      // Pression longue (~1s)

