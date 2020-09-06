///////////////////////////////////////////////////////////////////////////////
///  @file Modele3D.h
///  @author  Frédéric Plourde
///  @brief   Déclare la classe CModele3DOBJ qui encapsule un modele3D spécifique
///           chargé à partir d'un fichier OBJ
///  @date    2008-10-27
///  @version 1.0
///
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_cross_product.hpp>
#include <glm/gtx/transform.hpp>

#include "Cst.h"
#include "Lumiere.h"
#include "Modele3dAbstrait.h"
#include "ObjParser/Geometry.h"
#include "ObjParser/ObjParser.h"
#include "Texture2D.h"

///////////////////////////////////////////////////////////////////////////////
///  CModele3DOBJ
///  Cette classe encapsule l'objet grahique principal du TP : un modèle 3D chargé
///  à partir d'un fichier OBJ
///
///  Base class CModele3DOBJAbstrait La classe de base encapsule tout modèle 3D openGL
///
///
///  @author Frédéric Plourde
///  @date   2008-10-27
///
///////////////////////////////////////////////////////////////////////////////
class CModele3dObj : public CModele3dAbstrait
{
public:
    /// constructeur spécifique pour création du modèle 3D ne comportant qu'une seule texture
    CModele3dObj(const std::string& nomFichierOBJ, const std::string& nomFichierTexture, float echelle = 1.0);
    /// constructeur spécifique pour création du modèle 3D comportant plusieurs textures
    CModele3dObj(const std::string& nomFichierOBJ, const std::vector<std::string>& nomsFichiersTextures,
                 float echelle = 1.0);
    /// destructeur par défaut
    ~CModele3dObj(void) override {}
    /// Fonction compatible avec opengl4 qui affiche le modèle 3D à partir des tableaux de sommets lus dans le fichier
    /// source OBJ.
    void dessiner() override;
    /// fonction qui charge en mémoire les sommets et les normales d'un modèle définit dans un fichier .OBJ.
    void chargerModeleOBJ(const std::string& fichierOBJ);
    /// fonction à implémenter pour personnaliser l'application de texture (à faire aussi : dans le constructeur,
    /// appeler "construire(&dessiner, &appliquerTextures)"
    void appliquerTextures() override;
    /// fonction qui permet de definir un nuanceur pour l'objet.
    // void attribuerNuanceur(CNuanceurProg nuanceur);

private:
    std::vector<Vertex>   g_Vertices_;  ///< le tableau des sommets du modèle 3D
    std::vector<Triangle> g_Triangles_; ///< le tableau des triangles du modèle 3D
    std::vector<Quad>     g_Quads_;     ///< le tableau des quads du modèle 3D

    CVecteur3 g_ObjCentroid_; ///< le centroïde du modèle3D

    GLuint sommets_vbo_;   ///< ID du VBO pour les sommets
    GLuint triangles_ibo_; ///< ID du IBO pour les triangles
    GLuint triangles_vao_; ///< ID du VAO pour les triangles
    GLint  size_;          ///< Nombre de floats à lire lors de l'affichage

    /// Programme de nuanceurs de l'objet
    // CNuanceurProg m_nuanceur_prog;
};
