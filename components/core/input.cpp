/*
===============================================================================
  input.cpp — Système d’entrée (AKA Edition — Version Fiable)
-------------------------------------------------------------------------------
  Rôle :
    - Polling haute fréquence (task_game)
    - Buffer des pressions pour éviter les press ratés
    - pressed() fiable
    - down() fiable
===============================================================================
*/

#include "input.h"
#include "gb_core.h"

extern gb_core g_core;

// État des boutons
static uint16_t prevState = 0;
static uint16_t currState = 0;

// Buffer des pressions (événements)
static uint16_t pressedBuffer = 0;

// -----------------------------------------------------------------------------
// Initialisation
// -----------------------------------------------------------------------------
void input_init()
{
    prevState = 0;
    currState = 0;
    pressedBuffer = 0;
}

// -----------------------------------------------------------------------------
// Polling (appelé à haute fréquence dans task_game)
// -----------------------------------------------------------------------------
void input_poll()
{
    prevState = currState;
    currState = g_core.buttons.state();

    // Détection des fronts montants
    uint16_t changed = prevState ^ currState;
    uint16_t newlyPressed = changed & currState;

    // On stocke les pressions dans un buffer
    pressedBuffer |= newlyPressed;
}

// -----------------------------------------------------------------------------
// Bouton pressé (front montant, fiable)
// -----------------------------------------------------------------------------
bool button_pressed(int btn)
{
    if (pressedBuffer & btn)
    {
        pressedBuffer &= ~btn; // consomme l’événement
        return true;
    }
    return false;
}

bool button_held(int btn)
{
    return button_down(btn);
}
// -----------------------------------------------------------------------------
// Bouton maintenu
// -----------------------------------------------------------------------------
bool button_down(int btn)
{
    return (currState & btn) != 0;
}

// -----------------------------------------------------------------------------
// Long press (optionnel)
// -----------------------------------------------------------------------------
static int longPressCounter[16] = {};

bool isLongPress(int btn)
{
    int idx = __builtin_ctz(btn);

    if (currState & btn)
    {
        if (++longPressCounter[idx] > 60)
        {
            longPressCounter[idx] = 0;
            return true;
        }
    }
    else
    {
        longPressCounter[idx] = 0;
    }

    return false;
}
