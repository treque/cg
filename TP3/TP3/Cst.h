#ifndef CST_H
#define CST_H

#include "Singleton.h"

///////////////////////////////////////////////////////////////////////////////
///  CCst
///  Cette classe conserve toutes les constantes globales du programme
///
///  Base class Singleton
///
///  @author Olivier Dionne
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
class CCst : public Singleton<CCst>
{
    SINGLETON_DECLARATION_CLASSE(CCst)

public:
    /// La position en x de la fenêtre GLUT
    static constexpr int g_FenetreX = 100;
    /// La position en y de la fenêtre GLUT
    static constexpr int g_FenetreY = 100;

    // le framerate cible de l'application graphique.
    static constexpr float frameRate = 50.0f;
};

#endif // CST_H
