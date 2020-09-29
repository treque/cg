#include "Scene.h"
#include "Plan.h"
#include "Quadrique.h"
#include "StringUtils.h"
#include "Triangle.h"
#include <fstream>
#include <map>

using namespace std;
using namespace Math3D;
using namespace Scene;

#define STRING_CHECKFIND(Buffer, String) (Buffer.find(String) != string::npos)

///////////////////////////////////////////////////////////////////////////////
///  private overloaded constructor  CScene \n
///  Description : Constructeur par défaut
///
///  @return None
///
///  @author Olivier Dionne
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
CScene::CScene(void)
    : m_ResLargeur(0)
    , m_ResHauteur(0)
    , m_TextureScene(0)
    , m_CouleurArrierePlan(CCouleur::NOIR)
    , m_IndiceRefractionScene(REAL(1.0))
    , m_NbRebondsMax(20)
    , m_EnergieMinRayon(REAL(0.01))
{
    m_Camera.Position    = CVecteur3::ZERO;
    m_Camera.PointVise   = CVecteur3::ZERO;
    m_Camera.Up          = CVecteur3::UNIT_Y;
    m_Camera.Orientation = CMatrice4::IDENTITE;
    m_Camera.Focale      = REAL(50.0);
    m_Camera.Angle       = 0;
}

///////////////////////////////////////////////////////////////////////////////
///  private destructor  ~CScene \n
///  Description : Destructeur
///
///  @return None
///
///  @author Olivier Dionne
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
CScene::~CScene(void)
{
    Liberer();
}

///////////////////////////////////////////////////////////////////////////////
///  public  AjouterSurface \n
///  Description : Ajoute une surface à la scène
///
///  @param [in]       Surface Scene::ISurface *const Un pointeur constant à la surface
///
///  @return None
///
///  @author Olivier Dionne
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
void CScene::AjouterSurface(ISurface* const Surface)
{
    m_Surfaces.push_back(Surface);
}

///////////////////////////////////////////////////////////////////////////////
///  public  AjouterLumiere \n
///  Description : Ajoute une lumière à la scène
///
///  @param [in]       Lumiere Scene::CLumiere *const    Pointeur constant vers une lumière
///
///  @return None
///
///  @author Olivier Dionne
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
void CScene::AjouterLumiere(CLumiere* const Lumiere)
{
    m_Lumieres.push_back(Lumiere);
}

///////////////////////////////////////////////////////////////////////////////
///  private  Initialize \n
///  Description : Initialise la caméra de scène et les surfaces avant de commencer
///                le lancer de rayons
///
///  @return None
///
///  @author Olivier Dionne
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
void CScene::Initialiser(void)
{
    InitialiserCamera();

    for (const auto& surface : m_Surfaces)
    {
        surface->Pretraitement();
    }

    m_InfoPixel.resize(static_cast<size_t>(m_ResLargeur * m_ResHauteur * 3));
}

///////////////////////////////////////////////////////////////////////////////
///  public  AjusterResolution \n
///  Description : Ajuster la résolution de la texture
///
///  @param [in]       ResX const int Resolution en largeut
///  @param [in]       ResY const int Resolution en hauteur
///
///  @return None
///
///  @author Olivier Dionne
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
void CScene::AjusterResolution(const int ResLargeur, const int ResHauteur)
{
    m_ResLargeur = ResLargeur;
    m_ResHauteur = ResHauteur;
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Set maximum ray bounce
///
///  @param [in]       NbRebondsMax const int      Le nombre maximal de rebonds
///
///  @return None
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
void CScene::AjusterNbRebondsMax(const int NbRebondsMax)
{
    m_NbRebondsMax = NbRebondsMax;
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Ajuste l'énergie minimale du rayon
///
///  @param [in]       EnergieMin const Math3D::REAL    L'énergie minimale du rayon
///
///  @return None
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
void CScene::AjusterEnergieMinimale(const REAL EnergieMin)
{
    m_EnergieMinRayon = EnergieMin;
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Ajuste l'indice de réfraction de la scène
///
///  @param [in]       IndiceRefraction const Math3D::REAL   Indice de réfraction
///
///  @return None
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
void CScene::AjusterIndiceRefraction(const REAL IndiceRefraction)
{
    m_IndiceRefractionScene = IndiceRefraction;
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Traite le fichier de données de la scène
///
///  @param [in]       Fichier const char *    Fichier à traiter
///
///  @return None
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
void CScene::TraiterFichierDeScene(const char* Fichier)
{
    fstream FichierScene(Fichier, ios::in);

    if (FichierScene.is_open())
    {
        EtatTraitementScene EtatCourant = TRAITEMENT_SCENE;
        EtatTraitementScene EtatNouveau = TRAITEMENT_SCENE;

        char   Line[NB_MAX_CAR_PAR_LIGNE];
        string Buffer;

        CLumiere* Lumiere = nullptr;
        ISurface* Surface = nullptr;

        float Val0, Val1, Val2;
        int   R, G, B;

        while (!FichierScene.eof())
        {
            FichierScene.getline(Line, NB_MAX_CAR_PAR_LIGNE);
            Buffer = Line;
            CStringUtils::Trim(Buffer, " ");

            // Passer les lignes vides et les commentaires
            if (Buffer.empty() || Buffer[0] == '*')
                continue;
            else
            {
                // Vérifier l'arrivée d'un nouvel état de traitement
                bool EstNouvelObjetScene = true;
                if (STRING_CHECKFIND(Buffer, "Lumiere:"))
                    EtatNouveau = TRAITEMENT_LUMIERE;
                else if (STRING_CHECKFIND(Buffer, "Poly:"))
                    EtatNouveau = TRAITEMENT_TRIANGLE;
                else if (STRING_CHECKFIND(Buffer, "Plane:"))
                    EtatNouveau = TRAITEMENT_PLAN;
                else if (STRING_CHECKFIND(Buffer, "Quad:"))
                    EtatNouveau = TRAITEMENT_QUADRIQUE;
                else
                    EstNouvelObjetScene = false;

                if (EstNouvelObjetScene)
                {
                    // Ajouter objet nouvellement traité à la scène
                    if (EtatCourant != TRAITEMENT_SCENE)
                    {
                        if (EtatCourant == TRAITEMENT_LUMIERE)
                            AjouterLumiere(Lumiere);
                        else
                            AjouterSurface(Surface);

                        Surface = nullptr;
                        Lumiere = nullptr;
                    }

                    // Substituer le nouvel état pour l'ancien
                    EtatCourant = EtatNouveau;

                    // créer un nouvel objet de scène puis le remplir
                    switch (EtatCourant)
                    {
                    case TRAITEMENT_LUMIERE:
                        Lumiere = new CLumiere();
                        cout << "[ACTION]: Creation d'une lumiere..." << endl;
                        break;
                    case TRAITEMENT_TRIANGLE:
                        Surface = new CTriangle();
                        cout << "[ACTION]: Creation d'un triangle..." << endl;
                        break;
                    case TRAITEMENT_PLAN:
                        Surface = new CPlan();
                        cout << "[ACTION]: Creation d'un plan..." << endl;
                        break;
                    case TRAITEMENT_QUADRIQUE:
                        Surface = new CQuadrique();
                        cout << "[ACTION]: Creation d'une quadrique..." << endl;
                        break;
                    case TRAITEMENT_SCENE: cout << "[ACTION]: Unhandled enum value!" << endl; break;
                    }
                }
                else
                {
                    // Remplir les informations génériques de l'objet courant
                    if (Surface != nullptr)
                    {
                        bool IsGenericSurfaceInfo = true;

                        if (STRING_CHECKFIND(Buffer, "color:"))
                        {
                            sscanf(Buffer.c_str(), "%s %i %i %i", Line, &R, &G, &B);
                            Surface->AjusterCouleur(CCouleur(R, G, B));
                        }
                        else if (STRING_CHECKFIND(Buffer, "ambient:"))
                        {
                            sscanf(Buffer.c_str(), "%s %f", Line, &Val0);
                            Surface->AjusterCoeffAmbiant(REAL(Val0));
                        }
                        else if (STRING_CHECKFIND(Buffer, "diffus:"))
                        {
                            sscanf(Buffer.c_str(), "%s %f", Line, &Val0);
                            Surface->AjusterCoeffDiffus(REAL(Val0));
                        }
                        else if (STRING_CHECKFIND(Buffer, "specular:"))
                        {
                            sscanf(Buffer.c_str(), "%s %f %f", Line, &Val0, &Val1);
                            Surface->AjusterCoeffSpeculaire(REAL(Val0));
                            Surface->AjusterCoeffBrillance(REAL(Val1));
                        }
                        else if (STRING_CHECKFIND(Buffer, "reflect:"))
                        {
                            sscanf(Buffer.c_str(), "%s %f", Line, &Val0);
                            Surface->AjusterCoeffReflexion(REAL(Val0));
                        }
                        else if (STRING_CHECKFIND(Buffer, "refract:"))
                        {
                            sscanf(Buffer.c_str(), "%s %f %f", Line, &Val0, &Val1);
                            Surface->AjusterCoeffRefraction(REAL(Val0));
                            Surface->AjusterIndiceRefraction(REAL(Val1));
                        }
                        else if (STRING_CHECKFIND(Buffer, "rotate:"))
                        {
                            sscanf(Buffer.c_str(), "%s %f %f %f", Line, &Val0, &Val1, &Val2);

                            CMatrice4 Transform = Surface->ObtenirTransformation();
                            Transform.RotationAutourDesX(Deg2Rad<REAL>(REAL(Val0)));
                            Transform.RotationAutourDesY(Deg2Rad<REAL>(REAL(Val1)));
                            Transform.RotationAutourDesZ(Deg2Rad<REAL>(REAL(Val2)));
                            Surface->AjusterTransformation(Transform);
                        }
                        else if (STRING_CHECKFIND(Buffer, "translate:"))
                        {
                            sscanf(Buffer.c_str(), "%s %f %f %f", Line, &Val0, &Val1, &Val2);
                            CMatrice4 Transform = Surface->ObtenirTransformation();
                            Transform.Translation(REAL(Val0), REAL(Val1), REAL(Val2));
                            Surface->AjusterTransformation(Transform);
                        }
                        else if (STRING_CHECKFIND(Buffer, "scale:"))
                        {
                            sscanf(Buffer.c_str(), "%s %f %f %f", Line, &Val0, &Val1, &Val2);
                            CMatrice4 Transform = Surface->ObtenirTransformation();
                            Transform.MiseAEchelle(REAL(Val0), REAL(Val1), REAL(Val2));
                            Surface->AjusterTransformation(Transform);
                        }
                        else
                            IsGenericSurfaceInfo = false;

                        if (IsGenericSurfaceInfo)
                            continue;
                    }

                    // Remplir les infos spécifiques à l'objet
                    switch (EtatCourant)
                    {
                    case TRAITEMENT_SCENE:

                        if (STRING_CHECKFIND(Buffer, "background:"))
                        {
                            sscanf(Buffer.c_str(), "%s %i %i %i", Line, &R, &G, &B);
                            AjusterCouleurArrierePlan(CCouleur(R, G, B));
                        }
                        else if (STRING_CHECKFIND(Buffer, "origin:"))
                        {
                            sscanf(Buffer.c_str(), "%s %f %f %f", Line, &Val0, &Val1, &Val2);
                            AjusterPositionCamera(CVecteur3(REAL(Val0), REAL(Val1), REAL(Val2)));
                        }
                        else if (STRING_CHECKFIND(Buffer, "eye:"))
                        {
                            sscanf(Buffer.c_str(), "%s %f %f %f", Line, &Val0, &Val1, &Val2);
                            AjusterPointViseCamera(CVecteur3(REAL(Val0), REAL(Val1), REAL(Val2)));
                        }
                        else if (STRING_CHECKFIND(Buffer, "up:"))
                        {
                            sscanf(Buffer.c_str(), "%s %f %f %f", Line, &Val0, &Val1, &Val2);
                            AjusterVecteurUpCamera(CVecteur3(REAL(Val0), REAL(Val1), REAL(Val2)));
                        }

                        break;

                    case TRAITEMENT_LUMIERE:

                        if (STRING_CHECKFIND(Buffer, "position:"))
                        {
                            sscanf(Buffer.c_str(), "%s %f %f %f", Line, &Val0, &Val1, &Val2);
                            Lumiere->SetPosition(CVecteur3(REAL(Val0), REAL(Val1), REAL(Val2)));
                        }
                        else if (STRING_CHECKFIND(Buffer, "intens:"))
                        {
                            sscanf(Buffer.c_str(), "%s %f", Line, &Val0);
                            Lumiere->SetIntensity(REAL(Val0));
                        }
                        else if (STRING_CHECKFIND(Buffer, "color:"))
                        {
                            sscanf(Buffer.c_str(), "%s %i %i %i", Line, &R, &G, &B);
                            Lumiere->AjusterCouleur(CCouleur(R, G, B));
                        }

                        break;

                    case TRAITEMENT_TRIANGLE:

                        if (STRING_CHECKFIND(Buffer, "point:"))
                        {
                            int PtIdx;
                            sscanf(Buffer.c_str(), "%s %i %f %f %f", Line, &PtIdx, &Val0, &Val1, &Val2);
                            dynamic_cast<CTriangle*>(Surface)->AjusterPoint(
                                PtIdx, CVecteur3(REAL(Val0), REAL(Val1), REAL(Val2)));
                        }

                        break;

                    case TRAITEMENT_PLAN:

                        if (STRING_CHECKFIND(Buffer, "v_linear:"))
                        {
                            sscanf(Buffer.c_str(), "%s %f %f %f", Line, &Val0, &Val1, &Val2);
                            dynamic_cast<CPlan*>(Surface)->AjusterNormale(
                                CVecteur3(REAL(Val0), REAL(Val1), REAL(Val2)));
                        }
                        else if (STRING_CHECKFIND(Buffer, "v_const:"))
                        {
                            sscanf(Buffer.c_str(), "%s %f", Line, &Val0);
                            dynamic_cast<CPlan*>(Surface)->AjusterConstante(REAL(Val0));
                        }

                        break;

                    case TRAITEMENT_QUADRIQUE:

                        if (STRING_CHECKFIND(Buffer, "v_quad:"))
                        {
                            sscanf(Buffer.c_str(), "%s %f %f %f", Line, &Val0, &Val1, &Val2);
                            dynamic_cast<CQuadrique*>(Surface)->AjusterQuadratique(
                                CVecteur3(REAL(Val0), REAL(Val1), REAL(Val2)));
                        }
                        else if (STRING_CHECKFIND(Buffer, "v_mixte:"))
                        {
                            sscanf(Buffer.c_str(), "%s %f %f %f", Line, &Val0, &Val1, &Val2);
                            dynamic_cast<CQuadrique*>(Surface)->AjusterMixte(
                                CVecteur3(REAL(Val0), REAL(Val1), REAL(Val2)));
                        }
                        else if (STRING_CHECKFIND(Buffer, "v_linear:"))
                        {
                            sscanf(Buffer.c_str(), "%s %f %f %f", Line, &Val0, &Val1, &Val2);
                            dynamic_cast<CQuadrique*>(Surface)->AjusterLineaire(
                                CVecteur3(REAL(Val0), REAL(Val1), REAL(Val2)));
                        }
                        else if (STRING_CHECKFIND(Buffer, "v_const:"))
                        {
                            sscanf(Buffer.c_str(), "%s %f", Line, &Val0);
                            dynamic_cast<CQuadrique*>(Surface)->AjusterConstante(REAL(Val0));
                        }

                        break;
                    }
                }
            }
        }

        // Fermer le fichier de scène
        FichierScene.close();

        // Ajouter le dernier objet traité
        if (Surface != nullptr)
            AjouterSurface(Surface);
        if (Lumiere != nullptr)
            AjouterLumiere(Lumiere);
    }
    else
        cerr << "[CScene::TraiterFichierDeScene()] : Incapable d'ouvrir " << Fichier << "." << endl;
}

///////////////////////////////////////////////////////////////////////////////
///  public  Liberer \n
///  Description : Désallouer toutes les données de scène
///
///  @return None
///
///  @author Olivier Dionne
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
void CScene::Liberer(void)
{
    // Supprimer les surfaces
    for (auto& surface : m_Surfaces)
    {
        delete surface;
    }
    m_Surfaces.clear();

    // Supprimer les lumières
    for (auto& lumiere : m_Lumieres)
    {
        delete lumiere;
    }
    m_Lumieres.clear();

    // Supprimer le rendu de scène
    glDeleteTextures(1, &m_TextureScene);
}

///////////////////////////////////////////////////////////////////////////////
///  public  AjusterPositionCamera \n
///  Description : Ajuster la position de la caméra
///
///  @param [in]       Position const Math3D::CVecteur3 & Position
///
///  @return None
///
///  @author Olivier Dionne
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
void CScene::AjusterPositionCamera(const CVecteur3& Position)
{
    m_Camera.Position = Position;
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Ajuster le point cisé par la caméra
///
///  @param [in]       PointVise const Math3D::CVecteur3 &  PointVise
///
///  @return None
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
void CScene::AjusterPointViseCamera(const CVecteur3& PointVise)
{
    m_Camera.PointVise = PointVise;
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Ajuster le vecteur up de la caméra
///
///  @param [in]       Up const Math3D::CVecteur3 &  Up
///
///  @return None
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
void CScene::AjusterVecteurUpCamera(const CVecteur3& Up)
{
    m_Camera.Up = Up;
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Ajuster la couleur de l'arrière-plan de la scène
///
///  @param [in]       Color const CCouleur &     Couleur ArrierePlan
///
///  @return None
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
void CScene::AjusterCouleurArrierePlan(const CCouleur& Couleur)
{
    m_CouleurArrierePlan = Couleur;
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Obtenir le rendu de la scène dans une texture openGL
///
///  @return GLuint Texture Id
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
GLuint CScene::ObtenirTextureGL(void)
{
    return m_TextureScene;
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Initialiser l'angle et la matrice "modelView" de la caméra
///
///  @return None
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
void CScene::InitialiserCamera(void)
{
    // Calculer l'angle de la caméra
    //
    //
    //                  lentille                  |
    //                    |                       |
    // écran/             |                       y2
    // film               |                       |
    // -+---------+-------+-------+---------------+
    //  |         |<- f ->|<- f ->|
    //  y1                |
    //  |                 |
    //  |
    //
    //  <------ d1 ------>|<--------- d2 --------->
    //
    // From: 1 / f   = 1 / d1 + 1 / d2
    //  and: y1 / y2 = d1 / d2
    //  and: d1      = DIM_FILM
    //       y2      = ( d2 / f - 1 ) * DIM_FILM
    //
    // Nous avons besoin de trouver atan( y2 / d2 ) pour l'ouverture du champ
    REAL d2        = CVecteur3::Distance(m_Camera.Position, m_Camera.PointVise);
    REAL y2        = (d2 / (m_Camera.Focale * REAL(0.001)) - 1) * DIM_FILM_CAM;
    m_Camera.Angle = (360 * atan2(y2 * REAL(0.5), d2)) / REAL(PI);

    // Calculer la matrice modelview de la caméra
    CVecteur3 N = CVecteur3::Normaliser(m_Camera.Position - m_Camera.PointVise);
    CVecteur3 V = CVecteur3::Normaliser(m_Camera.Up - N * CVecteur3::ProdScal(m_Camera.Up, N));
    CVecteur3 U = CVecteur3::ProdVect(V, N);

    m_Camera.Orientation = CMatrice4(U.x, U.y, U.z, REAL(0), V.x, V.y, V.z, REAL(0), N.x, N.y, N.z, REAL(0), REAL(0),
                                     REAL(0), REAL(0), REAL(0));
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Lancement des rayons (raytrace) dans la scène courante
///
///  @return None
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
void CScene::LancerRayons(void)
{
    Initialiser();

    // À COMPLÉTER ...

    // POUR chaque position Py de pixel de la grille virtuelle
    // {
    //      POUR chaque position Px de pixel de la grille virtuelle
    //      {
    //          Ajuster lorigine du rayon au centre de la caméra
    //
    //          Calculer la direction du rayon vers la coordonnée réelle
    //          du pixel ( Px,Py )
    //
    //          Ajuster l'orientation du rayon ( utiliser la matrice
    //          Orientation de la camera qui est déjà calculé pour vous ) et le normaliser
    //
    //          Initialiser les autres caractéristiques du rayon à :
    //              - Energie            = 1
    //              - NbRebonds          = 0
    //              - IndiceDeRefraction = 1
    //
    //          Lancer le rayon pour obtenir la couleur du pixel avec la fonction
    //          CScene::ObtenirCouleur()
    //
    //          Enregistrer les composantes R, G et B de la couleur du pixel dans la
    //          structure linéaire m_PixelInfo de taille ResolutionX * ResolutionY * 3
    //      }
    //  }
    const REAL aspectRatio = m_ResLargeur / m_ResHauteur;
    const REAL hauteur = tan( Deg2Rad<REAL>( m_Camera.Angle / 2.0 ) ) * 2.0;
    const REAL largeur = aspectRatio * hauteur;

    const REAL taillePixelX = hauteur / m_ResHauteur;
    const REAL taillePixelY = largeur / m_ResLargeur;

    for( int j = 0; j < m_ResHauteur; ++j )
    {
        REAL py = ( j - m_ResHauteur / 2 ) * taillePixelY;
        for( int i = 0; i < m_ResLargeur; ++i )
        {
            CRayon rayon = CRayon();
            rayon.AjusterOrigine( m_Camera.Position );

            REAL px = ( i - m_ResLargeur / 2 ) * taillePixelX;
            CVecteur3 pixelPos( px, py, -1 ); // coord dans son model

            rayon.AjusterDirection( CVecteur3::Normaliser( pixelPos * m_Camera.Orientation ) );

            rayon.AjusterEnergie( 1 );
            rayon.AjusterNbRebonds( 0 );
            rayon.AjusterIndiceRefraction( 1 );

            CCouleur col = ObtenirCouleur( rayon );

            unsigned int index = ( i + j * this->m_ResLargeur ) * 3;
            m_InfoPixel[ index ] = col.r;
            m_InfoPixel[ index + 1 ] = col.g;
            m_InfoPixel[ index + 2 ] = col.b;
        }
    }

    // Créer une texture openGL
    glGenTextures( 1, &m_TextureScene );
    glBindTexture( GL_TEXTURE_2D, m_TextureScene );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, m_ResLargeur, m_ResHauteur, 0, GL_RGB, GL_FLOAT, m_InfoPixel.data() );
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Obtenir la couleur du pixel pour un rayon donné
///
///  @param [in]       Rayon const CRayon &    Le rayon à tester
///
///  @return const CCouleur La couleur du pixel
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
const CCouleur CScene::ObtenirCouleur(const CRayon& Rayon) const
{
    CIntersection Result;
    CIntersection Tmp;

    for (const auto& surface : m_Surfaces)
    {
        Tmp = surface->Intersection(Rayon);
        if (Tmp.ObtenirDistance() > EPSILON &&
            (Tmp.ObtenirDistance() < Result.ObtenirDistance() || Result.ObtenirDistance() < 0))
            Result = Tmp;
    }

    // S'il n'y aucune intersection, retourner la couleur de l'arrière-plan
    // Sinon, retourner la couleur à l'intersection
    return (Result.ObtenirDistance() < 0) ? m_CouleurArrierePlan : ObtenirCouleurSurIntersection(Rayon, Result);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Obtient la couleur à un point d'intersection en particulier.
///         Calcule les contributions colorées de toutes les lumières, avec
///         les modèles de Phong et de Gouraud. Aussi, dépendemment des
///         propriétés de la surface en intersection, on réfléchi ou in
///         réfracte le rayon courant.
///         current ray.
///
///  @param [in]       Rayon const CRayon &    Le rayon à tester
///  @param [in]       Intersection const Scene::CIntersection &    L'ntersection spécifiée
///
///  @return const CCouleur La couleur à l'intersection donnée
///
///  @author Olivier Dionne
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
const CCouleur CScene::ObtenirCouleurSurIntersection(const CRayon& Rayon, const CIntersection& Intersection) const
{
    CCouleur Result =
        Intersection.ObtenirSurface()->ObtenirCouleur() * Intersection.ObtenirSurface()->ObtenirCoeffAmbiant();
    CVecteur3 IntersectionPoint = Rayon.ObtenirOrigine() + Intersection.ObtenirDistance() * Rayon.ObtenirDirection();

    // Calculer les contribution colorées des toutes les lumières dans la scène
    CRayon LumiereRayon;
    for (const auto& uneLumiere : m_Lumieres)
    {
        // Initialise le rayon de lumière (ou rayon d'ombre)
        LumiereRayon.AjusterOrigine(IntersectionPoint);
        LumiereRayon.AjusterDirection(uneLumiere->GetPosition() - IntersectionPoint);
        LumiereRayon.AjusterEnergie(1);
        LumiereRayon.AjusterIndiceRefraction(1);

        if (CVecteur3::ProdScal(LumiereRayon.ObtenirDirection(), Intersection.ObtenirNormale()) > 0)
        {
            // Obtenir la couleur à partir de la lumière
            CCouleur Filter         = ObtenirFiltreDeSurface(LumiereRayon);
            CCouleur LumiereCouleur = uneLumiere->ObtenirCouleur() * Filter;

            // Ajouter la contribution de Gouraud
            REAL GouraudFactor = uneLumiere->GetIntensity() * Intersection.ObtenirSurface()->ObtenirCoeffDiffus() *
                                 CVecteur3::ProdScal(Intersection.ObtenirNormale(), LumiereRayon.ObtenirDirection());
            Result += Intersection.ObtenirSurface()->ObtenirCouleur() * GouraudFactor * LumiereCouleur;

            // À COMPLÉTER
            // AJOUTER LA CONTRIBUTION SPÉCULAIRE DE PHONG...
            CVecteur3 reflectedLightRay = CVecteur3::Reflect( LumiereRayon.ObtenirDirection(), Intersection.ObtenirNormale() );

            double reflectedLightDotRay = CVecteur3::ProdScal( reflectedLightRay, Rayon.ObtenirDirection() );
            if( reflectedLightDotRay > 0 )
            {
                Result += LumiereCouleur
                    * pow( reflectedLightDotRay, Intersection.ObtenirSurface()->ObtenirCoeffBrillance() )
                    * uneLumiere->GetIntensity() * Intersection.ObtenirSurface()->ObtenirCoeffSpeculaire();
            }
        }
    }

    // Effectuer les réflexions de rayon
    REAL ReflectedRayonEnergy = Intersection.ObtenirSurface()->ObtenirCoeffReflexion() * Rayon.ObtenirEnergie();
    if (ReflectedRayonEnergy > m_EnergieMinRayon && Rayon.ObtenirNbRebonds() < m_NbRebondsMax)
    {
        CRayon ReflectedRayon;
        // À COMPLÉTER
        // Ajuster la direction du rayon réfracté
        // ReflectedRayon.AjusterDirection( ... );
        ReflectedRayon.AjusterOrigine(IntersectionPoint);
        ReflectedRayon.AjusterEnergie(ReflectedRayonEnergy);
        ReflectedRayon.AjusterNbRebonds(Rayon.ObtenirNbRebonds() + 1);

        //À decommenter apres ajustement de la direction!
        // Result += ObtenirCouleur( ReflectedRayon ) * Intersection.ObtenirSurface()->ObtenirCoeffReflexion();
    }

    // Effectuer les réfractions de rayon
    REAL RefractedRayonEnergy = Intersection.ObtenirSurface()->ObtenirCoeffRefraction() * Rayon.ObtenirEnergie();
    if (RefractedRayonEnergy > m_EnergieMinRayon && Rayon.ObtenirNbRebonds() < m_NbRebondsMax)
    {
        REAL      IndiceRefractionRatio;
        CRayon    RefractedRayon;
        CVecteur3 SurfaceNormal = Intersection.ObtenirNormale();

        if (Rayon.ObtenirIndiceRefraction() == Intersection.ObtenirSurface()->ObtenirIndiceRefraction())
        {
            // de l'intérieur, vers l'extérieur...
            RefractedRayon.AjusterIndiceRefraction(m_IndiceRefractionScene);
            IndiceRefractionRatio = Intersection.ObtenirSurface()->ObtenirIndiceRefraction() / m_IndiceRefractionScene;
            SurfaceNormal         = -SurfaceNormal;
        }
        else
        {
            // de l'extérieur, vers l'intérieur...
            RefractedRayon.AjusterIndiceRefraction(Intersection.ObtenirSurface()->ObtenirIndiceRefraction());
            IndiceRefractionRatio = m_IndiceRefractionScene / Intersection.ObtenirSurface()->ObtenirIndiceRefraction();
        }

        RefractedRayon.AjusterOrigine(IntersectionPoint);
        RefractedRayon.AjusterEnergie(RefractedRayonEnergy);
        RefractedRayon.AjusterNbRebonds(Rayon.ObtenirNbRebonds() + 1);
        // À COMPLÉTER
        // Ajuster la direction du rayon réfracté
        // ...

        // A decommenter apres ajustement de la direction!
        // Result += ObtenirCouleur( RefractedRayon ) * Intersection.ObtenirSurface()->ObtenirCoeffRefraction();
    }

    return Result;
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Obtenir le filtre du matériau de la surface. Le coefficient de
///         réfraction nous indique du même coup s'il y a transparence de la
///         surface.
///
///  @param [in, out]  LumiereRayon CRayon &     Le rayon de lumière (ou d'ombre) à tester
///
///  @return const CCouleur Le filtre de couleur
///
///  @author Olivier Dionne
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
const CCouleur CScene::ObtenirFiltreDeSurface(CRayon& LumiereRayon) const
{
    CCouleur      Filter = CCouleur::BLANC;
    CIntersection LumiereIntersection;

    REAL Distance = CVecteur3::Norme(LumiereRayon.ObtenirDirection());
    LumiereRayon.AjusterDirection(LumiereRayon.ObtenirDirection() / Distance);

    // TODO : À COMPLÉTER ...

    // Tester le rayon de lumière avec chaque surface de la scène
    // pour vérifier s'il y a intersection
    std::map<REAL, CIntersection> intersections;
    for( ISurface* surface : m_Surfaces )
    {
        CIntersection intersection = surface->Intersection( LumiereRayon );
        if( (intersection.ObtenirDistance() > EPSILON) 
            && (intersection.ObtenirDistance() < Distance))
        {
            intersections[intersection.ObtenirDistance()] = intersection;
        }
    }

    
    while( intersections.size() > 0 )
    {
        map<REAL, CIntersection>::iterator intersection = intersections.lower_bound(0.0);
        REAL coefRefraction = intersection->second.ObtenirSurface()->ObtenirCoeffRefraction();
        CCouleur color = intersection->second.ObtenirSurface()->ObtenirCouleur();
        Filter.r *= color.r * coefRefraction;
        Filter.g *= color.g * coefRefraction;;
        Filter.b *= color.b * coefRefraction;;
        intersections.erase( intersection );
    }

    return Filter;
}
