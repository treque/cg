#include "Quadrique.h"

#include <algorithm>

using namespace Scene;
using namespace Math3D;

///////////////////////////////////////////////////////////////////////////////
///  public overloaded constructor  CQuadrique \n
///  Description : Constructeur par défaut
///
///  @return None
///
///  @author Olivier Dionne
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
CQuadrique::CQuadrique(void)
    : ISurface()
    , m_Quadratique(CVecteur3::ZERO)
    , m_Lineaire(CVecteur3::ZERO)
    , m_Mixte(CVecteur3::ZERO)
    , m_Cst(REAL(0))
{
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Constructeur par défaut
///
///  @param [in]       Quadric const Scene::CQuadrique &   la quadrique à copier
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
CQuadrique::CQuadrique(const CQuadrique& Quadric)
    : ISurface(Quadric)
    , m_Quadratique(Quadric.m_Quadratique)
    , m_Lineaire(Quadric.m_Lineaire)
    , m_Mixte(Quadric.m_Mixte)
    , m_Cst(Quadric.m_Cst)
{
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Destructeur
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
CQuadrique::~CQuadrique(void)
{
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Opérateur de copie
///
///  @param [in]       Quadric const Scene::CQuadrique &    La quadrique à copier
///
///  @return Scene::CQuadrique & La quadrique modifiée
///
///  @author Olivier Dionne
///  @date   14/08/2008
///////////////////////////////////////////////////////////////////////////////
CQuadrique& CQuadrique::operator=(const CQuadrique& Quadric)
{
    ISurface::operator=(Quadric);
    m_Quadratique     = Quadric.m_Quadratique;
    m_Lineaire        = Quadric.m_Lineaire;
    m_Mixte           = Quadric.m_Mixte;
    m_Cst             = Quadric.m_Cst;
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Implémente le déboguage polymorphique par flux de sortie
///
///  @param [in, out]  Out std::ostream &    Le flux de sortie
///
///  @return std::ostream & Le flux de sortie modifié
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
std::ostream& CQuadrique::AfficherInfoDebug(std::ostream& Out) const
{
    using std::endl;
    Out << "[DEBUG]: Quadric.Quadratique       = " << m_Quadratique << endl;
    Out << "[DEBUG]: Quadric.Lineaire          = " << m_Lineaire << endl;
    Out << "[DEBUG]: Quadric.Mixte             = " << m_Mixte << endl;
    Out << "[DEBUG]: Quadric.Constante         = " << m_Cst;
    return Out;
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Pretraitement des données de la quadrique( appelé AVANT le lancer)
///
///  @return None
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
void CQuadrique::Pretraitement(void)
{
    // Algorithme tiré de ...
    // R. Goldman, "Two Approach to a Computer Model for Quadric Surfaces",
    // IEEE CG&A, Sept 1983, pp.21

    REAL A = m_Quadratique.x;
    REAL B = m_Quadratique.y;
    REAL C = m_Quadratique.z;
    REAL D = m_Mixte.z * REAL(0.5);
    REAL E = m_Mixte.x * REAL(0.5);
    REAL F = m_Mixte.y * REAL(0.5);
    REAL G = m_Lineaire.x * REAL(0.5);
    REAL H = m_Lineaire.y * REAL(0.5);
    REAL J = m_Lineaire.z * REAL(0.5);
    REAL K = m_Cst;

    CMatrice4 Q(A, D, F, G, D, B, E, H, F, E, C, J, G, H, J, K);

    CMatrice4 Inverse = m_Transformation.Inverse();

    Q = Inverse * Q * Inverse.Transpose();

    m_Quadratique.x = Q[0][0];
    m_Quadratique.y = Q[1][1];
    m_Quadratique.z = Q[2][2];
    m_Cst           = Q[3][3];
    m_Mixte.x       = Q[1][2] * REAL(2.0);
    m_Mixte.y       = Q[0][2] * REAL(2.0);
    m_Mixte.z       = Q[0][1] * REAL(2.0);
    m_Lineaire.x    = Q[0][3] * REAL(2.0);
    m_Lineaire.y    = Q[1][3] * REAL(2.0);
    m_Lineaire.z    = Q[2][3] * REAL(2.0);
}

void CQuadrique::InitialiserConstantes()
{
    A = m_Quadratique.x;
    E = m_Quadratique.y;
    H = m_Quadratique.z;

    B = m_Mixte.z / 2.0;
    F = m_Mixte.x / 2.0;
    C = m_Mixte.y / 2.0;

    D = m_Lineaire.x / 2.0;
    G = m_Lineaire.y / 2.0;
    I = m_Lineaire.z / 2.0;

    J = m_Cst;
}
///////////////////////////////////////////////////////////////////////////////
///  @brief Effectue l'intersection Rayon/Quadrique
///
///  @param [in]       Rayon const CRayon &    Le rayon à tester
///
///  @return Scene::CIntersection Le résultat de l'ntersection
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
CIntersection CQuadrique::Intersection(const CRayon& Rayon)
{
    CIntersection Result;
   
    /*
    * ax^2 + 2bxy + 2cxz + 2dxw + ey^2 + 2fyz + 2gyq + hz^2 + 2izw + jw^2 = 0
    * 
    * [ X Y Z 1 ] DOT     [ A B C D    DOT [ X        = 0
    *                       B E F G          Y
    *                       C F H I          Z
    *                       D G I J ]        1 ]
     */
     // La référence pour l'algorithme d'intersection des quadriques est :
     // Eric Haines, Paul Heckbert "An Introduction to Ray Tracing",
     // Academic Press, Edited by Andrew S. Glassner, pp.68-73 & 288-293

    InitialiserConstantes();

    const CVecteur3 o = Rayon.ObtenirOrigine();
    const CVecteur3 d = Rayon.ObtenirDirection();
    const REAL X = d.x;
    const REAL Y = d.y;
    const REAL Z = d.z;
    const REAL X_o = o.x;
    const REAL Y_o = o.y;
    const REAL Z_o = o.z;

    // Resolve intersection
    REAL Aq = A * X * X + 2 * B * X * Y + 2 * C * X * Z + E * Y * Y + 2 * F * Y * Z + H * Z * Z;
    REAL Bq = 2 * (A * X_o * X + B * (X_o * Y + X * Y_o) + C * (X_o * Z + X * Z_o) + D * X + E * Y_o * Y + F * (Y_o * Z + Y * Z_o) 
        + G * Y + H * Z_o * Z + I * Z);
    REAL Cq = A * X_o * X_o + 2 * B * X_o * Y_o + 2 * C * X_o * Z_o + 2 * D * X_o + E * Y_o * Y_o + 2 * F * Y_o * Z_o + 2 * G * Y_o 
        + H * Z_o * Z_o + 2 * I * Z_o + J;

    // S'il y a collision, ajuster les variables suivantes de la structure intersection :
    // Normale, Surface intersectée et la distance

    const REAL discriminant = Bq * Bq - 4 * Aq * Cq;
    REAL t0, t1, t;

    if (abs(Aq) < EPSILON) // handle div/0
    {
        t = -Cq / Bq;
        if (t > EPSILON)
        {
            Result.AjusterDistance(t);
            Result.AjusterSurface(this);
            Result.AjusterNormale(ObtenirNormale(o + d * t));
        }
    }
    else if (discriminant >= 0) // on va rarement avoir une seule solution a cause de l'imprecision
    {
        t0 = (-Bq - sqrt(discriminant)) / (2 * Aq);
        t1 = (-Bq + sqrt(discriminant)) / (2 * Aq);
        const REAL min = std::min(t0, t1);
        const REAL max = std::max(t0, t1);

        t = min > EPSILON ? min : max; // si t est negatif, cest quon lance le rayon en arriere..
        if (t > EPSILON)
        {
            Result.AjusterDistance(t);
            Result.AjusterSurface(this);
            Result.AjusterNormale(ObtenirNormale(o + d * t));
        }
    }
    return Result;
}

CVecteur3 CQuadrique::ObtenirNormale(const CVecteur3& point) const
{
    return CVecteur3::Normaliser(CVecteur3(2 * (A * point.x + B * point.y + C * point.z + D),
                                           2 * (B * point.x + E * point.y + F * point.z + G),
                                           2 * (C * point.x + F * point.y + H * point.z + I)));
}
///////////////////////////////////////////////////////////////////////////////
///  @brief Alloue une copie de la quadrique courante
///
///  @return Scene::CQuadrique *la copie de la quadrique
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
CQuadrique* CQuadrique::Copier(void) const
{
    return new CQuadrique(*this);
}
