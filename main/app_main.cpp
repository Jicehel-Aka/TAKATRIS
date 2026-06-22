//===============================================================================
// app_main.cpp — Point d’entrée du moteur TAKATRIs pour la Gamebuino AKA
//-------------------------------------------------------------------------------
//  Rôle :
//    - Initialiser l’ensemble du hardware (LCD, audio, input, SD, expander).
//    - Initialiser les modules génériques (graphics, audio, persist).
//    - Lancer les tâches FreeRTOS (jeu, input, audio).
//    - Fournir une boucle idle propre.
//
//  Auteur : Jean-Charles LEBEAU (Jicehel)
//    Création : 02/2026
//===============================================================================

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "input.h"
#include "audio.h"
#include "language.h"
#include "config.h"

// -----------------------------------------------------------------------------
//  Hardware AKA (lib Gamebuino AKA)
// -----------------------------------------------------------------------------
#include "gb_core.h"
#include "gb_graphics.h"

// -----------------------------------------------------------------------------
//  Core du moteur générique
// -----------------------------------------------------------------------------
#include "graphics.h"
#include "persist.h"
#include "driver/ledc.h" 

// -----------------------------------------------------------------------------
//  Logique de jeu Tetris
// -----------------------------------------------------------------------------
#include "game.h"        

// -----------------------------------------------------------------------------
//  Tâches FreeRTOS
// -----------------------------------------------------------------------------
#include "task_game.h"

gb_core g_core;


// ============================================================================
//  Initialisation hardware AKA
// ============================================================================
void hardware_init()
{
    printf("\n=== HARDWARE INIT (AKA Edition — Tetris) ===\n");

    printf("[HW] g_core.init()...\n");
    g_core.init();

    printf("[HW] gfx_init()...\n");
    gfx_init();

    printf("[HW] input_init()...\n");
    input_init();

    printf("[HW] audio_init()...\n");
    audio_init();

    printf("[HW] persist_init()...\n");
    persist_init();
	
	g_rotate_screen = false;   // mode paysage par défaut

    printf("=== HARDWARE INIT DONE ===\n\n");
}

// ============================================================================
//  POINT D’ENTRÉE AKA
// ============================================================================
extern "C" void app_main(void)
{
    printf("\n=============================================\n");
    printf("  Tetris — AKA Edition\n");
    printf("  (c) Jean-Charles — Février 2026\n");
    printf("=============================================\n\n");

    hardware_init();
	
	// Charger la langue depuis la NVS 
	language_load_from_nvs();

    // -------------------------------------------------------------------------
    //  Création des tâches FreeRTOS
    // -------------------------------------------------------------------------


    xTaskCreatePinnedToCore(
        task_game,
        "GameTask",
        8192,
        nullptr,
        6,
        nullptr,
        1
    );

    printf("[Tetris] Tâches lancées. Entrée en idle loop.\n");

    while (true)
        vTaskDelay(pdMS_TO_TICKS(1000));
}
