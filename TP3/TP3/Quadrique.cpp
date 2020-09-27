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

    double xD = Rayon.ObtenirDirection().x;
    double yD = Rayon.ObtenirDirection().y;
    double zD = Rayon.ObtenirDirection().z;

    double xO = Rayon.ObtenirOrigine().x;
    double yO = Rayon.ObtenirOrigine().y;
    double zO = Rayon.ObtenirOrigine().z;


    double Aq = m_Quadratique.x * xD * xD
        + m_Mixte.z * xD * yD
        + m_Mixte.y * xD * zD
        + m_Quadratique.y * yD * yD
        + m_Mixte.x * yD * zD
        + m_Quadratique.z * zD * zD;

    double Bq = 2 * m_Quadratique.x * xO * xD 
        + m_Mixte.z * (xO * yD + xD * yO)
        + m_Mixte.y * (xO * zD + xD * zO)
        + m_Lineaire.x * xD
        + 2 * m_Quadratique.y * yO * yD
        + m_Mixte.x * (yO * zD + yD * zO)
        + m_Lineaire.y * yD
        + 2 * m_Quadratique.z * zO * zD
        + m_Lineaire.z * zD;

    double Cq = m_Quadratique.x * xO * xO
        + m_Mixte.z * xO * yO
        + m_Mixte.y * xO * zO
        + m_Lineaire.x * xO
        + m_Quadratique.y * yO * yO
        + m_Mixte.x * yO * zO
        + m_Lineaire.y * yO
        + m_Quadratique.z * zO * zO
        + m_Lineaire.z * zO
        + m_Cst;

    double rootPart = Bq * Bq - 4 * Aq * Cq;

    // TODO : Maybe put something in result here?
    if( rootPart < 0 )
    {
        Result.AjusterDistance( -1 );
        return Result;
    }
        

    if( Aq = 0 )
    {
        Result.AjusterDistance( -1 );
        return Result;
    }


    double t0 = ( -Bq + sqrt(rootPart) ) / ( 2 * Aq );
    double t1 = ( -Bq - sqrt(rootPart) ) / ( 2 * Aq );

    
    if( ( t0 < 0 ) && ( t1 < 0 ) )
    {
        Result.AjusterDistance( -1 );
        return Result;
    }

    CVecteur3 iPoint0;
    iPoint0.x = xO + t0 * xD;
    iPoint0.y = yO + t0 * yD;
    iPoint0.z = zO + t0 * zD;

    CVecteur3 iPoint1;
    iPoint1.x = xO + t1 * xD;
    iPoint1.y = yO + t1 * yD;
    iPoint1.z = zO + t1 * zD;

    CVecteur3 nPoint0;
    nPoint0.x = 2 * m_Quadratique.x * iPoint0.x
        + m_Mixte.z * iPoint0.y
        + m_Mixte.y * iPoint0.z
        + m_Lineaire.x;

    nPoint0.y = m_Mixte.z * iPoint0.x
        + 2 * m_Quadratique.y * iPoint0.y
        + m_Mixte.x * iPoint0.z
        + m_Lineaire.y;

    nPoint0.z = m_Mixte.y * iPoint1.x
        + m_Mixte.x * iPoint1.y
        + 2 * m_Quadratique.z * iPoint1.z
        + m_Lineaire.z;

    if( CVecteur3::ProdScal( nPoint0, Rayon.ObtenirDirection() ) > 0 )
        nPoint0 = -nPoint0;


    CVecteur3 nPoint1;
    nPoint1.x = 2 * m_Quadratique.x * iPoint1.x
        + m_Mixte.z * iPoint1.y
        + m_Mixte.y * iPoint1.z
        + m_Lineaire.x;

    nPoint1.y = m_Mixte.z * iPoint1.x
        + 2 * m_Quadratique.y * iPoint1.y
        + m_Mixte.x * iPoint1.z
        + m_Lineaire.y;

    nPoint1.z = m_Mixte.y * iPoint1.x
        + m_Mixte.x * iPoint1.y
        + 2 * m_Quadratique.z * iPoint1.z
        + m_Lineaire.z;

    if( CVecteur3::ProdScal( nPoint1, Rayon.ObtenirDirection() ) > 0 )
        nPoint1 = -nPoint1;

    double d;
    CVecteur3 n;

    if( t0 < t1 )
    {
        d = t0;
        n = nPoint0;
    }
    else
    {
        d = t1;
        n = nPoint1;
    }
    
    Result.AjusterDistance( d );
    Result.AjusterNormale( n );
    Result.AjusterSurface( this );


    // TODO: À COMPLÉTER ...

    // La référence pour l'algorithme d'intersection des quadriques est :
    // Eric Haines, Paul Heckbert "An Introduction to Ray Tracing",
    // Academic Press, Edited by Andrew S. Glassner, pp.68-73 & 288-293

    // S'il y a collision, ajuster les variables suivantes de la structure intersection :
    // Normale, Surface intersectée et la distance

    return Result;
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
