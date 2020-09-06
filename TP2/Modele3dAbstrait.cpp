///////////////////////////////////////////////////////////////////////////////
///  @file Modele3DAbstrait.cpp
///  @author  Frédéric Plourde
///  @brief   Définit la classe CModele3DAbstrait qui encapsule un modèle 3D
///           openGL dont le graphisme est spécifié par l'entremise d'un
///           pointeur de fonction
///  @date    2007-12-13
///  @version 1.0
///////////////////////////////////////////////////////////////////////////////

#include "Modele3dAbstrait.h"
#include "Cst.h"
#include "Texture2D.h"

///////////////////////////////////////////////////////////////////////////////
///  @brief Construit un modèle 3D openGL générique au et ne comportant qu'une seule texture interne
///
///  @author Frédéric Plourde
///  @date   2008-02-20
///////////////////////////////////////////////////////////////////////////////
CModele3dAbstrait::CModele3dAbstrait(float echelle, const std::string& nomFichierTexture,
                                     TEXTypeEnum texType = TYPE_TEXTURE2D, bool /* compilerTex */, bool mipmap)
    : texType_(texType)
    , echelle_(echelle)
    , mipmap_(mipmap)
{
    // mémoriser le nom de fichier
    if (!nomFichierTexture.empty())
    {
        nomsFichiersTextures_.push_back(nomFichierTexture);
    }

    this->chargerTextures();
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Construit un modèle 3D openGL générique et comportant plusieurs
///  textures spécifiques dans un std::vecteur.
///
///  @author Frédéric Plourde
///  @date   2008-02-20
///////////////////////////////////////////////////////////////////////////////
CModele3dAbstrait::CModele3dAbstrait(float echelle, const std::vector<std::string>& nomsFichiersTextures,
                                     TEXTypeEnum texType = TYPE_TEXTURE2D, bool /* compilerTex */, bool mipmap)
    : texType_(texType)
    , echelle_(echelle)
    , mipmap_(mipmap)
    , nomsFichiersTextures_(nomsFichiersTextures)
{
    this->chargerTextures();
}

CModele3dAbstrait::~CModele3dAbstrait(void)
{
}

void CModele3dAbstrait::chargerTextures()
{
    // créer la texture si spécifiée
    if (nomsFichiersTextures_.size() >= 1)
    {
        for (uint i = 0; i < nomsFichiersTextures_.size(); i++)
        {
            switch (texType_)
            {
            case TYPE_TEXTURE2D:
            {
                textures_.push_back(new CTexture2D(nomsFichiersTextures_[i], mipmap_));
                break;
            }
            case TYPE_TEXTURE_CUBEMAP:
            {
                textures_.push_back(new CTextureCubemap(nomsFichiersTextures_[i]));
                break;
            }
            case TYPE_TEXTURE_CUBEMAP_HDR: std::cerr << "Type de texture non reconnu" << std::endl; break;
            }
        }
    }
    else
    {
        // signifie qu'il n'y aura pas de texture d'appliquée.
    }
}

void CModele3dAbstrait::appliquerTextures()
{
    if (nomsFichiersTextures_.size() >= 1)
    {
        // sinon, texturer de façon générique pour chaque texture
        for (uint i = 0; i < textures_.size(); i++)
        {
            textures_[i]->appliquer(WRAP_S | WRAP_T | WRAP_R | MIN_FILTER | MAG_FILTER, GL_CLAMP, GL_CLAMP, GL_CLAMP,
                                    GL_LINEAR, GL_LINEAR, 0, 0, 0, 0, 0, 0, mipmap_, CCst::unitesTextures[i]);

            // s'il y a plusieurs textures de spécifiées en couche, appliquer
            // un modulate générique
            if (textures_.size() > 1)
            {
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Ajoute une texture spécifique (préconstruite) a l'ensemble des textures
///  du modèle.
///
///  @param [in,out]  texture CTextureAbstraite *   le pointeur sur la texture abstraite.
///
///  @return Aucune
///
///  @author Frédéric Plourde
///  @date   2008-03-09
///////////////////////////////////////////////////////////////////////////////
void CModele3dAbstrait::ajouterTexture(CTextureAbstraite* texture)
{
    textures_.push_back(texture);
}

/// fonction qui permet de definir un nuanceur pour l'objet.
void CModele3dAbstrait::attribuerNuanceur(const CNuanceurProg& nuanceur)
{
    m_nuanceur_prog = nuanceur;
}

/// fonction qui permet de definir un nuanceur pour l'objet.
void CModele3dAbstrait::activerNuanceur()
{
    m_nuanceur_prog.activer();
}
