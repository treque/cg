///////////////////////////////////////////////////////////////////////////////
///  @file Modele3DAbstrait.h
///  @author  Frédéric Plourde
///  @brief   Déclare la classe CModele3DAbstrait qui encapsule un modèle 3D
///           openGL dont le graphisme est spécifié par l'entremise d'un
///           pointeur de fonction
///  @date    2007-12-13
///  @version 1.0
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>

#include "NuanceurProg.h"
#include "TextureAbstraite.h"
#include "TextureCubemap.h"

///////////////////////////////////////////////////////////////////////////////
///  CModele3DAbstrait
///  Cette classe encapsule tout objet graphique openGL.
///
///  @remarks Pour créer un objet graphique (utilisant une liste d'affichage openGL),
///           on devrait DÉRIVER une classe personnalisé de CModele3DAbstrait. la
///           fonction protégée "construire" est disponible à toute classe dérivée
///           et doit impérativement être appelée à la construction de la classe
///           dérivée afin de fournir à la classe de base abstraite les pointeurs
///           de fonction sur la fonction d'affichage (au moins) et la fonction
///           de d'application des textures (faccult.).  Le graphisme "glBegin...glEnd"
///           devrait donc toujours faire partie de la classe dérivée et être pointée
///           par le pointeur de fonction "dessinerFunc" passé à "construire()".
///
///  @author Frédéric Plourde
///  @date   2008-03-11   <TODO: insérer initiales ici> <TODO: insérer le texte ici>
///
///////////////////////////////////////////////////////////////////////////////
class CModele3dAbstrait
{
public:
    /// constructeur créant un modèle 3D générique comportant une seule texture
    CModele3dAbstrait(float echelle, const std::string& nomFichierTexture, TEXTypeEnum texType, bool compilerTex = true,
                      bool mipmap = false);
    /// constructeur créant un modèle 3D générique comportant plusieurs textures
    CModele3dAbstrait(float echelle, const std::vector<std::string>& nomsFichiersTextures, TEXTypeEnum texType,
                      bool compilerTex = true, bool mipmap = false);
    /// destructeur par défaut
    virtual ~CModele3dAbstrait(void);
    /// ajoute une texture spécifique (préconstruite)au modèle
    void ajouterTexture(CTextureAbstraite* texture);
    /// doit etre implementé par les classes enfant pour afficher le modele
    virtual void dessiner() = 0;

    //////////////////////////////////////////
    // Les Accesseurs...                     /
    //////////////////////////////////////////
    /// obtient l'échelle du modèle 3D
    inline float obtenirEchelle() const { return echelle_; }
    /// modifie l'échelle du modèle 3D
    inline void modifierEchelle(float echelle) { echelle_ = echelle; }
    /// obtient une référence sur la texture
    inline const auto& obtenirTextures() const { return textures_; }
    /// Fonction permettant d'attribuer un nuanceur au modele
    void attribuerNuanceur(const CNuanceurProg& nuanceur);

    /// Fonction permettant d'activer le nuanceur
    void activerNuanceur();

protected:
    virtual void chargerTextures();   ///< Construit les différentes textures spécifiée
    virtual void appliquerTextures(); ///< On applique les textures

    std::vector<CTextureAbstraite*> textures_;       ///< la liste des textures openGL du modèle 3D
    CNuanceurProg                   m_nuanceur_prog; ///< Programme de nuanceurs de l'objet

private:
    TEXTypeEnum texType_; ///< le type de textures associées à l'objet (GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP, etc...)
    float echelle_; ///< facteur d'échelle général appliqué dans la liste d'affichage (glScale) lors de la compilation
    bool mipmap_;   ///< doit-on mipmapper ?

    std::vector<std::string> nomsFichiersTextures_; ///< la liste des noms de fichiers des textures
};
