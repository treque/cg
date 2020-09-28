#ifndef QUADRIC_H
#define QUADRIC_H

#include "ISurface.h"
#include "Vecteur3.h"

namespace Scene {

///////////////////////////////////////////////////////////////////////////////
///  CQuadrique
///  Cette classe implémente un Quadrique (ISurface)
///
///  Base class Scene::ISurface
///
///  @author Olivier Dionne
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
class CQuadrique : public ISurface
{
private:
    /// Termes quadratiques
    CVecteur3 m_Quadratique;
    /// Termes linéaires
    CVecteur3 m_Lineaire;
    /// Termes Linéaires
    CVecteur3 m_Mixte;
    /// Terme constant
    REAL m_Cst;

    REAL A;
    REAL E;
    REAL H;
    REAL B;
    REAL F;
    REAL C;
    REAL D;
    REAL G;
    REAL I;
    REAL J;

protected:
    /// Protected interface implementation
    virtual std::ostream& AfficherInfoDebug(std::ostream& Out) const;

public:
    /// Constructeurs
    CQuadrique(void);
    CQuadrique(const CQuadrique& Quadric);
    /// Destructeurs
    virtual ~CQuadrique(void);
    /// Opérateur copie
    CQuadrique& operator=(const CQuadrique& Quadric);

    /// Méthodes d'ajustement
    inline void AjusterQuadratique(const CVecteur3& Quadratic) { m_Quadratique = Quadratic; }
    inline void AjusterLineaire(const CVecteur3& Linear) { m_Lineaire = Linear; }
    inline void AjusterMixte(const CVecteur3& Mixed) { m_Mixte = Mixed; }
    inline void AjusterConstante(const REAL Cst) { m_Cst = Cst; }

    /// Méthodes d'obtention
    inline CVecteur3 ObtenirQuadratique(void) const { return m_Quadratique; }
    inline CVecteur3 ObtenirLineaire(void) const { return m_Lineaire; }
    inline CVecteur3 ObtenirMixte(void) const { return m_Mixte; }
    inline REAL      ObtenirConstante(void) const { return m_Cst; }
    CVecteur3        ObtenirNormale(const CVecteur3& point) const;

    /// Public interface implementation
    virtual void          Pretraitement(void);
    virtual CIntersection Intersection(const CRayon& Rayon);
    virtual CQuadrique*   Copier(void) const;

    void InitialiserConstantes();
};
} // namespace Scene

#endif // QUADRIC_H
