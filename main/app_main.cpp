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

//===============================================================================
//  Combo RUN + MENU → Retour au Loader OTA (TAKATRIS / Gamebuino AKA)
//-------------------------------------------------------------------------------
//  Rôle :
//    - Détecter RUN + MENU maintenus 500 ms.
//    - Basculer sur la partition OTA_1 (loader AKA).
//    - Redémarrer proprement l’ESP32‑S3.
//
//  Dépendances :
//    - button_held()  → input.cpp
//    - g_core.get_millis() → moteur AKA
//===============================================================================

#include <esp_ota_ops.h>
#include <esp_partition.h>

static uint32_t combo_start = 0;

void checkReturnToLoader()
{
    // Lecture des boutons via le moteur AKA
    bool home_held = button_held(BTN_RUN);   // RUN = bouton HOME
    bool menu_held = button_held(BTN_MENU);  // MENU

    if (home_held && menu_held)
    {
        // Début du combo
        if (combo_start == 0)
            combo_start = g_core.get_millis();

        // Maintenu 500 ms → retour loader
        else if (g_core.get_millis() - combo_start >= 500)
        {
            combo_start = 0;

            const esp_partition_t* loader = esp_partition_find_first(
                ESP_PARTITION_TYPE_APP,
                ESP_PARTITION_SUBTYPE_APP_OTA_1,
                nullptr
            );

            if (loader)
            {
                esp_ota_set_boot_partition(loader);
                esp_restart();
            }
        }
    }
    else
    {
        // Combo relâché → reset
        combo_start = 0;
    }
}
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

    // -------------------------------------------------------------------------
    //  Boucle idle
    //  - Polling input (obligatoire pour BTN_RUN / BTN_MENU)
    //  - Vérification du combo RUN+MENU
    //  - Petit délai pour éviter de saturer le CPU
    // -------------------------------------------------------------------------
	while (true)
	{
		checkReturnToLoader();   // Vérifie RUN + MENU
		vTaskDelay(pdMS_TO_TICKS(10));
	}

}