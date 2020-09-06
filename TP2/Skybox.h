///////////////////////////////////////////////////////////////////////////////
///  @file Skybox.h
///  @author  Frédéric Plourde
///  @brief   Déclare la classe CSkybox qui encapsule un skybox openGL avec
///           texture cubemap en format cross
///  @date    2007-12-11
///  @version 1.0
///
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include <string>

#include "Modele3dAbstrait.h"

class CSkybox : public CModele3dAbstrait
{
public:
    /// constructeur créant un skybox avec texture spécifiée unique (obligatoire)
    CSkybox(const std::string& nomFichierTexture, const float echelle);
    /// destructeur par défaut
    ~CSkybox(void) override;

    /// fonction à implémenter pour définir la géométrie openGL du skybox (est statique, car est référée par un pointeur
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
