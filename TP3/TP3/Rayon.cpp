#include "Rayon.h"

///////////////////////////////////////////////////////////////////////////////
///  public overloaded constructor  CRayon \n
///  Description : Constructeur par défaut
///
///  @return None
///
///  @author Olivier Dionne
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
CRayon::CRayon(void)
    : m_Origine(CVecteur3::ZERO)
    , m_Direction(CVecteur3::ZERO)
    , m_IndiceDeRefraction(REAL(0.0))
    , m_Energie(REAL(0.0))
    , m_NbRebonds(0)
{
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Constructeur par défaut
///
///  @param [in]       Rayon const CRayon &    Rayon à copier
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
CRayon::CRayon(const CRayon& Rayon)
    : m_Origine(Rayon.m_Origine)
    , m_Direction(Rayon.m_Direction)
    , m_IndiceDeRefraction(Rayon.m_IndiceDeRefraction)
    , m_Energie(Rayon.m_Energie)
    , m_NbRebonds(Rayon.m_NbRebonds)
{
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Destructeur
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
CRayon::~CRayon(void)
{
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Opérateur de copie
///
///  @param [in]       Rayon const CRayon &    Le rayon à copier
///
///  @return CRayon & The modified ray
///
///  @author Olivier Dionne
///  @date   14/08/2008
///////////////////////////////////////////////////////////////////////////////
CRayon& CRayon::operator=(const CRayon& Rayon)
{
    m_Origine            = Rayon.m_Origine;
    m_Direction          = Rayon.m_Direction;
    m_IndiceDeRefraction = Rayon.m_IndiceDeRefraction;
    m_Energie            = Rayon.m_Energie;
    m_NbRebonds          = Rayon.m_NbRebonds;
    return (*this);
}
