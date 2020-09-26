#include "Plan.h"

using namespace Scene;
using namespace Math3D;

///////////////////////////////////////////////////////////////////////////////
///  public overloaded constructor  CPlan \n
///  Description : Constructeur par défaut
///
///  @return None
///
///  @author Olivier Dionne
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
CPlan::CPlan(void)
    : ISurface()
    , m_Normale(CVecteur3::ZERO)
    , m_Cst(REAL(0))
{
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Constructeur par défaut
///
///  @param [in]       Plan const Scene::CPlan &    Le plan à copier
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
CPlan::CPlan(const CPlan& Plan)
    : ISurface(Plan)
    , m_Normale(Plan.m_Normale)
    , m_Cst(Plan.m_Cst)
{
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Destructeur
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
CPlan::~CPlan(void)
{
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Opérateur de copie
///
///  @param [in]       Plan const Scene::CPlan &    Le plan à copier
///
///  @return Scene::CPlan & Le plan courant
///
///  @author Olivier Dionne
///  @date   14/08/2008
///////////////////////////////////////////////////////////////////////////////
CPlan& CPlan::operator=(const CPlan& Plan)
{
    ISurface::operator=(Plan);
    m_Normale         = Plan.m_Normale;
    m_Cst             = Plan.m_Cst;
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Implémente la méthode "debug" de la classe de base pour les sorties
///
///  @param [in, out]  Out std::ostream &    Le flux de sortie
///
///  @return std::ostream & Le flux de sortie modifié
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
std::ostream& CPlan::AfficherInfoDebug(std::ostream& Out) const
{
    Out << "[DEBUG]: Plan.Normal            = " << m_Normale << std::endl;
    Out << "[DEBUG]: Plan.Cst               = " << m_Cst;
    return Out;
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Prétraitement des infos du plan ( appelé AVANT le lancer de rayons)
///
///  @return None
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
void CPlan::Pretraitement(void)
{
    m_Normale = CVecteur3::Normaliser(m_Normale * m_Transformation);

    CVecteur3 Pt;
    for (int i = 0; i < 3; i++)
    {
        if (std::abs(m_Normale[i]) > 0)
            Pt[i] = -(m_Cst / m_Normale[i]);
    }

    Pt    = Pt * m_Transformation;
    m_Cst = CVecteur3::ProdScal(-m_Normale, Pt);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Effectue l'intersection rayon/plan et retourne le résultat
///
///  @param [in]       Rayon const CRayon &   Le rayon à tester
///
///  @return Scene::CIntersection Le résultat de l'ntersection
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
CIntersection CPlan::Intersection(const CRayon& Rayon)
{
    CIntersection Result;

    // From http://www.siggraph.org/education/materials/HyperGraph/raytrace/rayplane_intersection.htm

    REAL Vd = CVecteur3::ProdScal(m_Normale, Rayon.ObtenirDirection());

    if (std::abs(Vd) > EPSILON)
    {
        Result.AjusterSurface(this);
        Result.AjusterDistance(-(CVecteur3::ProdScal(m_Normale, Rayon.ObtenirOrigine()) + m_Cst) / Vd);
        Result.AjusterNormale(m_Normale);
    }

    return Result;
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Allocates a copy of the current plane
///
///  @return Scene::CPlan * copie du nouveau plan ( si utilisé, ne pas oublier de l'effacer)
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
CPlan* CPlan::Copier(void) const
{
    return new CPlan(*this);
}
