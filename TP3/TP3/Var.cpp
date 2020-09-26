#include "Var.h"

int CVar::g_FenetreID      = 0;
int CVar::g_LargeurFenetre = 512;
int CVar::g_HauteurFenetre = 256;
// int CVar::g_LargeurFenetre  = 4096;
// int CVar::g_HauteurFenetre  = 2048;
int CVar::g_LargeurViewport = 0;
int CVar::g_HauteurViewport = 0;

Scene::CScene* CVar::g_GestionnaireDeScene = nullptr;
