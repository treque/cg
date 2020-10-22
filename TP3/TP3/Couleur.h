#ifndef COULEUR_H
#define COULEUR_H

#include "MathUtils.h"
#include <assert.h>

using Math3D::REAL;

///////////////////////////////////////////////////////////////////////////////
///  CCouleur
///  Cette classe implémente une couleur RGB en float dont les valeurs varient
///  entre 0.0 et 1.0
///
///  @author Olivier Dionne
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
class CCouleur
{
private:
    /// Conversion de ubyte à float
    static constexpr float UBYTE_2_FLOAT = 1.0f / 255.0f;

public:
    /// Constante de couleur NOIRE
    const static CCouleur NOIR;
    /// Constante de couleur BLANCHE
    const static CCouleur BLANC;

    /// Composantes RGB de la couleur
    float r, g, b;

    /// Constructeurs
    inline CCouleur(void);
    inline CCouleur(const float R, const float G, const float B);
    inline CCouleur(const int R, const int G, const int B);
    inline explicit CCouleur(const float Val);
    inline CCouleur(const CCouleur& Color);

    /// Opérateurs unitaires
    inline float&    operator[](const int i);
    inline CCouleur& operator=(const CCouleur& Color);

    inline CCouleur& operator+=(const CCouleur& Color);
    inline CCouleur& operator-=(const CCouleur& Color);
    inline CCouleur& operator*=(const CCouleur& Color);
    inline CCouleur& operator*=(const REAL Val);

    /// Opérateurs binaires
    inline const CCouleur        operator+(const CCouleur& C2) const;
    inline const CCouleur        operator-(const CCouleur& C2) const;
    inline const CCouleur        operator*(const CCouleur& C2) const;
    inline const CCouleur        operator*(const REAL Val) const;
    inline friend const CCouleur operator*(const REAL Val, const CCouleur& Color);

    inline friend std::ostream& operator<<(std::ostream& Out, const CCouleur& Color);

    /// Définir la couleur courante
    inline void Ajuster(const float R, const float G, const float B);
};

///////////////////////////////////////////////////////////////////////////////
///  @brief Constructeur par défaut
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
inline CCouleur::CCouleur()
    : r(0.0f)
    , g(0.0f)
    , b(0.0f)
{
}

///////////////////////////////////////////////////////////////////////////////
///  @brief inline public overloaded constructor  CCouleur
///
///  @param [in]       R const float    Rouge
///  @param [in]       G const float    Vert
///  @param [in]       B const float    Bleu
///
///  @author Olivier Dionne
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
inline CCouleur::CCouleur(const float R, const float G, const float B)
    : r(Math3D::Borner<float>(R, 0.0f, 1.0f))
    , g(Math3D::Borner<float>(G, 0.0f, 1.0f))
    , b(Math3D::Borner<float>(B, 0.0f, 1.0f))
{
}

///////////////////////////////////////////////////////////////////////////////
///  @brief inline public overloaded constructor  CCouleur
///
///  @param [in]       R const int    Rouge
///  @param [in]       G const int    Vert
///  @param [in]       B const int    Bleu
///
///  @author Olivier Dionne
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
inline CCouleur::CCouleur(const int R, const int G, const int B)
    : r(R * UBYTE_2_FLOAT)
    , g(G * UBYTE_2_FLOAT)
    , b(B * UBYTE_2_FLOAT)
{
}

///////////////////////////////////////////////////////////////////////////////
///  @brief inline public overloaded constructor  CCouleur
///
///  @param [in]       Val const float   Fixer les composantes aux valeurs fournies
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
inline CCouleur::CCouleur(const float Val)
    : r(Math3D::Borner<float>(Val, 0.0f, 1.0f))
    , g(Math3D::Borner<float>(Val, 0.0f, 1.0f))
    , b(Math3D::Borner<float>(Val, 0.0f, 1.0f))
{
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Constructeur copie
///
///  @param [in]       Color const CCouleur &    Couleur à copier
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
inline CCouleur::CCouleur(const CCouleur& Color)
    : r(Color.r)
    , g(Color.g)
    , b(Color.b)
{
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Opérateur d'indexation de la couleur (lecteur/écriture)
///
///  @param [in]       i const int    Index
///
///  @return float &
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
inline float& CCouleur::operator[](const int i)
{
    assert(i > -1 && i < 3);
    return *(&r + i);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Opérateur d'égalité
///
///  @param [in]       Color const CCouleur &    La couleur à copier
///
///  @return CCouleur & La couleur courante
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
inline CCouleur& CCouleur::operator=(const CCouleur& Color)
{
    r = Color.r;
    g = Color.g;
    b = Color.b;

    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Opérateur d'addition
///
///  @param [in]       Color const CCouleur &    Couleurs à ajouter
///
///  @return CCouleur & La couleur courante
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
inline CCouleur& CCouleur::operator+=(const CCouleur& Color)
{
    r += Color.r;
    g += Color.g;
    b += Color.b;

    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Opérateur de soustraction
///
///  @param [in]       Color const CCouleur &    Couleur à soustraire
///
///  @return CCouleur & La couleur courante
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
inline CCouleur& CCouleur::operator-=(const CCouleur& Color)
{
    r -= Color.r;
    g -= Color.g;
    b -= Color.b;

    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Opérateur de modulation de couleur (*)
///
///  @param [in]       Color const CCouleur &    Couleur à moduler
///
///  @return CCouleur & La couleur courante
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
inline CCouleur& CCouleur::operator*=(const CCouleur& Color)
{
    r *= Color.r;
    g *= Color.g;
    b *= Color.b;

    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Opérateur de multiplication scalaire
///
///  @param [in]       Val const Math3D::REAL    Le scalaire par lequel multiplier
///
///  @return CCouleur & La couleur courante
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
inline CCouleur& CCouleur::operator*=(const REAL Val)
{
    r *= static_cast<float>(Val);
    g *= static_cast<float>(Val);
    b *= static_cast<float>(Val);

    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Opérateur d'addition de couleur
///
///  @param [in]       C2 const CCouleur &    Couleur 2
///
///  @return const CCouleur La couleur résultante
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
inline const CCouleur CCouleur::operator+(const CCouleur& C2) const
{
    return CCouleur(r + C2.r, g + C2.g, b + C2.b);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Opérateur de soustraction de 2 couleurs
///
///  @param [in]       C2 const CCouleur &    Couleur 2
///
///  @return const CCouleur La couleur résultante
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
inline const CCouleur CCouleur::operator-(const CCouleur& C2) const
{
    return CCouleur(r - C2.r, g - C2.g, b - C2.b);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Opérateur de modulation de couleur
///
///  @param [in]       C2 const CCouleur &    Couleur 2
///
///  @return const CCouleur La couleur résultante de la modulation
///
///  @author Olivier Dionne
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
inline const CCouleur CCouleur::operator*(const CCouleur& C2) const
{
    CCouleur Result;

    Result.r = r * C2.r;
    Result.g = g * C2.g;
    Result.b = b * C2.b;

    return Result;
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Opérateur de multiplication scalaire d'une couleur
///
///  @param [in]       Val const Math3D::REAL    Scalaire
///
///  @return const CCouleur La couleur résultante
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
inline const CCouleur CCouleur::operator*(const REAL Val) const
{
    CCouleur    Result;
    const float valf = static_cast<float>(Val);

    Result.r = r * valf;
    Result.g = g * valf;
    Result.b = b * valf;

    return Result;
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Opérateur de multiplication scalaire d'une couleur
///
///  @param [in]       Val const Math3D::REAL    Scalaire
///  @param [in]       Color const CCouleur &    Couleur
///
///  @return const CCouleur La couleur résultante
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
inline const CCouleur operator*(const REAL Val, const CCouleur& Color)
{
    return Color * Val;
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Opérateur de sortie texte
///
///  @param [in,out]  Out std::ostream &    Flux de sortie
///  @param [in]      Color const CCouleur &    La couleur à afficher
///
///  @return std::ostream & Le flux de sortie modifié
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
inline std::ostream& operator<<(std::ostream& Out, const CCouleur& Color)
{
    Out << std::setprecision(3) << "[ " << std::setw(3) << Color.r << ", " << Color.g << ", " << Color.b << " ]";
    return Out;
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Ajuste une couleur aux composantes RGB spécifiées
///
///  @param [in]       R const float    Rouge
///  @param [in]       G const float    Vert
///  @param [in]       B const float    Bleu
///
///  @return None
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
inline void CCouleur::Ajuster(const float R, const float G, const float B)
{
    r = Math3D::Borner<float>(R, 0.0f, 1.0f);
    g = Math3D::Borner<float>(G, 0.0f, 1.0f);
    b = Math3D::Borner<float>(B, 0.0f, 1.0f);
}

#endif // COULEUR_H
