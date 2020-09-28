#include "Triangle.h"

using namespace Scene;
using namespace Math3D;

///////////////////////////////////////////////////////////////////////////////
///  public overloaded constructor  CTriangle \n
///  Description : Constructeur par défaut
///
///  @return None
///
///  @author Olivier Dionne
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
CTriangle::CTriangle(void)
    : ISurface()
    , m_Normale(CVecteur3::ZERO)
{
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Constructeur par défaut
///
///  @param [in]       Triangle const Scene::CTriangle &    Le triangle à copier
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
CTriangle::CTriangle(const CTriangle& Triangle)
    : ISurface(Triangle)
    , m_Normale(Triangle.m_Normale)
{
    for (int i = 0; i < 3; i++)
        m_Pts[i] = Triangle.m_Pts[i];
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Destructeur
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
CTriangle::~CTriangle(void)
{
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Opérateur de copie
///
///  @param [in]       Triangle const Scene::CTriangle &    Le triangle à copier
///
///  @return Scene::CTriangle & Le triangle modifié
///
///  @author Olivier Dionne
///  @date   14/08/2008
///////////////////////////////////////////////////////////////////////////////
CTriangle& CTriangle::operator=(const CTriangle& Triangle)
{
    ISurface::operator=(Triangle);
    m_Normale         = Triangle.m_Normale;

    for (int i = 0; i < 3; i++)
        m_Pts[i] = Triangle.m_Pts[i];

    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Implémente le déboguage polymorphique par flux de sortie
///
///  @param [in, out]  Out std::ostream &   Le flux de sortie
///
///  @return std::ostream & Le flux de sortie modifié
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
std::ostream& CTriangle::AfficherInfoDebug(std::ostream& Out) const
{
    using std::endl;
    Out << "[DEBUG]: Triangle.Point1         = " << m_Pts[0] << endl;
    Out << "[DEBUG]: Triangle.Point2         = " << m_Pts[1] << endl;
    Out << "[DEBUG]: Triangle.Point3         = " << m_Pts[2] << endl;
    Out << "[DEBUG]: Triangle.Normale        = " << m_Normale;
    return Out;
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Pretraitement des données du triangle ( Appelé AVANT le lancer)
///
///  @return None
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
void CTriangle::Pretraitement(void)
{
    for (int i = 0; i < 3; i++)
        m_Pts[i] = m_Pts[i] * m_Transformation;
    CalculerNormale();
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Effectue l'intersection Rayon/Triangle
///
///  @param [in]       Rayon const CRayon &    Le rayon à tester
///
///  @return Scene::CIntersection Le résultat de l'ntersection
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
CIntersection CTriangle::Intersection(const CRayon& Rayon)
{
    CIntersection Result;

    // À COMPLÉTER ...

    // Voici deux références pour acomplir le développement :
    // 1) Tomas Akenine-Moller and Eric Haines "Real-Time Rendering 2nd Ed." 2002, p.581
    // 2) Son article: http://www.graphics.cornell.edu/pubs/1997/MT97.pdf

    // Notez que la normale du triangle est déjà calculée lors du prétraitement
    // il suffit que de la passer à la structure d'intersection.

    return Result;
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Alloue une copie du triangle courant
///
///  @return Scene::CTriangle * Nouvelle copie du triangle
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
CTriangle* CTriangle::Copier(void) const
{
    return new CTriangle(*this);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Calculer la normale du triangle à partir des côtés
///
///  @return None
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
void CTriangle::CalculerNormale(void)
{
    CVecteur3 Edge1 = m_Pts[1] - m_Pts[0];
    CVecteur3 Edge2 = m_Pts[2] - m_Pts[0];
    m_Normale       = CVecteur3::Normaliser(CVecteur3::ProdVect(Edge1, Edge2));
}
