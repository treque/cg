///////////////////////////////////////////////////////////////////////////////
///  @file Gazon.h
///  @author  Frédéric Plourde
///  @brief   Déclare la classe CGazon qui encapsule une patch BSplinaire openGL
///           avec antialiasing
///  @date    2007-12-11
///  @version 1.0
///
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Cst.h"
#include "Modele3dAbstrait.h"
#include "SurfaceBSplinaire.h"

class CSea: CModele3DAbstrait
{
public:
    /// constructeur créant une patch bsplinaire (gazon) avec texture spécifiée unique (obligatoire)
    CSea(const char* nomFichierTexture, const float largeurGazon = 1.0, const float longueurGazon = 1.0);
    /// destructeur par défaut
    ~CSea(void) override;

    /// fonction à implémenter pour définir la géométrie openGL du gazon (est statique, car est référée par un pointeur
    /// de fonction)
    void dessiner() override;
    /// fonction à implémenter pour définir un texturage personnalisé
    void appliquerTextures() override;

private:
    void   setupVAO();
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ibo;
    GLint  m_taille;
};
